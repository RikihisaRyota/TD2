#include "Enemy.h"
#include "ImGuiManager.h"

void Enemy::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.rotation_ = { 0.0f,11.0f,0.0f };
}

void Enemy::Update() {
	obb_.center_ = worldTransform_.translation_;
	GetOrientations(MakeRotateXYZMatrix(worldTransform_.rotation_), obb_.orientations_);
	obb_.size_ = worldTransform_.scale_;

	ImGui::Begin("Enemy");
	ImGui::DragFloat3("position", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragInt("flag", &isHit_, 1);
	ImGui::End();

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_,viewProjection);
}

void Enemy::SetPosition(const Vector3& position) { 
	worldTransform_.translation_.x = position.x; 
	worldTransform_.translation_.y = position.y + -2; 
	worldTransform_.translation_.z = position.z; 
}