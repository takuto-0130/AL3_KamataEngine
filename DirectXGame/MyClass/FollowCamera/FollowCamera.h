#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
class FollowCamera {
public:
	void Initialize();

	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }

	/// <summary>
	/// ビュー行列取得
	/// </summary>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

private:
	ViewProjection viewProjection_;
	const WorldTransform* target_ = nullptr;
};
