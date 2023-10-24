#pragma once

#include "WorldTransform.h"

class Audio;
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
	void SetMoveSoundHandle(size_t handle) { moveSoundHandle_ = handle; }
	void Debug();
private:
	void MoveLimit();
	float kPowerMin_ = 0.0f;
	float kPowerMax_ = 0.3f;
	float kInertia_ = 0.98f;
	float kAngle_ = 60.0f;

	float kLegAngle_ = 35.0f;

	Player* player_;
	Input* input_;
	Audio* audio_;

	WorldTransform worldTransform_;
	Vector3 acceleration_;
	Vector3 velocity_;
	bool direction_;
	// ボスに食べられたとき
	bool isEating_;
	float rotateVelocity_;
	// プレイヤーの体の向き
	float playerCurrentRotate_;
	float playerNextRotate_;
	// プレイヤーの脚の向き
	bool isSwell_;
	uint32_t swellCount_;
	float playerCurrentLegLeftRotate_;
	float playerCurrentLegRightRotate_;
	// サウンドハンドル
	size_t moveSoundHandle_;
};

