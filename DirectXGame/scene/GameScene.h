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
#include "MyClass/Collider.h"
#include "MyClass/Collider/ColliderManager.h"
#include "DebugCamera.h"
#include <sstream>

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
	/// 敵弾を追加
	/// </summary>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵のスポーン
	/// </summary>
	void PopEnemy(Vector3 position);

	/// <summary>
	/// 敵のスポーンデータの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵のスポーン更新
	/// </summary>
	void UpdateEnemyPopCommands();

private:


	const std::list<Enemy*>& GetEnemys() const { return enemys_; }
	const std::list<EnemyBullet*>& GetEnemyBullets() const { return enemyBullets_; }


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	uint32_t texHandle_ = 0;
	Model* model_ = nullptr;
	ViewProjection viewProjection_;

	Player* player_ = nullptr;

	std::list<Enemy*> enemys_;
	std::list<EnemyBullet*> enemyBullets_;
	bool isEnemyPopWait_ = false;
	int32_t enemyPopTimer_ = 0;

	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;

	Skydome* skydome_ = nullptr;
	Model* skydomeModel_ = nullptr;

	RailCamera* railCamera_ = nullptr;

	ColliderManager* colliderManager_ = nullptr;

	std::stringstream EnemyPopCommands_;

	std::vector<Vector3> controlPoints_;
	std::vector<Vector3> pointsDrawing_;
	const size_t segmentCount = 100;
};
