#pragma once
#include <list>
#include <MyClass/Collider.h>
#include <MyClass/Player/Player.h>
#include <MyClass/Enemy/Enemy.h>
#include <MyClass/Enemy/EnemyBullet.h>

class ColliderManager {
public:
	ColliderManager();
	~ColliderManager();

	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="enemys">敵リスト</param>
	/// <param name="enemyBullets">敵弾リスト</param>
	void Update(Player* player, Enemy* enemy, const std::list<EnemyBullet*> enemyBullets);

private:

	void ColliderListRegistration(Player* player,Enemy* enemy, const std::list<EnemyBullet*> enemyBullets);

	void ColliderListClear();

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckAllCollisionPair(Collider* colliderA, Collider* colliderB);

	void CheckAllCollisions();

private:
	std::list<Collider*> colliders_;
};
