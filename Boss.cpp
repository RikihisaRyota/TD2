#include "Boss.h"

#include <cassert>

#include "CollisionManager.h"
#include "Draw.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include "Player.h"


Boss::Boss() {}

Boss::~Boss() {
	for (size_t i = 0; i < static_cast<size_t>(Parts::kCount); i++) {
		delete models_.at(i);
	}
	models_.clear();
}

void Boss::Initialize(std::vector<Model*> models) {
	for (size_t i = 0; i < static_cast<size_t>(Parts::kCount); i++) {
		models_.emplace_back(models.at(i));
	}
	animationCount_ = 0;
	HP_ = kHP_;

	worldTransform_.Initialize();
	worldTransform_.scale_ = { 20.0f,20.0f,20.0f };
	worldTransform_.translation_ = { -20.0f,-15.0f,0.0f };
	worldTransform_.UpdateMatrix();
	motion_.Initialize();
	motion_.parent_ = &worldTransform_;
	motion_.UpdateMatrix();
	for (size_t i = 0; i < static_cast<size_t>(Parts::kCount); i++) {
		WorldTransform part;
		part.Initialize();
		part.parent_ = &motion_;
		part.UpdateMatrix();
		parts_.emplace_back(part);
	}
	HitBoxInitialize();
}

void Boss::Update() {
	if (player_->GetBehavior() == Player::Behavior::kLanding) {
		float t = std::clamp(static_cast<float>(animationCount_) / static_cast<float>(kAnimationMax_),0.0f,1.0f);
		if (player_->GetWeightNum() >= HP_) {
			DeathAnimation(t);
		}
		else {
			AttackAnimation(t);
		}
		UpdateMatrix();
		HitBoxUpdate();
		animationCount_++;
		if (animationCount_ >= kAnimationMax_) {
			if (player_->GetWeightNum() >= HP_) {
				player_->SetTranslation(player_->GetInitialPosition());
			}
			else {
				player_->SetTranslation(Vector3(0.0f, -15.0f, 0.0f));
				player_->GetPlayerMove()->SetAcceleration(Vector3(1.5f,0.0f,0.0f));
			}
			player_->SetBehavior(Player::Behavior::kMove);
			Reset();
		}
	}
}

void Boss::Draw(const ViewProjection& viewProjection) {
	for (size_t i = 0; i < static_cast<size_t>(Parts::kCount); i++) {
		models_.at(i)->Draw(parts_.at(i), viewProjection);
	}
}

void Boss::Reset() {
	animationCount_ = 0;
	HP_ = kHP_;
	worldTransform_.scale_ = { 20.0f,20.0f,20.0f };
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.translation_ = { -20.0f,-15.0f,0.0f };
	
	motion_.scale_ = { 1.0f,1.0f,1.0f };
	motion_.rotation_ = { 0.0f,0.0f,0.0f };
	motion_.translation_ = { 0.0f,0.0f,0.0f };
	for (auto& parts : parts_) {
		parts.scale_ = { 1.0f,1.0f,1.0f };
		parts.rotation_ = { 0.0f,0.0f,0.0f };
		parts.translation_ = { 0.0f,0.0f,0.0f };
	}
	UpdateMatrix();
	HitBoxUpdate();
}

void Boss::Debug() {
	ImGui::Begin("Boss");
	float hp = static_cast<float>(kHP_);
	ImGui::SliderFloat("HP", &hp,0.0f,10.0f);
	kHP_ = static_cast<uint32_t>(hp);
	ImGui::End();
}

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
	DrawSphere(sphere_, viewProjection, Vector4(1.0f, 0.0f, 1.0f, 1.0f));
}

void Boss::UpdateMatrix() {
	worldTransform_.UpdateMatrix();
	motion_.UpdateMatrix();
	for (auto& part : parts_) {
		part.UpdateMatrix();
	}
}

void Boss::DeathAnimation(float t) {
	motion_.translation_ = Lerp(Vector3(0.0f,0.0f,0.0f), Vector3(-20.0f, 0, 0.0f), t);
	motion_.translation_ += Vector3(0.0f, rnd.NextFloatRange(-1.0f, 1.0f), 0.0f);
	motion_.rotation_.z += 0.1f;
}

void Boss::AttackAnimation(float t) {
	parts_.at(static_cast<size_t>(Parts::kOnJaw)).rotation_.z = Lerp(0.0f, -DegToRad(20.0f),t);
	parts_.at(static_cast<size_t>(Parts::kLowerJaw)).rotation_.z = Lerp(0.0f, DegToRad(20.0f),t);
}
