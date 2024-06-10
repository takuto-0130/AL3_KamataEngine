#include "Enemy.h"
#include "./MyClass/math/mathFunc.h"
#include "./MyClass/math/operatorOverload.h"
#include "./MyClass/Player/Player.h"
#include "GameScene.h"


// 敵の移動の速さ
const float kEnemySpeed = -0.2f;

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
	ChangeState(std::make_unique<EnemyStateApproach>(this));
}

//void Enemy::Approach() {
//	velocity_ = {0, 0, kEnemySpeed};
//	worldTransform_.translation_ += velocity_;
//
//	bulletInterval_--;
//	if (bulletInterval_ == 0) {
//		Fire();
//		bulletInterval_ = kBulletInterval;
//	}
//
//	if (worldTransform_.translation_.z < 0.0f) {
//		phase_ = Phase::LEAVE;
//	}
//}
//
//void Enemy::Leave() {
//	velocity_ = {kEnemySpeed, 0, 0};
//	worldTransform_.translation_ += velocity_;
//}

//void (Enemy::*Enemy::spFuncTable[])() = {
//	&Enemy::Approach, 
//	&Enemy::Leave
//};

void Enemy::Update() {
	//(this->*spFuncTable[static_cast<size_t>(phase_)])();
	state_->Update();
	/*Fire();
	bulletInterval_--;
	if (bulletInterval_ == 0) {
		Fire();
		bulletInterval_ = kBulletInterval;
	}*/
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, texHandle_); 
}

void Enemy::Fire() {
	//assert(player_);
	const float kBulletSpeed = 1.0f;
	Vector3 velocity{};
	if (player_ == nullptr) {
		Vector3 enemyToPlayer = {0, 0, -1};
		Vector3 nomalize = Normalize(enemyToPlayer);
		velocity = nomalize * kBulletSpeed;
	} else {
		Vector3 playerPos = player_->GetWorldPosition();
		Vector3 enemyPos = GetWorldPosition();
		Vector3 enemyToPlayer = playerPos - enemyPos;
		Vector3 nomalize = Normalize(enemyToPlayer);
		velocity = nomalize * kBulletSpeed;
	}

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);


	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::OnCollision() { 
	isDead_ = true;
}

const Vector3 Enemy::GetWorldPosition() { 
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Enemy::ChangeState(std::unique_ptr<BaseEnemyState> state) { state_ = std::move(state); }

EnemyStateApproach::EnemyStateApproach(Enemy* enemy) : BaseEnemyState("State Approach", enemy) {}

void EnemyStateApproach::Update() {
	enemy_->MoveTranslate({0, 0, kEnemySpeed});
	if (enemy_->GetWorldPosition().z < 0.0f) {
		enemy_->ChangeState(std::make_unique<EnemyStateLeave>(enemy_));
	}
	bulletInterval_--;
	if (bulletInterval_ == 0) {
		enemy_->Fire();
		bulletInterval_ = kBulletInterval;
	}
}

EnemyStateLeave::EnemyStateLeave(Enemy* enemy) : BaseEnemyState("State Leave", enemy) {}

void EnemyStateLeave::Update() { 
	enemy_->MoveTranslate({kEnemySpeed, 0, 0});
}

BaseEnemyState::~BaseEnemyState() {}
