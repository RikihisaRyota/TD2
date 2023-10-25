#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"

class TreasureBox;
class Player;
class FollowCamera {
public:

	void Initialize(bool flg);
	void Update();

	void SetTarget(WorldTransform* target) { target_ = target; }

	void SetPlayer(Player* player) { player_ = player; }

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	void Debug();

	void SetTreasureBox(TreasureBox* TreasureBox) {treasureBox_ = TreasureBox;}

	void SetAnimationMax(float time) { animationMax_ = time; }

	void SetIsFirst(bool flag);
private:
	Player* player_;
	ViewProjection viewProjection_;
	WorldTransform* target_ = nullptr;
	TreasureBox* treasureBox_;
	// 追従対象の残像
	Vector3 interTarget_ = {};
	// デバック用
	Vector3 nowOffset_;
	Vector3 dropOffset_;
	Vector3 debugOffset_;
	Vector3 bossAttackOffset_;
	uint32_t bossCamaraCount_ = 0;
	float dropDelayInterpolationLate_;
	float delayInterpolationLate_;
	float kCameraLimit_ = 885.0f;

	Vector3 pos = { 56.0f,0.0f, -87.0f };
	bool isInGame_;

	// クリアカメラ
	float animationTime_;
	float animationMax_;
	// 最初だけカウント
	float firstCount_;
	float firstCountMax_;
	bool isFirst_;
};