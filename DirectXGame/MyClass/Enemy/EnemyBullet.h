#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <assert.h>
#include <MyClass/Player/Player.h>

static const int32_t kEnemyBulletLifeTime = 60 * 5;

static const float kEnemyBulletSpeed = 4.0f;

class EnemyBullet {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	bool IsDead() const { return isDead_; }

	void Draw(const ViewProjection& viewProjection);

	void OnCollision();

	const Vector3 GetWorldPosition();

	void DirectionSet();

	void Homing();

	const float GetRadius() { return radius_; }

	void SetPlayer(Player* player) { player_ = player; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t texHandle_ = 0;
	Vector3 velocity_{};
	int32_t deathTimer_ = kEnemyBulletLifeTime;
	bool isDead_ = false;
	const float radius_ = 1.5f;
	Player* player_ = nullptr;
};
