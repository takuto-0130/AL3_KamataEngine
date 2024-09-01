#pragma once
#include "MyClass/BaseCharacter/BaseCharacter.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include <assert.h>
#include <optional>
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "PlayerBullet.h"
#include <list>

const int kPlayerMaxHP = 15;

struct WorkDash {
	uint32_t dashParamater_ = 0;
	int dashFrame = 13;
	int dashCurrentTime_ = 0;
	float dashLength_ = 15.0f;
	Vector3 goalVector_{};
};
class LockOn;
const int kPlayerBulletInterval = 12;

const float kDashGaugeUp = 0.8f;
const float kDashGaugeMax = 480.0f;
const float kDashDecrease = 160.0f;

const float kLimitRange = 80.0f;

/// <summary>
/// 自キャラ
/// </summary>
class Player : public BaseCharacter {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデルデータ配列</param>
	void Initialize(const std::vector<Model*>& models) override;

	void Reset();

	void InitializeFloatingGimmick();

	void BehaviorAttackInitialize();

	void BehaviorDashInitialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	void BehaviorRootUpdate();

	void BehaviorAttackUpdate();

	void BehaviorDashUpdate();

	void BehaviorJumpUpdate();

	void Move();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) override;

	void DrawUI();

	void UpdateFlotingGimmick();

	void SetViewProjection(const ViewProjection* viewProjection) { 
		viewProjection_ = viewProjection;
	}

	void SetRotare(const float& rotate) { worldTransform_.rotation_.y = rotate; }

	void ApplyGlobalVariables();

	void Attack();

	void PlayerUpdateMatrix();

	Vector3 GetWorldPosition() const;

	Vector3 GetHeadWorldPosition() const;

	const Vector3 GetWorldPosition3DReticle();

	void SetLockOn(const LockOn& lockOn) { lockOn_ = &lockOn; }

	void MoveLimit();

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	void OnCollision() override;

	bool IsDead() const { return isDead_; }

private:
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformHammer_;

	float floatingParamater_ = 0.0f;
	const float kCharacterSpeed_ = 0.2f;
	float attackParamater_ = 0.0f;

	const ViewProjection* viewProjection_ = nullptr;
	float rotateY = 0;
	Input* input_ = nullptr;

	std::unique_ptr<Model> model_;
	std::list<PlayerBullet*> bullets_;
	int32_t bulletInterval_ = 0;

	WorldTransform worldTransform3DReticle_;
	std::unique_ptr<Sprite> sprite2DReticle_;
	uint32_t textureReticle;

	std::unique_ptr<Sprite> spriteEdge_;
	uint32_t textureEdge;

	std::unique_ptr<Sprite> spriteGauge_;
	uint32_t textureGauge;

	std::unique_ptr<Sprite> spriteCoolDown_;
	uint32_t textureCoolDown;

	bool isLockOn_ = false;

	float dashGauge = 0;

	const LockOn* lockOn_ = nullptr;

	const float radius_ = 1.0f;

	int HP_;

	bool isDead_ = false;

#pragma region

	int furiageWaite_ = 0;
	int attackCurrent_ = 0;
	int attackWait_ = 0;
	int attackAfterWait_ = 0;


	int furiageTime_ = 12;
	int attackWaitTime_ = 20;
	int attackTime_ = 10;
	int attackAfterWaitTime_ = 20;
#pragma endregion // 攻撃モーション用

	WorkDash workDash_;

	enum class Behavior {
		kRoot,		// !< 通常状態
		kAttack,	// !< 攻撃中
		kDash,		// !< ダッシュ中
		kJump,		// !< ジャンプ中
	};
	Behavior behavior_ = Behavior::kRoot;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
};

enum PlayerParts {
	kBody,
	kHead,
	kL_arm,
	kR_arm,
	kHammer
};
