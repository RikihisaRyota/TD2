#pragma once

#include <vector>

#include "CubeRenderer.h"

class Player;
class Frame {
public:
	void Initialize();
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void SetPlayer(Player* player) { player_ = player; }
private:
	void Debug();
	void UpdateMatrix();
	float width_;
	float height_;
	Player* player_;
	std::unique_ptr<CubeRenderer> floor_;
	WorldTransform floorWorldTransform_;
	std::unique_ptr<CubeRenderer> rightWall_;
	WorldTransform rightWallWorldTransform_;
	std::unique_ptr<CubeRenderer> leftWall_;
	WorldTransform leftWallWorldTransform_;
};

