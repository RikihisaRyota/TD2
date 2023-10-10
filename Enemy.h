#pragma once

#include "Collision.h"
#include "Model.h"
#include "Player.h"
#include "WorldTransform.h"
#include "MyMath.h"
#include "OBB.h"
#include <functional>

class Enemy {
public:
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void SetPosition(const Vector3& position);

	OBB GetObb() { return obb_; }
	int SetFlag(int flag) { return isHit_ = flag; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;

	OBB obb_;
	int isHit_ = false;
};