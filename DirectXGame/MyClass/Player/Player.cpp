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
	worldTransformHammer_.parent_ = &worldTransformBody_;

	worldTransformHammer_.Initialize();
	worldTransformHammer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformHammer_.translation_ = {0.0f, 1.4f, 0.0f};

	InitializeFloatingGimmick();
	InitializeAttackGimmick();
	input_ = Input::GetInstance();
}

void Player::InitializeFloatingGimmick() { floatingParamater_ = 0.0f; }

void Player::InitializeAttackGimmick() { 
	attackParamater_ = float(M_PI) / 2.0f;
	furiageWaite_ = 0;
	attackWait_ = 0;
	attackCurrent_ = 0;
	attackAfterWait_ = 0;
}

void Player::Update() { 
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_) { 
		case Behavior::kRoot:
		default:
			InitializeFloatingGimmick();
			break;
		case Behavior::kAttack:
			InitializeAttackGimmick();
			break;
		}
		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_) {
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	}
	int a = attackWaitTime_;
	int b = attackTime_;
	int c = attackAfterWaitTime_;
	int d = furiageTime_;
	ImGui::Begin("Attack");
	ImGui::SliderInt("before", &d, 0, 100);
	ImGui::SliderInt("wait", &a, 0, 100);
	ImGui::SliderInt("attack", &b, 0, 100);
	ImGui::SliderInt("after", &c, 0, 100);
	ImGui::End();
	attackWaitTime_ = a;
	attackTime_ = b;
	attackAfterWaitTime_ = c;
	furiageTime_ = d;
}

void Player::BehaviorRootUpdate() {
	UpdateFlotingGimmick();
	Move();

	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformHammer_.UpdateMatrix();
	if (input_->TriggerKey(DIK_SPACE)) {
		behaviorRequest_ = Behavior::kAttack;
	}
}

void Player::BehaviorAttackUpdate() {
	// 1フレームでのパラメーター加算値
	const float waitStep = 2.0f * float(M_PI) / (furiageTime_ * 4.0f);

	const float step = 2.0f * float(M_PI) / (attackTime_ * 4.0f);

	const float amplitude = 1.9f;

	if (furiageWaite_ <= furiageTime_) {
		furiageWaite_++;
		attackParamater_ -= waitStep;
	}
	else if (attackWait_ < attackWaitTime_) {
		attackWait_++;
	}
	else if (attackCurrent_ <= attackTime_) {
		attackCurrent_++;
		attackParamater_ += step;
	} 
	else if (attackAfterWait_ < attackAfterWaitTime_) {
		attackAfterWait_++;
	}
	else {
		behaviorRequest_ = Behavior::kRoot;
	}
	
	float offset = -0.2f;

	worldTransformHammer_.rotation_.x = std::sin(attackParamater_) * amplitude + offset;

	offset = -3.4f;
	worldTransformL_arm_.rotation_.x = std::sin(attackParamater_) * amplitude + offset;
	worldTransformR_arm_.rotation_.x = std::sin(attackParamater_) * amplitude + offset;
	
	Move();
	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformHammer_.UpdateMatrix();
}

void Player::Move() {
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
}

void Player::Draw(const ViewProjection& viewProjection) { 
	models_[kBody]->Draw(worldTransformBody_, viewProjection);
	models_[kHead]->Draw(worldTransformHead_, viewProjection);
	models_[kL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kR_arm]->Draw(worldTransformR_arm_, viewProjection);
	if (behavior_ == Behavior::kAttack) {
		models_[kHammer]->Draw(worldTransformHammer_, viewProjection);
	}
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


	worldTransformBody_.translation_.y = std::sin(floatingParamater_) * amplitude;

	const float amplitudeArm = -0.4f;
	worldTransformL_arm_.rotation_.x = std::sin(floatingParamater_) * amplitudeArm;
	worldTransformR_arm_.rotation_.x = std::sin(floatingParamater_) * amplitudeArm;
}
