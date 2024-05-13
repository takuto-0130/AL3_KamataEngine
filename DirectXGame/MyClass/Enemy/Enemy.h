#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <assert.h>
#include <./MyClass/Enemy/EnemyBullet.h>

class Player;

static const int kBulletInterval = 60;

class Enemy {
public:

	~Enemy();

	void SetPlayer(Player* player) { player_ = player; }

	void Initialize(Model* model, uint32_t texHandle);

	void ApproachInitialize();

	void Approach();

	void Leave();

	void Update();

	void Draw(ViewProjection& viewProjection);

	void Fire();

	Vector3 GetWorldPosition();

private:

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t texHandle_ = 0;
	Vector3 velocity_{};
	bool isDead_ = false;
	enum Phase { 
		APPROACH,
		LEAVE 
	};
	int32_t phase_ = APPROACH;
	std::list<EnemyBullet*> bullets_;
	int32_t bulletInterval_ = 0;
	Player* player_ = nullptr;
};
