#include "Enemy.h"


// 敵の移動の速さ
const float kEnemySpeed = 0.2f;

void Enemy::Initialize(Model* model, uint32_t texHandle) {
	assert(model);

	model_ = model;
	texHandle_ = texHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {10, 10, 0};
	velocity_ = {0, 0, kEnemySpeed};
}

void Enemy::Update() {
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, texHandle_);
}
