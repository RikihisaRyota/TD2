#pragma once

#include "Collider.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"

class EnemyBullet :public Collider {
public:
	void Initialize(Model* model, const Vector3& position, float radius);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void Reset();

	bool GetIsAlive() { return isAlive_; }
	void SetViewProjection(ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
private:
	// 当たり判定
	void OnCollision(uint32_t type, Sphere* sphere)override;
	void HitBoxInitialize() override;
	void HitBoxUpdate() override;
	void HitBoxDraw(const ViewProjection& viewProjection) override;

	Model* model_;
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	float radius_ = 1.0f;
	bool isAlive_;
};


