#include "Player.h"

#include <cassert>

#include "MyMath.h"
#include "ImGuiManager.h"

#ifndef M_PI
#define M_PI 3.14
#endif

#define deg_to_rad(deg) (deg * (M_PI/180))

void Player::Initialize(Model* model) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();

	input_ = Input::GetInstance();
}

void Player::Update() {
	Move();
	Debug();
}

void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

void Player::Move() {
	Vector3 move = { 0, 0, 0 };

	if (input_->TriggerKey(DIK_A)) {
		float angle = 135.0f;
		Vector3 direction = { cosf(DegToRad(angle)), sinf(DegToRad(angle)), 0 };
		direction.Normalize();
		move = direction;
	}

	if (input_->TriggerKey(DIK_D)) {
		float angle = 45.0f;
		Vector3 direction = { cosf(DegToRad(angle)), sinf(DegToRad(angle)), 0 };
		direction.Normalize();
		move = direction;
	}
	if (input_->PushKey(DIK_SPACE)) {
		worldTransform_.translation_.y -= kDropSpeed_;
	}
	velocity_ += move * kSpeed_;
	velocity_ *= kInertia;
	// 重力
	worldTransform_.translation_.y -= kGravity_;
	worldTransform_.translation_ += velocity_;
	// 地面
	if (worldTransform_.translation_.y <= 0.0f) {
		worldTransform_.translation_.y = 0.0f;
	}
	worldTransform_.translation_ += move;
	worldTransform_.UpdateMatrix();
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPosition;

	worldPosition.x = worldTransform_.matWorld_.m[3][0];
	worldPosition.y = worldTransform_.matWorld_.m[3][1];
	worldPosition.z = worldTransform_.matWorld_.m[3][2];

	return worldPosition;
}

void Player::Debug() {
	ImGui::Begin("Player");
	ImGui::SliderFloat("Speed", &kSpeed_, 0.0f, 10.0f);
	ImGui::SliderFloat("Gravity", &kGravity_, 0.0f, 2.0f);
	ImGui::SliderFloat("DropSpeed", &kDropSpeed_, 0.0f, 20.0f);
	ImGui::End();
}
