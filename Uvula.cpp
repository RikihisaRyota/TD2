#include "Uvula.h"

#include "Draw.h"
#include "MyMath.h"
#include "Player.h"
#include "CollisionManager.h"

void Uvula::Initialize(Model* head, Model* body) {
	headModel_ = head;
	bodyModel_ = body;
	Reset();
	HitBoxInitialize();
}

void Uvula::Reset() {
	headWorldTransform_.Initialize();
	headWorldTransform_.translation_.x = -10.0f;
	headWorldTransform_.UpdateMatrix();
	createModelCount_ = 0;
	bodyModels_.clear();
	bodyWorldTransforms_.clear();
	angle_ = 0.0f;
	isPlayerChase_ = true;
	HitBoxUpdate();
}

void Uvula::Update() {
	if (isPlayerChase_) {
		// プレイヤーを追っている最中
		angle_ += 0.1f;
		float chase = Lerp(kChaseMin_, kChaseMax_, player_->GetTranslation().x / kWidth_);
		headWorldTransform_.translation_ = Lerp(headWorldTransform_.translation_, player_->GetTranslation(), chase);
		if (createModelCount_ >= kCreateModelInterval_) {
			WorldTransform bodyWorldTransform{};
			bodyWorldTransform.Initialize();
			bodyWorldTransform.translation_ = headWorldTransform_.translation_;
			bodyWorldTransform.UpdateMatrix();
			bodyWorldTransforms_.emplace_back(bodyWorldTransform);
			bodyModels_.emplace_back(bodyModel_);
			createModelCount_ = 0;
		}
		headWorldTransform_.UpdateMatrix();
		HitBoxUpdate();
		createModelCount_++;
	}
	else {
		// 地面に着いたときにリセット、プレイヤーを追いかけるフラグはOFF
		if (player_->GetBehavior() == Player::Behavior::kLanding) {
			Reset();
			isPlayerChase_ = false;
		}
		// 引っ張られているとき
		if(player_->GetBehavior() == Player::Behavior::kPullingMove ||
			player_->GetBehavior() == Player::Behavior::kJump ||
			player_->GetBehavior() == Player::Behavior::kString) {
			headWorldTransform_.translation_ = player_->GetTranslation();
			headWorldTransform_.translation_.x -= 2.0f;
			headWorldTransform_.UpdateMatrix();
		}
		// プレイヤーがMoveに入ったら追いかけるフラグON
		if (player_->GetBehavior() == Player::Behavior::kMove) {
			isPlayerChase_ = true;
		}
	}
}

void Uvula::Draw(const ViewProjection& viewProjection) {
	headModel_->Draw(headWorldTransform_, viewProjection);
	for (size_t i = 0; i < bodyModels_.size(); i++) {
		bodyModels_.at(i)->Draw(bodyWorldTransforms_.at(i), viewProjection);
	}
}

void Uvula::OnCollision(uint32_t type, Sphere* sphere) {
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
		Reset();
		isPlayerChase_ = false;
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

void Uvula::HitBoxInitialize() {
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeBoss);
	// 衝突対象を自分以外に設定
	SetCollisionMask(~kCollisionAttributeBoss);
	// Sphere
	sphere_ = {
		.center_{headWorldTransform_.translation_},
		.radius_{radius_ },
	};
}

void Uvula::HitBoxUpdate() {
	// Sphere
	sphere_ = {
		.center_{headWorldTransform_.translation_},
		.radius_{radius_ },
	};
}

void Uvula::HitBoxDraw(const ViewProjection& viewProjection) {
	DrawSphere(sphere_, viewProjection, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
}