#pragma once
#include "MyClass/BaseCharacter/BaseCharacter.h"
#include "Input.h"
#include "Sprite.h"
#include <assert.h>
#include <optional>

/// <summary>
/// 自キャラ
/// </summary>
class Player : public BaseCharacter {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデルデータ配列</param>
	void Initialize(const std::vector<Model*>& models) override;

	void InitializeFloatingGimmick();

	void InitializeAttackGimmick();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	void BehaviorRootUpdate();

	void BehaviorAttackUpdate();

	void Move();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) override;

	void UpdateFlotingGimmick();

	void SetViewProjection(const ViewProjection* viewProjection) { 
		viewProjection_ = viewProjection;
	}

private:
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformHammer_;

	float floatingParamater_ = 0.0f;
	const float kCharacterSpeed_ = 0.5f;
	float attackParamater_ = 0.0f;

	const ViewProjection* viewProjection_ = nullptr;
	float rotateY = 0;
	Input* input_ = nullptr;


	
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


	enum class Behavior {
		kRoot,
		kAttack
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
