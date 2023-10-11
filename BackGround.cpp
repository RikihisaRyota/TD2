#include "BackGround.h"

void BackGround::Initialize(uint32_t textureHandle) {
	plane_.reset(PlaneRenderer::Create());
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 100.0f,100.0f,100.0f };
	worldTransform_.translation_ = { 0.0f,30.0f,0.0f };
	worldTransform_.UpdateMatrix();
	textureHandle_ = textureHandle;
}

void BackGround::Draw(const ViewProjection& viewProjection) {
	plane_->Draw(worldTransform_,viewProjection,textureHandle_);
}
