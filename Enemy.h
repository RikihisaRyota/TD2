#pragma once

#include "Collider.h"
#include "Model.h"
#include "WorldTransform.h"

class Enemy :public Collider{
public:
	void Initialize(Model* model, const Vector3& position, uint32_t type);
	void Update();
	void Draw(const ViewProjection& viewProjection);
private:
	// 当たり判定
	// 衝突したら呼び出される処理
	void OnCollision(uint32_t type, Sphere* sphere)override;
	void HitBoxInitialize() override;
	void HitBoxUpdate() override;
	void HitBoxDraw(const ViewProjection& viewProjection) override;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t type_;
	float radius_ = 1.0f;
};