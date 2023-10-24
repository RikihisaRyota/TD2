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

void EnemyManager::Initialize(const std::vector<Model*>& type0, const std::vector<Model*>& type1, const std::vector<Model*>& type2 )
{
    models_type0_ = type0;
    models_type1_ = type1;
	models_type2_ = type2;
	height_ = 0.0f;
	width_ = 0.0f;
    Reset();
}

void EnemyManager::Update() {
	if (player_->GetIsLanding()) {
		Reset();
		// CSVからデータの読み込み
		std::unique_ptr<CSV> csv = std::make_unique<CSV>();

		if (spawn0_) {
			csv->LoadCSV("Spaw0");
		}
		else if (spawn1_) {
			csv->LoadCSV("Spaw1");
		}
		else if (spawn2_) {
			csv->LoadCSV("Spaw2");
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
    enemy->Initialize(models_type0_, models_type1_, models_type2_, position, type);
	enemy->SetViewProjection(viewProjection_);
	enemy->SetEnemyBulletManager(enemyBulletManager_);
	enemy->SetPlayer(player_);
	enemy->SetHeight(height_);
	enemy->SetWidth(width_);
	enemies_.emplace_back(enemy);
}

void EnemyManager::SetShotTime(int shotTime)
{
	for (auto& enemy : enemies_) {
		enemy->SetShotTime(shotTime);
	}
}

void EnemyManager::SetMaxSize(float maxSize)
{
	for (auto& enemy : enemies_) {
		enemy->SetMaxSize(maxSize);
	}
}

void EnemyManager::SetEaseSecond_Grow(float easeSecond_Grow)
{
	for (auto& enemy : enemies_) {
		enemy->SetEaseSecond_Grow(easeSecond_Grow);
	}
}

void EnemyManager::SetEaseSecond_Shot(float easeSecond_Shot)
{
	for (auto& enemy : enemies_) {
		enemy->SetEaseSecond_Shot(easeSecond_Shot);
	}
}

void EnemyManager::SetOnceUpSize(float onceUpSize)
{
	for (auto& enemy : enemies_) {
		enemy->SetOnceUpSize(onceUpSize);
	}
}

void EnemyManager::SetInitialRadius(float initialRadius)
{
	for (auto& enemy : enemies_) {
		enemy->SetInitialRadius(initialRadius);
	}
}

void EnemyManager::SetBulletShotCount(uint32_t bulletShotCount)
{
	for (auto& enemy : enemies_) {
		enemy->SetBulletShotCount(bulletShotCount);
	}
}

void EnemyManager::SetScaleUpValue_Shot(float scaleUpValue_Shot)
{
	for (auto& enemy : enemies_) {
		enemy->SetScaleUpValue_Shot(scaleUpValue_Shot);
	}
}

void EnemyManager::SetRotateValue_Shot(float rotateValue_Shot)
{
	for (auto& enemy : enemies_) {
		enemy->SetRotateValue_Shot(rotateValue_Shot);
	}
}

void EnemyManager::SetHeight(float height)
{
	height_ = height;
}

void EnemyManager::SetWidth(float width)
{
	width_ = width;
}
