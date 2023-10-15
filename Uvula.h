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
	// 当たり判定
	void OnCollision()override;
	void HitBoxInitialize() override;
	void HitBoxUpdate() override;
	void HitBoxDraw(const ViewProjection& viewProjection) override;
private:
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
