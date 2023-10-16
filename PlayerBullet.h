#pragma once

#include "Collider.h"
#include "WorldTransform.h"
#include "Model.h"

class PlayerBullet : public Collider {
public:
	void Initialize(Model* model,const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void Reset();
	
	bool GetIsAlive() { return isAlive_; }
	void SetViewProjection(const ViewProjection& viewProjection) { viewProjection_ = viewProjection; }

private:
	float kSpeed_ = 2.0f;
	// 当たり判定
	void OnCollision(uint32_t type, Sphere* sphere)override;
	void HitBoxInitialize() override;
	void HitBoxUpdate() override;
	void HitBoxDraw(const ViewProjection& viewProjection) override;
	
	Model* model_;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	float radius_;
	bool isAlive_;
};
