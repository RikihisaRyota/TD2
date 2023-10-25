#include "Enemy.h"
#include "ImGuiManager.h"


#include "CollisionManager.h"
#include "Draw.h"
#include "MyMath.h"

void Enemy::Initialize(const std::vector<Model*>& type0, const std::vector<Model*>& type1, const std::vector<Model*>& type2, const Vector3& position, uint32_t type) {
	models_type0_ = type0;
	models_type1_ = type1;
	models_type2_ = type2;
	type_ = type;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	radius_ = initialRadius_;
	float scale = radius_ * 0.5f;
	worldTransform_.scale_ = { scale ,scale ,scale };
	worldTransform_.UpdateMatrix();
	maxSize_ = 1.5f;

	worldTransform_type0_Head_.Initialize();
	worldTransform_type0_Head_.scale_ = { scale ,scale ,scale };
	worldTransform_type0_Head_.rotation_.y = 9.5f;
	worldTransform_type0_Leg_.Initialize();
	worldTransform_type0_Leg_.parent_ = &worldTransform_type0_Head_;
	worldTransform_type1_Body_.Initialize();
	worldTransform_type1_Body_.scale_ = { scale ,scale ,scale };
	worldTransform_type1_Body_.rotation_.y = 9.6f;
	worldTransform_type1_Prick_.Initialize();
	worldTransform_type1_Prick_.parent_ = &worldTransform_type1_Body_;
	worldTrasnform_type2_.Initialize();
	worldTrasnform_type2_.scale_ = { scale, scale, scale };

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

void Enemy::Initialize(const std::vector<Model*>& type0, const std::vector<Model*>& type1, const std::vector<Model*>& type2, const Vector3& max, const Vector3& min, uint32_t type)
{
	models_type0_ = type0;
	models_type1_ = type1;
	models_type2_ = type2;
	type_ = type;

	easeMax_Vector3_ = max;
	easeMin_Vector3_ = min;

	worldTransform_.Initialize();

	radius_ = initialRadius_;
	float scale = radius_ * 0.5f;
	maxSize_ = 1.5f;

	worldTransform_type0_Head_.Initialize();
	worldTransform_type0_Head_.scale_ = { scale ,scale ,scale };
	worldTransform_type0_Head_.rotation_.y = 9.5f;
	worldTransform_type0_Leg_.Initialize();
	worldTransform_type0_Leg_.parent_ = &worldTransform_type0_Head_;
	worldTransform_type1_Body_.Initialize();
	worldTransform_type1_Body_.scale_ = { scale ,scale ,scale };
	worldTransform_type1_Body_.rotation_.y = 9.6f;
	worldTransform_type1_Prick_.Initialize();
	worldTransform_type1_Prick_.parent_ = &worldTransform_type1_Body_;
	worldTrasnform_type2_.Initialize();
	worldTrasnform_type2_.scale_ = { scale, scale, scale };

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
	splitFlag_ = true;

	behaviorRequest_ = Behavior::kSplit;

}

void Enemy::Update() {
	if (isAlive_) {
		EnemyCreateFlag = false;
		if (radius_ > initialRadius_ * maxSize_) {
			if (type_ == static_cast<uint32_t>(EnemyType::kOctopus) || type_ == static_cast<uint32_t>(EnemyType::kfeed)) {
				behaviorRequest_ = Behavior::kSplit;
			}
			else {
				behaviorRequest_ = Behavior::kStandby;
			}
		}

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

	// 演出用
	worldTransform_type0_Head_.translation_ = worldTransform_.translation_;
	worldTransform_type0_Head_.UpdateMatrix();
	worldTransform_type0_Leg_.UpdateMatrix();

	worldTransform_type1_Body_.translation_ = worldTransform_.translation_;
	worldTransform_type1_Body_.UpdateMatrix();
	worldTransform_type1_Prick_.UpdateMatrix();

	worldTrasnform_type2_.translation_ = worldTransform_.translation_;
	worldTrasnform_type2_.UpdateMatrix();

	//Debug();

#ifdef DEBUG

	Debug();

#endif // DEBUG


}

void Enemy::Draw(const ViewProjection& viewProjection) {
	if (type_ == static_cast<uint32_t>(EnemyType::kOctopus)) {
		models_type0_[kHead]->Draw(worldTransform_type0_Head_, viewProjection);
		models_type0_[kLeg]->Draw(worldTransform_type0_Leg_, viewProjection);
	}
	else if (type_ == static_cast<uint32_t>(EnemyType::kSpike)) {
		models_type1_[kBody]->Draw(worldTransform_type1_Body_, viewProjection);
		models_type1_[kPrick]->Draw(worldTransform_type1_Prick_, viewProjection);
	}
	else if (type_ == static_cast<uint32_t>(EnemyType::kfeed)) {
		models_type2_[0]->Draw(worldTrasnform_type2_, viewProjection);
	}
}


void Enemy::SetMatWorld() {
	worldTransform_type0_Head_.translation_ = worldTransform_.translation_;
	worldTransform_type0_Head_.UpdateMatrix();
	worldTransform_type0_Leg_.UpdateMatrix();

	worldTransform_type1_Body_.translation_ = worldTransform_.translation_;
	worldTransform_type1_Body_.UpdateMatrix();
	worldTransform_type1_Prick_.UpdateMatrix();

	worldTrasnform_type2_.translation_ = worldTransform_.translation_;
	worldTrasnform_type2_.UpdateMatrix();
}

void Enemy::OnCollision(uint32_t type, Sphere* sphere) {
	switch (type) {
	case static_cast<size_t>(CollisionManager::Type::kPlayerVSEnemy):
	{
		if (player_->GetIsPulling()) {
			isAlive_ = false;
			isDrawing_ = false;
		}
		else {
			if (type_ == static_cast<uint32_t>(EnemyType::kOctopus)) {
				isAlive_ = false;
				isDrawing_ = false;
			}
			if (type_ == static_cast<uint32_t>(EnemyType::kfeed)) {
				isAlive_ = false;
				isDrawing_ = false;
			}
			if (type_ == static_cast<uint32_t>(EnemyType::kSpike)) {
				if (player_->GetBehavior() != Player::Behavior::kStun &&
					!player_->GetInvincible()) {
					player_->SetBehavior(Player::Behavior::kStun);
				}
			}
		}

	}
	break;
	case static_cast<size_t>(CollisionManager::Type::kPlayerVSEnemyBullet):
	{

	}
	break;
	case static_cast<size_t>(CollisionManager::Type::kPlayerBulletVSEnemy):
	{
		if (!splitFlag_) {
			behaviorRequest_ = Behavior::kGrow;
		}
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
		if (!splitFlag_) {
			behaviorRequest_ = Behavior::kGrow;
			if (type_ == static_cast<uint32_t>(EnemyType::kOctopus) || type_ == static_cast<uint32_t>(EnemyType::kfeed)) {
				behaviorRequest_ = Behavior::kGrow;
			}
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

void Enemy::BehaviorRequestCheck() {
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

void Enemy::StandbyInitialize() {
	times_[Behavior::kStandby] = 0;
}

void Enemy::ShotInitialize() {
	times_[Behavior::kShot] = 0;
	easeTime_ = 0;
}

void Enemy::SplitInitialize() {
	times_[Behavior::kSplit] = 0;
	radius_ = initialRadius_;
	worldTransform_type0_Head_.scale_ = { radius_ * 0.5f ,radius_ * 0.5f ,radius_ * 0.5f };
	worldTransform_type1_Body_.scale_ = { radius_ * 0.5f ,radius_ * 0.5f ,radius_ * 0.5f };
	worldTrasnform_type2_.scale_ = { radius_ * 0.5f ,radius_ * 0.5f ,radius_ * 0.5f };
	easeMax_[0] = initialRadius_ * 0.5f;
	easeMin_[0] = 0;
}

void Enemy::DamageInitialize() {
	times_[Behavior::kDamage] = 0;
}

void Enemy::ClingInitialize() {
	times_[Behavior::kCling] = 0;
}

void Enemy::GrowInitialize() {
	times_[Behavior::kGrow] = 0;
  	easeMin_[0] = radius_;
	easeMax_[0] = radius_ + onceUpSize_;
	easeTime_ = 0;
}

void Enemy::StandbyUpdate() {
	if (type_ == static_cast<uint32_t>(EnemyType::kOctopus)) {
		if (isDrawing_) {
			times_[Behavior::kStandby]++;
			if (times_[Behavior::kStandby] >= shotTime_ * radius_) {
				behaviorRequest_ = Behavior::kShot;
				times_[Behavior::kStandby] = 0;
			}
		}
	}
	else if (type_ == static_cast<uint32_t>(EnemyType::kSpike)) {

	}
}

void Enemy::ShotUpdate() {
	if (type_ == static_cast<uint32_t>(EnemyType::kOctopus)) {
		if (!player_->GetIsPulling()) {
			times_[Behavior::kShot]++;

			if (times_[Behavior::kShot] > bulletShotCount_) {
				float easedT = 1.0f - std::powf(1.0f - easeTime_, 3.0f);
				float scale = (1.0f - easedT) * easeMin_[0] + easedT * easeMax_[0];
				worldTransform_type0_Head_.scale_ = { scale, scale, scale };
				if (easeTime_ < 1.0f) {
					easeTime_ += easeSecond_Shot_;
				}
				else {
					enemyBulletManager_->CreateBullet(
						{ worldTransform_.translation_.x, worldTransform_.translation_.y - (2.0f * radius_), worldTransform_.translation_.z },
						radius_);
					worldTransform_type0_Leg_.translation_.y = 0.0f;
					behaviorRequest_ = Behavior::kStandby;
				}
			}
			else if (times_[Behavior::kShot] < bulletShotCount_) {
				worldTransform_type0_Head_.scale_ += {scaleUpValue_Shot_, scaleUpValue_Shot_, scaleUpValue_Shot_};
				worldTransform_type0_Leg_.rotation_.y += rotateValue_Shot_;
				easeMax_[0] = radius_ * 0.5f;
				easeMin_[0] = worldTransform_type0_Head_.scale_.x;
			}

		}
	}
	else {
		behaviorRequest_ = Behavior::kStandby;
	}
}

void Enemy::SplitUpdate() {
	bool check = false;
	if (!splitFlag_) {
		while (!check) {
			EnemyCreateFlag = true;
			splitFlag_ = true;
			easeMax_[0] = initialRadius_ * 0.5f;
			easeMin_[0] = 0;
			if (type_ == static_cast<uint32_t>(EnemyType::kOctopus) || type_ == static_cast<uint32_t>(EnemyType::kfeed)) {
				float degree = float(rand() / 360);
				splitPos_Max_ = {
					.x{cosf(degree) * 15.0f},
					.y{sinf(degree) * 15.0f},
					.z{0}
				};
				Vector3 Center = worldTransform_.translation_;
				easeMax_Vector3_ = { splitPos_Max_ + Center };
				easeMin_Vector3_ = worldTransform_.translation_;
				splitPos_Max_ *= -1.0f;
				splitPos_Max_ += Center;
				splitPos_Min_ = Center;

				if (easeMax_Vector3_.y > height_ || easeMax_Vector3_.y < -height_) {
					check = false;
				}
				else if (splitPos_Max_.y > height_ || splitPos_Max_.y < -height_) {
					check = false;
				}
				else {
					check = true;
				}
				
			}
			else {
				check = true;
			}
		}

	}
	else {

		float easedT = easeTime_ * easeTime_ * easeTime_;
		float radius = (1.0f - easedT) * easeMin_[0] + easeMax_[0] * easedT;
		worldTransform_type0_Head_.scale_ = { radius ,radius ,radius };
		worldTransform_type1_Body_.scale_ = { radius ,radius ,radius };
		Vector3 pos;
 		pos.x = (1.0f - easedT) * easeMin_Vector3_.x + easeMax_Vector3_.x * easedT;
		pos.y = (1.0f - easedT) * easeMin_Vector3_.y + easeMax_Vector3_.y * easedT;
		pos.z = (1.0f - easedT) * easeMin_Vector3_.z + easeMax_Vector3_.z * easedT;
		worldTransform_.translation_ = pos;


		if (easeTime_ < 1.0f) {
			easeTime_ += easeSecond_Split_;
		}
		else {
			behaviorRequest_ = Behavior::kStandby;
			splitFlag_ = false;
		}

	}
}

void Enemy::DamageUpdate() {
	worldTransform_type0_Head_.rotation_.z += 0.1f;
	worldTransform_type1_Body_.rotation_.z += 0.1f;
	if (times_[Behavior::kDamage] == DamageTime_) {
		worldTransform_type0_Head_.rotation_.z = 0.0f;
		worldTransform_type1_Body_.rotation_.z = 0.0f;
		behaviorRequest_ = Behavior::kStandby;
	}
	times_[Behavior::kDamage]++;
}

void Enemy::ClingUpdate() {


}

void Enemy::GrowUpdate() {
	if (radius_ < initialRadius_ * maxSize_) {

		if (type_ == static_cast<uint32_t>(EnemyType::kOctopus)) {
			const float c1 = 1.70158f;
			const float c2 = c1 + 1.0f;
			float easedT = 1 + c2 * std::powf(easeTime_ - 1.0f, 3.0) + c1 * std::powf(easeTime_ - 1.0f, 2.0f);
			float radius = (1.0f - easedT) * easeMin_[0] + easeMax_[0] * easedT;
			radius_ = radius;
			worldTransform_type0_Head_.scale_ = { radius_ * 0.5f ,radius_ * 0.5f ,radius_ * 0.5f };
		}
		else if (type_ == static_cast<uint32_t>(EnemyType::kSpike)) {
			float easedT = easeTime_ * easeTime_;
			float radius = (1.0f - easedT) * easeMin_[0] + easeMax_[0] * easedT;
			radius_ = radius;
			worldTransform_type1_Body_.scale_ = { radius_ * 0.5f ,radius_ * 0.5f ,radius_ * 0.5f };
		}
		else if (type_ == static_cast<uint32_t>(EnemyType::kfeed)) {
			float easedT = easeTime_ * easeTime_;
			float radius = (1.0f - easedT) * easeMin_[0] + easeMax_[0] * easedT;
			radius_ = radius;
			worldTrasnform_type2_.scale_ = { radius_ * 0.5f, radius_ * 0.5f, radius_ * 0.5f };
		}

		if (easeTime_ < 1.0f) {
			easeTime_ += easeSecond_Grow_;
		}
		else {
 			behaviorRequest_ = Behavior::kStandby;
		}
	}
	else {
		if (type_ == static_cast<uint32_t>(EnemyType::kOctopus) || type_ == static_cast<uint32_t>(EnemyType::kfeed)) {
			behaviorRequest_ = Behavior::kSplit;
		}
		else {
			behaviorRequest_ = Behavior::kStandby;
		}
	}
}

