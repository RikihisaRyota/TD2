#include "Fade.h"

Fade::~Fade() {

}

void Fade::Initialize() {
	textureHandle_ = TextureManager::Load("Resources/Images/FadeBlack.png");
	
	sprite_[0] = Sprite::Create(textureHandle_, { 640,360 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	sprite_[1] = Sprite::Create(textureHandle_, { 640,360 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);

	fadeColor_[0] = 0.0f;
	fadeColor_[1] = 1.0f;
}

void Fade::FadeInUpdate() {
	if (isCount_[0] == true) {
		fadeColor_[0] += 0.03f;
	}

	if (fadeColor_[0] == 1.2f) {
		isCount_[0] = false;
	}

	ImGui::Begin("FadeIn");
	ImGui::DragFloat("Color", &fadeColor_[0], 0.03f);
	ImGui::End();
}

void Fade::FadeOutUpdate() {
	if (isCount_[1] == true) {
		fadeColor_[1] -= 0.03f;
	}

	if (fadeColor_[1] == -1.2f) {
		isCount_[1] = false;
	}

	ImGui::Begin("FadeOut");
	ImGui::DragFloat("Color", &fadeColor_[1], 0.03f);
	ImGui::End();
}

void Fade::FadeInDraw() {
	sprite_[0]->SetColor({ 1.0f,1.0f,1.0f,fadeColor_[0] });
	sprite_[0]->Draw();
}

void Fade::FadeOutDraw() {
	sprite_[1]->SetColor({ 1.0f,1.0f,1.0f,fadeColor_[1] });
	sprite_[1]->Draw();
}

void Fade::Finalize() {
	for (int i = 0; i < 2; i++) {
		sprite_[i]->Release();
	}
}