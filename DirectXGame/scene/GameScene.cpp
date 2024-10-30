#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "./MyClass/math/mathFunc.h"
#include "./MyClass/math/operatorOverload.h"
#include <fstream>
#include <istream>

#ifdef _DEBUG
#include "imgui.h"
#endif // DEBUG_


const Vector3 TPSRailCameraPos{0, 0, -30};

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_;
	delete player_;
	delete debugCamera_;
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
	delete skydome_;
	delete skydomeModel_;
	delete railCamera_;
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	delete colliderManager_;
}

void GameScene::Initialize() {
#pragma region // インスタンスの取得や初期化
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	PrimitiveDrawer::GetInstance()->Initialize();
	viewProjection_.Initialize();
#pragma endregion

#pragma region // プレイヤーの初期化
	texHandle_ = TextureManager::Load("./Resources/cube/cube.jpg");
	model_ = Model::Create();

	player_ = new Player();
	Vector3 playerPos{0, 0, 30};
	player_->Initialize(model_, texHandle_, playerPos);
	TextureManager::Load("./Resources/reticle.png");
#pragma endregion

	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	skydomeModel_ = Model::CreateFromOBJ("skydome");
	skydome_ = new Skydome();
	skydome_->Initialize(skydomeModel_);

	railCamera_ = new RailCamera();
	railCamera_->Initialize(TPSRailCameraPos, {0, 0, 0});

	player_->SetParent(&railCamera_->GetWorldTransform());

	colliderManager_ = new ColliderManager();

	//LoadEnemyPopData();

#pragma region // レール設定
	controlPoints_ = {
	    {0,  0,  0},
        {10, 10, 10},
        {10, 15, 20},
        {20, 15, 30},
        {20, 0,  35},
        {30, 0,  40}
    };
	segmentCount = oneSegmentCount * controlPoints_.size();
	for (size_t i = 0; i < segmentCount + 1; i++) {
		float t = 1.0f / segmentCount * i;
		Vector3 pos = CatmullRomPosition(controlPoints_, t);
		pointsDrawing_.push_back(pos);
	}
	for (Vector3& pos : controlPoints_) {
		PopEnemy(pos);
	}
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
#pragma endregion
}

void GameScene::Update() { 

	RailCameraDebug();
	//RailCameraMove();
	RailCustom();

	if (input_->TriggerKey(DIK_P)) {
		RailReDrawing();
	}

	changeFPSTPS();
	
	ViewProjectionUpdate();

	//CharacterUpdate();

	skydome_->Update();

	DebugCameraUpdate();
}

void GameScene::Draw() {

#pragma region 描画前
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
#pragma endregion

#pragma region 背景
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	// ↓背景


	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	skydome_->Draw(viewProjection_);

	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}

	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}


	//player_->Draw(viewProjection_);


	for (size_t i = 0; i < segmentCount; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(pointsDrawing_[i], pointsDrawing_[i + 1], {1.0f, 0.0f, 0.0f, 1.0f});
	}



	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	//player_->DrawUI(viewProjection_);


	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {

	enemyBullets_.push_back(enemyBullet);
}

void GameScene::PopEnemy(Vector3 position) {

	Enemy* enemy = new Enemy();
	enemy->Initialize(model_, texHandle_, position);
	enemy->SetPlayer(player_);
	enemy->SetGameScene(this);
	
	enemys_.push_back(enemy);
}

void GameScene::LoadEnemyPopData() { 
	std::ifstream file;
	file.open("Resources/EnemyPop.csv");
	assert(file.is_open());

	EnemyPopCommands_ << file.rdbuf();

	file.close();
}

void GameScene::UpdateEnemyPopCommands() {

	if (isEnemyPopWait_) {
		enemyPopTimer_--;
		if (enemyPopTimer_ <= 0) {
			isEnemyPopWait_ = false;
		}
		return;
	}

	std::string line;

	while (std::getline(EnemyPopCommands_, line)) {
		std::istringstream line_stream(line);
		std::string word;
		std::getline(line_stream, word, ',');
		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("POP") == 0) {
			std::getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			std::getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			std::getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			PopEnemy({x, y, z});
		}
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			isEnemyPopWait_ = true;
			enemyPopTimer_ = waitTime;

			break;
		}
	}
}

void GameScene::ViewProjectionUpdate() {
	railCamera_->Update();
	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
	viewProjection_.TransferMatrix();
}

void GameScene::CharacterUpdate() {
	player_->Update(viewProjection_);

	// UpdateEnemyPopCommands();

	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}
	player_->SetEnemy(enemys_);

	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	// colliderManager_->Update(player_, GetEnemys(), GetEnemyBullets());
}

void GameScene::changeFPSTPS() {
	/*if (input_->TriggerKey(DIK_F)) {
		if (isFPS_ == true) {
			isFPS_ = false;
			railCamera_->SetParent(nullptr);
			player_->SetParent(&railCamera_->GetWorldTransform());
			railCamera_->Translate(TPSRailCameraPos);
		} else {
			isFPS_ = true;
			player_->SetParent(nullptr);
			railCamera_->SetParent(&player_->GetWorldTransform());
		}
	}
	if (isFPS_) {
		railCamera_->Translate(player_->GetWorldPosition());
		railCamera_->Rotate(player_->GetRotate());
	}*/
}

void GameScene::RailCustom() {
#ifdef _DEBUG
	int32_t i = 0;
	ImGui::Begin("Rail");
	ImGui::Text("ReDraw : P");
	for (Vector3& pos : controlPoints_) {
		i++;
		/*ImGui::Text("%d.", i);
		ImGui::SameLine();*/
		std::string label = "controlPoint." + std::to_string(i);
		ImGui::DragFloat3(label.c_str(), &pos.x, 0.1f);
	}
	if (ImGui::Button("addControlPoint")) {
		Vector3 pos = controlPoints_.back();
		controlPoints_.push_back(pos);
		segmentCount = oneSegmentCount * controlPoints_.size();
		RailReDrawing();
	}
	ImGui::End();
#endif // _DEBUG
}

void GameScene::RailReDrawing() {
	pointsDrawing_.clear();
	for (size_t i = 0; i < segmentCount + 1; i++) {
		float t = 1.0f / segmentCount * i;
		Vector3 pos = CatmullRomPosition(controlPoints_, t);
		pointsDrawing_.push_back(pos);
	}
	enemys_.remove_if([](Enemy* enemy) {
		if (!enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});
	for (Vector3& pos : controlPoints_) {
		PopEnemy(pos);
	}
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}
}

void GameScene::DebugCameraUpdate() {
	debugCamera_->Update();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_L)) {
		if (isDebugCameraActive_ == true) {
			isDebugCameraActive_ = false;
		} else {
			isDebugCameraActive_ = true;
		}
	}
#endif // _DEBUG
	if (isDebugCameraActive_) {
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		//viewProjection_.UpdateMatrix();
	}
}

void GameScene::RailCreate() { 
	Vector3 frontVec{};
	Vector3 upVec{};
	Vector3 leftVec = Cross(upVec, frontVec);
	
}

void GameScene::RailCameraMove() {
	if (cameraForwardT <= 1.0f) {
		cameraEyeT += cameraSegmentCount;
		cameraForwardT += cameraSegmentCount;
		Vector3 eye = CatmullRomPosition(controlPoints_, cameraEyeT);
		eye.y += 0.5f;
		railCamera_->Translate(eye);
		Vector3 forward = CatmullRomPosition(controlPoints_, cameraForwardT);
		forward.y += 0.5f;
		forward = forward - eye;
		if (cameraForwardT <= 1.0f) {
			Vector3 rotateCametra{};
			// Y軸周り角度(θy)
			rotateCametra.y = std::atan2(forward.x, forward.z);
			float length = Length({forward.x, 0, forward.z});
			// X軸周り角度(θx)
			rotateCametra.x = std::atan2(-forward.y, length);
			railCamera_->Rotate(rotateCametra);
		}
	}
#ifdef _DEBUG
	ImGui::Begin("RailCamera");
	ImGui::Text("eye%.03f", cameraEyeT);
	ImGui::Text("forward%.03f", cameraForwardT);
	ImGui::End();
#endif // _DEBUG
}

void GameScene::RailCameraDebug() { 
#ifdef _DEBUG
	ImGui::Begin("RailCamera");
	if (ImGui::Button("StartCamera")) {
		isRailCameraMove_ = true;
	}
	if (ImGui::Button("StopCamera")) {
		isRailCameraMove_ = false;
	}
	if (ImGui::Button("ResetCamera")) {
		ResetRailCamera();
		RailCameraMove();
	}
	if (isRailCameraMove_) {
		RailCameraMove();
	}
	ImGui::End();
#endif // _DEBUG
}

void GameScene::SetSegment() {
	float segmentDenominator = kDivisionSpan * controlPoints_.size();
	cameraSegmentCount = 1.0f / segmentDenominator;
}

void GameScene::ResetRailCamera() {
	float segmentDenominator = kDivisionSpan * controlPoints_.size();
	cameraEyeT = 0;
	cameraForwardT = 30.0f / segmentDenominator;
}
