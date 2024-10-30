#include "RailCamera.h"
#include "MyClass/math/Matrix4x4Func.h"
#include "imgui.h"

void RailCamera::Initialize(const Vector3& worldPos, const Vector3& radian) { 
	worldTransform_.translation_ = worldPos;
	worldTransform_.rotation_ = radian;
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
}

void RailCamera::Update() { 
	//worldTransform_.rotation_.y += 0.001f;
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	ImGui::Begin("Camera");
	ImGui::InputFloat3("CameraTranslate", &worldTransform_.translation_.x);
	ImGui::InputFloat3("CameraRotation", &worldTransform_.rotation_.x);
	ImGui::End();
}

void RailCamera::Rotate(const Vector3& rotate) { worldTransform_.rotation_ = rotate; }

void RailCamera::Translate(const Vector3& translate) { worldTransform_.translation_ = translate; }

void RailCamera::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }