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

	void OBJtoOBB(); // WorldTransformをOBBへ変換

	void SetPosition(const Vector3& position);

	OBB GetObb() { return obb_; } // OBBの取得用

	int SetFlag(int flag) { return isHit_ = flag; } // フラグセッタ（ImGui用にintです）

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t type_;

	OBB obb_; // 当たり判定用
	int isHit_ = false;
};