#include "EnemyBullet.h"
#include "TextureManager.h"
#include "Enemy.h"
#include <MyClass/math/mathFunc.h>
#include <MyClass/math/operatorOverload.h>

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	texHandle_ = TextureManager::Load("red1x1.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {0.3f, 0.3f, 1.0f};
	velocity_ = velocity;
	DirectionSet();
	SetRadius(radius_);
	SetCollisionAttribute(kCollisionAttributeEnemyBullet);
	SetCollisionMask((kCollisionAttributeEnemy + kCollisionAttributeEnemyBullet) ^ 0x00000000);
}

void EnemyBullet::Update() {
	SetRadius(radius_);
	Homing();
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) { 
	if (worldTransform_.GetConstBuffer().Get()) {
		model_->Draw(worldTransform_, viewProjection, texHandle_);
	}
}

void EnemyBullet::OnCollision() { 
	isDead_ = true;
}

Vector3 EnemyBullet::GetWorldPosition() const {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void EnemyBullet::DirectionSet() {
	// Y軸周り角度(θy)
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float length = Length({velocity_.x, 0, velocity_.z});
	// X軸周り角度(θx)
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, length);
}

void EnemyBullet::Homing() { 
	Vector3 toPlayer = player_->GetWorldPosition() - GetWorldPosition();
	toPlayer = Normalize(toPlayer);
	velocity_ = Normalize(velocity_);
	velocity_ = Slerp(velocity_, toPlayer, 0.05f) * 1.0f;
	DirectionSet();
}