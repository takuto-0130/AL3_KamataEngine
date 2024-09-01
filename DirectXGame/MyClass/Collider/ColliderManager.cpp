#include "ColliderManager.h"
#include "MyClass/math/mathFunc.h"
#include "MyClass/math/operatorOverload.h"

ColliderManager::ColliderManager() {}
ColliderManager::~ColliderManager() { 
	ColliderListClear();
}
void ColliderManager::Update(Player* player, Enemy* enemy, const std::list<EnemyBullet*> enemyBullets) { 
	ColliderListClear();
	ColliderListRegistration(player, enemy, enemyBullets);
	CheckAllCollisions();
}

void ColliderManager::ColliderListRegistration(Player* player, Enemy* enemy, const std::list<EnemyBullet*> enemyBullets) {
	const std::list<PlayerBullet*>& playerBullets = player->GetBullets();

	colliders_.push_back(player);
	//for (Enemy* enemy : enemys) {
		colliders_.push_back(enemy);
	//}
	for (EnemyBullet* enemtBullet : enemyBullets) {
		colliders_.push_back(enemtBullet);
	}
	for (PlayerBullet* playerBullet : playerBullets) {
		colliders_.push_back(playerBullet);
	}

}

void ColliderManager::ColliderListClear() { 
	colliders_.clear();
}

void ColliderManager::CheckAllCollisionPair(Collider* colliderA, Collider* colliderB) {
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) || (colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask())) {
		return;
	}
	Vector3 posA, posB;
	posA = colliderA->GetWorldPosition();
	posB = colliderB->GetWorldPosition();
	if (Length(posB - posA) <= colliderA->GetRadius() + colliderB->GetRadius()) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}

void ColliderManager::CheckAllCollisions() {
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;
			CheckAllCollisionPair(colliderA, colliderB);
		}
	}
}
