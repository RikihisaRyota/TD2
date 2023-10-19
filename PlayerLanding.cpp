#include "PlayerLanding.h"

#include "Player.h"
#include "Input.h"

PlayerLanding::PlayerLanding() {
	input_ = Input::GetInstance();
}

PlayerLanding::~PlayerLanding() {}

void PlayerLanding::Initialize() {
	player_->SetIsLanding(true);
}

void PlayerLanding::Update() {}

void PlayerLanding::Draw(const ViewProjection& viewProjection) {}

void PlayerLanding::Debug() {}
