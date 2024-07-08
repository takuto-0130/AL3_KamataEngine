#include "Player.h"

void Player::Initialize(std::vector<std::unique_ptr<Model>>& models, uint32_t texHandle) { 
	assert(models[0]);
	for (size_t i = 0; i < models.size(); ++i) {
		models_.push_back(std::move(models[i]));
	}
	texHandle_ = texHandle;
	worldTransform_.Initialize();
	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
}

void Player::Update() { 
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection) { 
	for (size_t i = 0; i < models_.size(); ++i) {
		models_[i]->Draw(worldTransform_, viewProjection);
	}
	models_[0]->Draw(worldTransform_, viewProjection);
}
