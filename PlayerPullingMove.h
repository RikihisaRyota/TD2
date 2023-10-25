#pragma once

#include "WorldTransform.h"

class Input;
class Player;
class PlayerPullingMove {
public:
	PlayerPullingMove();
	~PlayerPullingMove();
	void Initialize();
	void Update();

	void SetPlayer(Player* player) { player_ = player; }
	void SetAcceleration(const Vector3& acceleration) { acceleration_ = acceleration; }
	void Debug();
private:
	void MoveLimit();
	float kPowerMin_ = 0.33f;
	float kPowerMax_ = 0.3f;
	float kGravityMax_ = 0.067f;
	float kGravityMin_ = 0.060f;
	float kGravityLimitMin_ = 0.09f;
	float kGravityLimitMax_ = 0.084f;
	float kAngle_ = 60.0f;

	Player* player_;
	Input* input_;

	WorldTransform worldTransform_;
	Vector3 velocity_;
	Vector3 acceleration_;
	bool direction_;
	// プレイヤーの向き
	float playerCurrentRotate_;
	float playerNextRotate_;
};

