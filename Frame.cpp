#include "Frame.h"
#include "Frame.h"

#include <assert.h>

#include "Draw.h"
#include "EnemyManager.h"
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
	for (auto& wall : rightWalls_) {
		delete  wall;
	}
}

void Frame::Initialize(std::vector<Model*>model, bool isInGame) {
	width_ = 1000.0f;
	height_ = 50.0f;
	const float kDispersionInterval = 2.0f;
	const float kDispersionRotate = 5.0f;
	isInGame_ = isInGame;
	// 床
	for (int i = 0; i < width_ / 24.0f; i++) {
		Wall* topWall = new Wall();
		if (rnd.NextIntLimit() % 4 == 0) {
			topWall->model_ = model.at(0);
		}
		else {
			topWall->model_ = model.at(1);
		}
		topWall->worldTransform_.Initialize();
		topWall->worldTransform_.translation_ = { (float(i - 5)) * kRockInterval_ + rnd.NextFloatRange(-kDispersionInterval,kDispersionInterval) ,height_ + kRockFrameDistance_ ,0.0f };
		topWall->worldTransform_.rotation_.z = DegToRad(180.0f);
		topWall->worldTransform_.rotation_.z += DegToRad(rnd.NextFloatRange(-kDispersionRotate, kDispersionRotate));
		if (rnd.NextIntLimit() % 3 == 0) {
			topWall->worldTransform_.rotation_.y = DegToRad(180.0f);
		}
		topWall->worldTransform_.UpdateMatrix();
		topWall->isAlive_ = true;
		topWalls_.emplace_back(topWall);
		Wall* bottomWall = new Wall();
		if (rnd.NextIntLimit() % 4 == 0) {
			bottomWall->model_ = model.at(1);
		}
		else {
			bottomWall->model_ = model.at(0);
		}
		bottomWall->worldTransform_.Initialize();
		bottomWall->worldTransform_.translation_ = { (float(i - 5)) * kRockInterval_ + rnd.NextFloatRange(-kDispersionInterval,kDispersionInterval) ,-height_ - kRockFrameDistance_ ,0.0f };
		bottomWall->worldTransform_.rotation_.z += DegToRad(rnd.NextFloatRange(-kDispersionRotate, kDispersionRotate));
		if (rnd.NextIntLimit() % 3 == 0) {
			bottomWall->worldTransform_.rotation_.y = DegToRad(180.0f);
		}
		bottomWall->worldTransform_.UpdateMatrix();
		bottomWall->isAlive_ = true;
		bottomWalls_.emplace_back(bottomWall);
	}
	for (int i = 0; i < height_ / 24.0f; i++) {
		Wall* rightWall = new Wall();
		if (rnd.NextIntLimit() % 2 == 0) {
			rightWall->model_ = model.at(1);
		}
		else {
			rightWall->model_ = model.at(0);
		}
		rightWall->worldTransform_.Initialize();
		rightWall->worldTransform_.translation_ = { width_ + kRock_X,float(i - 1) * kRock_Y  ,0.0f };
		rightWall->worldTransform_.rotation_.z += DegToRad(90.0f);
		rightWall->worldTransform_.rotation_.z += DegToRad(rnd.NextFloatRange(-kDispersionRotate, kDispersionRotate));
		rightWall->worldTransform_.UpdateMatrix();
		rightWall->isAlive_ = true;
		rightWalls_.emplace_back(rightWall);
	}

	if (isInGame_) {
		UpdateMatrix();
	}
}

void Frame::Update() {
	for (size_t i = 0; i < topWalls_.size(); i++) {
		if (!IsInsideFrustum(Sphere(topWalls_.at(i)->worldTransform_.translation_, 20.0f), *viewProjection_)) {
			topWalls_.at(i)->isAlive_ = false;
			bottomWalls_.at(i)->isAlive_ = false;
		}
		else {
			topWalls_.at(i)->isAlive_ = true;
			bottomWalls_.at(i)->isAlive_ = true;
		}
	}
	for (size_t i = 0; i < rightWalls_.size(); i++) {
	  	if (!IsInsideFrustum(Sphere(rightWalls_.at(i)->worldTransform_.translation_, 20.0f), *viewProjection_)) {
			rightWalls_.at(i)->isAlive_ = false;
		}
		else {
			rightWalls_.at(i)->isAlive_ = true;
		}
	}
	if (isInGame_) {
		Debug();
	}
}

void Frame::Draw(const ViewProjection& viewProjection) {
	for (size_t i = 0; i < topWalls_.size(); i++) {
		if (topWalls_.at(i)->isAlive_) {
			topWalls_.at(i)->model_->Draw(topWalls_.at(i)->worldTransform_, viewProjection);
			bottomWalls_.at(i)->model_->Draw(bottomWalls_.at(i)->worldTransform_, viewProjection);
		}
	}
	for (size_t i = 0; i < rightWalls_.size(); i++) {
		if (rightWalls_.at(i)->isAlive_) {
			rightWalls_.at(i)->model_->Draw(rightWalls_.at(i)->worldTransform_, viewProjection);
		}
	}
}

void Frame::Debug() {
	ImGui::Begin("Frame");
	ImGui::SliderFloat("width", &width_, 20.0f, 1000.0f);
	ImGui::SliderFloat("height", &height_, 20.0f, 100.0f);
	ImGui::SliderFloat("RockInterval", &kRockInterval_, 15.0f, 50.0f);
	ImGui::SliderFloat("RockFrameDistance", &kRockFrameDistance_, 0.0f, 50.0f);
	ImGui::SliderFloat("RockScale", &kRockScale_, 0.0f, 15.0f);
	ImGui::SliderFloat("kRock_X ", &kRock_X, 0.0f, 100.0f);
	ImGui::SliderFloat("kRock_Y  ", &kRock_Y, 0.0f, 100.0f);
	ImGui::End();
	UpdateMatrix();
}

void Frame::UpdateMatrix() {
	for (int i = 0; i < topWalls_.size(); i++) {
		topWalls_.at(i)->worldTransform_.translation_ = { float(i - 5) * kRockInterval_ ,height_ + kRockFrameDistance_ ,0.0f };
		topWalls_.at(i)->worldTransform_.scale_ = { kRockScale_ ,kRockScale_ ,kRockScale_ };
		topWalls_.at(i)->worldTransform_.UpdateMatrix();
		bottomWalls_.at(i)->worldTransform_.translation_ = { float(i - 5) * kRockInterval_ ,-height_ - kRockFrameDistance_ ,0.0f };
		bottomWalls_.at(i)->worldTransform_.scale_ = { kRockScale_ ,kRockScale_ ,kRockScale_ };
		bottomWalls_.at(i)->worldTransform_.UpdateMatrix();
	}
	for (int t = 0; t < rightWalls_.size(); t++) {
		rightWalls_.at(t)->worldTransform_.translation_ = { width_ + kRock_X,float(t - 1) * kRock_Y  ,0.0f };
		rightWalls_.at(t)->worldTransform_.scale_ = { kRockScale_ ,kRockScale_ ,kRockScale_ };
		rightWalls_.at(t)->worldTransform_.UpdateMatrix();
	}
	player_->SetHeight(height_);
	player_->SetWidth(width_);
	enemyManager_->SetHeight(height_);
	enemyManager_->SetWidth(width_);
	uvula_->SetWidth(width_);
}
