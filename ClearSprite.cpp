#include "ClearSprite.h"

#include "ImGuiManager.h"
#include "TreasureBox.h"
#include "MyMath.h"
#include "cMaterial.h"

ClearSprite::~ClearSprite() {
	for (auto& plane : planes_) {
		delete plane;
	}
}

void ClearSprite::Initialize(uint32_t textureHandle) {
	PlaneRenderer* planeRenderer = PlaneRenderer::Create();
	planes_.emplace_back(planeRenderer);
	textureHandle_ = textureHandle;
	spriteScaleStart_ = 1.0f;
	spriteScaleEnd_ = 12.0f;
	spriteTranslateStart_ = { 56.0f,-7.0f,0.0f };
	spriteTranslateEnd_ = { 56.0f,4.0f,0.0f };
	worldTransfrom_.Initialize();
	worldTransfrom_.scale_ = { spriteScaleStart_ ,spriteScaleStart_ * 0.3f ,spriteScaleStart_ };
	worldTransfrom_.translation_ = spriteTranslateStart_;
	worldTransfrom_.UpdateMatrix();
	animationTime_ = 0.0f;
	animationMax_ = 120.0f;
}

void ClearSprite::Update() {
	if (treasureBox_->GetState() == TreasureBox::State::kOpen) {
		float t = animationTime_ / animationMax_;
		ImGui::Begin("Sprite");
		ImGui::DragFloat3("TranslateStart", &spriteTranslateStart_.x, 1.0f);
		ImGui::DragFloat3("TranslateEnd", &spriteTranslateEnd_.x, 1.0f);
		ImGui::DragFloat("scaleStart", &spriteScaleStart_, 1.0f);
		ImGui::DragFloat("scaleEnd", &spriteScaleEnd_, 1.0f);
		ImGui::DragFloat("animation", &animationTime_, 1.0f);
		ImGui::DragFloat("animationMax", &animationMax_, 1.0f);
		ImGui::End();
		float scale = Lerp(spriteScaleStart_, spriteScaleEnd_, std::clamp(t, 0.0f, 1.0f));
		worldTransfrom_.scale_ = { scale,scale * 0.3f ,scale };
		Vector3 pos = Lerp(spriteTranslateStart_, spriteTranslateEnd_, std::clamp(t, 0.0f, 1.0f));
		worldTransfrom_.translation_ = pos;
		worldTransfrom_.translation_.y += std::sinf(DegToRad(animationTime_));
		worldTransfrom_.UpdateMatrix();
		animationTime_++;
	}
}

void ClearSprite::Draw(const ViewProjection& viewProjection) {
	if (treasureBox_->GetState() == TreasureBox::State::kOpen) {
		for (size_t i = 0; i < static_cast<size_t>(Parts::kCount); i++) {
			planes_.at(i)->Draw(worldTransfrom_, viewProjection, textureHandle_);
		}
	}
}
