#pragma once

#include <array>

#include "CubeRenderer.h"

class Player;
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
private:
	void Debug();
	void UpdateMatrix();
	float width_;
	float height_;
	Player* player_;

	std::array<std::unique_ptr<CubeRenderer>, Wall::kCount> walls_;
	std::array<WorldTransform, Wall::kCount> worldTransforms_;
};

