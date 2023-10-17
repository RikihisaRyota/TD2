#include "Enemy.h"
#include "ImGuiManager.h"


#include "CollisionManager.h"
#include "Draw.h"
#include "EnemyManager.h"
#include "MyMath.h"

void Enemy::Initialize(Model* model, const Vector3& position, uint32_t type) {
	assert(model);
	model_ = model;

	type_ = type;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.rotation_ = { 0.0f,11.0f,0.0f };
	worldTransform_.UpdateMatrix();

	times_.clear();
	for (int i = 0; i < kCount; i++) {
		uint32_t time = 0;
		times_.push_back(time);
	}

	HitBoxInitialize();

	input_ = Input::GetInstance();
}

void Enemy::Update() {
	EnemyCreateFlag = false;
	if (worldTransform_.scale_.x >= 5.0f) {
		behaviorRequest_ = Behavior::kSplit;
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
	worldTransform_.UpdateMatrix();
	HitBoxUpdate();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

void Enemy::OnCollision(uint32_t type, Sphere* sphere) {
	switch (type) {
	case static_cast<size_t>(CollisionManager::Type::kPlayerVSEnemy):
	{

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
		behaviorRequest_ = Behavior::kGrow;
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
		.radius_{worldTransform_.scale_.x },
	};
}

void Enemy::HitBoxUpdate() {
	// Sphere
	sphere_ = {
		.center_{worldTransform_.translation_},
		.radius_{worldTransform_.scale_.x },
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
}

void Enemy::SplitInitialize()
{
	times_[Behavior::kSplit] = 0;
	worldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };
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
		times_[Behavior::kStandby]++;
		if (times_[Behavior::kStandby] == ShotTime_ * worldTransform_.scale_.x) {
			behaviorRequest_ = Behavior::kShot;
		}
	}
}

void Enemy::ShotUpdate()
{
	/*Vector3 shotPos_ = { worldTransform_.translation_.x, worldTransform_.translation_.y - 5.0f * worldTransform_.scale_.x, worldTransform_.translation_.z };
	enemyBulletManager_->CreateBullet(shotPos_, worldTransform_.scale_);*/
	behaviorRequest_ = Behavior::kStandby;

}

void Enemy::SplitUpdate()
{
	EnemyCreateFlag = true;
	float degree = float(rand() / 360);
	splitPos_ = {
		.x{cosf(degree) * 10.0f},
		.y{sinf(degree) * 10.0f},
		.z{0}
	};
	Vector3 Center = worldTransform_.translation_;
	worldTransform_.translation_ = { splitPos_ + Center};
	splitPos_ *= -1.0f;
	splitPos_ += Center;
	behaviorRequest_ = Behavior::kStandby;
}

void Enemy::DamageUpdate()
{
	worldTransform_.rotation_.x += 0.1f;
	if (times_[Behavior::kDamage] == DamageTime_) {
		worldTransform_.rotation_.x = 0.0f;
		behaviorRequest_ = Behavior::kStandby;
	}
	times_[Behavior::kDamage]++;
}

void Enemy::ClingUpdate()
{


}

void Enemy::GrowUpdate()
{
	if (worldTransform_.scale_.x < 5.0f) {
		worldTransform_.scale_ += Vector3(1.0f, 1.0f, 1.0f);
		behaviorRequest_ = Behavior::kStandby;
	}
	else {
		behaviorRequest_ = Behavior::kSplit;
	}
}
