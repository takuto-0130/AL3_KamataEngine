#include "Enemy.h"
#include "./MyClass/math/mathFunc.h"
#include "./MyClass/math/operatorOverload.h"
#include "./MyClass/Player/Player.h"
#include "GameScene.h"


// 敵の移動の速さ
const float kEnemySpeed = -0.005f;

Enemy::~Enemy() {}

void Enemy::ApproachInitialize() { bulletInterval_ = kBulletInterval; }

void Enemy::Initialize(Model* model, uint32_t texHandle, Vector3 position) {
	assert(model);
	ApproachInitialize();
	model_ = model;
	texHandle_ = texHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = {0, 0, kEnemySpeed};
	bulletInterval_ = kBulletInterval;
}

void Enemy::Approach() {
	velocity_ = {0, 0, kEnemySpeed};
	worldTransform_.translation_ += velocity_;

	bulletInterval_--;
	if (bulletInterval_ == 0) {
		Fire();
		bulletInterval_ = kBulletInterval;
	}

	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = LEAVE;
	}
}

void Enemy::Leave() {
	velocity_ = {kEnemySpeed, 0, 0};
	worldTransform_.translation_ += velocity_;
}

void Enemy::Update() {
	switch (phase_) { 
	case APPROACH:
	default:
		Approach();
		break;
	case LEAVE:
		Leave();
		break;
	}
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, texHandle_); 
}

void Enemy::Fire() {
	assert(player_);
	const float kBulletSpeed = 1.0f;
	Vector3 playerPos = player_->GetWorldPosition();
	Vector3 enemyPos = GetWorldPosition();
	Vector3 enemyToPlayer = playerPos - enemyPos;
	Vector3 nomalize = Normalize(enemyToPlayer);
	Vector3 velocity = nomalize * kBulletSpeed;

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);


	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::OnCollision() { 
	isDead_ = true;
}

Vector3 Enemy::GetWorldPosition() { 
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
