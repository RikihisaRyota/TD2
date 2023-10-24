#pragma once

#include <array>

#include "Model.h"
#include "Random.h"

class EnemyManager;
class Player;
class Uvula;
class Frame {
public:
	struct Wall {
		Model* model_;
		WorldTransform worldTransform_;
		bool isAlive_;
	};

public:
	~Frame();
	void Initialize(std::vector<Model*>model);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void SetPlayer(Player* player) { player_ = player; }
	void SetUvula(Uvula* uvula) { uvula_ = uvula; }
	void SetViewProjection(ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
	void SetEnemyManager(EnemyManager* enemyManager) { enemyManager_ = enemyManager; }
private:
	void Debug();
	void UpdateMatrix();

	float kRockInterval_ = 18.0f;
	float kRockFrameDistance_ = 4.0f;

	float width_;
	float height_;
	Player* player_;
	Uvula* uvula_;
	EnemyManager* enemyManager_;
	ViewProjection* viewProjection_;
	std::vector<Wall*> topWalls_;
	std::vector<Wall*> bottomWalls_;
	Random::RandomNumberGenerator rnd;
};

