#pragma once

#include <vector>

#include "Enemy.h"
#include "EnemyBulletManager.h"
#include "EnemyEditor.h"
#include "Player.h"
#include "ViewProjection.h"

class EnemyManager {
public:
	struct EnemyState {
		Vector3 position;
		uint32_t type;
	};
public:
	~EnemyManager();
	void Initialize(Model* model);
	void Initialize(const std::vector<Model*>& type0, const std::vector<Model*>& type1);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void Reset();
	void Create(const Vector3& position, uint32_t type);

	std::vector<Enemy*>& GetEnemies() { return enemies_; }

	void SetPlayer(Player* player) { player_ = player; }
	void SetViewProjection(ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
	void SetEnemyBulletManager(EnemyBulletManager* enemyBulletManager) { enemyBulletManager_ = enemyBulletManager; }
	
	void SetIsDebug(bool flg) { isDebug_ = flg; }
private:
	Model* model_;
	Input* input_;
	EnemyBulletManager* enemyBulletManager_;
	std::vector<Enemy*> enemies_;
	ViewProjection* viewProjection_;
	Player* player_;
	std::vector<Model*> models_type0_;
	std::vector<Model*> models_type1_;
	// デバック
	bool isDebug_;
};

