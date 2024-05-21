#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <assert.h>


// 寿命<frame>
static const int32_t kLifeTime = 60 * 5;

/// <summary>
/// 自キャラの弾
/// </summary>
class PlayerBullet {
public:

	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);


	void Update();

	bool IsDead() const { return isDead_; }


	void Draw(const ViewProjection& viewProjection);

	void OnCollision();

	Vector3 GetWorldPosition();

	const float GetRadius() { return radius_; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t texHandle_ = 0;
	Vector3 velocity_{};
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
	const float radius_ = 1.5f;
};
