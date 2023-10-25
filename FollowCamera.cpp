#include "FollowCamera.h"
#include "MyMath.h"

#include "Input.h"
#include "ImGuiManager.h"
#include "Player.h"
#include "TreasureBox.h"

void FollowCamera::Initialize(bool flg) {
	viewProjection_.Initialize();
	debugOffset_ = { 40.0f, 0.0f, -250.0f };
	dropOffset_ = { -40.0f, 0.0f, -250.0f };
	bossAttackOffset_ = { -15.0f,0.0f,-300.0f };
	delayInterpolationLate_ = 0.5f;
	nowOffset_ = debugOffset_;
	dropDelayInterpolationLate_ = 0.8f;
	isInGame_ = flg;
	animationTime_ = 0.0f;
	if (isInGame_) {
		firstCount_ = 0.0f;
		firstCountMax_ = 6000.0f;
		isFirst_ = true;
		viewProjection_.translation_ = Vector3(0.0f, 0.0f, 0.0f) + nowOffset_;
		interTarget_ = Vector3(0.0f, 0.0f, 0.0);
	}
	else {
		interTarget_ = Vector3(80.0f, 0.0f, 0.0);
		viewProjection_.translation_ = Vector3(80.0f, 0.0f, 0.0f) + nowOffset_;
	}
	viewProjection_.UpdateMatrix();
}

void FollowCamera::Update() {
	if (isInGame_) {
		if (target_) {
			float kInterpolationLate;
			// プレイヤーが落下中か
			if (player_->GetIsPulling() && !player_->GetIsLanding()) {
				nowOffset_ = Lerp(nowOffset_, dropOffset_, 0.1f);
			}
			if (!player_->GetIsPulling() && !player_->GetIsLanding()) {
				nowOffset_ = Lerp(nowOffset_, debugOffset_, 0.1f);
			}
			if (player_->GetBehavior() == Player::Behavior::kDoNothing) {
				const float BossCameraCountMax = 60.0f;
				nowOffset_ = Lerp(nowOffset_, bossAttackOffset_, static_cast<float>(bossCamaraCount_) / BossCameraCountMax);
				bossCamaraCount_++;
				if (bossCamaraCount_ >= BossCameraCountMax) {
					bossCamaraCount_ = 0;
					player_->SetBehavior(Player::Behavior::kLanding);
				}

			}
			if (isFirst_) {
				float t = firstCount_ / firstCountMax_;
				kInterpolationLate = Lerp(0.01f,dropDelayInterpolationLate_,t);
				firstCount_ += 1.0f;
				if (firstCount_ >= firstCountMax_) {
					isFirst_ = false;
					firstCount_ = 0.0f;
				}
			}
			else {
				kInterpolationLate = dropDelayInterpolationLate_;
			}
			// 追従座標の補間
			target_->translation_.x = std::clamp(target_->translation_.x, -100.0f, kCameraLimit_);

			interTarget_.x = Lerp(interTarget_.x, target_->translation_.x, kInterpolationLate);
			Matrix4x4 rotateMatrix = MakeRotateXYZMatrix(viewProjection_.rotation_);

			nowOffset_ = TransformNormal(nowOffset_, rotateMatrix);
			viewProjection_.translation_ = interTarget_ + nowOffset_;
		}

		if (Input::GetInstance()->IsControllerConnected()) {
			XINPUT_STATE joyState;
			Input::GetInstance()->GetJoystickState(0, joyState);
			const float kRadian = 0.02f;
			viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kRadian;
		}
		viewProjection_.UpdateMatrix();
	}
	else {
		if (treasureBox_->GetState() == TreasureBox::State::kOpen) {
			float t = animationTime_ / animationMax_;
			viewProjection_.translation_ = Lerp(Vector3(80.0f, 0.0f, 0.0f) + nowOffset_, pos, std::clamp(t, 0.0f, 1.0f));
			viewProjection_.UpdateMatrix();
			animationTime_ += 1.0f;
		}
	}
}

void FollowCamera::Debug() {
	ImGui::Begin("Camera");
	if (ImGui::TreeNode("Nomal")) {
		ImGui::SliderFloat("Offset_x", &debugOffset_.x, -50.0f, 50.0f);
		ImGui::SliderFloat("Offset_y", &debugOffset_.y, 0.0f, 50.0f);
		ImGui::SliderFloat("Offset_z", &debugOffset_.z, 0.0f, -200.0f);
		ImGui::SliderFloat("Delay", &delayInterpolationLate_, 0.0f, 1.0f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Drop")) {
		ImGui::SliderFloat("Offset_y", &dropOffset_.y, 0.0f, -50.0f);
		ImGui::SliderFloat("Offset_z", &dropOffset_.z, 0.0f, -200.0f);
		ImGui::SliderFloat("Delay", &dropDelayInterpolationLate_, 0.0f, 1.0f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("pos")) {
		ImGui::DragFloat3("pos", &viewProjection_.translation_.x, 1.0f);
		ImGui::TreePop();
	}
	ImGui::SliderFloat("limit", &kCameraLimit_, 800.0f, 1200.0f);
	ImGui::End();
}

void FollowCamera::SetIsFirst(bool flag) {
	isFirst_ = flag;
	firstCount_ = 0.0f;
	firstCount_ = 0.0f;
	firstCountMax_ = 6000.0f;
	isFirst_ = true;
	viewProjection_.translation_ = Vector3(0.0f, 0.0f, 0.0f) + nowOffset_;
	interTarget_ = Vector3(0.0f, 0.0f, 0.0);
	viewProjection_.UpdateMatrix();
}
