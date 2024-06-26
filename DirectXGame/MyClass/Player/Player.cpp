#include "Player.h"

void Player::Initialize(Model* model, uint32_t texHandle) { 
	model_ = model;
	texHandle_ = texHandle;
}

void Player::Update() { 
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, texHandle_);
}
