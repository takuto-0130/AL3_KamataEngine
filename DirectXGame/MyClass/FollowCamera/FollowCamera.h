#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <list>
#include "MyClass/Player/Player.h"

class LockOn;

class FollowCamera {
public:
	void Initialize();

	void Reset();

	void Update();

	void SetTarget(const WorldTransform* target);

	const Vector3 Offset();

	/// <summary>
	/// ビュー行列取得
	/// </summary>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	void SetLockOn(const LockOn& lockOn) { lockOn_ = &lockOn; }

	void SetPlayer(Player& player) { player_ = &player; }

private:
	ViewProjection viewProjection_;
	const WorldTransform* target_ = nullptr;
	const WorldTransform* cameraTarget_ = nullptr;
	Vector3 interTarget_{};
	float destinationAngleY_ = 0;
	Matrix4x4 rotateMatrix_{};

	const LockOn* lockOn_ = nullptr;

	Player* player_ = nullptr;
};
