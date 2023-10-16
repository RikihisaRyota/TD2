#include "PlayerBulletManager.h"

PlayerBulletManager::~PlayerBulletManager() {
	Reset();
}

void PlayerBulletManager::Initialize(Model* model) {
	model_ = model;
	Reset();
}

void PlayerBulletManager::Update() {
	for (auto& bullet : bullets_) {
		bullet->Update();
	}
}

void PlayerBulletManager::Draw(const ViewProjection& viewProjection) {
	for (auto& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void PlayerBulletManager::CreateBullet(const Vector3& position) {
	PlayerBullet* bullet = new PlayerBullet();
	bullet->SetViewProjection(viewProjection_);
	bullet->Initialize(model_,position);
	bullets_.emplace_back(bullet);
}

void PlayerBulletManager::Reset() {
	for (auto& bullet : bullets_) {
		bullet->Reset();
	}
	bullets_.clear();
}
