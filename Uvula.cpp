#include "Uvula.h"

#include "Draw.h"
#include "MyMath.h"
#include "Player.h"
#include "CollisionManager.h"

void Uvula::Initialize(Model* head, Model* body) {
	headModel_ = head;
	bodyModel_ = body;
	headWorldTransform_.Initialize();
	bodyWorldTransforms_.Initialize();
	Reset();
	HitBoxInitialize();
}

void Uvula::Reset() {
	headWorldTransform_.translation_ = kInitialPosition_;
	headWorldTransform_.UpdateMatrix();
	bodyWorldTransforms_.scale_ = { 1.0f,2.0f,1.0f };
	bodyWorldTransforms_.rotation_ = { 0.0f,0.0f,0.0f };
	bodyWorldTransforms_.translation_ = kInitialPosition_;
	bodyWorldTransforms_.UpdateMatrix();

	isPlayerChase_ = true;
	HitBoxUpdate();

	isDebug_ = false;
}

void Uvula::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_M)) {
		isDebug_ ^= true;
	}
	if (isDebug_) {
		if (isPlayerChase_) {
			// プレイヤーを追っている最中
			float chase = Lerp(kChaseMin_, kChaseMax_, player_->GetTranslation().x / kWidth_);
			headWorldTransform_.translation_ = Lerp(headWorldTransform_.translation_, player_->GetTranslation(), chase);
			Vector3 distance = headWorldTransform_.translation_ - kInitialPosition_;
			bodyWorldTransforms_.scale_.x = distance.x * 0.5f;
			Vector3 rotate{};
			if (distance.Length() > 0) {
				rotate = Normalize(distance);
			}
			float angle = std::atan2(rotate.y, rotate.x);
			bodyWorldTransforms_.rotation_.z = angle;
			bodyWorldTransforms_.translation_ = distance * 0.5f + kInitialPosition_;
			headWorldTransform_.UpdateMatrix();
			bodyWorldTransforms_.UpdateMatrix();
			HitBoxUpdate();
		}
		else {
			// 地面に着いたときにリセット、プレイヤーを追いかけるフラグはOFF
			if (player_->GetBehavior() == Player::Behavior::kLanding) {
				Reset();
				isPlayerChase_ = false;
			}
			// 引っ張られているとき
			if (player_->GetIsPulling()) {
				headWorldTransform_.translation_ = player_->GetTranslation();
				headWorldTransform_.translation_.x -= 2.0f;
				Vector3 distance = headWorldTransform_.translation_ - kInitialPosition_;
				bodyWorldTransforms_.scale_.x = distance.x * 0.5f;
				Vector3 rotate{};
				if (distance.Length() > 0) {
					rotate = Normalize(distance);
				}
				float angle = std::atan2(rotate.y, rotate.x);
				bodyWorldTransforms_.rotation_.z = angle;
				bodyWorldTransforms_.translation_ = distance * 0.5f + kInitialPosition_;
				headWorldTransform_.UpdateMatrix();
				bodyWorldTransforms_.UpdateMatrix();
			}
			// プレイヤーがMoveに入ったら追いかけるフラグON
			if (player_->GetBehavior() == Player::Behavior::kMove) {
				isPlayerChase_ = true;
			}
		}
	}
	
}

void Uvula::Draw(const ViewProjection& viewProjection) {
	if (!player_->GetIsLanding()) {
		headModel_->Draw(headWorldTransform_, viewProjection);
		bodyModel_->Draw(bodyWorldTransforms_, viewProjection);
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