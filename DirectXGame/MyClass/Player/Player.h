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
#include <list>

const int kPlayerBulletInterval = 20;

/// <summary>
/// 自キャラ
/// </summary>
class Player {
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

	void OnCollision();

	Vector3 GetWorldPosition();

	Vector3 GetWorldPosition3DReticle();

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	const float GetRadius() { return radius_; }

	void SetParent(const WorldTransform* parent);

private:

	void SetReticlePosition(ViewProjection& viewProjection);

	void Move();

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
};
