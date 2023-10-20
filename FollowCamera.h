#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"

class Player;
class FollowCamera {
public:
	void Initialize();

	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }

	void SetPlayer(Player* player) { player_ = player; }

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	void Debug();
private:
	Player* player_;
	ViewProjection viewProjection_;
	const WorldTransform* target_ = nullptr;
	// 追従対象の残像
	Vector3 interTarget_ = {};
	// デバック用
	Vector3 nowOffset_;
	Vector3 dropOffset_;
	Vector3 debugOffset_;
	Vector3 bossAttackOffset_;
	uint32_t bossCamaraCount_ = 0;
	float dropDelayInterpolationLate_;
	float delayInterpolationLate_;
};