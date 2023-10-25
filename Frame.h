#pragma once

#include <array>

#include "Model.h"

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
	void Initialize(std::vector<Model*>model,bool isInGame);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void SetPlayer(Player* player) { player_ = player; }
	void SetUvula(Uvula* uvula) { uvula_ = uvula; }
	void SetViewProjection(ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
private:
	void Debug();
	void UpdateMatrix();

	float width_;
	float height_;
	Player* player_;
	Uvula* uvula_;
	ViewProjection* viewProjection_;
	std::vector<Wall*> topWalls_;
	std::vector<Wall*> bottomWalls_;

	bool isInGame_;
};

