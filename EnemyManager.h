#pragma once

#include <vector>

#include "Enemy.h"
#include "EnemyBulletManager.h"

class EnemyManager {
public:
	struct EnemyState {
		Vector3 position;
		uint32_t type;
	};
public:
	~EnemyManager();
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void Reset();
	void Create(const Vector3& position, uint32_t type);

	std::vector<Enemy*>& GetEnemies() { return enemies_; }

	void SetEnemyBulletManager(EnemyBulletManager* enemyBulletManager) { enemyBulletManager_ = enemyBulletManager; }
private:
	Model* model_;
	Input* input_;
	EnemyBulletManager* enemyBulletManager_;
	std::vector<Enemy*> enemies_;
	std::vector<EnemyState> addEnemyStates_;
};

