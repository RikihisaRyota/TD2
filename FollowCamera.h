#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"

class FollowCamera {
public:
	void Initialize();

	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	void Debug();
private:
	ViewProjection viewProjection_;
	const WorldTransform* target_ = nullptr;
	// 追従対象の残像
	Vector3 interTarget_ = {};
	// デバック用
	Vector3 debugOffset_;
	float delayInterpolationLate_;
};