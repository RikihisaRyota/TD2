#include "PlayerString.h"

#include <algorithm>
#include <string>

#include "Input.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include "Player.h"


PlayerString::PlayerString() {
	input_ = Input::GetInstance();
	worldTransform_.Initialize();
	head_ = PlaneRenderer::Create();
}

PlayerString::~PlayerString() {
	delete head_;
	for (auto& plane : plane_) {
		delete plane;
	}
	plane_.clear();
	stringWorldTransform_.clear();
}

void PlayerString::Initialize() {
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.translation_ = player_->GetTranslation();
	worldTransform_.UpdateMatrix();
	velocity_ = { 0.0f,0.0f,0.0f };
	acceleration_ = { 0.0f,0.0f,0.0f };
	shootOutVector_ = { 0.0f,0.0f,0.0f };
	angle_ = kInitialAngle_;
	extendCount_ = 0;
	stringWorldTransform_.clear();
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
}

void PlayerString::Extend() {
	Vector3 move{};
	if ((input_->TriggerKey(DIK_SPACE) &&
		stringWorldTransform_.size() >= 4) ||
		extendCount_ >= kExtendCountMax_) {
		isExtend_ = false;
		// 最後の位置を保存
		WorldTransform saveWorldTransform;
		saveWorldTransform.Initialize();
		saveWorldTransform.scale_ = worldTransform_.scale_;
		saveWorldTransform.translation_ = worldTransform_.translation_;
		saveWorldTransform.UpdateMatrix();
		stringWorldTransform_.emplace_back(saveWorldTransform);
		plane_.emplace_back(PlaneRenderer::Create());
		// プレイヤーのいる場所に戻す
		worldTransform_.translation_ = player_->GetTranslation();
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
	if (move.Length()) {
		move.Normalize();
	}
	velocity_ = move * kSpeed_;
	worldTransform_.translation_ += velocity_;
	// 徐々にサイズを大きくしていく
	float scale = kPlaneSize_ * static_cast<float>(extendCount_) / static_cast<float>(kExtendCountMax_);
	worldTransform_.scale_ = { scale,scale,1.0f };
	worldTransform_.UpdateMatrix();
	// 場所を保存
	if (setStringWorldTransformCount_ % kSetStringWorldTransformInterval == 0) {
		WorldTransform saveWorldTransform;
		saveWorldTransform.Initialize();
		saveWorldTransform.scale_ = worldTransform_.scale_;
		saveWorldTransform.translation_ = worldTransform_.translation_;
		saveWorldTransform.UpdateMatrix();
		stringWorldTransform_.emplace_back(saveWorldTransform);
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

	if (shrinkCount_ >= kShrinkCountMax_ || stringWorldTransform_.size() < 4) {
		size_t endIndex = stringWorldTransform_.size() - 1;
		shootOutVector_ = stringWorldTransform_[endIndex].translation_ - stringWorldTransform_[endIndex - 1].translation_;
		shootOutVector_.Normalize();
		player_->SetBehavior(Player::Behavior::kJump);
	}
	else {
		float t = static_cast<float>(shrinkCount_) / static_cast<float>(kShrinkCountMax_);
		// インデックスを計算
		size_t numPoints = stringWorldTransform_.size();
		size_t startIndex = static_cast<size_t>(t * (numPoints - 1));
		size_t endIndex = (std::min)(startIndex + 1, numPoints - 1);

		// tInSegmentを計算
		float tInSegment = t * (numPoints - 1) - startIndex;

		// 線形補間して位置を設定
		worldTransform_.translation_ = Lerp(stringWorldTransform_[startIndex].translation_, stringWorldTransform_[endIndex].translation_, tInSegment);
		worldTransform_.UpdateMatrix();
		// プレイヤーのワールド変換を設定
		player_->SetTranslation(worldTransform_.translation_);
	}

}


void PlayerString::Draw(const ViewProjection& viewProjection) {
	for (size_t i = 0; i < stringWorldTransform_.size(); i++) {
		plane_.at(i)->Draw(stringWorldTransform_.at(i), viewProjection);
	}
	if (isExtend_) {
		head_->Draw(worldTransform_, viewProjection);
	}
}

void PlayerString::Debug() {
	ImGui::Begin("Player");
	if (ImGui::TreeNode("kString")) {
		ImGui::Text("position\nx:%.4f,y:%.4f,z:%.4f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
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
		if (stringWorldTransform_.empty()) {
			if (ImGui::TreeNode("String")) {
				uint32_t count = 0;
				for (auto& world : stringWorldTransform_) {
					ImGui::Text(("translation" + std::to_string(count)).c_str());
					ImGui::Text("x:%.4f,y:%.4f,z:%.4f", world.translation_.x, world.translation_.y, world.translation_.z);
					count++;
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	ImGui::End();
}
