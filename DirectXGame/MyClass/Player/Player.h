#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <assert.h>

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(std::vector<std::unique_ptr<Model>>& models, uint32_t texHandle);

	void InitializeFloatingGimmick();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	void UpdateFlotingGimmick();

	const WorldTransform& GetWorldTransform() { return worldTransformBase_; }

	void SetViewProjection(const ViewProjection* viewProjection) { 
		viewProjection_ = viewProjection;
	}

private:
	std::vector<std::unique_ptr<Model>> models_;
	uint32_t texHandle_ = 0;
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	float floatingParamater_ = 0.0f;
	const float kCharacterSpeed_ = 0.5f;

	const ViewProjection* viewProjection_ = nullptr;
};

enum PlayerParts {
	Body,
	Head,
	L_arm,
	R_arm
};
