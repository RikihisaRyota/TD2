#include "Enemy.h"
#include "ImGuiManager.h"

void Enemy::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	obb_.center_ = position;
	obb_.size_ = { 0.01f,0.01f,0.01f };
}

void Enemy::Update() {
	ImGui::Begin("Enemy");
	ImGui::DragFloat3("position", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragInt("flag", &isCombined_, 1);
	ImGui::End();

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_,viewProjection);
}