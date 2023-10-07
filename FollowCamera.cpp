#include "FollowCamera.h"
#include "MyMath.h"

#include "Input.h"
#include "ImGuiManager.h"

void FollowCamera::Initialize() {
	viewProjection_.Initialize();
	debugOffset_ = { 0.0f, 12.0f, -65.0f };
	delayInterpolationLate_ = 0.02f;
}

void FollowCamera::Update() {
	if (target_) {
		Vector3 offset = debugOffset_;
		// 追従座標の補間
		const float kInterpolationLate = delayInterpolationLate_;
		interTarget_ = Lerp(interTarget_, target_->translation_, kInterpolationLate);
		Matrix4x4 rotateMatrix = MakeRotateXYZMatrix(viewProjection_.rotation_);

		offset = TransformNormal(offset, rotateMatrix);
		viewProjection_.translation_ = interTarget_ + offset;
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
	ImGui::SliderFloat("Offset_y",&debugOffset_.y,0.0f,50.0f);
	ImGui::SliderFloat("Offset_z",&debugOffset_.z,0.0f,-100.0f);
	ImGui::SliderFloat("Delay",&delayInterpolationLate_,0.0f,1.0f);
	ImGui::End();
}
