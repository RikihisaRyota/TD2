#include "PlayerStun.h"

#include "MyMath.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "Player.h"

PlayerStun::PlayerStun() {
	input_ = Input::GetInstance();
	worldTransform_.Initialize();
}

PlayerStun::~PlayerStun() {}

void PlayerStun::Initialize() {
	stunCount_ = 0;
	worldTransform_ = player_->GetWorldTransform();
}

void PlayerStun::Update() {
	if (stunCount_ <= kStunMax_) {
		stunCount_++;
		float t = static_cast<float>(stunCount_) / static_cast<float>(kStunMax_);
		worldTransform_.rotation_.z = Lerp(0.0f, 360.0f*kRotationCountMax_,t);
		worldTransform_.UpdateMatrix();
		player_->SetWorldTransform(worldTransform_);
	}
	else {
		worldTransform_.rotation_.z = 0.0f;
		worldTransform_.UpdateMatrix();
		player_->SetWorldTransform(worldTransform_);
		player_->SetInvincible(true);
		player_->SetBehavior(Player::Behavior::kMove);
	}
}

void PlayerStun::Draw(const ViewProjection& viewProjection) {}

void PlayerStun::Debug() {
	ImGui::Begin("Player");
	if (ImGui::TreeNode("kStun")) {
		float max = static_cast<float>(kStunMax_);
		ImGui::SliderFloat("StunMax",&max,0.0f,120.0f);
		kStunMax_ = static_cast<uint32_t>(max);
		float angle = static_cast<float>(kRotationCountMax_);
		ImGui::SliderFloat("何回転するか",&angle,1.0f,5.0f);
		kRotationCountMax_ = static_cast<uint32_t>(angle);
		ImGui::TreePop();
	}
	ImGui::End();
}
