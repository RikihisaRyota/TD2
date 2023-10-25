#include "TitlePlayer.h"

#include "MyMath.h"
#include "ImGuiManager.h"

void TitlePlayer::Initialize(std::vector<Model*> models) {
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	worldTransform_.Initialize();
	float scale = kRadius_ * 0.5f;
	worldTransform_.scale_ = { scale ,scale ,scale };
	worldTransform_.rotation_ = { 0.0f,11.0f,0.0f };
	worldTransform_.translation_ = kInitialPosition_;
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
		models_.emplace_back(models.at(i));
	}

	direction_ = true;
	acceleration_ = { 0.0f,0.0f,0.0f };
	velocity_ = { 0.0f,0.0f,0.0f };
	isSceneChange_ = false;
}

void TitlePlayer::Update() {
	if (!isSceneChange_) {
		if (input_->TriggerKey(DIK_SPACE) ||
			worldTransform_.translation_.x <= LimitMin_) {
			// 音
			audio_->SoundPlayWave(moveSoundHandle_);
			float angle = 0.0f;
			Vector3 move{};
			// direction_がtrueで左
			if (direction_) {
				angle = DegToRad(kAngle_);
			}
			else {
				angle = DegToRad(-kAngle_);
			}
			move = { std::cosf(angle),std::sinf(angle),0.0f };
			velocity_ = move * kPower_;
			acceleration_ = move * kPower_;

			direction_ ^= true;

			playerNextRotate_ = angle;
			isSwell_ = true;
			playerCurrentLegLeftRotate_ = 0.0f;
			playerCurrentLegRightRotate_ = 0.0f;

		}

		playerCurrentRotate_ = LenpShortAngle(playerCurrentRotate_, playerNextRotate_, 0.1f);
		if (isSwell_) {
			const uint32_t kSwellMax = 5;
			float t = static_cast<float>(swellCount_) / static_cast<float>(kSwellMax);
			float legAngle = DegToRad(kLegAngle_);
			playerCurrentLegLeftRotate_ = LenpShortAngle(playerCurrentLegLeftRotate_, -legAngle, t);
			playerCurrentLegRightRotate_ = LenpShortAngle(playerCurrentLegRightRotate_, legAngle, t);
			swellCount_++;
			if (swellCount_ >= kSwellMax) {
				isSwell_ = false;
				swellCount_ = 0;
			}
		}
		else {
			playerCurrentLegLeftRotate_ = LenpShortAngle(playerCurrentLegLeftRotate_, DegToRad(10.0f), 0.1f);
			playerCurrentLegRightRotate_ = LenpShortAngle(playerCurrentLegRightRotate_, DegToRad(-10.0f), 0.1f);
		}
		motion_.rotation_ = Vector3(playerCurrentRotate_, 0.0f, 0.0f);
		parts_.at(static_cast<size_t>(Parts::kLegLeft)).rotation_ = Vector3(playerCurrentLegLeftRotate_);
		parts_.at(static_cast<size_t>(Parts::kLegRight)).rotation_ = Vector3(playerCurrentLegRightRotate_);

		Vector3 vector = Vector3(-20.0f, -12.0f, 0.0f) - worldTransform_.translation_;
		vector.Normalize();
		Vector3 gravity = vector * kGravity;
		acceleration_ += gravity;
		velocity_ += acceleration_;
		velocity_ *= kInertia_;
		acceleration_ *= 0.5;
		worldTransform_.translation_ += velocity_;
		MoveLimit();
		worldTransform_.UpdateMatrix();
		motion_.UpdateMatrix();
		for (auto& parts : parts_) {
			parts.UpdateMatrix();
		}
		if (worldTransform_.translation_.x >= LimitMax_) {
			isSceneChange_ = true;
		}
	}
}

void TitlePlayer::Draw(const ViewProjection& viewProjection) {
	for (size_t i = 0; i < static_cast<size_t>(Parts::kCount); i++) {
		models_.at(i)->Draw(parts_.at(i), viewProjection);
	}
}

void TitlePlayer::MoveLimit() {
	float playerSize = kRadius_;
	float width = 1000.0f;
	float height = 50.0f;
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -1000.0f + playerSize, 1000.0f - playerSize);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -50.0f + playerSize, 50.0f - playerSize);
	if (worldTransform_.translation_.x <= -width + playerSize ||
		worldTransform_.translation_.x >= width - playerSize) {
		acceleration_.x = 0.0f;
		velocity_.x = 0.0f;
	}
	if (worldTransform_.translation_.y <= -height + playerSize ||
		worldTransform_.translation_.y >= height - playerSize) {
		acceleration_.y = 0.0f;
		velocity_.y = 0.0f;
	}
	worldTransform_.UpdateMatrix();
	motion_.UpdateMatrix();
	for (auto& parts : parts_) {
		parts.UpdateMatrix();
	}
}
