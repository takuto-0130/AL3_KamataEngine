#include "FollowCamera.h"
#include "MyClass/math/operatorOverload.h"
#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

#include "DirectXCommon.h"
#include "Input.h"
#include "MyClass/math/mathFunc.h"
#include "MyClass/Player/LockOn.h"

void FollowCamera::Initialize() { 
	viewProjection_.Initialize();
	viewProjection_.farZ = 1000.0f;
	rotateMatrix_ = MakeIdentity4x4();
}

void FollowCamera::Reset() {
	if (target_) {
		interTarget_ = target_->translation_;
		destinationAngleY_ = target_->rotation_.y;
	}
	viewProjection_.rotation_.y = LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, 0.1f);
	Vector3 offset = Offset();
	viewProjection_.translation_ = interTarget_ + offset;
}

void FollowCamera::InitiaReset() {
	if (target_) {
		interTarget_ = target_->translation_;
		destinationAngleY_ = target_->rotation_.y;
	}
	viewProjection_.rotation_.y = LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, 1.0f);
	Vector3 offset = Offset();
	viewProjection_.translation_ = interTarget_ + offset;
}

void FollowCamera::Update() { 
	if (lockOn_ && lockOn_->ExsitTarget()) {
		Vector3 lockOnPos = lockOn_->GetTargetPosition();
		interTarget_ = target_->translation_;
		Vector3 sub = lockOnPos - (interTarget_ + Offset());
		destinationAngleY_ = std::atan2(sub.x, sub.z);
		viewProjection_.rotation_.y = LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, 0.3f);
		sub = lockOnPos - interTarget_;
		float rotareY = std::atan2(sub.x, sub.z);
		player_->SetRotare(rotareY);
		player_->PlayerUpdateMatrix();
	} else if (target_) {
		interTarget_ = Lerp(interTarget_, target_->translation_, .2f);

		XINPUT_STATE joyState;
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			const float cameraRotate = 0.05f;
			destinationAngleY_ += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * cameraRotate;
			if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_RIGHT_THUMB) {
				interTarget_ = target_->translation_;
				destinationAngleY_ = target_->rotation_.y;
			}
		}
		viewProjection_.rotation_.y = LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, 0.1f);
	}

	Vector3 offset = Offset();
	viewProjection_.translation_ = interTarget_ + offset;

	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();
#ifdef _DEBUG

	ImGui::Begin("Player");
	ImGui::DragFloat3("viewProjection", &viewProjection_.translation_.x, 0.01f);
	ImGui::End();

#endif // _DEBUG
}

void FollowCamera::SetTarget(const WorldTransform* target) { 
	target_ = target;
	Reset();
}

const Vector3 FollowCamera::Offset() {
	Vector3 offset = {3.0f, 3.0f, -10.0f};
	rotateMatrix_ = MakeRotateYMatrix(viewProjection_.rotation_.y);
	offset = TransformNormal(offset, rotateMatrix_);
	return offset;
}
