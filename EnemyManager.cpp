#include "EnemyManager.h"

#include "MyMath.h"

EnemyManager::~EnemyManager() {
	Reset();
}

void EnemyManager::Initialize(Model* model) {
	model_ = model;
	Reset();
	input_ = Input::GetInstance();
}

void EnemyManager::Update() {
	addEnemyStates_.clear();

	for (auto& enemy : enemies_) {
		enemy->Update();
		if (enemy->GetEnemyCreateFlag()) {
			EnemyState add = {
				.position{enemy->GetSplitPos()},
				.type{enemy->GetType()}
			};
			addEnemyStates_.emplace_back(add);
		}
	}


	for (auto& addEnemyState : addEnemyStates_) {
		Create(addEnemyState.position, addEnemyState.type);
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
	addEnemyStates_.clear();
}

void EnemyManager::Create(const Vector3& position, uint32_t type) {
	Enemy* enemy = new Enemy();
	enemy->SetEnemyBulletManager(enemyBulletManager_);
	enemy->Initialize(model_, position, type);
	enemies_.emplace_back(enemy);
}

