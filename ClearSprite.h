#pragma once

#include "PlaneRenderer.h"
class TreasureBox;
class ClearSprite {
public:
	enum class Parts {
		kClearSprite,
		kPressSpace,

		kCount,
	};
public:
	~ClearSprite();
	void Initialize(std::vector<uint32_t> textureHandle);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void FrontDraw(const ViewProjection& viewProjection);
	void SetTreasureBox(TreasureBox* TreasureBox) { treasureBox_ = TreasureBox; }
	void SetAnimationMax(float time) { animationMax_ = time; }
private:
	TreasureBox* treasureBox_;
	std::vector<PlaneRenderer*> planes_;
	WorldTransform worldTransfrom_;
	WorldTransform spaceWorldTransfrom_;

	bool animationFlag_;
	float animationTime_;
	float animationMax_;

	float spriteScaleStart_;
	float spriteScaleEnd_;
	Vector3 spriteTranslateStart_;
	Vector3 spriteTranslateEnd_;

	std::vector<uint32_t> textureHandle_;
};

