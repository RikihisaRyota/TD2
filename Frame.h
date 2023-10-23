#pragma once

#include <array>

#include "CubeRenderer.h"

class EnemyManager;
class Player;
class Uvula;
class Frame {
public:
	enum Wall {
		kFloor,
		kCeiling,
		kTop,
		kBottom,

		kCount,
	};
public:
	void Initialize();
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void SetPlayer(Player* player) { player_ = player; }
	void SetUvula(Uvula* uvula) { uvula_ = uvula; }
	void SetEnemyManager(EnemyManager* EnemyManager) {	enemyManager_ = EnemyManager; }
private:
	void Debug();
	void UpdateMatrix();

	float width_;
	float height_;
	Player* player_;
	Uvula* uvula_;
	EnemyManager* enemyManager_;
	std::array<std::unique_ptr<CubeRenderer>, Wall::kCount> walls_;
	std::array<WorldTransform, Wall::kCount> worldTransforms_;
};

