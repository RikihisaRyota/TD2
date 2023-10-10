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

	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.rotation_ = { 0.0f,11.0f,0.0f };

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
	obb_.center_ = worldTransform_.translation_;
	GetOrientations(MakeRotateXYZMatrix(worldTransform_.rotation_), obb_.orientations_);
	obb_.size_ = worldTransform_.scale_;

	Vector3 move = { 0, 0, 0 };

	// SPACE押している間も重力がかかり落下スピードが上がるかも
	if (input_->PushKey(DIK_SPACE)) {
		acceleration_.y -= kDropSpeed_;
		if (input_->PushKey(DIK_A)) {
			move.x = -1.0f;
			move *= kDropHorizontalSpeed_;
		}

		if (input_->PushKey(DIK_D)) {
			move.x = 1.0f;
			move *= kDropHorizontalSpeed_;
		}
		velocity_.x = move.x;
	}
	else {
		if (input_->TriggerKey(DIK_A)) {
			Vector3 direction = { cosf(DegToRad(kLeftAngle_)), sinf(DegToRad(kLeftAngle_)), 0 };
			direction.Normalize();
			move += direction;
			acceleration_.y = kSpeed_;
		}
		if (input_->TriggerKey(DIK_D)) {
			Vector3 direction = { cosf(DegToRad(kRightAngle_)), sinf(DegToRad(kRightAngle_)), 0 };
			direction.Normalize();
			move += direction;
			acceleration_.y = kSpeed_;
		}
		// 斜め移動時の速度を正規化
		if (move.Length() > 0) {
			move.Normalize();
			velocity_ = move;
		}
	}
	// 重力を適用
	if (acceleration_.y >= -kDropMaxSpeed_) {
		acceleration_.y -= kGravity_;
	}
	// 移動
	velocity_ += acceleration_;
	velocity_ *= kInertia_;
	worldTransform_.translation_ += velocity_;
	// 地面に接触した場合、速度をリセット
	if (worldTransform_.translation_.y <= 0.0f) {
		worldTransform_.translation_.y = 0.0f;
		acceleration_.y = 0.0f;
	}
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
	ImGui::Text("velocity:x:%f,y:%f,z:%f", velocity_.x, velocity_.y, velocity_.z);
	ImGui::Text("acceleration:x:%f,y:%f,z:%f", acceleration_.x, acceleration_.y, acceleration_.z);
	ImGui::Text("pos:x:%f,y:%f:z:%f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::SliderFloat("Speed", &kSpeed_, 0.0f, 1.0f);
	ImGui::SliderFloat("Gravity", &kGravity_, 0.0f, 0.1f);
	ImGui::SliderFloat("Inertia", &kInertia_, 0.0f, 1.0f);
	ImGui::SliderFloat("DropSpeed", &kDropSpeed_, 0.0f, 1.0f);
	ImGui::SliderFloat("DropHorizontalSpeed", &kDropHorizontalSpeed_, 0.1f, 0.5f);
	ImGui::SliderFloat("DropMaxSpeed", &kDropMaxSpeed_, 0.0f, 1.0f);
	ImGui::SliderFloat("RightAngle", &kRightAngle_, 0.0f, 90.0f);
	ImGui::SliderFloat("LeftAngle", &kLeftAngle_, 90.0f, 180.0f);
	ImGui::End();
}
