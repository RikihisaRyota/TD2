#include "EnemyManager.h"

#include "MyMath.h"

EnemyManager::~EnemyManager() {
	Reset();
}

void EnemyManager::Initialize(Model* model) {
	model_ = model;
	Reset();
}

void EnemyManager::Update() {
	for (auto& enemy : enemies_) {
		enemy->Update();
	}
}

void EnemyManager::Draw(const ViewProjection& viewProjection) {
	for (auto& enemy : enemies_) {
		enemy->Draw(viewProjection);
	}
}

void EnemyManager::Reset() {
	for (auto& enemy : enemies_) {
		delete enemy;
	}
	enemies_.clear();
}

void EnemyManager::Create(const Vector3& position, uint32_t type) {
	Enemy* enemy = new Enemy();
	enemy->Initialize(model_, position, type);
	enemy->SetEnemyBulletManager(enemyBulletManager_);
	enemies_.emplace_back(enemy);
}

