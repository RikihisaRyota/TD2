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
	float kChaseMin_ = 0.02f;
	float kChaseMax_ = 0.045f;

	float kWidth_=1000.0f;
	// 当たり判定
	void OnCollision(uint32_t type, Sphere* sphere)override;
	void HitBoxInitialize() override;
	void HitBoxUpdate() override;
	void HitBoxDraw(const ViewProjection& viewProjection) override;

	uint32_t kCreateModelInterval_ = 5;
	float radius_ = 3.0f;
	Player* player_;
	Model* headModel_;
	Model* bodyModel_;
	std::vector<Model*> bodyModels_;
	WorldTransform headWorldTransform_;
	std::vector<WorldTransform> bodyWorldTransforms_;
	uint32_t createModelCount_;
	float angle_;
	bool isPlayerChase_;
};
