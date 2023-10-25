#include "TreasureBox.h"

#include "MyMath.h"
#include "ImGuiManager.h"

void TreasureBox::Initialize(std::vector<Model*> models) {
	for (size_t i = 0; i < static_cast<size_t>(Parts::kCount); i++) {
		models_.emplace_back(models.at(i));
	}
	boxWorldTranslateStart_ = { 0.0f,-7.0f,0.0f };
	boxWorldTranslateEnd_ = { 56.0f,-7.0f,0.0f };

	upperRotateStart_ = { 0.0f,0.0f,0.0f };
	upperRotateEnd_ = { DegToRad(120.0f),0.0f,0.0f };

	animationTime_ = 0.0f;

	worldTransfrom_.Initialize();
	worldTransfrom_.scale_ = { 3.0f,3.0f,3.0f };
	worldTransfrom_.translation_ = boxWorldTranslateStart_;
	motion_.Initialize();
	motion_.parent_ = &worldTransfrom_;
	motion_.UpdateMatrix();
	for (size_t i = 0; i < static_cast<size_t>(Parts::kCount); i++) {
		WorldTransform parts{};
		parts.Initialize();
		parts.parent_ = &motion_;
		parts.UpdateMatrix();
		parts_.emplace_back(parts);
	}
}

void TreasureBox::Update() {
	animationTime_ += 1.0f;

	float t = 0.0f;
	switch (state_) {
	case TreasureBox::State::kCome:
	{
		t = animationTime_ / ComeAnimationMax_;
		worldTransfrom_.translation_ = Lerp(boxWorldTranslateStart_, boxWorldTranslateEnd_, std::clamp(t, 0.0f, 1.0f));
		if (t >= 1.0f) {
			state_ = State::kOpen;
			animationTime_ = 0.0f;
		}
	}
	break;
	case TreasureBox::State::kOpen:
	{
		t = animationTime_ / OpenAnimationMax_;
		parts_.at(static_cast<size_t>(Parts::kHuta)).rotation_ = Lerp(upperRotateStart_, upperRotateEnd_, std::clamp(t, 0.0f, 1.0f));

	}
	break;
	}

	worldTransfrom_.UpdateMatrix();
	motion_.UpdateMatrix();
	for (auto& parts : parts_) {
		parts.UpdateMatrix();
	}
}

void TreasureBox::Draw(const ViewProjection& viewProjection) {
	for (size_t i = 0; i < static_cast<size_t>(Parts::kCount); i++) {
		models_.at(i)->Draw(parts_.at(i), viewProjection);
	}
}
