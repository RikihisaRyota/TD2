#include "Player.h"

#include <cassert>

#include "MyMath.h"
#include "ImGuiManager.h"

void Player::Initialize(Model* model) {
	assert(model);
	model_ = model;

	input_ = Input::GetInstance();

	worldTransform_.Initialize();

	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.rotation_ = { 0.0f,11.0f,0.0f };

	lifeTimeCount_ = 0;

	isDrop_ = false;

	isHitStop_ = false;
}

void Player::Update() {
	if (!isHitStop_) {
		OBJtoOBB();
		Move();
		Debug();
	}
	else {
		hitStopCount_++;
		if (hitStopCount_ >= hitStopMax_) {
			hitStopCount_ = 0;
			isHitStop_ = false;
		}
	}
}

void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

void Player::OBJtoOBB() {
	// .objをOBBへ変更（当たり判定へ）
	obb_.center_ = worldTransform_.translation_;
	GetOrientations(MakeRotateXYZMatrix(worldTransform_.rotation_), obb_.orientations_);
	obb_.size_ = worldTransform_.scale_;
}

void Player::Move() {
	Vector3 move = { 0, 0, 0 };
	float nowWeight = static_cast<float>((kWeightMax_ - weightCount_)) / static_cast<float> (kWeightMax_);
	// SPACE押している間も重力がかかり落下スピードが上がるかも
	if (input_->PushKey(DIK_SPACE)) {
		isDrop_ = true;
		if (input_->PushKey(DIK_A)) {
			angle_ += addAngle_;
		}

		if (input_->PushKey(DIK_D)) {
			angle_ -= addAngle_;
		}
		float angle = DegToRad(angle_);
		move.x = std::cosf(angle);
		move.y = std::sinf(angle);
		// 斜め移動時の速度を正規化
		if (move.Length() > 0) {
			move.Normalize();
			velocity_ = move * kDashSpeed_;
		}
	}
	else if (input_->IsKeyReleased(DIK_SPACE)) {
		float angle = DegToRad(angle_);
		move.x = std::cosf(angle);
		move.y = std::sinf(angle);
		acceleration_.x = move.x * kJumpSpeed_;
		acceleration_.y = move.y * kJumpSpeed_;
		// 斜め移動時の速度を正規化
		if (move.Length() > 0) {
			move.Normalize();
			velocity_ = move * kDashSpeed_;
		}
	}
	else {
		isDrop_ = false;
		if (input_->PushKey(DIK_A)) {
			move = Vector3(-1.0f, 0.0f, 0.0f);
		}
		if (input_->PushKey(DIK_D)) {
			move = Vector3(1.0f, 0.0f, 0.0f);
		}
		// 斜め移動時の速度を正規化
		if (move.Length() > 0) {
			move.Normalize();
			velocity_ = move * kSpeed_;
		}
		// 重力を適用
		if (acceleration_.y >= -kDropMaxSpeed_) {
			acceleration_.y -= kGravity_;
		}
	}
	// 移動
	acceleration_.x *= 0.8f;
	velocity_ += acceleration_;
	velocity_ *= kInertia_;
	worldTransform_.translation_ += velocity_;
	// 地面に接触した場合、速度をリセット
	if (worldTransform_.translation_.y <= 0.0f) {
		worldTransform_.translation_.y = 0.0f;
		acceleration_.y = 0.0f;
		weightCount_ = 0;
		if (!input_->PushKey(DIK_SPACE)) {
			angle_ = 90.0f;
		}
	}
	// 枠内に収める処理
	if (worldTransform_.translation_.x <= -kWidth_ + worldTransform_.scale_.x * 2.0f) {
		worldTransform_.translation_.x = -kWidth_ + worldTransform_.scale_.x * 2.0f;
	}
	else if (worldTransform_.translation_.x >= kWidth_ - worldTransform_.scale_.x * 2.0f) {
		worldTransform_.translation_.x = kWidth_ - worldTransform_.scale_.x * 2.0f;
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
	float lifeTime = static_cast<float>(lifeTimeCount_);
	float lifeTimeMax = static_cast<float>(kLifeTimeMax_);
	ImGui::SliderFloat("lifeTime", &lifeTime, 0.0f, lifeTimeMax);
	ImGui::SliderFloat("ifeTimeMax_", &lifeTimeMax, 0.0f, 10.0f);
	lifeTimeCount_ = static_cast<uint32_t> (lifeTime);
	kLifeTimeMax_ = static_cast<uint32_t> (lifeTimeMax);
	float hitStopCount = static_cast<float>(hitStopCount_);
	float hitStopMax = static_cast<float>(hitStopMax_);
	ImGui::SliderFloat("hitStopCount", &hitStopCount, 0.0f, hitStopMax);
	ImGui::SliderFloat("hitStopMax", &hitStopMax, 0.0f, 10.0f);
	hitStopCount_ = static_cast<uint32_t> (hitStopCount);
	hitStopMax_ = static_cast<uint32_t> (hitStopMax);
	if (ImGui::TreeNode("PlayerMove")) {
		ImGui::SliderFloat("Speed", &kSpeed_, 0.0f, 1.0f);
		ImGui::SliderFloat("Gravity", &kGravity_, 0.0f, 0.1f);
		ImGui::SliderFloat("Inertia", &kInertia_, 0.0f, 1.0f);
		ImGui::SliderFloat("DropSpeed", &kDropSpeed_, 0.0f, 1.0f);
		ImGui::SliderFloat("DropHorizontalSpeed", &kDropHorizontalSpeed_, 0.1f, 0.5f);
		ImGui::SliderFloat("DropMaxSpeed", &kDropMaxSpeed_, 0.0f, 1.0f);
		ImGui::SliderFloat("RightAngle", &kRightAngle_, 0.0f, 90.0f);
		ImGui::SliderFloat("LeftAngle", &kLeftAngle_, 90.0f, 180.0f);
		float weight = static_cast<float>(weightCount_);
		ImGui::SliderFloat("WeightCount", &weight, 0.0f, 10.0f);
		weightCount_ = static_cast<uint32_t>(weight);
		ImGui::SliderFloat("AddAngle", &addAngle_, 0.0f, 10.0f);
		ImGui::SliderFloat("DashSpeed", &kDashSpeed_, 0.0f, 1.0f);
		ImGui::SliderFloat("JumpSpeed", &kJumpSpeed_, 0.0f, 2.0f);
		ImGui::TreePop();
	}
	ImGui::End();
}
