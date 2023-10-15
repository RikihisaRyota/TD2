#include "Uvula.h"

#include "Draw.h"
#include "MyMath.h"
#include "Player.h"

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
		headWorldTransform_.translation_ = Lerp(headWorldTransform_.translation_, player_->GetTranslation(), 0.02f);
		headWorldTransform_.translation_.y += std::sinf(angle_) * 0.5f;
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
		headWorldTransform_.translation_ = player_->GetTranslation();
		headWorldTransform_.translation_.x -= 2.0f;
		headWorldTransform_.UpdateMatrix();
	}
	if (player_->GetTranslation().x <= 0.0f) {
		isPlayerChase_ = true;
		Reset();
	}
}

void Uvula::Draw(const ViewProjection& viewProjection) {
	headModel_->Draw(headWorldTransform_, viewProjection);
	for (size_t i = 0; i < bodyModels_.size(); i++) {
		bodyModels_.at(i)->Draw(bodyWorldTransforms_.at(i), viewProjection);
	}
}

void Uvula::OnCollision() {
	Reset();
	isPlayerChase_ = false;
}

void Uvula::HitBoxInitialize() {
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	// 衝突対象を自分以外に設定
	SetCollisionMask(~kCollisionAttributeEnemy);
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