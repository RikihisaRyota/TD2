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

void ClearSprite::Initialize(std::vector<uint32_t> textureHandle) {
	for (size_t i = 0; i < static_cast<size_t>(Parts::kCount); i++) {
		PlaneRenderer* planeRenderer = PlaneRenderer::Create();
		planes_.emplace_back(planeRenderer);
		textureHandle_.emplace_back(textureHandle.at(i));
	}
	animationFlag_ = false;
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

	spaceWorldTransfrom_.Initialize();
	spaceWorldTransfrom_.scale_ = { 12.0f,2.0f,1.0f };
	spaceWorldTransfrom_.translation_ = { 56.0f,-11.0f,-16.0f };
}

void ClearSprite::Update() {
	if (treasureBox_->GetState() == TreasureBox::State::kOpen) {
		float t = animationTime_ / animationMax_;
		float scale = Lerp(spriteScaleStart_, spriteScaleEnd_, std::clamp(t, 0.0f, 1.0f));
		worldTransfrom_.scale_ = { scale,scale * 0.3f ,scale };
		Vector3 pos = Lerp(spriteTranslateStart_, spriteTranslateEnd_, std::clamp(t, 0.0f, 1.0f));
		worldTransfrom_.translation_ = pos;
		worldTransfrom_.translation_.y += std::sinf(DegToRad(animationTime_));
		worldTransfrom_.UpdateMatrix();
		animationTime_++;
		if (t >= 1.0f) {
			animationFlag_ = true;
		}
	}
	ImGui::Begin("sprite");
	ImGui::DragFloat3("scale", &spaceWorldTransfrom_.scale_.x, 1.0f);
	ImGui::DragFloat3("pos", &spaceWorldTransfrom_.translation_.x, 1.0f);
	ImGui::End();
	spaceWorldTransfrom_.UpdateMatrix();
}

void ClearSprite::Draw(const ViewProjection& viewProjection) {
	if (treasureBox_->GetState() == TreasureBox::State::kOpen) {
		planes_.at(static_cast<size_t>(Parts::kClearSprite))->Draw(worldTransfrom_, viewProjection, textureHandle_.at(static_cast<size_t>(Parts::kClearSprite)));
	
	}
}

void ClearSprite::FrontDraw(const ViewProjection& viewProjection) {
	if (animationFlag_) {
		planes_.at(static_cast<size_t>(Parts::kPressSpace))->Draw(spaceWorldTransfrom_, viewProjection, textureHandle_.at(static_cast<size_t>(Parts::kPressSpace)));
	}
}
