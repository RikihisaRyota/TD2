#pragma once

#include "WorldTransform.h"
#include "ViewProjection.h"

class Input;
class Player;
class PlayerStun {
public:
	PlayerStun();
	~PlayerStun();

	void Initialize();
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void SetPlayer(Player* player) { player_ = player; }
	void Debug();
private:
	uint32_t kStunMax_ = 30;
	uint32_t kRotationCountMax_ = 1;

	Player* player_;
	Input* input_;

	WorldTransform worldTransform_;
	uint32_t stunCount_;
};

