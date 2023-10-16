#pragma once

#include "Collider.h"
#include "WorldTransform.h"
#include "Model.h"

class EnemyBullet :public Collider {
public:
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void Reset();

	bool GetIsAlive() { return isAlive_; }
private:
	// 当たり判定
	void OnCollision(uint32_t type, Sphere* sphere)override;
	void HitBoxInitialize() override;
	void HitBoxUpdate() override;
	void HitBoxDraw(const ViewProjection& viewProjection) override;

	Model* model_;
	WorldTransform worldTransform_;
	float radius_ = 1.0f;
	bool isAlive_;
};

