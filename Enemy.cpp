#include "Enemy.h"
#include "ImGuiManager.h"


#include "CollisionManager.h"
#include "Draw.h"
#include "MyMath.h"

void Enemy::Initialize(const std::vector<Model*>& type0, const std::vector<Model*>& type1, const Vector3& position, uint32_t type) {
	models_type0_ = type0;
	models_type1_ = type1;
	type_ = type;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	float scale = radius_ * 0.5f;
	worldTransform_.scale_ = { scale ,scale ,scale };
	worldTransform_.UpdateMatrix();

	worldTransform_type0_[kHead].Initialize();
	worldTransform_type0_[kHead].scale_ = { scale ,scale ,scale };
	worldTransform_type0_[kHead].rotation_.y = 9.5f;
	worldTransform_type0_[kLeg].Initialize();
	worldTransform_type0_[kLeg].parent_ = &worldTransform_type0_[kHead];
	worldTransform_type1_[kBody].Initialize();
	worldTransform_type1_[kBody].scale_ = { scale ,scale ,scale };
	worldTransform_type1_[kBody].rotation_.y = 9.6f;
	worldTransform_type1_[kPrick].Initialize();
	worldTransform_type1_[kPrick].parent_ = &worldTransform_type1_[kBody];

	times_.clear();
	for (int i = 0; i < kCount; i++) {
		uint32_t time = 0;
		times_.push_back(time);
	}

	HitBoxInitialize();

	input_ = Input::GetInstance();
	isAlive_ = true;
	isDrawing_ = true;
	EnemyCreateFlag = false;
}

void Enemy::Update() {
	if (isAlive_) {
		EnemyCreateFlag = false;
		if (input_->PushKey(DIK_0)) {
			behaviorRequest_ = Behavior::kStandby;
		}
		if (input_->PushKey(DIK_1)) {
			behaviorRequest_ = Behavior::kShot;
		}
		if (input_->ExitKey(DIK_2)) {
			behaviorRequest_ = Behavior::kSplit;
		}
		if (input_->PushKey(DIK_3)) {
			behaviorRequest_ = Behavior::kDamage;
		}
		if (input_->PushKey(DIK_4)) {
			behaviorRequest_ = Behavior::kCling;
		}
		if (input_->ExitKey(DIK_5)) {
			behaviorRequest_ = Behavior::kGrow;
		}
		BehaviorRequestCheck();
		switch (behavior_) {
		case Behavior::kStandby:
			StandbyUpdate();
			break;
		case Behavior::kShot:
			ShotUpdate();
			break;
		case Behavior::kSplit:
			SplitUpdate();
			break;
		case Behavior::kDamage:
			DamageUpdate();
			break;
		case Behavior::kCling:
			ClingUpdate();
			break;
		case Behavior::kGrow:
			GrowUpdate();
			break;
		}

		if (!IsInsideFrustum(sphere_, *viewProjection_)) {
			isDrawing_ = false;
		}
		else {
			isDrawing_ = true;
		}
	}
	
	worldTransform_.UpdateMatrix();
	HitBoxUpdate();

	if (player_->GetIsLanding()) {
		isAlive_ = true;
		isDrawing_ = true;
		radius_ = InitialRadius_;
		worldTransform_type0_[kHead].scale_ = { radius_ * 0.5f ,radius_ * 0.5f ,radius_ * 0.5f };
		worldTransform_type1_[kBody].scale_ = { radius_ * 0.5f ,radius_ * 0.5f ,radius_ * 0.5f };

	}


	// 演出用
	worldTransform_type0_[kHead].translation_ = worldTransform_.translation_;
	worldTransform_type0_[kHead].UpdateMatrix();
	worldTransform_type0_[kLeg].UpdateMatrix();

	worldTransform_type1_[kBody].translation_ = worldTransform_.translation_;
	worldTransform_type1_[kBody].UpdateMatrix();
	worldTransform_type1_[kPrick].UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	if (type_ == static_cast<size_t>(EnemyType::kOctopus)) {
		models_type0_[kHead]->Draw(worldTransform_type0_[kHead], viewProjection);
		models_type0_[kLeg]->Draw(worldTransform_type0_[kLeg], viewProjection);
	}
	else if (type_ == static_cast<size_t>(EnemyType::kSpike)) {
		models_type1_[kBody]->Draw(worldTransform_type1_[kBody], viewProjection);
		models_type1_[kPrick]->Draw(worldTransform_type1_[kPrick], viewProjection);
	}
}


void Enemy::SetMatWorld() {
	worldTransform_type0_[kHead].translation_ = worldTransform_.translation_;
	worldTransform_type0_[kHead].UpdateMatrix();
	worldTransform_type0_[kLeg].translation_ = worldTransform_.translation_;
	worldTransform_type0_[kLeg].UpdateMatrix();

	worldTransform_type1_[kBody].translation_ = worldTransform_.translation_;
	worldTransform_type1_[kBody].UpdateMatrix();
}

void Enemy::OnCollision(uint32_t type, Sphere* sphere) {
	switch (type) {
	case static_cast<size_t>(CollisionManager::Type::kPlayerVSEnemy):
	{
		if (player_->GetIsPulling()) {
 			isAlive_ = false;
			isDrawing_ = false;
		}
	}
	break;
	case static_cast<size_t>(CollisionManager::Type::kPlayerVSEnemyBullet):
	{

	}
	break;
	case static_cast<size_t>(CollisionManager::Type::kPlayerBulletVSEnemy):
	{
		behaviorRequest_ = Behavior::kGrow;
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
		if (type_ == static_cast<uint32_t>(EnemyType::kOctopus)) {
			behaviorRequest_ = Behavior::kGrow;
		}
	}
	break;
	default:
		break;
	}
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
		.radius_{radius_},
	};
}

void Enemy::HitBoxDraw(const ViewProjection& viewProjection) {
	DrawSphere(sphere_, viewProjection, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
}

void Enemy::BehaviorRequestCheck()
{
	if (behaviorRequest_) {
		// ふるまいを変更
		behavior_ = behaviorRequest_.value();
		// 各ふるまいごとの初期化を実行
		switch (behavior_) {
		case Behavior::kStandby:
			StandbyInitialize();
			break;
		case Behavior::kShot:
			ShotInitialize();
			break;
		case Behavior::kSplit:
			SplitInitialize();
			break;
		case Behavior::kDamage:
			DamageInitialize();
			break;
		case Behavior::kCling:
			ClingInitialize();
			break;
		case Behavior::kGrow:
			GrowInitialize();
			break;
		}
		// ふるまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
}

void Enemy::StandbyInitialize()
{
	times_[Behavior::kStandby] = 0;
}

void Enemy::ShotInitialize()
{
	times_[Behavior::kShot] = 0;
	easeTime_ = 0;
}

void Enemy::SplitInitialize()
{
	times_[Behavior::kSplit] = 0;
	radius_ = InitialRadius_;
	worldTransform_type0_[kHead].scale_ = { radius_ * 0.5f ,radius_ * 0.5f ,radius_ * 0.5f };
	worldTransform_type1_[kBody].scale_ = { radius_ * 0.5f ,radius_ * 0.5f ,radius_ * 0.5f };
}

void Enemy::DamageInitialize()
{
	times_[Behavior::kDamage] = 0;
}

void Enemy::ClingInitialize()
{
	times_[Behavior::kCling] = 0;
}

void Enemy::GrowInitialize()
{
	times_[Behavior::kGrow] = 0;
}

void Enemy::StandbyUpdate()
{
	if (type_ == static_cast<uint32_t>(EnemyType::kOctopus)) {
		if (isDrawing_) {
			times_[Behavior::kStandby]++;
			if (times_[Behavior::kStandby] >= ShotTime_ * radius_) {
				behaviorRequest_ = Behavior::kShot;
				times_[Behavior::kStandby] = 0;
			}
		}
	}
	else if (type_ == static_cast<uint32_t>(EnemyType::kSpike)) {

	}
}

void Enemy::ShotUpdate()
{
	if (type_ == static_cast<uint32_t>(EnemyType::kOctopus)) {
		if (!player_->GetIsPulling()) {
			times_[Behavior::kShot]++;

			if (times_[Behavior::kShot] == 20) {
				enemyBulletManager_->CreateBullet(
					{ worldTransform_.translation_.x, worldTransform_.translation_.y - (2.0f * radius_), worldTransform_.translation_.z },
					radius_);
				worldTransform_type0_[kLeg].translation_.y = 0.0f;
				easeMax_ = radius_ * 0.5f;
				easeMin_ = worldTransform_type0_[kHead].scale_.x;
			}
			else if (times_[Behavior::kShot] > 20) {
				float easedT = 1.0f - std::powf(1.0f - easeTime_, 3.0f);
				float scale = (1.0f - easedT) * easeMin_ + easedT * easeMax_;
				worldTransform_type0_[kHead].scale_ = { scale, scale, scale };
				if (easeTime_ < 1.0f) {
					easeTime_ += 0.01f;
				}
				else {
					behaviorRequest_ = Behavior::kStandby;
				}
			}
			else if (times_[Behavior::kShot] < 20) {
				worldTransform_type0_[kHead].scale_ += {0.01f, 0.01f, 0.01f};
				worldTransform_type0_[kLeg].translation_.y += 0.01f;
				worldTransform_type0_[kLeg].rotation_.y += 1.0f;
		
			}

		}
	}
	else {
		behaviorRequest_ = Behavior::kStandby;
	}
}

void Enemy::SplitUpdate()
{
	EnemyCreateFlag = true;
	if (type_ == static_cast<uint32_t>(EnemyType::kOctopus)) {
		float degree = float(rand() / 360);
		splitPos_ = {
			.x{cosf(degree) * 10.0f},
			.y{sinf(degree) * 10.0f},
			.z{0}
		};
		Vector3 Center = worldTransform_.translation_;
		worldTransform_.translation_ = { splitPos_ + Center };
		splitPos_ *= -1.0f;
		splitPos_ += Center;
	}
	else {
		splitPos_ = worldTransform_.translation_;
		worldTransform_.translation_.y += 5.0f;
		splitPos_.y  -= 5.0f;
	}
	behaviorRequest_ = Behavior::kStandby;
}

void Enemy::DamageUpdate()
{
	worldTransform_type0_[kHead].rotation_.z += 0.1f;
	worldTransform_type1_[kBody].rotation_.z += 0.1f;
	if (times_[Behavior::kDamage] == DamageTime_) {
		worldTransform_type0_[kHead].rotation_.z = 0.0f;
		worldTransform_type1_[kBody].rotation_.z = 0.0f;
		behaviorRequest_ = Behavior::kStandby;
	}
	times_[Behavior::kDamage]++;
}

void Enemy::ClingUpdate()
{


}

void Enemy::GrowUpdate()
{
	if (radius_ < 5.0f) {
		radius_ += 1.0f;
		if (type_ == static_cast<uint32_t>(EnemyType::kOctopus)) {
			worldTransform_type0_[kHead].scale_ = { radius_ * 0.5f ,radius_ * 0.5f ,radius_ * 0.5f };
		}
		else if (type_ == static_cast<uint32_t>(EnemyType::kSpike)) {
			worldTransform_type1_[kBody].scale_ = { radius_ * 0.5f ,radius_ * 0.5f ,radius_ * 0.5f };
		}
		behaviorRequest_ = Behavior::kStandby;
	}
	else {
		behaviorRequest_ = Behavior::kSplit;
	}
}
