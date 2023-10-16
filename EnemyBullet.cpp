#include "EnemyBullet.h"

#include "Draw.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();
	isAlive_ = true;
	HitBoxInitialize();
}

void EnemyBullet::Update() {
	worldTransform_.UpdateMatrix();
	HitBoxUpdate();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_,viewProjection);
}

void EnemyBullet::Reset() {
	delete this;
}

void EnemyBullet::OnCollision(uint32_t type, Sphere* sphere) {
	uint32_t a = type;
	Sphere s = *sphere;
}

void EnemyBullet::HitBoxInitialize() {
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemyBullet);
	// 衝突対象を自分以外に設定
	SetCollisionMask(~kCollisionAttributeEnemyBullet);
	// Sphere
	sphere_ = {
		.center_{worldTransform_.translation_},
		.radius_{radius_ },
	};
}

void EnemyBullet::HitBoxUpdate() {
	// Sphere
	sphere_ = {
		.center_{worldTransform_.translation_},
		.radius_{radius_ },
	};
}

void EnemyBullet::HitBoxDraw(const ViewProjection& viewProjection) {
	DrawSphere(sphere_, viewProjection, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
}
