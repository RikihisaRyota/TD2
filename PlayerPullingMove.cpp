#include "PlayerPullingMove.h"

#include <algorithm>

#include "ImGuiManager.h"
#include "Input.h"
#include "Player.h"

PlayerPullingMove::PlayerPullingMove() {
	input_ = Input::GetInstance();
	worldTransform_.Initialize();
	acceleration_ = { 0.0f,0.0f,0.0f };
}

PlayerPullingMove::~PlayerPullingMove() {}

void PlayerPullingMove::Initialize() {
	worldTransform_ = player_->GetWorldTransform();
	velocity_ = { 0.0f,0.0f,0.0f };

}

void PlayerPullingMove::Update() {
	Vector3 move{};
	if (input_->PushKey(DIK_A)) {
		move += { 0.0f, 1.0f, 0.0f };
	}
	if (input_->PushKey(DIK_D)) {
		move += { 0.0f, -1.0f, 0.0f };
	}
	if (input_->TriggerKey(DIK_SPACE)) {
		player_->SetBehavior(Player::Behavior::kString);
	}
	if (move.Length() > 0) {
		move.Normalize();
	}
	// 左右移動
	velocity_ = move * kSpeed_;
	// 重力
	if (worldTransform_.translation_.x > 0.0f) {
		acceleration_.x -= kGravity_;
		// 上限なし
		acceleration_.x = std::clamp(acceleration_.x, kGravityMax_, 100.0f);
	}
	velocity_ += acceleration_;
	worldTransform_.translation_ += velocity_;
	acceleration_.y *= 0.9f;
	// 地面に着いたら
	if (worldTransform_.translation_.x <= 0.0f) {
		worldTransform_.translation_.x = 0.0f;
		acceleration_ = { 0.0f ,0.0f ,0.0f };
	}
	MoveLimit();
	worldTransform_.UpdateMatrix();
	player_->SetWorldTransform(worldTransform_);
}

void PlayerPullingMove::Debug() {
	ImGui::Begin("Player");
	if (ImGui::TreeNode("kPullingMove")) {
		ImGui::Text("velocity\nx:%.4f,y:%.4f,z:%.4f", velocity_.x, velocity_.y, velocity_.z);
		ImGui::Text("acceleration\nx:%.4f,y:%.4f,z:%.4f", acceleration_.x, acceleration_.y, acceleration_.z);
		ImGui::SliderFloat("Speed", &kSpeed_, 0.0f, 1.0f);
		ImGui::SliderFloat("Gravity", &kGravity_, 0.0f, 1.0f);
		ImGui::SliderFloat("GravityMax_", &kGravityMax_, -5.0f, -1.0f);

		ImGui::TreePop();
	}
	ImGui::End();
}

void PlayerPullingMove::MoveLimit() {
	float playerSize = 2.0f;
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -player_->GetWidth() + playerSize, player_->GetWidth() - playerSize);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -player_->GetHeight() + playerSize, player_->GetHeight() - playerSize);
	worldTransform_.UpdateMatrix();
}
