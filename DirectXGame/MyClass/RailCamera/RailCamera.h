#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"

/// <summary>
/// レールカメラ
/// </summary>
class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& worldPos, const Vector3& radian);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	
	void Rotate(const Vector3& rotate);

	/// <summary>
	/// ビュー行列取得
	/// </summary>
	ViewProjection& GetViewProjection() { return viewProjection_; }

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};
