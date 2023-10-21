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
	void SetIsEating(bool flg) { isEating_ = flg; }
	void SetRotateVelocity(float velocity) { rotateVelocity_ = velocity; }
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
	float playerCurrentRotate_;
	float playerNextRotate_;
	bool direction_;
	bool isEating_;
	float rotateVelocity_;
};

