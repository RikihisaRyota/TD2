#include "Enemy.h"
#include "ImGuiManager.h"

void Enemy::Initialize(Model* model, const Vector3& position, uint32_t type) {
	assert(model);
	model_ = model;

	type_ = type;

	worldTransform_.Initialize();

	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.translation_ = position;
}

void Enemy::Update() {
	OBJtoOBB();

	ImGui::Begin("Enemy");
	ImGui::DragFloat3("position", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragInt("flag", &isHit_, 1);
	ImGui::End();

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

void Enemy::OBJtoOBB() {
	// .objをOBBへ変更（当たり判定へ）
	obb_.center_ = worldTransform_.translation_;
	GetOrientations(MakeRotateXYZMatrix(worldTransform_.rotation_), obb_.orientations_);
	obb_.size_ = worldTransform_.scale_;
}

// 敵のポジションをプレイヤーに固定
void Enemy::SetParent(const WorldTransform* parent) { 
	worldTransform_.parent_ = parent;
}
