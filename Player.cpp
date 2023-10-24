#include "Player.h"

#include <cassert>

#include "CollisionManager.h"
#include "Draw.h"
#include "MyMath.h"
#include "ImGuiManager.h"
#include "Input.h"


Player::~Player() {

}

void Player::Initialize(std::vector<Model*> models) {
	for (size_t i = 0; i < static_cast<size_t>(Parts::kPartsCount); i++) {
		models_.emplace_back(models.at(i));
	}
	worldTransform_.Initialize();
	motion_.Initialize();
	motion_.parent_ = &worldTransform_;
	for (size_t i = 0; i < static_cast<size_t>(Parts::kPartsCount); i++) {
		WorldTransform part;
		part.Initialize();
		part.parent_ = &motion_;
		part.UpdateMatrix();
		parts_.emplace_back(part);
	}

	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	playerJump_ = std::make_unique<PlayerJump>();
	playerJump_->SetPlayer(this);

	playerMove_ = std::make_unique<PlayerMove>();
	playerMove_->SetPlayer(this);

	playerPullingMove_ = std::make_unique<PlayerPullingMove>();
	playerPullingMove_->SetPlayer(this);

	playerString_ = std::make_unique<PlayerString>();
	playerString_->SetPlayer(this);
	playerString_->SetViewProjection(viewProjection_);

	playerStun_ = std::make_unique<PlayerStun>();
	playerStun_->SetPlayer(this);

	playerLanding_ = std::make_unique<PlayerLanding>();
	playerLanding_->SetPlayer(this);
	Hp_ = 3;
	Reset();
	HitBoxInitialize();

}

void Player::UpdateMatrix() {
	worldTransform_.UpdateMatrix();
	motion_.UpdateMatrix();
	for (auto& parts : parts_) {
		parts.UpdateMatrix();
	}
}

void Player::Reset() {
	isGameOver_ = false;
	radius_ = kRadiusMin_;

	float scale = radius_ * 0.5f;
	worldTransform_.scale_ = { scale ,scale ,scale };
	worldTransform_.rotation_ = { 0.0f,11.0f,0.0f };
	worldTransform_.translation_ = kInitialPosition_;
	UpdateMatrix();

	playerJump_->Initialize({ 0.0f,0.0f,0.0f });
	playerMove_->Initialize();
	playerPullingMove_->Initialize();
	playerString_->Initialize();

	isPulling_ = false;
	isLanding_ = false;
	weightCount_ = 0;
	isInvincible_ = false;
	invincibleCount_ = 0;

	isHitStop_ = false;

	

	HitBoxUpdate();
}

void Player::Update() {
	switch (behavior_) {
	case Player::kMove:
		playerMove_->Update();
		break;
	case Player::kPullingMove:
		playerPullingMove_->Update();
		break;
	case Player::kString:
		playerString_->Update();
		break;
	case Player::kJump:
		playerJump_->Update();
		break;
	case Player::kLanding:
		playerLanding_->Update();
		break;
	case Player::kStun:
		playerStun_->Update();
		break;
	case Player::kDoNothing:
		break;
	}
	InvincibleUpdate();
	MoveLimit();
	HitBoxUpdate();
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
	case Player::kStun:
		break;
	case Player::kDoNothing:
		break;
	}
	for (size_t i = 0; i < Parts::kPartsCount; i++) {
		models_.at(i)->Draw(parts_.at(i), viewProjection);
	}
}

void Player::Debug() {
	playerMove_->Debug();
	playerPullingMove_->Debug();
	playerString_->Debug();
	playerJump_->Debug();
	playerStun_->Debug();
	ImGui::Begin("Player");
	ImGui::Text("translation\n");
	ImGui::Text("x:%.4f,y:%.4f,z:%.4f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	float weightCount = static_cast<float>(weightCount_);
	float weightMax = static_cast<float>(kWeightMax_);
	ImGui::SliderFloat("weightCount", &weightCount, 0.0f, weightMax);
	ImGui::SliderFloat("weightMax", &weightMax, 0.0f, 20.0f);
	weightCount_ = static_cast<uint32_t>(weightCount);
	kWeightMax_ = static_cast<uint32_t>(weightMax);
	float invincibleCount = static_cast<float>(invincibleCount_);
	float invincibleMax = static_cast<float>(kInvincibleMax_);
	ImGui::SliderFloat("invincibleCount", &invincibleCount, 0.0f, invincibleMax);
	ImGui::SliderFloat("invincibleMax", &invincibleMax, 0.0f, 60.0f);
	invincibleCount_ = static_cast<uint32_t>(invincibleCount);
	kInvincibleMax_ = static_cast<uint32_t>(invincibleMax);
	float behavior = static_cast<float>(behavior_);
	ImGui::Text("Behavior:%f", behavior);
	ImGui::Text("isPulling:%d", isPulling_);
	ImGui::Text("isLanding:%d", isLanding_);
	ImGui::Text("HP:%d", Hp_);
	ImGui::End();
}

void Player::OnCollision(uint32_t type, Sphere* sphere) {
	switch (type) {
	case static_cast<size_t>(CollisionManager::Type::kPlayerVSEnemy):
	{
		if (behavior_ != Player::Behavior::kStun &&
			!isInvincible_) {
			isHitStop_ = true;
			audio_->SoundPlayWave(enemyEatSoundHandle_);
			weightCount_++;
			radius_ = Lerp(kRadiusMin_, kRadiusMax_, static_cast<float>(weightCount_) / static_cast<float>(kWeightMax_));
			float scale = radius_ * 0.5f;
			worldTransform_.scale_ = { scale ,scale ,scale };
			UpdateMatrix();
		}
	}
	break;
	case static_cast<size_t>(CollisionManager::Type::kPlayerVSEnemyBullet):
	{
		if (behavior_ != Player::Behavior::kStun &&
			!isInvincible_) {
			behaviorRequest_ = Player::Behavior::kStun;
			BehaviorInitialize();
		}
	}
	break;
	case static_cast<size_t>(CollisionManager::Type::kPlayerVSBoss):
	{
		if (!isPulling_) {
			isPulling_ = true;
			behaviorRequest_ = kPullingMove;
			BehaviorInitialize();
		}
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
	if (!isPulling_) {
	sphere_ = {
		.center_{worldTransform_.translation_},
		.radius_{radius_ * 0.5f},
	};
	}
	else {
		sphere_ = {
		.center_{worldTransform_.translation_},
		.radius_{radius_},
		};
	}
}

void Player::HitBoxDraw(const ViewProjection& viewProjection) {
	DrawSphere(sphere_, viewProjection, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
}

void Player::SubtractionPlayerHP() {
	Hp_--;
	if (Hp_ <= 0) {
		isGameOver_ = true;
	}
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
			playerPullingMove_->Initialize();
			break;
		case Behavior::kString:
			playerString_->Initialize();
			break;
		case Behavior::kJump:
			playerJump_->Initialize(playerString_->GetShootOutVector());
			break;
		case Behavior::kLanding:
			playerLanding_->Initialize();
			break;
		case Player::kStun:
			playerStun_->Initialize();
			break;
		case Player::kDoNothing:
			break;
		}
		// ふるまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
}

void Player::MoveLimit() {
	float playerSize = 2.0f;
	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kWidth_ + playerSize, kWidth_ - playerSize);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kHeight_ + playerSize, kHeight_ - playerSize);
	worldTransform_.UpdateMatrix();
}

void Player::InvincibleUpdate() {
	if (isInvincible_) {
		invincibleCount_++;
		for (auto& model : models_) {
			model->GetMaterial(0)->SetColor(Vector4(1.0f, 0.0f, 1.0f, 1.0f));
		}
		if (invincibleCount_ >= kInvincibleMax_) {
			isInvincible_ = false;
			invincibleCount_ = 0;
			for (auto& model : models_) {
				model->GetMaterial(0)->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}
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

void Player::SetMotionScale(const Vector3& scale) {
	motion_.scale_ = scale;
	motion_.UpdateMatrix();
}

void Player::SetMotionRotation(const Vector3& rotation) {
	motion_.rotation_ = rotation;
	motion_.UpdateMatrix();
}

void Player::SetMotionTranslation(const Vector3& translation) {
	motion_.translation_ = translation;
	motion_.UpdateMatrix();
}

void Player::SetMotionWorldTransform(const WorldTransform& worldTransform) {
	motion_ = worldTransform;
	motion_.UpdateMatrix();
}

void Player::SetPartsScale(const Vector3& scale, size_t num) {
	parts_.at(num).scale_ = scale;
	parts_.at(num).UpdateMatrix();
}

void Player::SetPartsRotation(const Vector3& rotation, size_t num) {
	parts_.at(num).rotation_ = rotation;
	parts_.at(num).UpdateMatrix();
}

void Player::SetPartsTranslation(const Vector3& translation, size_t num) {
	parts_.at(num).translation_ = translation;
	parts_.at(num).UpdateMatrix();
}

void Player::SetPartsWorldTransform(const WorldTransform& worldTransform, size_t num) {
	parts_.at(num) = worldTransform;
	parts_.at(num).UpdateMatrix();
}

void Player::SetBehavior(const std::optional<Behavior>& behaviorRequest) {
	behaviorRequest_ = behaviorRequest;
	BehaviorInitialize();
}
