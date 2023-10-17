#include "Boss.h"

#include <cassert>

#include "CollisionManager.h"
#include "Draw.h"
#include "Player.h"


void Boss::Initialize(Model* model) {
	assert(model);
	model_ = model;
	shakeCount_ = 0;

	worldTransform_.Initialize();
	worldTransform_.scale_ = { 20.0f,20.0f,20.0f };
	worldTransform_.translation_ = {-20.0f,-15.0f,0.0f};
	worldTransform_.UpdateMatrix();
	HitBoxInitialize();
}

void Boss::Update() {
	if (player_->GetBehavior() == Player::Behavior::kLanding) {
		if (shakeCount_ <= kSakeMax_) {
		worldTransform_.translation_.x += rnd.NextFloatRange(-1.0f,1.0f);
		worldTransform_.translation_.y += rnd.NextFloatRange(-1.0f,1.0f);
		shakeCount_++;
		}
		else {
			worldTransform_.translation_ = { -20.0f,-15.0f,0.0f };
			shakeCount_ = 0;
		player_->SetBehavior(Player::Behavior::kMove);
		}
		worldTransform_.UpdateMatrix();
		HitBoxUpdate();
	}
}

void Boss::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_,viewProjection);
}

void Boss::Reset() {}

void Boss::Debug() {}

void Boss::OnCollision(uint32_t type, Sphere* sphere) {
	switch (type) {
	case static_cast<size_t>(CollisionManager::Type::kPlayerVSEnemy):
	{

	}
	break;
	case static_cast<size_t>(CollisionManager::Type::kPlayerVSEnemyBullet):
	{

	}
	break;
	case static_cast<size_t>(CollisionManager::Type::kPlayerVSBoss):
	{
		
	}
	break;
	case static_cast<size_t>(CollisionManager::Type::kPlayerBulletVSEnemy):
	{

	}
	break;
	case static_cast<size_t>(CollisionManager::Type::kPlayerBulletVSEnemyBullet):
	{

	}
	break;
	case static_cast<size_t>(CollisionManager::Type::kEnemyVSEnemy):
	{

	}
	break;
	case static_cast<size_t>(CollisionManager::Type::kEnemyVSEnemyBullet):
	{

	}
	break;
	default:
		break;
	}
}

void Boss::HitBoxInitialize() {
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeBoss);
	// 衝突対象を自分以外に設定
	SetCollisionMask(~kCollisionAttributeBoss);
	// Sphere
	sphere_ = {
		.center_{worldTransform_.translation_},
		.radius_{radius_ },
	};
}

void Boss::HitBoxUpdate() {
	// Sphere
	sphere_ = {
		.center_{worldTransform_.translation_},
		.radius_{radius_ },
	};
}

void Boss::HitBoxDraw(const ViewProjection& viewProjection) {
	DrawSphere(sphere_, viewProjection, Vector4(1.0f,0.0f,1.0f,1.0f));
}
