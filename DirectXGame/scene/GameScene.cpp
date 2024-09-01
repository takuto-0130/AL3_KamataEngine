#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#ifdef _DEBUG

#include "imgui.h"

#endif // _DEBUG
#include "PrimitiveDrawer.h"


GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete colliderManager_;
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	model_.reset(Model::Create());
	viewProjection_.Initialize();
	viewProjection_.farZ = 1000.0f;

	playerModel_.reset(Model::CreateFromOBJ("PlayerBody"));
	playerModels_.push_back(std::move(playerModel_));
	playerModel_.reset(Model::CreateFromOBJ("PlayerHead"));
	playerModels_.push_back(std::move(playerModel_));
	playerModel_.reset(Model::CreateFromOBJ("PlayerLeftArm"));
	playerModels_.push_back(std::move(playerModel_));
	playerModel_.reset(Model::CreateFromOBJ("PlayerRightArm"));
	playerModels_.push_back(std::move(playerModel_));
	playerModel_.reset(Model::CreateFromOBJ("Hammer"));
	playerModels_.push_back(std::move(playerModel_));

	player_ = std::make_unique<Player>();
	std::vector<Model*> playerModels = {
	    playerModels_[PlayerParts::kBody].get(), playerModels_[PlayerParts::kHead].get(), 
		playerModels_[PlayerParts::kL_arm].get(), playerModels_[PlayerParts::kR_arm].get(),
		playerModels_[PlayerParts::kHammer].get()
	};
	player_->Initialize(playerModels);


	enemyModel_.reset(Model::CreateFromOBJ("EnemyBody"));
	enemyModels_.push_back(std::move(enemyModel_));
	enemyModel_.reset(Model::CreateFromOBJ("EnemyLeftArm"));
	enemyModels_.push_back(std::move(enemyModel_));
	enemyModel_.reset(Model::CreateFromOBJ("EnemyRightArm"));
	enemyModels_.push_back(std::move(enemyModel_));
	enemy_ = std::make_unique<Enemy>();
	std::vector<Model*> enemyModels = {
		enemyModels_[EnemyParts::kEnemyBody].get(), enemyModels_[EnemyParts::kEnemyL_arm].get(), enemyModels_[EnemyParts::kEnemyR_arm].get()
	};
	enemy_->Initialize(enemyModels);
	enemy_->SetPlayer(player_.get());
	enemy_->SetGameScene(this);
	enemy_->SetBulletModel(model_.get());

	
	debugCamera_ = std::make_unique <DebugCamera>(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	skydomeModel_.reset(Model::CreateFromOBJ("skydome"));
	skydome_ = std::make_unique <Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	groundModel_.reset(Model::CreateFromOBJ("Ground"));
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get());

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(&player_->GetWorldTransform());
	followCamera_->SetPlayer(*player_.get());
	player_->SetViewProjection(&followCamera_->GetViewProjection());

	PrimitiveDrawer::GetInstance()->Initialize();
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Initialize();
	followCamera_->SetLockOn(*lockOn_);
	player_->SetLockOn(*lockOn_);

	colliderManager_ = new ColliderManager();

	textureGameOver = TextureManager::Load("./Resources/gameOver.png");
	spriteGameOver_.reset(Sprite::Create(textureGameOver, {0, 0}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0}));

	textureGameClear = TextureManager::Load("./Resources/gameClear.png");
	spriteGameClear_.reset(Sprite::Create(textureGameClear, {0, 0}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0}));

	textureTitle = TextureManager::Load("./Resources/title.png");
	spriteTitle_.reset(Sprite::Create(textureTitle, {0, 0}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0}));

	endTimer = 0;
}

void GameScene::Reset() {
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->OnCollision();
	}
	endTimer = 0;
	enemy_->Reset();
	player_->Reset();
	followCamera_->InitiaReset();
}

void GameScene::Update() { 
	if (!isTitle) {
		player_->Update();
		if (!player_->IsDead() && !enemy_->IsDead()) {
			enemy_->Update();
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

		if (!player_->IsDead() && !enemy_->IsDead()) {
			colliderManager_->Update(player_.get(), enemy_.get(), GetEnemyBullets());
			lockOn_->Update(enemy_, viewProjection_);
		} else {
			endTimer++;
		}

		followCamera_->Update();
		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();

		skydome_->Update();
		ground_->Update();
		ReturnToTitle();
	} else {
		titleTimer++;
		XINPUT_STATE joyState;
		if (Input::GetInstance()->GetJoystickState(0, joyState) && (titleTimer > kTitleTime)) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				isTitle = false;
				titleTimer = 0;
			}
		}
	}

	//================
	// デバッグカメラ
	//================
	debugCamera_->Update();
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_L)) {
		if (isDebugCameraActive_ == true) {
			isDebugCameraActive_ = false;
		} else {
			isDebugCameraActive_ = true;
		}
	}
	if (input_->TriggerKey(DIK_R)) {
		enemy_->Reset();
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
	ground_->Draw(viewProjection_);

	enemy_->Draw(viewProjection_);
	if (!player_->IsDead() && !enemy_->IsDead() && !isTitle) {
		for (EnemyBullet* bullet : enemyBullets_) {
			bullet->Draw(viewProjection_);
		}
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

	enemy_->DrawUI();
	player_->DrawUI();
	lockOn_->Draw();
	if (player_->IsDead()) {
		spriteGameOver_->Draw();
	}
	if (enemy_->IsDead()) {
		spriteGameClear_->Draw();
	}
	if (isTitle) {
		spriteTitle_->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	enemyBullets_.push_back(enemyBullet); }

void GameScene::ReturnToTitle() {
	if (endTimer > kEndTimer) {
		XINPUT_STATE joyState;
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				Reset();
				isTitle = true;
			}
		}
	}

}
