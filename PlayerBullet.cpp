#include "PlayerBullet.h"

#include "Draw.h"
#include "MyMath.h"
#include "CollisionManager.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	model_ = model;
	radius_ = 4.0f;
	float scale = radius_ * 0.5f;
	worldTransform_.Initialize();
	worldTransform_.scale_ = { scale ,scale ,scale };
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();
	isAlive_ = true;
	HitBoxInitialize();
}

void PlayerBullet::Update() {
	worldTransform_.translation_.x += kSpeed_;
	worldTransform_.UpdateMatrix();
	HitBoxUpdate();
	if (!IsInsideFrustum(sphere_, *viewProjection_)) {
		isAlive_ = false;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

void PlayerBullet::Reset() {
	delete this;
}

void PlayerBullet::OnCollision(uint32_t type, Sphere* sphere) {
	switch (type) {
	case static_cast<size_t>(CollisionManager::Type::kPlayerVSEnemy):
		break;
	case static_cast<size_t>(CollisionManager::Type::kPlayerVSEnemyBullet):
		break;
	case static_cast<size_t>(CollisionManager::Type::kPlayerVSBoss):
		break;
	case static_cast<size_t>(CollisionManager::Type::kPlayerBulletVSEnemy):
		isAlive_ = false;
		break;
	case static_cast<size_t>(CollisionManager::Type::kPlayerBulletVSEnemyBullet):
		break;
	case static_cast<size_t>(CollisionManager::Type::kEnemyVSEnemy):
		break;
	case static_cast<size_t>(CollisionManager::Type::kEnemyVSEnemyBullet):
		break;
	}
}

void PlayerBullet::HitBoxInitialize() {
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayerBullet);
	// 衝突対象を自分以外に設定
	SetCollisionMask(~kCollisionAttributePlayerBullet);
	// Sphere
	sphere_ = {
		.center_{worldTransform_.translation_},
		.radius_{radius_ },
	};
}

void PlayerBullet::HitBoxUpdate() {
	// Sphere
	sphere_ = {
		.center_{worldTransform_.translation_},
		.radius_{radius_ },
	};
}

void PlayerBullet::HitBoxDraw(const ViewProjection& viewProjection) {
	DrawSphere(sphere_, viewProjection, Vector4(0.0f, 1.0f, 0.0f, 0.0f));
}
