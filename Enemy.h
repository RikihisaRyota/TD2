#pragma once

#include "Collision.h"
#include "Model.h"
#include "Player.h"
#include "WorldTransform.h"
#include "OBB.h"
#include <functional>

class Enemy {
public:
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void SetPosition(const Vector3& position) { worldTransform_.translation_.x = position.x; worldTransform_.translation_.y = position.y + 1; worldTransform_.translation_.z = position.z; }

	OBB GetObb() { return obb_; }
	void SetFlag(int flag) { isCombined_ = flag; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;

	OBB obb_;
	int isCombined_ = false;
};