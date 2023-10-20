#include "CollisionManager.h"

#include "Collision.h"


void CollisionManager::Update(Player* player, PlayerBulletManager* playerBullet, EnemyManager* enemyManager, EnemyBulletManager* EnemyBulletManager, Uvula* uvula) {
	colliders_.clear();
	CheckAllCollisions(player, playerBullet, enemyManager, EnemyBulletManager, uvula);
}

void CollisionManager::CheckAllCollisions(Player* player, PlayerBulletManager* playerBullet, EnemyManager* enemyManager, EnemyBulletManager* EnemyBulletManager, Uvula* uvula) {
	// プレイヤーをリストに追加
	colliders_.emplace_back(player);
	// プレイヤーの弾
	for (auto& bullet : playerBullet->GetPlayerBullets()) {
		colliders_.emplace_back(bullet);
	}
	// 敵をリストに追加
	for (auto& enemy : enemyManager->GetEnemies()) {
		if (enemy->GetIsAlive()) {
			colliders_.emplace_back(enemy);
		}
	}
	// 敵の弾
	for (auto& bullet : EnemyBulletManager->GetEnemyBullets()) {
		colliders_.emplace_back(bullet);
	}
	// ボスのベロ
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
	// 衝突フィルタリング
	if ((collisionAttributeA & kCollisionAttributePlayer &&
		collisionAttributeB & kCollisionAttributePlayerBullet) ||
		(collisionAttributeB & kCollisionAttributePlayer &&
			collisionAttributeA & kCollisionAttributePlayerBullet)
		) {
		return;
	}
	Sphere* a = colliderA->GetSphere();
	Sphere* b = colliderB->GetSphere();
	if (IsCollision(*a, *b)) {
		// プレイヤー対敵
		if (collisionAttributeA & kCollisionAttributePlayer &&
			collisionAttributeB & kCollisionAttributeEnemy) {
			colliderA->OnCollision(static_cast<uint32_t>(Type::kPlayerVSEnemy), b);
			colliderB->OnCollision(static_cast<uint32_t>(Type::kPlayerVSEnemy), a);
		}
		// プレイヤー対敵の弾
		else if (collisionAttributeA & kCollisionAttributePlayer &&
			collisionAttributeB & kCollisionAttributeEnemyBullet) {
			colliderA->OnCollision(static_cast<uint32_t>(Type::kPlayerVSEnemyBullet), b);
			colliderB->OnCollision(static_cast<uint32_t>(Type::kPlayerVSEnemyBullet), a);
		}
		// プレイヤー対ボス
		else if (collisionAttributeA & kCollisionAttributePlayer &&
			collisionAttributeB & kCollisionAttributeBoss) {
			colliderA->OnCollision(static_cast<uint32_t>(Type::kPlayerVSBoss), b);
			colliderB->OnCollision(static_cast<uint32_t>(Type::kPlayerVSBoss), a);
		}
		// プレイヤーの弾対敵
		else if (collisionAttributeA & kCollisionAttributePlayerBullet &&
			collisionAttributeB & kCollisionAttributeEnemy) {
			colliderA->OnCollision(static_cast<uint32_t>(Type::kPlayerBulletVSEnemy), b);
			colliderB->OnCollision(static_cast<uint32_t>(Type::kPlayerBulletVSEnemy), a);
		}// プレイヤーの弾対敵の弾
		else if (collisionAttributeA & kCollisionAttributePlayerBullet &&
			collisionAttributeB & kCollisionAttributeEnemyBullet) {
			colliderA->OnCollision(static_cast<uint32_t>(Type::kPlayerBulletVSEnemyBullet), b);
			colliderB->OnCollision(static_cast<uint32_t>(Type::kPlayerBulletVSEnemyBullet), a);
		}
		// 敵対敵
		else if (collisionAttributeA & kCollisionAttributeEnemy &&
			collisionAttributeB & kCollisionAttributeEnemy) {
			colliderA->OnCollision(static_cast<uint32_t>(Type::kEnemyVSEnemy), b);
			colliderB->OnCollision(static_cast<uint32_t>(Type::kEnemyVSEnemy), a);
		}
		// 敵対敵の弾
		else if(collisionAttributeA & kCollisionAttributeEnemy &&
			collisionAttributeB & kCollisionAttributeEnemyBullet) {
			colliderA->OnCollision(static_cast<uint32_t>(Type::kEnemyVSEnemyBullet), b);
			colliderB->OnCollision(static_cast<uint32_t>(Type::kEnemyVSEnemyBullet), a);
		}
	}
}
