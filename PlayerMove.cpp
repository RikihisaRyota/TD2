#include "PlayerMove.h"

#include <algorithm>

#include "ImGuiManager.h"
#include "Input.h"
#include "Player.h"

PlayerMove::PlayerMove() {
	input_ = Input::GetInstance();
	worldTransform_.Initialize();
	acceleration_ = { 0.0f,0.0f,0.0f };
}

PlayerMove::~PlayerMove() {}

void PlayerMove::Initialize() {
	worldTransform_ = player_->GetWorldTransform();
	velocity_ = { 0.0f,0.0f,0.0f };
	
}

void PlayerMove::Update() {
	Vector3 move{};
	if (input_->PushKey(DIK_A)) {
		move += { -1.0f,0.0f,0.0f };
	}
	if (input_->PushKey(DIK_D)) {
		move += { 1.0f, 0.0f, 0.0f };
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
	if (worldTransform_.translation_.y > 0.0f) {
		acceleration_.y -= kGravity_;
		// 上限なし
		acceleration_.y = std::clamp(acceleration_.y, kGravityMax_, 100.0f);
	}
	velocity_ += acceleration_;
	worldTransform_.translation_ += velocity_;
	acceleration_.x *= 0.9f;
	// 地面に着いたら
	if (worldTransform_.translation_.y <= 0.0f) {
		worldTransform_.translation_.y = 0.0f;
		acceleration_ = { 0.0f ,0.0f ,0.0f };
	}
	player_->SetWorldTransform(worldTransform_);
}

void PlayerMove::Debug() {
	ImGui::Begin("Player");
	if (ImGui::TreeNode("kMove")) {
		ImGui::Text("velocity\nx:%.4f,y:%.4f,z:%.4f", velocity_.x, velocity_.y, velocity_.z);
		ImGui::Text("acceleration\nx:%.4f,y:%.4f,z:%.4f", acceleration_.x, acceleration_.y, acceleration_.z);
		ImGui::SliderFloat("Speed", &kSpeed_, 0.0f, 1.0f);
		ImGui::SliderFloat("Gravity", &kGravity_, 0.0f, 1.0f);
		ImGui::SliderFloat("GravityMax_", &kGravityMax_, -5.0f, -1.0f);
		
		ImGui::TreePop();
	}
	ImGui::End();
}
