#include "TitleBoss.h"

#include "MyMath.h"
#include "ImGuiManager.h"
#include "TitlePlayer.h"

void TitleBoss::Initialize(std::vector<Model*> models) {
	animationTime_ = 0.0f;
	sceneChange_ = false;
	for (size_t i = 0; i < static_cast<size_t>(Parts::kCount); i++) {
		models_.emplace_back(models.at(i));
	}
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 20.0f,20.0f,20.0f };
	worldTransform_.translation_ = { 20.0f,-3.0f,0.0f };
	worldTransform_.UpdateMatrix();
	motion_.Initialize();
	motion_.parent_ = &worldTransform_;
	motion_.UpdateMatrix();
	for (size_t i = 0; i < static_cast<size_t>(Parts::kCount); i++) {
		WorldTransform part;
		part.Initialize();
		part.parent_ = &motion_;
		part.UpdateMatrix();
		parts_.emplace_back(part);
	}
}

void TitleBoss::Update() {
	if (!titlePlayer_->GetSceneFlag()) {
		float t = std::fmodf(animationTime_ / animationMax_, 2.0f);
		if (t > 1.0f) {
			t = 2.0f - t;
		}
		if (t <= 0.0f) {
			animationTime_ = 0.0f;
		}
		parts_.at(static_cast<size_t>(Parts::kSharkHead)).rotation_.z = Lerp(0.0f, -DegToRad(20.0f), t);
		parts_.at(static_cast<size_t>(Parts::kSharkJaw)).rotation_.z = Lerp(0.0f, DegToRad(10.0f), t);
		animationTime_ += 1.0f;
	}
	else {
		worldTransform_.translation_.x += 2.5f;
		motion_.translation_.y = 0.0f;
		motion_.translation_.y += rnd.NextFloatRange(-0.5f,0.5f);
		if (worldTransform_.translation_.x >= limitMax_) {
			sceneChange_ = true;
		}
	}
	worldTransform_.UpdateMatrix();
	motion_.UpdateMatrix();
	for (auto& parts : parts_) {
		parts.UpdateMatrix();
	}
}

void TitleBoss::Draw(const ViewProjection& viewProjection) {
	for (size_t i = 0; i < static_cast<size_t>(Parts::kCount); i++) {
		models_.at(i)->Draw(parts_.at(i), viewProjection);
	}
}
