#include "Player.h"

#include <cassert>

#include "CollisionManager.h"
#include "Draw.h"
#include "MyMath.h"
#include "ImGuiManager.h"
#include "Input.h"


void Player::Initialize(Model* model) {
	assert(model);
	model_ = model;

	input_ = Input::GetInstance();

	playerJump_ = std::make_unique<PlayerJump>();
	playerJump_->SetPlayer(this);
	
	playerMove_ = std::make_unique<PlayerMove>();
	playerMove_->SetPlayer(this);
	
	playerPullingMove = std::make_unique<PlayerPullingMove>();
	playerPullingMove->SetPlayer(this);
	
	playerString_ = std::make_unique<PlayerString>();
	playerString_->SetPlayer(this);
	
	Reset();
	HitBoxInitialize();
	
	isPulling_ = false;
}

void Player::Reset() {
	weightCount_ = 0;
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.rotation_ = { 0.0f,11.0f,0.0f };
	worldTransform_.translation_ = { 20.0f,0.0f,0.0f };
	worldTransform_.UpdateMatrix();

	playerJump_->Initialize({ 0.0f,0.0f,0.0f });
	playerMove_->Initialize();
	playerPullingMove->Initialize();
	playerString_->Initialize();
	HitBoxUpdate();
}

void Player::Update() {
	switch (behavior_) {
	case Player::kMove:
		playerMove_->Update();
		break;
	case Player::kPullingMove:
		playerPullingMove->Update();
		break;
	case Player::kString:
		playerString_->Update();
		break;
	case Player::kJump:
		playerJump_->Update();
		break;
	case Player::kLanding:
		break;
	}
	MoveLimit();
	HitBoxUpdate();
	playerMove_->Debug();
	playerPullingMove->Debug();
	playerString_->Debug();
	playerJump_->Debug();
	Debug();
}

void Player::Draw(const ViewProjection& viewProjection) {
	switch (behavior_) {
	case Player::kMove:
		break;
	case Player::kPullingMove:
		break;
	case Player::kString:
		playerString_->Draw(viewProjection);
		break;
	case Player::kJump:
		break;
	case Player::kLanding:
		break;
	}
	model_->Draw(worldTransform_, viewProjection);
}

void Player::Debug() {
	ImGui::Begin("Player");
	ImGui::Text("translation\n");
	ImGui::Text("x:%.4f,y:%.4f,z:%.4f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	float count = static_cast<float>(weightCount_);
	float max = static_cast<float>(kWeightMax_);
	ImGui::SliderFloat("weightCount",&count,0.0f, max);
	ImGui::SliderFloat("weightMax",&max,0.0f,20.0f);
	weightCount_ = static_cast<uint32_t>(count);
	kWeightMax_ = static_cast<uint32_t>(max);
	ImGui::End();
}

void Player::OBJtoOBB() {
	// .objをOBBへ変更（当たり判定へ）
	obb_.center_ = worldTransform_.translation_;
	GetOrientations(MakeRotateXYZMatrix(worldTransform_.rotation_), obb_.orientations_);
	obb_.size_ = worldTransform_.scale_;
}

void Player::OnCollision(uint32_t type, Sphere* sphere) {
	switch (type) {
	case static_cast<size_t>(CollisionManager::Type::kPlayerVSEnemy):
	{
		// 引っ張られていたら
		if (isPulling_) {
			weightCount_++;
		}
	}
	break;
	case static_cast<size_t>(CollisionManager::Type::kPlayerVSEnemyBullet):
	{

	}
	break;
	case static_cast<size_t>(CollisionManager::Type::kPlayerVSBoss):
	{
		behaviorRequest_ = kPullingMove;
		BehaviorInitialize();
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

void Player::HitBoxInitialize() {
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);
	// Sphere
	sphere_ = {
		.center_{worldTransform_.translation_},
		.radius_{radius_ },
	};
}

void Player::HitBoxUpdate() {
	// Sphere
	sphere_ = {
		.center_{worldTransform_.translation_},
		.radius_{radius_ },
	};
}

void Player::HitBoxDraw(const ViewProjection& viewProjection) {
	DrawSphere(sphere_,viewProjection,Vector4(0.0f,1.0f,0.0f,1.0f));
}

void Player::BehaviorInitialize() {
	if (behaviorRequest_) {
		// ふるまいを変更
		behavior_ = behaviorRequest_.value();
		// 各ふるまいごとの初期化を実行
		switch (behavior_) {
		case Player::kMove:
			playerMove_->Initialize();
			weightCount_ = 0;
			break;
		case Behavior::kPullingMove:
			playerPullingMove->Initialize();
			break;
		case Behavior::kString:
			playerString_->Initialize();
			break;
		case Behavior::kJump:
			playerJump_->Initialize(playerString_->GetShootOutVector());
			break;
		}
		// ふるまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
}

void Player::MoveLimit() {
	float playerSize = 2.0f;
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kWidth_+playerSize, kWidth_- playerSize);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kHeight_+ playerSize, kHeight_- playerSize);
	worldTransform_.UpdateMatrix();
}

void Player::SetScale(const Vector3& scale) {
	worldTransform_.scale_ = scale;
	worldTransform_.UpdateMatrix();
}

void Player::SetRotation(const Vector3& rotation) {
	worldTransform_.rotation_ = rotation;
	worldTransform_.UpdateMatrix();
}

void Player::SetTranslation(const Vector3& translation) {
	worldTransform_.translation_ = translation;
	worldTransform_.UpdateMatrix();
}

void Player::SetWorldTransform(const WorldTransform& worldTransform) {
	worldTransform_ = worldTransform;
	worldTransform_.UpdateMatrix();
}

void Player::SetBehavior(const std::optional<Behavior>& behaviorRequest) {
	behaviorRequest_ = behaviorRequest;
	BehaviorInitialize();
}
