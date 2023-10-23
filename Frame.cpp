#include "Frame.h"
#include "Frame.h"

#include <assert.h>

#include "Player.h"
#include "ImGuiManager.h"
#include "Uvula.h"
#include "EnemyManager.h"

void Frame::Initialize() {
	width_ = 1000.0f;
	height_ = 50.0f;
	// 床
	for (size_t i = 0; i < walls_.size(); i++) {
		walls_[i].reset(CubeRenderer::Create());
		walls_[i]->SetColor(Vector4(0.9f, 0.9f, 0.5f, 0.8f));
		worldTransforms_[i].Initialize();
	}

	UpdateMatrix();
}

void Frame::Update() {
	Debug();
}

void Frame::Draw(const ViewProjection& viewProjection) {
	for (size_t i = 0; i < walls_.size(); i++) {
		walls_[i]->Draw(worldTransforms_[i], viewProjection);
	}
}

void Frame::Debug() {
	ImGui::Begin("Frame");
	ImGui::SliderFloat("width", &width_, 20.0f, 1000.0f);
	ImGui::SliderFloat("height", &height_, 20.0f, 100.0f);
	if (ImGui::TreeNode("Walls")) {
	for (size_t i = 0; i < walls_.size(); i++) {
		switch (i) {
		case Frame::kFloor:
			ImGui::SliderFloat3("Floor", &worldTransforms_[Wall::kFloor].translation_.x, -50.0f, 50.0f);
			break;
		case Frame::kCeiling:
			ImGui::SliderFloat3("Ceiling", &worldTransforms_[Wall::kCeiling].translation_.x, -50.0f, 50.0f);
			break;
		case Frame::kTop:
			ImGui::SliderFloat3("Top", &worldTransforms_[Wall::kTop].translation_.x, -50.0f, 50.0f);
			break;
		case Frame::kBottom:
			ImGui::SliderFloat3("Bottom", &worldTransforms_[Wall::kBottom].translation_.x, -50.0f, 50.0f);
			break;
		case Frame::kCount:
			break;
		default:
			break;
		}
	}
		ImGui::TreePop();
	}
	ImGui::End();
	UpdateMatrix();
}

void Frame::UpdateMatrix() {
	float playerSize = 2.0f;
	worldTransforms_[Wall::kFloor].scale_ = { 1.0f,height_,1.0f};
	worldTransforms_[Wall::kFloor].translation_ = { -playerSize ,0.0f,0.0f};
	worldTransforms_[Wall::kFloor].UpdateMatrix();
	worldTransforms_[Wall::kCeiling].scale_ = { 1.0f,height_,1.0f };
	worldTransforms_[Wall::kCeiling].translation_ = { width_ - playerSize ,0.0f,0.0f };
	worldTransforms_[Wall::kCeiling].UpdateMatrix();
	worldTransforms_[Wall::kTop].scale_ = { width_ * 0.5f,1.0f,1.0f };
	worldTransforms_[Wall::kTop].translation_ = { width_ * 0.5f - playerSize*1.5f,height_,0.0f };
	worldTransforms_[Wall::kTop].UpdateMatrix();
	worldTransforms_[Wall::kBottom].scale_ = { width_ * 0.5f,1.0f,1.0f };
	worldTransforms_[Wall::kBottom].translation_ = { width_ * 0.5f - playerSize * 1.5f,-height_,0.0f };
	worldTransforms_[Wall::kBottom].UpdateMatrix();
	player_->SetHeight(height_);
	player_->SetWidth(width_);
	//enemyManager_->SetHeight(height_);
	//enemyManager_->SetWidth(width_);
	uvula_->SetWidth(width_);
}
