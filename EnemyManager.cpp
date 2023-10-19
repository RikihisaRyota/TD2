#include "EnemyManager.h"

#include "CSV.h"
#include "ImGuiManager.h"
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
		if (enemy->GetIsAlive()) {
			enemy->Update();
		}
	}
}

void EnemyManager::Draw(const ViewProjection& viewProjection) {
	for (auto& enemy : enemies_) {
        if (enemy->GetIsDrawing() || isDebug_) {
            enemy->Draw(viewProjection);
        }
	}
}

void EnemyManager::Reset() {
	for (auto& enemy : enemies_) {
		delete enemy;
	}
	enemies_.clear();
	// 出刃kkう
	isDebug_ = false;
}

void EnemyManager::Create(const Vector3& position, uint32_t type) {
	Enemy* enemy = new Enemy();
	enemy->SetEnemyBulletManager(enemyBulletManager_);
	enemy->Initialize(model_, position, type);
	enemy->SetViewProjection(viewProjection_);
	enemy->SetEnemyBulletManager(enemyBulletManager_);
	enemy->SetPlayer(player_);
	enemies_.emplace_back(enemy);
}
