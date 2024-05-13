#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <assert.h>

class Enemy {
public:

	void Initialize(Model* model, uint32_t texHandle);

	void Update();

	void Draw(ViewProjection& viewProjection);

private:

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t texHandle_ = 0;
	Vector3 velocity_;
	bool isDead_ = false;

};
