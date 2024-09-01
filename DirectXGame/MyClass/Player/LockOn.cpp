#include "LockOn.h"

#include "MyClass/math/mathFunc.h"
#include "MyClass/math/Matrix4x4Func.h"
#include "MyClass/math/operatorOverload.h"
#include <TextureManager.h>

void LockOn::Initialize() {
	textureReticle = TextureManager::Load("./Resources/reticle.png");
	lockOnMark_.reset(Sprite::Create(textureReticle, {100,100}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	isTrigger = false;
}

void LockOn::Update(const std::unique_ptr<Enemy>& enemy, const ViewProjection& viewProjection) { 
	XINPUT_STATE joyState;
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) && !enemy->IsDead()) {
		if(isTrigger == false) {
			if (target_ != nullptr) {
				target_ = nullptr;
			} else {
				if (isLockOnRange(enemy, viewProjection)) {
					target_ = enemy.get();
				} 
			}
		}
		isTrigger = true;
	} else {
		isTrigger = false;
		if (!isLockOnRange(enemy, viewProjection) || enemy->IsDead()) {
			target_ = nullptr;
		} 
	}

	if (target_) {
		Vector3 positionWorld = enemy->GetOffsetPosition();
		Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
		Matrix4x4 matViewProjectionViewport = viewProjection.matView * viewProjection.matProjection * matViewport;
		positionWorld = Transform(positionWorld, matViewProjectionViewport);
		lockOnMark_->SetPosition({positionWorld.x, positionWorld.y});
	}
	
}

void LockOn::Draw() { 
	if (target_) {
		//lockOnMark_->Draw();
	}
}

bool LockOn::isLockOnRange(const std::unique_ptr<Enemy>& enemy, const ViewProjection& viewProjection) { 
	Vector3 positionWorld = enemy->GetOffsetPosition();
	Vector3 positionView = Transform(positionWorld, viewProjection.matView);

	// 距離条件チェック
	if (minDIstance_ <= positionView.z && positionView.z <= maxDistance_) {
		float arcTangent = std::atan2(std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y), positionView.z);

		// 角度条件チェック
		if (std::sqrt(arcTangent * arcTangent) <= angleRange_) {
			// 範囲内
			return true;
		}
	}
	// 範囲外
	return false;
}

Vector3 LockOn::GetTargetPosition() const { 
	if (target_) {
		return target_->GetOffsetPosition();
	}
	return Vector3(); 
}
