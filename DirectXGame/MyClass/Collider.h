#pragma once
#include <Vector3.h>

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
public:
	// 半径を取得
	const float GetRadius() { return radius_; }

	// 半径を設定
	const void SetRadius(const float& radius) { radius_ = radius; }

	virtual void OnCollision();

	virtual Vector3 GetWorldPosition() const = 0;

private:
	float radius_ = 0;
};
