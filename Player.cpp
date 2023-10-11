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
}

void Player::Update() {
	OBJtoOBB();
	Move();
	Debug();
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
		isDrop_ = false;
		if (input_->TriggerKey(DIK_A)) {
			Vector3 direction = { cosf(DegToRad(kLeftAngle_)), sinf(DegToRad(kLeftAngle_)), 0 };
			direction.Normalize();
			move += direction * nowWeight;
			acceleration_.y = kSpeed_ * nowWeight;
		}
		if (input_->TriggerKey(DIK_D)) {
			Vector3 direction = { cosf(DegToRad(kRightAngle_)), sinf(DegToRad(kRightAngle_)), 0 };
			direction.Normalize();
			move += direction * nowWeight;
			acceleration_.y = kSpeed_ * nowWeight;
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
	// 枠内に収める処理
	if (worldTransform_.translation_.x <= -kWidth_ + worldTransform_.scale_.x * 2.0f) {
		worldTransform_.translation_.x = -kWidth_ + worldTransform_.scale_.x * 2.0f;
	}
	else if (worldTransform_.translation_.x >= kWidth_ - worldTransform_.scale_.x * 2.0f) {
		worldTransform_.translation_.x = kWidth_ - worldTransform_.scale_.x * 2.0f;
	}
	// 地面以外にいたら
	if (worldTransform_.translation_.y > 0.0f) {
		lifeTimeCount_++;
		float t = static_cast<float>(kLifeTimeMax_ - lifeTimeCount_) / static_cast<float>(kLifeTimeMax_);
		model_->GetMaterial(0)->SetColor(Vector4(t, t, t, 1.0f));
	}
	else {
		lifeTimeCount_ = 0;
		float t = static_cast<float>(kLifeTimeMax_ - lifeTimeCount_) / static_cast<float>(kLifeTimeMax_);
		model_->GetMaterial(0)->SetColor(Vector4(t, t, t, 1.0f));
	}
	if (lifeTimeCount_ >= kLifeTimeMax_) {
		lifeTimeCount_ = 0;
		worldTransform_.translation_ = { 0.0f,0.0f,0.0f };
		velocity_ = { 0.0f,0.0f,0.0f };
		acceleration_ = { 0.0f,0.0f,0.0f };
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
	float lifeTimeMax= static_cast<float>(kLifeTimeMax_);
	ImGui::SliderFloat("lifeTime",&lifeTime,0.0f, lifeTimeMax);
	ImGui::SliderFloat("ifeTimeMax_",&lifeTimeMax,0.0f,10.0f);
	lifeTimeCount_ = static_cast<uint32_t> (lifeTime );
	kLifeTimeMax_ = static_cast<uint32_t> (lifeTimeMax);
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
		ImGui::TreePop();
	}
	ImGui::End();
}
