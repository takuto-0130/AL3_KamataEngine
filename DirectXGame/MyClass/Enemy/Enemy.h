#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <assert.h>
#include <./MyClass/Enemy/EnemyBullet.h>

class Player;
class GameScene;

static const int kBulletInterval = 90;

class Enemy {
public:

	~Enemy();

	void SetPlayer(Player* player) { player_ = player; }

	void Initialize(Model* model, uint32_t texHandle, Vector3 position);

	void ApproachInitialize();

	void Approach();

	void Leave();

	void Update();

	void Draw(ViewProjection& viewProjection);

	void Fire();

	void OnCollision();

	Vector3 GetWorldPosition();

	const float GetRadius() { return radius_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsDead() const { return isDead_; }

private:

	GameScene* gameScene_ = nullptr;

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t texHandle_ = 0;
	Vector3 velocity_{};
	bool isDead_ = false;
	enum class Phase { 
		APPROACH,
		LEAVE 
	};
	Phase phase_ = Phase::APPROACH;
	int32_t bulletInterval_ = 0;
	Player* player_ = nullptr;
	const float radius_ = 1.0f;

	static void (Enemy::*spFuncTable[])();
};
