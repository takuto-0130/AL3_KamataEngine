#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "./MyClass/math/mathFunc.h"
#include "./MyClass/math/operatorOverload.h"
#include <fstream>
#include <istream>

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
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	texHandle_ = TextureManager::Load("./Resources/cube/cube.jpg");
	model_ = Model::Create();
	viewProjection_.Initialize();

	player_ = new Player();
	Vector3 playerPos{0, 0, 40};
	player_->Initialize(model_, texHandle_, playerPos);
	TextureManager::Load("./Resources/reticle.png");

	PopEnemy({10,0,50});

	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	skydomeModel_ = Model::CreateFromOBJ("skydome");
	skydome_ = new Skydome();
	skydome_->Initialize(skydomeModel_);

	railCamera_ = new RailCamera();
	railCamera_->Initialize({0, 0, -30}, {0, 0.001f, 0});

	player_->SetParent(&railCamera_->GetWorldTransform());

	LoadEnemyPopData();
}

void GameScene::Update() { 
	railCamera_->Update();
	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
	viewProjection_.TransferMatrix();

	player_->Update(viewProjection_); 

	UpdateEnemyPopCommands();

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

	CheckAllocollisions();

	skydome_->Update();

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

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
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


	player_->Draw(viewProjection_);



	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	player_->DrawUI();


	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllocollisions() {
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	std::list<Collider*> colliders_;

	colliders_.push_back(player_);
	for (Enemy* enemy : enemys_) {
		colliders_.push_back(enemy);
	}
	for (EnemyBullet* enemtBullet : enemyBullets_) {
		colliders_.push_back(enemtBullet);
	}
	for (PlayerBullet* playerBullet : playerBullets) {
		colliders_.push_back(playerBullet);
	}

	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;
			CheckAllocollisionPair(colliderA, colliderB);
		}
	}
}

void GameScene::CheckAllocollisionPair(Collider* colliderA, Collider* colliderB) { 
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) || 
		(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask())) {
		return;
	}
	Vector3 posA, posB; 
	posA = colliderA->GetWorldPosition();
	posB = colliderB->GetWorldPosition();
	if (Length(posB - posA) <= colliderA->GetRadius() + colliderB->GetRadius()) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}

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
