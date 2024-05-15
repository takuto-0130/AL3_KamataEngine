#include "Skydome.h"
#include "assert.h"

void Skydome::Initialize(Model* model) { 
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {500.0f, 500.0f, 500.0f};
}

void Skydome::Update() { 
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection);
}
