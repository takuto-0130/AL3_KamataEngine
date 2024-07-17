#pragma once
#include "MyClass/BaseCharacter/BaseCharacter.h"
#include "Input.h"
#include "Sprite.h"
#include <assert.h>

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

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

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

	float floatingParamater_ = 0.0f;
	const float kCharacterSpeed_ = 0.5f;

	const ViewProjection* viewProjection_ = nullptr;
	float rotateY = 0;
};

enum PlayerParts {
	kBody,
	kHead,
	kL_arm,
	kR_arm
};
