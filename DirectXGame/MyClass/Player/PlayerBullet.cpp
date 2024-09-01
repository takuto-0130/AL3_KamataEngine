#include "PlayerBullet.h"
#include "TextureManager.h"
#include "Player.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) { 
	assert(model);
	model_ = model;
	texHandle_ = TextureManager::Load("blue1x1.png");
	worldTransform_.Initialize();
	worldTransform_.scale_ = {0.3f, 0.3f, 0.3f};
	worldTransform_.translation_ = position;
	velocity_ = velocity;
	SetRadius(radius_);
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask((kCollisionAttributePlayer + kCollisionAttributeEnemyBullet) ^ 0x00000000);
}

void PlayerBullet::Update() {
	//SetRadius(radius_);
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, texHandle_);

}

void PlayerBullet::OnCollision() { 
	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition() const {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
