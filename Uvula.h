#pragma once

#include <vector>
#include <memory>

#include "Collider.h"
#include "WorldTransform.h"
#include "Model.h"

class Player;
class Uvula : public Collider{
public:
	void Initialize(Model* head,Model* body);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void SetPlayer(Player* player) { player_ = player; }
	void Reset();

	void SetWidth(float width) { kWidth_ = width; }
private:
	float kChaseMin_ = 0.01f;
	float kChaseMax_ = 0.045f;
	float kWidth_=1000.0f;
	Vector3 kInitialPosition_ = { -20.0f,-10.0f,0.0f };
	// 当たり判定
	void OnCollision(uint32_t type, Sphere* sphere)override;
	void HitBoxInitialize() override;
	void HitBoxUpdate() override;
	void HitBoxDraw(const ViewProjection& viewProjection) override;

	float radius_ = 3.0f;
	Player* player_;
	Model* headModel_;
	Model* bodyModel_;
	WorldTransform headWorldTransform_;
	WorldTransform bodyWorldTransforms_;
	bool isPlayerChase_;
	bool isDebug_;
};
