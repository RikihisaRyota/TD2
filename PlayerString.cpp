#include "PlayerString.h"

#include <algorithm>
#include <string>

#include "Collision.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include "Player.h"


PlayerString::PlayerString() {
	input_ = Input::GetInstance();

	head_ = PlaneRenderer::Create();
}

PlayerString::~PlayerString() {
	delete head_;
	for (auto& plane : plane_) {
		delete plane;
	}
	plane_.clear();
	stringBody_.clear();
}

void PlayerString::Initialize() {
	playerWorldTransform_.Initialize();
	playerWorldTransform_.translation_ = player_->GetTranslation();
	playerWorldTransform_.UpdateMatrix();
	headWorldTransform_.Initialize();
	headWorldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	headWorldTransform_.translation_ = player_->GetTranslation();
	headWorldTransform_.UpdateMatrix();
	velocity_ = { 0.0f,0.0f,0.0f };
	acceleration_ = { 0.0f,0.0f,0.0f };
	shootOutVector_ = { 0.0f,0.0f,0.0f };
	angle_ = kInitialAngle_;
	extendCount_ = 0;
	stringBody_.clear();
	setStringWorldTransformCount_ = 0;
	isExtend_ = true;
	shrinkCount_ = 0;
	for (auto& plane : plane_) {
		delete plane;
	}
	plane_.clear();
}

void PlayerString::Update() {
	if (isExtend_) {
		// 伸ばす処理
		Extend();
	}
	else {
		// 縮む処理
		Shrink();
	}
	StringBodyUpdate();
	MoveLimit();
}

void PlayerString::Extend() {
	Vector3 move{};
	if ((input_->TriggerKey(DIK_SPACE) &&
		stringBody_.size() >= 4) ||
		extendCount_ >= kExtendCountMax_) {
		isExtend_ = false;
		// 最後の位置を保存
		StringBody save;
		save.worldTransform_.Initialize();
		save.worldTransform_.scale_ = headWorldTransform_.scale_;
		save.worldTransform_.translation_ = headWorldTransform_.translation_;
		save.worldTransform_.UpdateMatrix();
		save.isAlive_ = true;
		stringBody_.emplace_back(save);
		plane_.emplace_back(PlaneRenderer::Create());
		// プレイヤーのいる場所に戻す
		headWorldTransform_.translation_ = player_->GetTranslation();
		return;
	}
	if (input_->PushKey(DIK_A)) {
		angle_ += kAddAngle_;
	}
	if (input_->PushKey(DIK_D)) {
		angle_ -= kAddAngle_;
	}
	float RadAngle = DegToRad(angle_);
	move.x = std::cosf(RadAngle);
	move.y = std::sinf(RadAngle);
	if (move.Length() > 0.0f) {
		move.Normalize();
	}
	velocity_ = move * kSpeed_;
	headWorldTransform_.translation_ += velocity_;
	float gravity = Lerp(kGravityMin_, kGravityMax_, static_cast<float>(player_->GetWeightNum()) / static_cast<float>(player_->GetWeightMax()));
	if (playerWorldTransform_.translation_.x > 0.0f) {
		playerWorldTransform_.translation_.x -= gravity;
		for (auto& string : stringBody_) {
			string.worldTransform_.translation_.x -= gravity;
			string.worldTransform_.UpdateMatrix();
		}
		// 徐々にサイズを大きくしていく
		float scale = kPlaneSize_ * static_cast<float>(extendCount_) / static_cast<float>(kExtendCountMax_);
		headWorldTransform_.scale_ = { scale,scale,1.0f };
		headWorldTransform_.UpdateMatrix();
		playerWorldTransform_.UpdateMatrix();
		player_->SetTranslation(playerWorldTransform_.translation_);
		player_->UpdateMatrix();
	}
	else {
		playerWorldTransform_.translation_.x = 0.0f;
		playerWorldTransform_.UpdateMatrix();
		player_->SetWorldTransform(playerWorldTransform_);
		player_->UpdateMatrix();
		acceleration_ = { 0.0f ,0.0f ,0.0f };
		player_->SetBehavior(Player::Behavior::kDoNothing);
	}

	// 場所を保存
	if (setStringWorldTransformCount_ % kSetStringWorldTransformInterval == 0) {
		StringBody save;
		save.worldTransform_.Initialize();
		save.worldTransform_.scale_ = headWorldTransform_.scale_;
		save.worldTransform_.translation_ = headWorldTransform_.translation_;
		save.worldTransform_.UpdateMatrix();
		save.isAlive_ = true;
		stringBody_.emplace_back(save);
		plane_.emplace_back(PlaneRenderer::Create());
		setStringWorldTransformCount_ = 0;
	}
	// 伸びる時間
	extendCount_++;
	// 保存する時間
	setStringWorldTransformCount_++;
}

void PlayerString::Shrink() {

	shrinkCount_++;

	if (shrinkCount_ >= kShrinkCountMax_ || stringBody_.size() < 4) {
		size_t endIndex = stringBody_.size() - 1;
		uint32_t count = 1;
		while (shootOutVector_.Length() <= 0.0f) {
			shootOutVector_ = stringBody_[endIndex].worldTransform_.translation_ - stringBody_[endIndex - count].worldTransform_.translation_;
			count++;
		}
		shootOutVector_.Normalize();
		player_->SetBehavior(Player::Behavior::kJump);
		return;
	}
	else {
		float t = static_cast<float>(shrinkCount_) / static_cast<float>(kShrinkCountMax_);
		// インデックスを計算
		size_t numPoints = stringBody_.size();
		size_t startIndex = static_cast<size_t>(t * (numPoints - 1));
		size_t endIndex = (std::min)(startIndex + 1, numPoints - 1);

		// tInSegmentを計算
		float tInSegment = t * (numPoints - 1) - startIndex;

		// 線形補間して位置を設定
		headWorldTransform_.translation_ = Lerp(stringBody_[startIndex].worldTransform_.translation_, stringBody_[endIndex].worldTransform_.translation_, tInSegment);
		headWorldTransform_.UpdateMatrix();
		// プレイヤーのワールド変換を設定
		player_->SetTranslation(headWorldTransform_.translation_);
		player_->UpdateMatrix();
	}

}


void PlayerString::Draw(const ViewProjection& viewProjection) {
	for (size_t i = 0; i < stringBody_.size(); i++) {
		if (stringBody_.at(i).isAlive_) {
			plane_.at(i)->Draw(stringBody_.at(i).worldTransform_, viewProjection);
		}
	}
	if (isExtend_) {
		head_->Draw(headWorldTransform_, viewProjection);
	}
}

void PlayerString::Debug() {
	ImGui::Begin("Player");
	if (ImGui::TreeNode("kString")) {
		ImGui::Text("position\nx:%.4f,y:%.4f,z:%.4f", headWorldTransform_.translation_.x, headWorldTransform_.translation_.y, headWorldTransform_.translation_.z);
		ImGui::Text("velocity\nx:%.4f,y:%.4f,z:%.4f", velocity_.x, velocity_.y, velocity_.z);
		ImGui::Text("acceleration\nx:%.4f,y:%.4f,z:%.4f", acceleration_.x, acceleration_.y, acceleration_.z);
		ImGui::SliderFloat("Speed", &kSpeed_, 0.0f, 1.0f);
		ImGui::SliderFloat("AddAngle", &kAddAngle_, 0.0f, 10.0f);
		float extendCountMax = static_cast<float>(kExtendCountMax_);
		float extendCount = static_cast<float>(extendCount_);
		ImGui::SliderFloat("ExtendCount", &extendCount, 0.0f, extendCountMax);
		ImGui::SliderFloat("ExtendCountMax", &extendCountMax, 0.0f, 600.0f);
		extendCount_ = static_cast<uint32_t>(extendCount);
		kExtendCountMax_ = static_cast<uint32_t>(extendCountMax);
		ImGui::SliderFloat("GravityMin", &kGravityMin_, 0.0f, kGravityMax_);
		ImGui::SliderFloat("GravityMax", &kGravityMax_, kGravityMin_, 1.0f);
		if (stringBody_.empty()) {
			if (ImGui::TreeNode("String")) {
				uint32_t count = 0;
				for (auto& world : stringBody_) {
					ImGui::Text(("translation" + std::to_string(count)).c_str());
					ImGui::Text("x:%.4f,y:%.4f,z:%.4f", world.worldTransform_.translation_.x, world.worldTransform_.translation_.y, world.worldTransform_.translation_.z);
					count++;
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	ImGui::End();
}

void PlayerString::MoveLimit() {
	float playerSize = player_->GetSize();
	headWorldTransform_.translation_.x = std::clamp(headWorldTransform_.translation_.x, -player_->GetWidth() + playerSize, player_->GetWidth() - playerSize);
	headWorldTransform_.translation_.y = std::clamp(headWorldTransform_.translation_.y, -player_->GetHeight() + playerSize, player_->GetHeight() - playerSize);
	headWorldTransform_.UpdateMatrix();
}

void PlayerString::StringBodyUpdate() {
	for (auto& body : stringBody_) {
		if (IsCollision(Sphere(player_->GetTranslation(), 0.5f), Sphere(body.worldTransform_.translation_, 0.5f))) {
			body.isAlive_ = false;
		}
	}
}
