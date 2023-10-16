#pragma once

#include <vector>

#include "EnemyBullet.h"

class EnemyBulletManager {
public:
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void Reset();
	std::vector<EnemyBullet*>& GetEnemyBullets() {return enemyBullets_;}
private:
	Model* model_;
	std::vector<EnemyBullet*> enemyBullets_;
};

