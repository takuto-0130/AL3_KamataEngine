#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <assert.h>
#include "PlayerBullet.h"
#include "MyClass/Collider.h"
#include <list>

const int kPlayerBulletInterval = 20;

class Enemy;

/// <summary>
/// 自キャラ
/// </summary>
class Player : public Collider{
public:

	/// <summary>
	/// コンストクラタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t texHandle, const Vector3& pos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(ViewProjection& viewProjection);

	/// <summary>
	/// 回転
	/// </summary>
	void Rotate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	void OnCollision() override;

	Vector3 GetWorldPosition() const override;

	const Vector3 GetWorldPosition3DReticle();

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	const float GetRadius() { return radius_; }

	const Vector3 GetRotate() { return worldTransform_.rotation_; }

	void SetParent(const WorldTransform* parent);

	void SetEnemy(const std::list<Enemy*> enemys) { enemys_ = enemys; }

	void SingleLockOn(ViewProjection& viewProjection);

	ViewProjection& GetViewProjection() { return viewProjection_; }

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:

	void SetReticlePosition(ViewProjection& viewProjection);

	void Move();

	void ReticleLimit();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t texHandle_ = 0;
	Input* input_ = nullptr;
	std::list<PlayerBullet*> bullets_;
	const float radius_ = 1.0f;
	int32_t bulletInterval_ = 0;

	WorldTransform worldTransform3DReticle_;
	Sprite* sprite2DReticle_ = nullptr;

	std::list<Enemy*> enemys_;

	bool isLockOn_ = false;
	Vector3 LockOnPos_{};
	ViewProjection viewProjection_;
};
