#include "PlayerLanding.h"

#include "Player.h"
#include "Input.h"

PlayerLanding::PlayerLanding() {
	input_ = Input::GetInstance();
}

PlayerLanding::~PlayerLanding() {}

void PlayerLanding::Initialize() {
	uint32_t weightNum = player_->GetWeightNum();
	player_->Reset();
	player_->SetTranslation(Vector3(0.0f, -15.0f, 0.0f));
	player_->UpdateMatrix();
	player_->SetWeightNum(weightNum);
	player_->SetIsLanding(true);
}

void PlayerLanding::Update() {}

void PlayerLanding::Draw(const ViewProjection& viewProjection) {}

void PlayerLanding::Debug() {}
