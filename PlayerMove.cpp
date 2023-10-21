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
	player_->SetIsPulling(false);
	player_->SetIsLanding(false);
	player_->SetWeightNum(0);
	worldTransform_.translation_ = player_->GetTranslation();
	acceleration_ = { 0.0f,0.0f,0.0f };
	velocity_ = { 0.0f,0.0f,0.0f };
	playerNextRotate_ = 0.0f;
	playerCurrentRotate_ = 0.0f;
	direction_ = true;
	isEating_ = false;
	rotateVelocity_ = 0.0f;
}

void PlayerMove::Update() {
	if (input_->TriggerKey(DIK_SPACE)) {
		if (isEating_) {
			isEating_ = false;
		} 
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
		acceleration_ = { std::cosf(angle),std::sinf(angle),0.0f };
		velocity_ = move;
		acceleration_ *= kPower_;
		// 弾生成
		player_->GetPlayerBulletManager()->CreateBullet(worldTransform_.translation_);
		direction_ ^= true;
		playerNextRotate_ = angle;
		
	}
	if (!isEating_) {
		playerCurrentRotate_ = LenpShortAngle(playerCurrentRotate_, playerNextRotate_, 0.1f);
		player_->SetMotionRotation(Vector3(playerCurrentRotate_, 0.0f, 0.0f));
	}
	else {
		playerCurrentRotate_ = rotateVelocity_;
		rotateVelocity_ *= 0.99f;
		player_->SetMotionRotation(Vector3(playerCurrentRotate_, 0.0f, 0.0f));
	}
	velocity_ += acceleration_;
	velocity_ *= kInertia_;
	acceleration_ *= 0.5;
	worldTransform_.translation_ += velocity_;
	MoveLimit();
	worldTransform_.UpdateMatrix();
	player_->SetTranslation(worldTransform_.translation_);
	player_->UpdateMatrix();
}

void PlayerMove::Debug() {
	if (input_->TriggerKey(DIK_P)) {
		worldTransform_.translation_ = { 0.0f,0.0f,0.0f };
		worldTransform_.UpdateMatrix();
		player_->SetTranslation(worldTransform_.translation_);
		player_->UpdateMatrix();
		Initialize();
	}
	ImGui::Begin("Player");
	if (ImGui::TreeNode("kMove")) {
		ImGui::Text("velocity\nx:%.4f,y:%.4f,z:%.4f", velocity_.x, velocity_.y, velocity_.z);
		ImGui::Text("acceleration\nx:%.4f,y:%.4f,z:%.4f", acceleration_.x, acceleration_.y, acceleration_.z);
		ImGui::SliderFloat("Power", &kPower_, 0.0f, 0.1f);
		ImGui::SliderFloat("Inertia", &kInertia_, 0.0f, 1.0f);
		ImGui::SliderFloat("Angle", &kAngle_, 0.0f, 90.0f);
		ImGui::TreePop();
	}
	ImGui::End();
}

void PlayerMove::MoveLimit() {
	float playerSize = 2.0f;
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -player_->GetWidth() + playerSize, player_->GetWidth() - playerSize);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -player_->GetHeight() + playerSize, player_->GetHeight() - playerSize);
	if (worldTransform_.translation_.x <= -player_->GetWidth() + playerSize ||
		worldTransform_.translation_.x >= player_->GetWidth() - playerSize) {
		acceleration_.x = 0.0f;
		velocity_.x = 0.0f;
	}
	if (worldTransform_.translation_.y <= -player_->GetWidth() + playerSize ||
		worldTransform_.translation_.y >= player_->GetWidth() - playerSize) {
		acceleration_.y = 0.0f;
		velocity_.y = 0.0f;
	}
	worldTransform_.UpdateMatrix();
}