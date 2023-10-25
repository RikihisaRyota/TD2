#include "EnemyBullet.h"

#include "CollisionManager.h"
#include "Draw.h"
#include "MyMath.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, float radius) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	radius_ = radius;
	float scale = radius * 0.5f;
	worldTransform_.scale_ = {scale, scale, scale};
	worldTransform_.UpdateMatrix();
	isAlive_ = true;
	HitBoxInitialize();
}

void EnemyBullet::Update() {
	worldTransform_.translation_.y -= 0.1f;
	worldTransform_.UpdateMatrix();
	if (!IsInsideFrustum(sphere_, *viewProjection_)) {
		isAlive_ = false;
	}
	HitBoxUpdate();
	if (!IsInsideFrustum(sphere_, *viewProjection_)) {
		isAlive_ = false;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

void EnemyBullet::Reset() {
	delete this;
}

void EnemyBullet::OnCollision(uint32_t type, Sphere* sphere) {
	switch (type) {
	case static_cast<size_t>(CollisionManager::Type::kPlayerVSEnemy):
		break;
	case static_cast<size_t>(CollisionManager::Type::kPlayerVSEnemyBullet):
		isAlive_ = false;
		break;
	case static_cast<size_t>(CollisionManager::Type::kPlayerVSBoss):
		break;
	case static_cast<size_t>(CollisionManager::Type::kPlayerBulletVSEnemy):
		break;
	case static_cast<size_t>(CollisionManager::Type::kPlayerBulletVSEnemyBullet):
		break;
	case static_cast<size_t>(CollisionManager::Type::kEnemyVSEnemy):
		break;
	case static_cast<size_t>(CollisionManager::Type::kEnemyVSEnemyBullet):
		isAlive_ = false;
		break;
	}
}

void EnemyBullet::HitBoxInitialize() {
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemyBullet);
	// 衝突対象を自分以外に設定
	SetCollisionMask(~kCollisionAttributeEnemyBullet);
	// Sphere
	sphere_ = {
		.center_{worldTransform_.translation_},
		.radius_{radius_ },
	};
}

void EnemyBullet::HitBoxUpdate() {
	// Sphere
	sphere_ = {
		.center_{worldTransform_.translation_},
		.radius_{radius_ },
	};
}

void EnemyBullet::HitBoxDraw(const ViewProjection& viewProjection) {
	DrawSphere(sphere_, viewProjection, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
}
