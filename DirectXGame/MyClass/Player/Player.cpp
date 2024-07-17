#include "Player.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <imgui.h>
#include "MyClass/math/mathFunc.h"
#include "MyClass/math/Matrix4x4Func.h"


void Player::Initialize(const std::vector<Model*>& models) { 
	assert(models[0]);
	BaseCharacter::Initialize(models);
	
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformHead_.translation_ = {0.f, 1.6f, 0.f};
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation_ = {-0.55f, 1.22f, 0.f};
	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation_ = {0.55f, 1.22f, 0.f};
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	InitializeFloatingGimmick();
}

void Player::InitializeFloatingGimmick() { floatingParamater_ = 0.0f; }

void Player::Update() { 
	UpdateFlotingGimmick();
	XINPUT_STATE joyState;
	Vector3 move{0, 0, 0};
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float threshold = 0.7f;
		bool isMoving = false;
		move = {float(joyState.Gamepad.sThumbLX) / SHRT_MAX, 0, float(joyState.Gamepad.sThumbLY) / SHRT_MAX};
		if (Length(move) > threshold) {
			isMoving = true;
		}
		if (isMoving) {
			move.x += float(joyState.Gamepad.sThumbLX) / SHRT_MAX * kCharacterSpeed_;
			move.z += float(joyState.Gamepad.sThumbLY) / SHRT_MAX * kCharacterSpeed_;
			move = TransformNormal(move, MakeRotateYMatrix(viewProjection_->rotation_.y));
			worldTransform_.translation_ += move;
			// Y軸周り角度(θy)
			rotateY = std::atan2(move.x, move.z);
		}
	}
	worldTransform_.rotation_.y = LerpShortAngle(worldTransform_.rotation_.y, rotateY, 0.1f);
	
	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) { 
	models_[kBody]->Draw(worldTransformBody_, viewProjection);
	models_[kHead]->Draw(worldTransformHead_, viewProjection);
	models_[kL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kR_arm]->Draw(worldTransformR_arm_, viewProjection);

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
