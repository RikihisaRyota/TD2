#include "CollisionManager.h"

#include "Collision.h"


void CollisionManager::Update(Player* player, Uvula* uvula) {
	colliders_.clear();
	CheckAllCollisions(player,uvula);
}

void CollisionManager::CheckAllCollisions(Player* player, Uvula* uvula) {
	// プレイヤーをリストに追加
	colliders_.emplace_back(player);
	// 敵をリストに追加
	colliders_.emplace_back(uvula);

	// リスト内総当たり
	std::vector<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーAを取得する
		Collider* colliderA = *itrA;
		// イテレータBはイテレータAから回す
		std::vector<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;
			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	uint32_t collisionAttributeA = colliderA->GetCollisionAttribute();
	uint32_t collisionAttributeB = colliderB->GetCollisionAttribute();
	//// 衝突フィルタリング
	//if ((collisionAttributeA & kCollisionAttributeEnemy &&
	//	collisionAttributeB & kCollisionAttributeEnemyAttack) ||
	//	(collisionAttributeB & kCollisionAttributeEnemy &&
	//		collisionAttributeA & kCollisionAttributeEnemyAttack) ||
	//	(collisionAttributeA & kCollisionAttributePlayer &&
	//		collisionAttributeB & kCollisionAttributePlayerAttack) ||
	//	(collisionAttributeB & kCollisionAttributePlayer &&
	//		collisionAttributeA & kCollisionAttributePlayerAttack)) {
	//	return;
	//}
	if (IsCollision(*colliderA->GetSphere(), *colliderB->GetSphere())) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}
