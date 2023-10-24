#include "Boss.h"

#include <cassert>

#include "CollisionManager.h"
#include "Draw.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include "Player.h"


Boss::Boss() {}

Boss::~Boss() {

}

void Boss::Initialize(std::vector<Model*> models) {
	for (size_t i = 0; i < static_cast<size_t>(Parts::kCount); i++) {
		models_.emplace_back(models.at(i));
	}
	isClear_ = false;
	isAnimation_ = false;
	isRespawn_ = false;
	animationCount_ = 0;

	worldTransform_.Initialize();
	worldTransform_.scale_ = { 20.0f,20.0f,20.0f };
	worldTransform_.translation_ = { -20.0f,-10.0f,0.0f };
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
	Reset();
	HitBoxInitialize();
}

void Boss::Update() {
	if (player_->GetBehavior() == Player::Behavior::kLanding) {
		if (player_->GetWeightNum() >= HP_) {
			DeathAnimation();
		}
		else {
			AttackAnimation();
		}
		UpdateMatrix();
		HitBoxUpdate();
		// アニメーション終わり
		if (isAnimation_) {
			if (player_->GetWeightNum() >= HP_) {
				player_->SetTranslation(player_->GetInitialPosition());
				player_->SetBehavior(Player::Behavior::kMove);
				HP_ -= player_->GetWeightNum();
			}
			else {
				player_->SetTranslation(Vector3(0.0f, -15.0f, 0.0f));
				player_->SetBehavior(Player::Behavior::kMove);
				player_->GetPlayerMove()->SetAcceleration(Vector3(1.5f, 0.0f, 0.0f));
				player_->GetPlayerMove()->SetIsEating(true);
				player_->GetPlayerMove()->SetRotateVelocity(50.0f);
				player_->SubtractionPlayerHP();
			}
			Reset();
		}
	}
}

void Boss::Draw(const ViewProjection& viewProjection) {
	switch (bossType_) {
	case Boss::Type::kFirstBoss:
		models_.at(static_cast<size_t>(Parts::kShellfishUp))->Draw(parts_.at(static_cast<size_t>(Parts::kShellfishUp)), viewProjection);
		models_.at(static_cast<size_t>(Parts::kShellfishDown))->Draw(parts_.at(static_cast<size_t>(Parts::kShellfishDown)), viewProjection);
		break;
	case Boss::Type::kMiddleBoss:
		models_.at(static_cast<size_t>(Parts::kOnJaw))->Draw(parts_.at(static_cast<size_t>(Parts::kOnJaw)), viewProjection);
		models_.at(static_cast<size_t>(Parts::kLowerJaw))->Draw(parts_.at(static_cast<size_t>(Parts::kLowerJaw)), viewProjection);
		break;
	case Boss::Type::kLastBoss:
		break;
	}
}

void Boss::Reset() {
	isAnimation_ = false;
	isRespawn_ = false;
	animationCount_ = 0;
	switch (bossType_) {
	case Boss::kFirstBoss:
		HP_ = kFirstBossHP_;
		break;
	case Boss::kMiddleBoss:
		HP_ = kMiddleBossHP_;
		break;
	case Boss::kLastBoss:
		HP_ = kLastBossHP_;
		break;
	}
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
	float hp = static_cast<float>(kFirstBossHP_);
	ImGui::SliderFloat("FirstBossHP", &hp, 0.0f, 10.0f);
	kFirstBossHP_ = static_cast<uint32_t>(hp);
	hp = static_cast<float>(kMiddleBossHP_);
	ImGui::SliderFloat("MiddleBossHP", &hp, 0.0f, 20.0f);
	kMiddleBossHP_ = static_cast<uint32_t>(hp);
	hp = static_cast<float>(kLastBossHP_);
	ImGui::SliderFloat("LastBossHP", &hp, 0.0f, 30.0f);
	kLastBossHP_ = static_cast<uint32_t>(hp);
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

void Boss::DeathAnimation() {
	animationCount_++;
	if (animationCount_ >= kAnimationMax_) {
		if (isRespawn_) {
			isAnimation_ = true;
		}
		else {
			switch (bossType_) {
			case Boss::kFirstBoss:
				bossType_ = Boss::kMiddleBoss;
				break;
			case Boss::kMiddleBoss:
				bossType_ = Boss::kFirstBoss;
				break;
			case Boss::kLastBoss:
				isClear_ = true;
				break;
			case Boss::kCount:
				break;
			default:
				break;
			}
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
			animationCount_ = 0;
			isRespawn_ = true;
		}
	}
	float t = std::clamp(static_cast<float>(animationCount_) / static_cast<float>(kAnimationMax_), 0.0f, 1.0f);
	if (!isRespawn_) {
		motion_.translation_ = Lerp(Vector3(0.0f, 0.0f, 0.0f), Vector3(-20.0f, 0, 0.0f), t);
		motion_.translation_ += Vector3(0.0f, rnd.NextFloatRange(-1.0f, 1.0f), 0.0f);
		motion_.rotation_.z += 0.1f;
	}
	else {
		motion_.translation_ = Lerp(Vector3(-20.0f, 0.0f, 0.0f), Vector3(0.0f, 0, 0.0f), t);
		motion_.translation_ += Vector3(0.0f, rnd.NextFloatRange(-1.0f, 1.0f), 0.0f);
	}
}

void Boss::AttackAnimation() {
	float t = std::clamp(static_cast<float>(animationCount_) / static_cast<float>(kAnimationMax_), 0.0f, 1.0f);
	switch (bossType_) {
	case Boss::Type::kFirstBoss:
		parts_.at(static_cast<size_t>(Parts::kShellfishUp)).rotation_.z = Lerp(0.0f, DegToRad(30.0f), t);
		parts_.at(static_cast<size_t>(Parts::kShellfishDown)).rotation_.z = Lerp(0.0f, -DegToRad(30.0f), t);
		break;
	case Boss::Type::kMiddleBoss:
		parts_.at(static_cast<size_t>(Parts::kOnJaw)).rotation_.z = Lerp(0.0f, -DegToRad(30.0f), t);
		parts_.at(static_cast<size_t>(Parts::kLowerJaw)).rotation_.z = Lerp(0.0f, DegToRad(30.0f), t);
		break;
	case Boss::Type::kLastBoss:
		break;
	}
	animationCount_++;
	if (animationCount_ >= kAnimationMax_) {
		isAnimation_ = true;
	}
}
