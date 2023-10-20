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
	void MoveLimit();
	float kPower_ = 0.002f;
	float kInertia_ = 0.98f;
	float kAngle_ = 60.0f;

	Player* player_;
	Input* input_;

	WorldTransform worldTransform_;
	Vector3 acceleration_;
	Vector3 velocity_;

	bool direction_;
};

