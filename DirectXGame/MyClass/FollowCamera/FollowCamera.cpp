#include "FollowCamera.h"
#include "MyClass/math/operatorOverload.h"
#include "imgui.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "MyClass/math/mathFunc.h"

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

void FollowCamera::Update() { 
	if (target_) {
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
	}

	viewProjection_.rotation_.y = LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, 0.1f);
	Vector3 offset = Offset();
	viewProjection_.translation_ = interTarget_ + offset;

	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();
	ImGui::Begin("Player");
	ImGui::DragFloat3("viewProjection", &viewProjection_.translation_.x, 0.01f);
	ImGui::End();
}

void FollowCamera::SetTarget(const WorldTransform* target) { 
	target_ = target;
	Reset();
}

const Vector3 FollowCamera::Offset() {
	Vector3 offset = {0.0f, 2.0f, -10.0f};
	rotateMatrix_ = MakeRotateYMatrix(viewProjection_.rotation_.y);
	offset = TransformNormal(offset, rotateMatrix_);
	return offset;
}
