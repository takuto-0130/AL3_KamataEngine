#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
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

private:
	ViewProjection viewProjection_;
	const WorldTransform* target_ = nullptr;
	Vector3 interTarget_{};
	float destinationAngleY_ = 0;
	Matrix4x4 rotateMatrix_;
};
