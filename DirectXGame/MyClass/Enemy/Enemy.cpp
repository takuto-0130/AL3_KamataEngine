#include "Enemy.h"
#include <imgui.h>
#include <assert.h>
#include <MyClass/math/mathFunc.h>

void Enemy::Initialize(const std::vector<Model*>& models) {
	assert(models[0]);
	BaseCharacter::Initialize(models);
	worldTransform_.translation_ = {5.5f, 1, 5};
	worldTransform_.rotation_ = {0, 3.3f, 0};
	worldTransformBody_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation_ = {-0.4f, 0.25f, 0.f};
	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation_ = {0.4f, 0.25f, 0.f};
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	InitializeArmGimmick();
}

void Enemy::InitializeArmGimmick() { floatingParamater_ = 0.0f; }

void Enemy::Update() {
	UpdateArmGimmick();
	const float kEnemySpeed = 0.13f;
	Vector3 velocity(0, 0, kEnemySpeed);
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);
	worldTransform_.rotation_.y += 0.02f;
	worldTransform_.translation_ += velocity;
	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	ImGui::Begin("Enemy");
	ImGui::DragFloat3("L", &worldTransformL_arm_.translation_.x, 0.01f);
	ImGui::DragFloat3("R", &worldTransformR_arm_.translation_.x, 0.01f);
	ImGui::DragFloat("Ro", &worldTransform_.rotation_.y, 0.01f);
	ImGui::End();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	models_[kEnemyBody]->Draw(worldTransformBody_, viewProjection);
	models_[kEnemyL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kEnemyR_arm]->Draw(worldTransformR_arm_, viewProjection);
}

void Enemy::UpdateArmGimmick() {
	// 浮遊移動のサイクル<frame>
	const uint16_t cycle = 40;
	// 1フレームでのパラメーター加算値
	const float step = 2.0f * float(M_PI) / cycle;
	// 1step加算
	floatingParamater_ += step;
	//
	floatingParamater_ = std::fmod(floatingParamater_, 2.0f * float(M_PI));

	ImGui::Begin("Player");
	ImGui::DragFloat3("L", &worldTransformL_arm_.translation_.x, 0.01f);
	ImGui::DragFloat3("R", &worldTransformR_arm_.translation_.x, 0.01f);
	ImGui::End();

	const float amplitudeArm = -0.1f;
	worldTransformL_arm_.translation_.y = std::sin(floatingParamater_) * amplitudeArm + kOffsetArmHeight;
	worldTransformR_arm_.translation_.y = -(std::sin(floatingParamater_) * amplitudeArm) + kOffsetArmHeight;
}
