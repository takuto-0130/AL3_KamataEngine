#pragma once
#include <Vector3.h>
#include "MyClass/CollisionConfig.h"

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

	// 衝突属性(自分)を取得
	const uint32_t GetCollisionAttribute() { return collisionAttribute_; }
	// 衝突属性(自分)を設定
	void SetCollisionAttribute(const uint32_t& collisionAttribute) { collisionAttribute_ = collisionAttribute; }
	// 衝突マスク(相手)を取得
	const uint32_t GetCollisionMask() { return CollisionMask_; }
	// 衝突マスク(相手)を設定
	void SetCollisionMask(const uint32_t& CollisionMask) { CollisionMask_ = CollisionMask; }

private:
	float radius_ = 0;

	uint32_t collisionAttribute_ = 0xffffffff;
	uint32_t CollisionMask_ = 0xffffffff;
};
