#pragma once

#include "WorldTransform.h"

class Input;
class Player;
class PlayerMove {
public:
	PlayerMove();
	~PlayerMove();

	void Initialize();
	void Update();

	void SetPlayer(Player* player) { player_ = player; }
	void Debug();
private:
	float kPower_ = 0.5f;
	float kInertia_ = 0.95f;

	Player* player_;
	Input* input_;

	WorldTransform worldTransform_;
	Vector3 acceleration_;
	Vector3 velocity_;

	bool direction_;
};

