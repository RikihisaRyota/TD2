#include "PlayerJump.h"

#include "Input.h"
#include "ImGuiManager.h"
#include "Player.h"

PlayerJump::PlayerJump() {
	input_ = Input::GetInstance();
	worldTransform_.Initialize();
}

PlayerJump::~PlayerJump() {}

void PlayerJump::Initialize(const Vector3& outPutVector) {
	outPutVector_ = outPutVector;
}

void PlayerJump::Update() {
	Vector3 acceleration = outPutVector_ * kSpeed_;
	player_->GetPlayerMove()->SetAcceleration(acceleration);
	player_->SetBehavior(Player::Behavior::kMove);
}

void PlayerJump::Debug() {
	ImGui::Begin("Player");
	if (ImGui::TreeNode("kJump")) {
		ImGui::SliderFloat("Speed", &kSpeed_, 0.0f, 10.0f);
		ImGui::TreePop();
	}
	ImGui::End();
}
