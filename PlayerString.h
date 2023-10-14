#pragma once

#include <vector>

#include "WorldTransform.h"
#include "PlaneRenderer.h"

class Input;
class Player;
class PlayerString {
public:
	PlayerString();
	~PlayerString();
	void Initialize();
	void Update();
	void Extend();
	void Shrink();
	void Draw(const ViewProjection& viewProjection);
	const Vector3 GetShootOutVector()const { return shootOutVector_; }
	void SetPlayer(Player* player) { player_ = player; }
	void Debug();
private:
	float kAddAngle_ = 3.5f;
	uint32_t kExtendCountMax_ = 50;
	float kSpeed_ = 0.3f;
	float kInitialAngle_ = 0.0f;
	uint32_t kSetStringWorldTransformInterval = 4;
	float kPlaneSize_ = 2.0f;
	uint32_t kShrinkCountMax_ = 30;


	Player* player_;
	Input* input_;
	std::vector<PlaneRenderer*> plane_;

	PlaneRenderer* head_;
	
	WorldTransform worldTransform_;
	Vector3 velocity_;
	Vector3 acceleration_;
	Vector3 shootOutVector_;
	float angle_;

	std::vector<WorldTransform> stringWorldTransform_;
	uint32_t setStringWorldTransformCount_;
	uint32_t extendCount_;

	bool isExtend_;

	uint32_t shrinkCount_;
};

