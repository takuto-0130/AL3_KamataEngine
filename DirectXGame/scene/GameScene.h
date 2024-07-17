#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "MyClass/Player/Player.h"
#include "MyClass/Enemy/Enemy.h"
#include "MyClass/Skydome/Skydome.h"
#include "MyClass/Ground/Ground.h"
#include "MyClass/FollowCamera/FollowCamera.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include <memory>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;


	bool isDebugCameraActive_ = false;
	std::unique_ptr<DebugCamera> debugCamera_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	std::unique_ptr<Model> model_;
	ViewProjection viewProjection_;

	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> playerModel_;
	std::vector<std::unique_ptr<Model>> playerModels_;

	std::unique_ptr<Enemy> enemy_;
	std::unique_ptr<Model> enemyModel_;
	std::vector<std::unique_ptr<Model>> enemyModels_;

	std::unique_ptr<Model> skydomeModel_;
	std::unique_ptr<Skydome> skydome_;

	std::unique_ptr<Model> groundModel_;
	std::unique_ptr<Ground> ground_;

	std::unique_ptr<FollowCamera> followCamera_;
};