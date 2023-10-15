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
private:
	float kSpeed_ = 2.0f;
	// 当たり判定
	void OnCollision()override;
	void HitBoxInitialize() override;
	void HitBoxUpdate() override;
	void HitBoxDraw(const ViewProjection& viewProjection) override;
	
	Model* model_;
	WorldTransform worldTransform_;
	float radius_;
};
