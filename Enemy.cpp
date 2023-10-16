#include "Enemy.h"
#include "ImGuiManager.h"

#include "Draw.h"

void Enemy::Initialize(Model* model, const Vector3& position, uint32_t type) {
	assert(model);
	model_ = model;

	type_ = type;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.rotation_ = { 0.0f,11.0f,0.0f };
	worldTransform_.UpdateMatrix();
	HitBoxInitialize();
}

void Enemy::Update() {
	worldTransform_.UpdateMatrix();
	HitBoxUpdate();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

void Enemy::OnCollision(uint32_t type, Sphere* sphere) {

}

void Enemy::HitBoxInitialize() {
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	// 衝突対象を自分以外に設定
	SetCollisionMask(~kCollisionAttributeEnemy);
	// Sphere
	sphere_ = {
		.center_{worldTransform_.translation_},
		.radius_{radius_ },
	};
}

void Enemy::HitBoxUpdate() {
	// Sphere
	sphere_ = {
		.center_{worldTransform_.translation_},
		.radius_{radius_ },
	};
}

void Enemy::HitBoxDraw(const ViewProjection& viewProjection) {
	DrawSphere(sphere_, viewProjection,Vector4(1.0f,0.0f,0.0f,1.0f));
}
