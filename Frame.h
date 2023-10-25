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
	void SetEnemyManager(EnemyManager* enemyManager) {	enemyManager_ = enemyManager; }
	void SetUvula(Uvula* uvula) { uvula_ = uvula; }
	void SetViewProjection(ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
private:
	void Debug();
	void UpdateMatrix();

	float kRockInterval_ = 30.0f;
	float kRockFrameDistance_ = 14.0f;
	float kRockScale_ = 4.0f;

	float kRock_X = 20.0f;
	float kRock_Y = 30.0f;

	float width_;
	float height_;
	Player* player_;
	EnemyManager* enemyManager_;
	Uvula* uvula_;
	ViewProjection* viewProjection_;
	std::vector<Wall*> topWalls_;
	std::vector<Wall*> bottomWalls_;
	std::vector<Wall*> rightWalls_;
	Random::RandomNumberGenerator rnd;
};

