#include "FollowCamera.h"
#include "MyMath.h"

#include "Input.h"

void FollowCamera::Initialize() {
	viewProjection_.Initialize();
}

void FollowCamera::Update() {
	if (target_) {
		Vector3 offset = { 0.0f, 2.0f, -40.0f };
		Matrix4x4 rotateMatrix = MakeRotateXYZMatrix(viewProjection_.rotation_);

		offset = TransformNormal(offset, rotateMatrix);
		viewProjection_.translation_ = target_->translation_ + offset;
	}

	if (Input::GetInstance()->IsControllerConnected()) {
		XINPUT_STATE joyState;
		Input::GetInstance()->GetJoystickState(0, joyState);
		const float kRadian = 0.02f;
		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kRadian;
	}

	viewProjection_.UpdateMatrix();
}