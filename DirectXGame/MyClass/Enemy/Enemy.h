#pragma once
#include "MyClass/BaseCharacter/BaseCharacter.h"
class Enemy : public BaseCharacter {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデルデータ配列</param>
	void Initialize(const std::vector<Model*>& models) override;

	void InitializeArmGimmick();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) override;

	void UpdateArmGimmick();

private:
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	float floatingParamater_ = 0.0f;

	float kOffsetArmHeight = 0.25f;
};

enum EnemyParts { 
	kEnemyBody, 
	kEnemyL_arm, 
	kEnemyR_arm };
