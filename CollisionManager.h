#pragma once

#include <vector>

#include "Collider.h"
#include "Player.h"
#include "Uvula.h"

class CollisionManager {
public:
	void Update(Player* player, Uvula* uvula);
private:
	// メンバ関数
	void CheckAllCollisions(Player* player, Uvula* uvula);
	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
	// コライダー
	std::vector<Collider*> colliders_;
};

