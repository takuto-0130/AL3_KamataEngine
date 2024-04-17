#include "Player.h"

void Player::Initialize(Model* model, uint32_t texHandle) { 
	assert(model);

	model_ = model;
	texHandle_ = texHandle;
	worldTransform_.Initialize();
}

void Player::Update() {
	worldTransform_.TransferMatrix();

}

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, texHandle_);


}
