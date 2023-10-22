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

void EnemyManager::Initialize(const std::vector<Model*>& type0, const std::vector<Model*>& type1)
{
    models_type0_ = type0;
    models_type1_ = type1;
    Reset();
}

void EnemyManager::Update() {
	if (player_->GetIsLanding()) {
		Reset();
		// CSVからデータの読み込み
		std::unique_ptr<CSV> csv = std::make_unique<CSV>();

		if (spawn0_) {
			csv->LoadCSV("Spaw");
		}
		else if (spawn1_) {
			csv->LoadCSV("Spaw");
		}
		else if (spawn2_) {
			csv->LoadCSV("Spaw");
		}
		std::vector<CSV::Data> datas = csv->UpdateDataCommands();
		// 読み込んだデータから生成
		for (CSV::Data data : datas) {
			Create(data.position, data.type);
		}

	}

	for (auto& enemy : enemies_) {
		enemy->Update();
		if (enemy->GetIsCreateFlag()) {
			EnemyState split = {
				.position{enemy->GetSplitPos()},
				.type{enemy->GetType()}
			};
			splits_.emplace_back(split);
		}
	}

	for (auto& split : splits_) {
		Create(split.position, split.type);
	}
	splits_.clear();

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
	//enemy->Initialize(model_, position, type);
    enemy->Initialize(models_type0_, models_type1_, position, type);
	enemy->SetViewProjection(viewProjection_);
	enemy->SetEnemyBulletManager(enemyBulletManager_);
	enemy->SetPlayer(player_);
	enemies_.emplace_back(enemy);
}
