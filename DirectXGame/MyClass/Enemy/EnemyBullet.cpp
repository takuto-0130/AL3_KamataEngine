#include "EnemyBullet.h"
#include "TextureManager.h"
#include "Enemy.h"
#include <MyClass/math/Matrix4x4Func.h>
#include <MyClass/math/mathFunc.h>

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	texHandle_ = TextureManager::Load("white1x1.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {0.5f, 0.5f, 3.0f};
	velocity_ = velocity;
	//
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	//Matrix4x4 RotateMatrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	//Vector3 velocityZ = Transform(velocity_, RotateMatrix);
	////
	//worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);
	float length = Length({velocity_.x, 0, velocity_.z});
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, length);
}

void EnemyBullet::Update() {
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, texHandle_); }

void EnemyBullet::OnCollision() { 
	isDead_ = true;
}

Vector3 EnemyBullet::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
