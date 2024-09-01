#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "MyClass/Player/Player.h"
#include "MyClass/Enemy/Enemy.h"
#include "MyClass/Enemy/EnemyBullet.h"
#include "MyClass/Skydome/Skydome.h"
#include "MyClass/Ground/Ground.h"
#include "MyClass/FollowCamera/FollowCamera.h"
#include "MyClass/Collider/ColliderManager.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include <memory>
#include "MyClass/Player/LockOn.h"

const int kEndTimer = 30;
const int kTitleTime = 30;

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

	void Reset();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 敵弾を追加
	/// </summary>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	void ReturnToTitle();

private:
	Enemy* GetEnemys() const { return enemy_.get(); }
	const std::list<EnemyBullet*>& GetEnemyBullets() const { return enemyBullets_; }

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
	std::list<EnemyBullet*> enemyBullets_;

	std::unique_ptr<Model> skydomeModel_;
	std::unique_ptr<Skydome> skydome_;

	std::unique_ptr<Model> groundModel_;
	std::unique_ptr<Ground> ground_;

	std::unique_ptr<FollowCamera> followCamera_;

	// ロックオン
	std::unique_ptr<LockOn> lockOn_;

	ColliderManager* colliderManager_ = nullptr;

	std::unique_ptr<Sprite> spriteGameOver_;
	uint32_t textureGameOver;

	std::unique_ptr<Sprite> spriteGameClear_;
	uint32_t textureGameClear;

	std::unique_ptr<Sprite> spriteTitle_;
	uint32_t textureTitle;

	int32_t endTimer = 0;

	int32_t titleTimer = 0;
	bool isTitle = true;
};