#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MyClass/Player/Player.h"
#include "MyClass/Enemy/Enemy.h"
#include "MyClass/Skydome/Skydome.h"
#include "MyClass/RailCamera/RailCamera.h"
#include "DebugCamera.h"

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

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllocollisions();


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	uint32_t texHandle_ = 0;
	Model* model_ = nullptr;
	ViewProjection viewProjection_;
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;

	Skydome* skydome_ = nullptr;
	Model* skydomeModel_ = nullptr;

	RailCamera* railCamera_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
