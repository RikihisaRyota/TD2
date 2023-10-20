#include "FollowCamera.h"
#include "MyMath.h"

#include "Input.h"
#include "ImGuiManager.h"
#include "Player.h"

void FollowCamera::Initialize() {
	viewProjection_.Initialize();
	debugOffset_ = { 40.0f, 0.0f, -90.0f };
	dropOffset_ = { -40.0f, 0.0f, -90.0f };
	bossAttackOffset_ = { -15.0f,0.0f,-150.0f };
	delayInterpolationLate_ = 0.5f;
	dropDelayInterpolationLate_ = 0.8f;
}

void FollowCamera::Update() {
	if (target_) {
		float kInterpolationLate;
		// プレイヤーが落下中か
		if (player_->GetIsPulling()&& !player_->GetIsLanding()) {
			nowOffset_ =Lerp(nowOffset_, dropOffset_,0.1f);
			//kInterpolationLate = dropDelayInterpolationLate_;
		}
		if (!player_->GetIsPulling() && !player_->GetIsLanding()) {
			nowOffset_ =Lerp(nowOffset_, debugOffset_,  0.1f);
			//kInterpolationLate = delayInterpolationLate_;
		}
		if (player_->GetBehavior()==Player::Behavior::kDoNothing) {
			const float BossCameraCountMax = 60.0f;
			nowOffset_ = Lerp(nowOffset_, bossAttackOffset_, static_cast<float>(bossCamaraCount_)/ BossCameraCountMax);
			bossCamaraCount_++;
			if (bossCamaraCount_ >= BossCameraCountMax) {
				bossCamaraCount_ = 0;
				player_->SetBehavior(Player::Behavior::kLanding);
			}

		}
		kInterpolationLate = dropDelayInterpolationLate_;
		// 追従座標の補間
		interTarget_ = Lerp(interTarget_, target_->translation_, kInterpolationLate);
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

	Debug();
}

void FollowCamera::Debug() {
	ImGui::Begin("Camera");
	if (ImGui::TreeNode("Nomal")) {
		ImGui::SliderFloat("Offset_x", &debugOffset_.x, -50.0f, 50.0f);
		ImGui::SliderFloat("Offset_y", &debugOffset_.y, 0.0f, 50.0f);
		ImGui::SliderFloat("Offset_z", &debugOffset_.z, 0.0f, -100.0f);
		ImGui::SliderFloat("Delay", &delayInterpolationLate_, 0.0f, 1.0f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Drop")) {
		ImGui::SliderFloat("Offset_y", &dropOffset_.y, 0.0f, -50.0f);
		ImGui::SliderFloat("Offset_z", &dropOffset_.z, 0.0f, -100.0f);
		ImGui::SliderFloat("Delay", &dropDelayInterpolationLate_, 0.0f, 1.0f);
		ImGui::TreePop();
	}
	ImGui::End();
}
