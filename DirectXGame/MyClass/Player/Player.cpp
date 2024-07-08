#include "Player.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <imgui.h>

void Player::Initialize(std::vector<std::unique_ptr<Model>>& models, uint32_t texHandle) { 
	assert(models[0]);
	for (size_t i = 0; i < models.size(); ++i) {
		models_.push_back(std::move(models[i]));
	}
	texHandle_ = texHandle;
	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformHead_.translation_ = {0.f, 1.6f, 0.f};
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation_ = {-0.55f, 1.22f, 0.f};
	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation_ = {0.55f, 1.22f, 0.f};
	worldTransformBody_.parent_ = &worldTransformBase_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	InitializeFloatingGimmick();
}

void Player::InitializeFloatingGimmick() { floatingParamater_ = 0.0f; }

void Player::Update() { 
	UpdateFlotingGimmick();
	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
}

void Player::Draw(ViewProjection& viewProjection) { 
	models_[Body]->Draw(worldTransformBody_, viewProjection);
	models_[Head]->Draw(worldTransformHead_, viewProjection);
	models_[L_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[R_arm]->Draw(worldTransformR_arm_, viewProjection);

}

void Player::UpdateFlotingGimmick() { 
	// 浮遊移動のサイクル<frame>
	const uint16_t cycle = 120;
	// 1フレームでのパラメーター加算値
	const float step = 2.0f * float(M_PI) / cycle;

	const float amplitude = 0.2f;
	// 1step加算
	floatingParamater_ += step;
	// 
	floatingParamater_ = std::fmod(floatingParamater_, 2.0f * float(M_PI));

	ImGui::Begin("Player");
	ImGui::DragFloat3("head", &worldTransformHead_.translation_.x, 0.01f);
	ImGui::DragFloat3("L", &worldTransformL_arm_.translation_.x, 0.01f);
	ImGui::DragFloat3("R", &worldTransformR_arm_.translation_.x, 0.01f);
	ImGui::End();

	worldTransformBody_.translation_.y = std::sin(floatingParamater_) * amplitude;

	const float amplitudeArm = -0.4f;
	worldTransformL_arm_.rotation_.x = std::sin(floatingParamater_) * amplitudeArm;
	worldTransformR_arm_.rotation_.x = std::sin(floatingParamater_) * amplitudeArm;
}
