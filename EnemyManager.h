#pragma once

#include <vector>

#include "Enemy.h"

class EnemyManager {
public:
	~EnemyManager();
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void Reset();
	void Create(const Vector3& position,uint32_t type);

	std::vector<Enemy*>& GetEnemies() { return enemies_; }
private:
	Model* model_;
	std::vector<Enemy*> enemies_;
};

