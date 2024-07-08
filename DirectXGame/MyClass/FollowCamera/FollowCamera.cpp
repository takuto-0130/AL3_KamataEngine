#include "FollowCamera.h"
#include "MyClass/math/operatorOverload.h"
#include "imgui.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "MyClass/math/mathFunc.h"

void FollowCamera::Initialize() { 
	viewProjection_.Initialize();
	viewProjection_.farZ = 1000.0f;
}

void FollowCamera::Update() { 
	if (target_) {
		Vector3 offset = {0.0f, 2.0f, -10.0f};

		XINPUT_STATE joyState;
		Matrix4x4 rotateMatrix;
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			const float cameraRotate = 0.05f;
			viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * cameraRotate;
			rotateMatrix = MakeRotateYMatrix(viewProjection_.rotation_.y);
		}
		offset = TransformNormal(offset, rotateMatrix);

		viewProjection_.translation_ = target_->translation_ + offset;
	}

	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();
	ImGui::Begin("Player");
	ImGui::DragFloat3("head", &viewProjection_.translation_.x, 0.01f);
	/*ImGui::DragFloat3("L", &viewProjection_.translation_.x, 0.01f);
	ImGui::DragFloat3("R", &viewProjection_.translation_.x, 0.01f);*/
	ImGui::End();
}
