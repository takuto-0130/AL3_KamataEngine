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
#include <./MyClass/math/TimedCall.h>
#include "MyClass/Collider.h"

class Player;
class GameScene;

static const int kBulletInterval = 90;

class Enemy;

class BaseEnemyState {
public:
	BaseEnemyState(const std::string& name, Enemy* enemy) : name_(name), enemy_(enemy), bulletInterval_(10){};

	// デストラクタを仮想関数にしないと全部一緒に消えちゃう
	virtual ~BaseEnemyState();

	// 純粋仮想関数 ※派生クラスに実装を強制する
	virtual void Update() = 0;

	//virtual void DebugLog();

protected:
	std::string name_;
	Enemy* enemy_ = nullptr;
	int32_t bulletInterval_;
};

class EnemyStateApproach : public BaseEnemyState {
public:
	EnemyStateApproach(Enemy* enemy);
	//~EnemyStateApproach();
	void Update();/*

private:
	int32_t bulletInterval_ = 0;*/
};

class EnemyStateLeave : public BaseEnemyState {
public:
	EnemyStateLeave(Enemy* enemy);
	//~EnemyStateLeave();
	void Update();
};

class Enemy : public Collider {
public:

	~Enemy();

	void SetPlayer(Player* player) { player_ = player; }

	void Initialize(Model* model, uint32_t texHandle, Vector3 position);

	void ApproachInitialize();

	void Approach();

	void Leave();

	void Update();

	void Draw(ViewProjection& viewProjection);

	void Fire();

	void OnCollision() override;

	Vector3 GetWorldPosition() const override;

	void FireAndReset();

	void TimerSet();

	void TimerClear();

	void MoveTranslate(const Vector3& velocity) { worldTransform_.translation_ += velocity; }

	const float GetRadius() { return radius_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsDead() const { return isDead_; }

	void ChangeState(std::unique_ptr<BaseEnemyState> state);

private:

	GameScene* gameScene_ = nullptr;

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t texHandle_ = 0;
	Vector3 velocity_{};
	bool isDead_ = false;
	enum class Phase { 
		APPROACH,
		LEAVE 
	};
	Phase phase_ = Phase::APPROACH;
	int32_t bulletInterval_ = 0;
	Player* player_ = nullptr;
	const float radius_ = 1.0f;

	static void (Enemy::*spFuncTable[])();

	std::unique_ptr<BaseEnemyState> state_;
	std::list<TimedCall*> timeCalls_;
};
