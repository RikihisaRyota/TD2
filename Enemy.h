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
	void Initialize(Model* model, const Vector3& position, uint32_t type);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void OBJtoOBB();

	void SetPosition(const Vector3& position);

	OBB GetObb() { return obb_; } 

	void SetFlag(bool flag) { isHit_ = flag; }
	bool GetFlag() { return isHit_; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t type_;

	OBB obb_;
	bool isHit_ = false;
};