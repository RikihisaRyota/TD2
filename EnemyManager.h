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
	void Initialize(const std::vector<Model*>& type0, const std::vector<Model*>& type1, const std::vector<Model*>& type2);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void Reset();
	void Create(const Vector3& position, uint32_t type);

	std::vector<Enemy*>& GetEnemies() { return enemies_; }
	int GetShotTime() { return enemies_[0]->GetShotTime(); }
	uint32_t GetBulletShotCount() { return enemies_[0]->GetBulletShotCount(); }
	float GetMaxSize() { return enemies_[0]->GetMaxSize(); }
	float GetEaseSecond_Grow() { return enemies_[0]->GetEaseSecond_Grow(); }
	float GetEaseSecond_Shot() { return enemies_[0]->GetEaseSecond_Shot(); }
	float GetOnceUpSize() { return enemies_[0]->GetOnceUpSize(); }
	float GetInitialRadius() { return enemies_[0]->GetInitialRadius(); }
	float GetScaleUpValue_Shot() { return enemies_[0]->GetScaleUpValue_Shot(); }
	float GetRotateValue_Shot() { return enemies_[0]->GetRotateValue_Shot(); }

	void SetPlayer(Player* player) { player_ = player; }
	void SetViewProjection(ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
	void SetEnemyBulletManager(EnemyBulletManager* enemyBulletManager) { enemyBulletManager_ = enemyBulletManager; }
	
	void SetIsDebug(bool flg) { isDebug_ = flg; }
	void SetSpawn0(bool spawn0) { spawn0_ = spawn0; }
	void SetSpawn1(bool spawn1) { spawn1_ = spawn1; }
	void SetSpawn2(bool spawn2) { spawn2_ = spawn2; }

	void SetShotTime(int shotTime);
	void SetMaxSize(float maxSize);
	void SetEaseSecond_Grow(float easeSecond_Grow);
	void SetEaseSecond_Shot(float easeSecond_Shot);
    void SetOnceUpSize(float onceUpSize);
    void SetInitialRadius(float initialRadius);
	void SetBulletShotCount(uint32_t bulletShotCount);
	void SetScaleUpValue_Shot(float scaleUpValue_Shot);
	void SetRotateValue_Shot(float rotateValue_Shot);

	void SetHeight(float height);
	void SetWidth(float width);

private:
	Model* model_;
	Input* input_;
	EnemyBulletManager* enemyBulletManager_;
	std::vector<Enemy*> enemies_;
	ViewProjection* viewProjection_;
	Player* player_;
	std::vector<Model*> models_type0_;
	std::vector<Model*> models_type1_;
	std::vector<Model*> models_type2_;
	std::vector<EnemyState> splits_;

	float height_;
	float width_;
	// デバック
	bool isDebug_;
	// CSV選択
	bool spawn0_ = true;
	bool spawn1_ = false;
	bool spawn2_ = false;
};

