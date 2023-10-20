#include "EnemyBulletManager.h"

#include "MyMath.h"

EnemyBulletManager::~EnemyBulletManager()
{
	Reset();
}

void EnemyBulletManager::Initialize(Model* model) {
	model_ = model;
	Reset();
}

void EnemyBulletManager::Update() {
	for (auto it = enemyBullets_.begin(); it != enemyBullets_.end();) {
		EnemyBullet* bullet = *it;

		bullet->Update();

		if (!bullet->GetIsAlive()) {
			bullet->Reset();
			it = enemyBullets_.erase(it); // 削除し、次の要素を指すイテレータを取得
		}
		else {
			++it; // 次の要素へ進む
		}
	}
	if (player_->GetIsPulling()) {
		Reset();
	}
}

void EnemyBulletManager::Draw(const ViewProjection& viewProjection) {
	for (auto& bullet : enemyBullets_) {
		bullet->Draw(viewProjection);
	}
}

void EnemyBulletManager::CreateBullet(const Vector3& position, float radius)
{
	EnemyBullet* bullet = new EnemyBullet();
	bullet->SetViewProjection(viewProjection_);
	bullet->Initialize(model_, position, radius);
	bullet->SetViewProjection(viewProjection_);
	enemyBullets_.emplace_back(bullet);
}

void EnemyBulletManager::Reset() {
	for (auto& bullet : enemyBullets_) {
		delete bullet;
	}
	enemyBullets_.clear();
}