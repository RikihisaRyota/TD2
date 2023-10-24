#include "PlayerPullingMove.h"

#include <algorithm>

#include "ImGuiManager.h"
#include "Input.h"
#include "Player.h"
#include "MyMath.h"

PlayerPullingMove::PlayerPullingMove() {
	input_ = Input::GetInstance();
	worldTransform_.Initialize();
	acceleration_ = { 0.0f,0.0f,0.0f };
}

PlayerPullingMove::~PlayerPullingMove() {}

void PlayerPullingMove::Initialize() {
	player_->SetMotionRotation(Vector3(0.0f, 0.0f, 0.0f));
	for (size_t i = 0; i < static_cast<size_t>(Player::Parts::kPartsCount); ++i) {
		player_->SetPartsRotation(Vector3(0.0f, 0.0f, 0.0f), i);
	}
	player_->SetIsPulling(true);
	direction_ = false;
	playerNextRotate_ = 0.0f;
	playerCurrentRotate_ = 0.0f;
	velocity_ = { 0.0f,0.0f,0.0f };
	acceleration_ = { 0.0f,0.0f,0.0f };
}

void PlayerPullingMove::Update() {
	worldTransform_ = player_->GetWorldTransform();
	Vector3 move{};
	float angle = 0.0f;
	if (input_->TriggerKey(DIK_SPACE)) {
		direction_ ^= true;
		if (direction_) {
			angle = DegToRad(kAngle_);
		}
		else {
			angle = DegToRad(-kAngle_);
		}
		float power = Lerp(kPowerMax_, kPowerMin_, static_cast<float>(player_->GetWeightNum()) / static_cast<float>(player_->GetWeightMax()));
		move = { std::cosf(angle),std::sinf(angle),0.0f };
		velocity_ = move * power;
		acceleration_ = move * power;
		playerNextRotate_ = angle;
	}

	if (move.Length() > 0) {
		move.Normalize();
	}
	// 重力
	if (worldTransform_.translation_.x > 0.0f) {
		float gravity = Lerp(kGravityMin_, kGravityMax_, static_cast<float>(player_->GetWeightNum()) / static_cast<float>(player_->GetWeightMax()));
		float gravityLimit = Lerp(kGravityLimitMin_, kGravityLimitMax_, static_cast<float>(player_->GetWeightNum()) / static_cast<float>(player_->GetWeightMax()));
		Vector3 vector = Vector3(-20.0f, -10.0f, 0.0f) - worldTransform_.translation_;
		vector.Normalize();
		acceleration_ += vector * gravity;
		acceleration_.x = std::clamp(acceleration_.x, -gravityLimit, 10.0f);
	}
	velocity_ += acceleration_;
	velocity_ *= 0.98f;
	acceleration_.y *= 0.5f;
	worldTransform_.translation_ += velocity_;
	MoveLimit();

	playerCurrentRotate_ = LenpShortAngle(playerCurrentRotate_, playerNextRotate_, 0.1f);
	player_->SetMotionRotation(Vector3(playerCurrentRotate_, 0.0f, 0.0f));
}

void PlayerPullingMove::Debug() {
	ImGui::Begin("Player");
	if (ImGui::TreeNode("kPullingMove")) {
		ImGui::Text("velocity\nx:%.4f,y:%.4f,z:%.4f", velocity_.x, velocity_.y, velocity_.z);
		ImGui::Text("acceleration\nx:%.4f,y:%.4f,z:%.4f", acceleration_.x, acceleration_.y, acceleration_.z);
		if (ImGui::TreeNode("プレイヤーが軽いとき")) {
			ImGui::SliderFloat("SpeedMax", &kPowerMax_, kPowerMin_, 1.0f);
			ImGui::SliderFloat("GravityMin_", &kGravityMin_, 0.0f, kGravityMax_);
			ImGui::SliderFloat("GravityLimitMin", &kGravityLimitMin_, 0.0f, kGravityLimitMax_);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("プレイヤーが重いとき")) {
			ImGui::SliderFloat("SpeedMin", &kPowerMin_, 0.0f, kPowerMax_);
			ImGui::SliderFloat("GravityMax_", &kGravityMax_, kGravityMin_, 0.1f);
			ImGui::SliderFloat("GravityLimitMax", &kGravityLimitMax_, kGravityLimitMin_, 0.1f);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::End();
}

void PlayerPullingMove::MoveLimit() {
	float playerSize = player_->GetSize();
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -player_->GetWidth() + playerSize, player_->GetWidth() - playerSize);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -player_->GetHeight() + playerSize, player_->GetHeight() - playerSize);
	worldTransform_.UpdateMatrix();
	player_->SetWorldTransform(worldTransform_);
	player_->UpdateMatrix();
	// 地面に着いたら
	if (worldTransform_.translation_.x <= 0.0f) {
		worldTransform_.translation_.x = 0.0f;
		acceleration_ = { 0.0f ,0.0f ,0.0f };
		player_->SetBehavior(Player::Behavior::kDoNothing);
	}
}
