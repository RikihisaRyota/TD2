#include "Frame.h"
#include "Frame.h"

#include <assert.h>

#include "Player.h"
#include "ImGuiManager.h"

void Frame::Initialize() {
	width_ = 20.0f;
	height_ = 300.0f;
	// 床
	floor_.reset(CubeRenderer::Create());
	floor_->SetColor(Vector4(0.9f, 0.9f, 0.5f, 0.8f));
	floorWorldTransform_.Initialize();

	rightWall_.reset(CubeRenderer::Create());
	rightWall_->SetColor(Vector4(0.9f, 0.9f, 0.5f, 0.8f));
	rightWallWorldTransform_.Initialize();
	
	leftWall_.reset(CubeRenderer::Create());
	leftWall_->SetColor(Vector4(0.9f, 0.9f, 0.5f, 0.8f));
	leftWallWorldTransform_.Initialize();

	UpdateMatrix();
}

void Frame::Update() {
	Debug();
}

void Frame::Draw(const ViewProjection& viewProjection) {
	floor_->Draw(floorWorldTransform_, viewProjection);
	rightWall_->Draw(rightWallWorldTransform_, viewProjection);
	leftWall_->Draw(leftWallWorldTransform_, viewProjection);
}

void Frame::Debug() {
	ImGui::Begin("Frame");
	ImGui::SliderFloat("width", &width_, 20.0f, 100.0f);
	player_->SetWidth(width_);
	ImGui::SliderFloat("height", &height_, 20.0f, 100.0f);
	player_->SetHeight(height_);
	ImGui::End();
	UpdateMatrix();
}

void Frame::UpdateMatrix() {
	floorWorldTransform_.translation_ = { 0.0f,-2.0f,0.0f };
	floorWorldTransform_.scale_ = { width_,1.0f,1.0f };
	floorWorldTransform_.UpdateMatrix();

	rightWallWorldTransform_.translation_ = { width_,height_ - 3.0f,0.0f };
	rightWallWorldTransform_.scale_ = { 1.0f,height_,1.0f };
	rightWallWorldTransform_.UpdateMatrix();

	leftWallWorldTransform_.translation_ = { -width_,height_ - 3.0f,0.0f };
	leftWallWorldTransform_.scale_ = { 1.0f,height_,1.0f };
	leftWallWorldTransform_.UpdateMatrix();
}
