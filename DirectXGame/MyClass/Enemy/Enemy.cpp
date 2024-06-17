#include "Enemy.h"
#include "./MyClass/math/mathFunc.h"
#include "./MyClass/math/operatorOverload.h"
#include "./MyClass/Player/Player.h"
#include "GameScene.h"


// 敵の移動の速さ
const float kEnemySpeed = -0.2f;

Enemy::~Enemy() {
	for (TimedCall* timeCall : timeCalls_) {
		delete timeCall;
	}
}

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

void Enemy::Update() {
	state_->Update();
	timeCalls_.remove_if([](TimedCall* timeCall) { 
		if (timeCall->IsFinished()) {
			delete timeCall;
			return true;
		}
		return false;
	});
	for (TimedCall* timeCall : timeCalls_) {
		timeCall->Update();
	}
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
	newBullet->SetPlayer(player_);

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

void Enemy::FireAndReset() {
	Fire();
	timeCalls_.push_back(new TimedCall(std::bind(&Enemy::FireAndReset, this), kBulletInterval));
}

void Enemy::TimerSet() { 
	timeCalls_.push_back(new TimedCall(std::bind(&Enemy::FireAndReset, this), kBulletInterval));
}

void Enemy::TimerClear() {
	timeCalls_.remove_if([](TimedCall* timeCall) {
		if (true) {
			delete timeCall;
			return true;
		}
		return false;
	});
}

void Enemy::ChangeState(std::unique_ptr<BaseEnemyState> state) { state_ = std::move(state); }

EnemyStateApproach::EnemyStateApproach(Enemy* enemy) : BaseEnemyState("State Approach", enemy) { enemy_->TimerSet(); }

void EnemyStateApproach::Update() {
	enemy_->MoveTranslate({0, 0, kEnemySpeed});
	if (enemy_->GetWorldPosition().z < 0.0f) {
		enemy_->ChangeState(std::make_unique<EnemyStateLeave>(enemy_));
	}
}

EnemyStateLeave::EnemyStateLeave(Enemy* enemy) : BaseEnemyState("State Leave", enemy) {}

void EnemyStateLeave::Update() { 
	enemy_->MoveTranslate({kEnemySpeed, 0, 0});
	enemy_->TimerClear();
}

BaseEnemyState::~BaseEnemyState() {}
