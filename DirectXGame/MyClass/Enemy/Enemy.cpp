#include "Enemy.h"


// 敵の移動の速さ
const float kEnemySpeed = -0.2f;

void Enemy::Initialize(Model* model, uint32_t texHandle) {
	assert(model);

	model_ = model;
	texHandle_ = texHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0, 3, 20};
	velocity_ = {0, 0, kEnemySpeed};
}

void Enemy::Approach() {
	velocity_ = {0, 0, kEnemySpeed};
	worldTransform_.translation_ += velocity_;
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = LEAVE;
	}
}

void Enemy::Leave() {
	velocity_ = {kEnemySpeed, 0, 0};
	worldTransform_.translation_ += velocity_;
}

void Enemy::Update() {
	switch (phase_) { 
	case APPROACH:
	default:
		Approach();
		break;
	case LEAVE:
		Leave();
		break;
	}


	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, texHandle_);
}
