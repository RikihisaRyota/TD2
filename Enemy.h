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

	void OBJtoOBB(); // WorldTransform��OBB�֕ϊ�

	void SetPosition(const Vector3& position);

	OBB GetObb() { return obb_; } // OBB�̎擾�p

	int SetFlag(int flag) { return isHit_ = flag; } // �t���O�Z�b�^�iImGui�p��int�ł��j

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t type_;

	OBB obb_; // �����蔻��p
	int isHit_ = false;
};