#pragma once

#include <vector>

#include "EnemyBullet.h"
#include "ViewProjection.h"

class EnemyBulletManager {
public:
	~EnemyBulletManager();
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void CreateBullet(const Vector3& position, const Vector3& scale);
	void Reset();
	std::vector<EnemyBullet*>& GetEnemyBullets() { return enemyBullets_; }
	//void SetViewProjection(ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
private:
	Model* model_;
	std::vector<EnemyBullet*> enemyBullets_;
};

