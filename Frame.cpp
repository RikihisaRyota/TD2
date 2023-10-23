#include "Frame.h"
#include "Frame.h"

#include <assert.h>

#include "Draw.h"
#include "Player.h"
#include "ImGuiManager.h"
#include "Uvula.h"
#include "MyMath.h"

Frame::~Frame() {
	for (auto& wall : topWalls_) {
		delete  wall;
	}
	for (auto& wall : bottomWalls_) {
		delete  wall;
	}
}

void Frame::Initialize(std::vector<Model*>model) {
	width_ = 1000.0f;
	height_ = 50.0f;
	// 床
	for (size_t i = 0; i < width_ / 15.0f; i++) {
		Wall* topWall = new Wall();
		topWall->model_ = model.at(0);
		topWall->worldTransform_.Initialize();
		topWall->worldTransform_.translation_ = { float(i) * 15.0f ,height_ + 5.0f ,0.0f };
		topWall->worldTransform_.rotation_.z = DegToRad(180.0f);
		topWall->worldTransform_.UpdateMatrix();
		topWall->isAlive_ = true;
		topWalls_.emplace_back(topWall);
		Wall* bottomWall = new Wall();
		bottomWall->model_ = model.at(0);
		bottomWall->worldTransform_.Initialize();
		bottomWall->worldTransform_.translation_ = { float(i) * 15.0f ,-height_ - 5.0f ,0.0f };
		bottomWall->worldTransform_.UpdateMatrix();
		bottomWall->isAlive_ = true;
		bottomWalls_.emplace_back(bottomWall);
	}

	UpdateMatrix();
}

void Frame::Update() {
	for (size_t i = 0; i < topWalls_.size(); i++) {
		if (!IsInsideFrustum(Sphere(topWalls_.at(i)->worldTransform_.translation_, 5.0f), *viewProjection_)) {
			topWalls_.at(i)->isAlive_ = false;
			bottomWalls_.at(i)->isAlive_ = false;
		}
		else {
			topWalls_.at(i)->isAlive_ = true;
			bottomWalls_.at(i)->isAlive_ = true;
		}
	}
	Debug();
}

void Frame::Draw(const ViewProjection& viewProjection) {
	for (size_t i = 0; i < topWalls_.size(); i++) {
		if (topWalls_.at(i)->isAlive_) {
			topWalls_.at(i)->model_->Draw(topWalls_.at(i)->worldTransform_, viewProjection);
			bottomWalls_.at(i)->model_->Draw(bottomWalls_.at(i)->worldTransform_, viewProjection);
		}
	}
}

void Frame::Debug() {
	ImGui::Begin("Frame");
	ImGui::SliderFloat("width", &width_, 20.0f, 1000.0f);
	ImGui::SliderFloat("height", &height_, 20.0f, 100.0f);
	ImGui::End();
	UpdateMatrix();
}

void Frame::UpdateMatrix() {
	player_->SetHeight(height_);
	player_->SetWidth(width_);
	uvula_->SetWidth(width_);
}
