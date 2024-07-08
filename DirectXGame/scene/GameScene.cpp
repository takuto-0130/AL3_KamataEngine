#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"


GameScene::GameScene() {}

GameScene::~GameScene() {
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	texHandle_ = TextureManager::Load("./Resources/cube/cube.jpg");
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

	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels_, texHandle_);


	debugCamera_ = std::make_unique <DebugCamera>(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	skydomeModel_.reset(Model::CreateFromOBJ("skydome"));
	skydome_ = std::make_unique <Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	groundModel_.reset(Model::CreateFromOBJ("Ground"));
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get());


}

void GameScene::Update() { 
	player_->Update();

	skydome_->Update();
	ground_->Update();

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
#endif // _DEBUG
	if (isDebugCameraActive_) {
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
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

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
