#include "PlayerMove.h"

#include "Input.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include "Player.h"

PlayerMove::PlayerMove() {
	input_ = Input::GetInstance();
	worldTransform_.Initialize();
}

PlayerMove::~PlayerMove() {}

void PlayerMove::Initialize() {
	worldTransform_.translation_ = player_->GetTranslation();
	direction_ = false;
}

void PlayerMove::Update() {
	Vector3 move{};
	if (input_->TriggerKey(DIK_SPACE)) {
		// direction_がtrueで左
		if (direction_) {
			float angle = DegToRad(45.0f);
			acceleration_.x = kPower_;
			move = { std::cosf(angle),std::sinf(angle),0.0f};
		}
		else {
			float angle = DegToRad(-45.0f);
			acceleration_.x = kPower_;
			move = { std::cosf(angle),std::sinf(angle),0.0f};
		}
		direction_ ^= true;
	}
	velocity_ = move;
	velocity_ += acceleration_;
	acceleration_ *= kInertia_;
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
	player_->SetTranslation(worldTransform_.translation_);
}

void PlayerMove::Debug() {
	ImGui::Begin("Player");
	if (ImGui::TreeNode("kMove")) {
		ImGui::Text("velocity\nx:%.4f,y:%.4f,z:%.4f", velocity_.x, velocity_.y, velocity_.z);
		ImGui::Text("acceleration\nx:%.4f,y:%.4f,z:%.4f", acceleration_.x, acceleration_.y, acceleration_.z);
		ImGui::SliderFloat("Power", &kPower_, 0.0f, 10.0f);
		ImGui::SliderFloat("Inertia", &kInertia_, 0.0f, 1.0f);
		ImGui::TreePop();
	}
	ImGui::End();
}
