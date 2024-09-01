#pragma once
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include <list>
#include "MyClass/Enemy/Enemy.h"

const float kDegreeToRadian = 120.0f;

class LockOn {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const std::unique_ptr<Enemy>& enemy, const ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	bool isLockOnRange(const std::unique_ptr<Enemy>& enemy, const ViewProjection& viewProjection);

	Vector3 GetTargetPosition() const;

	bool ExsitTarget() const { return target_ ? true : false; }

private:
	// ロックオンマーク用のスプライト
	std::unique_ptr<Sprite> lockOnMark_;
	uint32_t textureReticle;

	bool isTrigger = false;
	
	// ロックオン対象
	const Enemy* target_ = nullptr;
	
	float minDIstance_ = 5.0f;
	float maxDistance_ = 200.0f;
	float angleRange_ = 20.0f * kDegreeToRadian;
};
