#include "Fade.h"

Fade::~Fade() {

}

void Fade::Initialize() {
	textureHandle_ = TextureManager::Load("Resources/Images/FadeBlack.png");
	sprite_ = Sprite::Create(textureHandle_, { 640,360 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
}

void Fade::Update() {
	if (isCount_ == 1) {
		//fadeColor_ = 0.0f;
		fadeColor_ += 0.02f;
	}

	if (fadeColor_ > 2.0f) {
		isCount_ = 0;
	}

	if (isCount_ == 2) {
		//fadeColor_ = 1.0f;
		fadeColor_ -= 0.02f;
	}

	if (fadeColor_ < -1.0f) {
		isCount_ = 0;
	}

	ImGui::Begin("Fade");
	ImGui::InputInt("isCount", &isCount_);
	ImGui::InputFloat("fadeColor", &fadeColor_);
	ImGui::End();
}

void Fade::Finalize() {
	sprite_->Release();
}

void Fade::Draw() {
	sprite_->SetColor({ 1.0f,1.0f,1.0f,fadeColor_ });
	sprite_->Draw();
}