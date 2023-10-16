#include "PlayerBullet.h"

#include "Draw.h"
#include "MyMath.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();
	radius_ = 2.0f;
}

void PlayerBullet::Update() {
	worldTransform_.translation_.x += kSpeed_;
	worldTransform_.UpdateMatrix();
	if (!IsInsideFrustum(sphere_, viewProjection_)) {
		Reset();
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

void PlayerBullet::Reset() {
	delete this;
}

void PlayerBullet::OnCollision() {
	delete this;
}

void PlayerBullet::HitBoxInitialize() {
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayerBullet);
	// 衝突対象を自分以外に設定
	SetCollisionMask(~kCollisionAttributePlayerBullet);
	// Sphere
	sphere_ = {
		.center_{worldTransform_.translation_},
		.radius_{radius_ },
	};
}

void PlayerBullet::HitBoxUpdate() {
	// Sphere
	sphere_ = {
		.center_{worldTransform_.translation_},
		.radius_{radius_ },
	};
}

void PlayerBullet::HitBoxDraw(const ViewProjection& viewProjection) {
	DrawSphere(sphere_,viewProjection,Vector4(0.0f,1.0f,0.0f,0.0f));
}
