#pragma once

#include <vector>

#include "Collider.h"
#include "EnemyManager.h"
#include "EnemyBulletManager.h"
#include "Player.h"
#include "PlayerBulletManager.h"
#include "Uvula.h"

class CollisionManager {
public:
	enum class Type {
		kPlayerVSEnemy,
		kPlayerBulletVSEnemy,
		kPlayerBulletVSEnemyBullet,
		kPlayerVSEnemyBullet,
		kEnemyVSEnemyBullet,
		kPlayerVSBoss,

		kCount,
	};
	void Update(Player* player, PlayerBulletManager* playerBullet, EnemyManager* enemyManager, EnemyBulletManager* EnemyBulletManager, Uvula* uvula);
private:
	// メンバ関数
	void CheckAllCollisions(Player* player, PlayerBulletManager* playerBullet,EnemyManager* enemyManager, EnemyBulletManager* EnemyBulletManager,Uvula* uvula);
	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
	// コライダー
	std::vector<Collider*> colliders_;
};

