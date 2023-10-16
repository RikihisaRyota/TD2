#include "PlayerBulletManager.h"

PlayerBulletManager::~PlayerBulletManager() {
	Reset();
}

void PlayerBulletManager::Initialize(Model* model) {
	model_ = model;
	Reset();
}

void PlayerBulletManager::Update() {
	for (auto it = bullets_.begin(); it != bullets_.end();) {
		PlayerBullet* bullet = *it;

		bullet->Update();

		if (!bullet->GetIsAlive()) {
			bullet->Reset();
			it = bullets_.erase(it); // 削除し、次の要素を指すイテレータを取得
			//delete bullet;
		}
		else {
			++it; // 次の要素へ進む
		}
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
