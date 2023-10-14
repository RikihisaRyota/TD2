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
	void SetAcceleration(const Vector3& acceleration) { acceleration_ = acceleration; }
	void Debug();
private:
	float kSpeed_ = 0.4f;
	float kGravity_ = 0.1f;
	float kGravityMax_ = -1.5f;
	
	
	Player* player_;
	Input* input_;

	WorldTransform worldTransform_;
	Vector3 velocity_;
	Vector3 acceleration_;
};

