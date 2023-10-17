#pragma once

#include "WorldTransform.h"

class Input;
class Player;
class PlayerJump {
public:
	PlayerJump();
	~PlayerJump();
	void Initialize(const Vector3& outPutVector);
	void Update();

	void SetPlayer(Player* player) { player_ = player; }
	void Debug();
private:
	float kSpeed_ = 2.0f;

	Player* player_;
	Input* input_;

	WorldTransform worldTransform_;
	Vector3 outPutVector_;
};

