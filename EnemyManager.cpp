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

	for (auto& enemy : enemies_) {

		if (enemy->GetIsAlive()) {
			enemy->Update();
		}
	}
#ifdef _DEBUG
    uint32_t count = 0;
    ImGui::Begin("Enemy");
    for (auto it = enemies_.begin(); it != enemies_.end(); ) {
        auto& enemy = *it;

        if (ImGui::TreeNode((std::to_string(count)).c_str())) {
            Vector3 translation = enemy->GetWorldTransform().translation_;
            Vector3 rotation = enemy->GetWorldTransform().rotation_;
            ImGui::DragFloat3("position", &translation.x, 0.1f);
            ImGui::DragFloat3("rotation", &rotation.x, 0.01f);
            if (ImGui::Button("TYPE:0")) {
                enemy->SetType(0);
                
            }
            if (ImGui::Button("TYPE:1")) {
                enemy->SetType(1);
            }
            enemy->SetTranslation(translation);
            enemy->SetRotate(rotation);

            if (ImGui::Button("Delete")) {
                delete enemy;
                it = enemies_.erase(it); // イテレータを更新
            }
            else {
                ++it; // イテレータを次の要素に進める
                count++;
            }

            ImGui::TreePop();
        }
        else {
            ++it; // イテレータを次の要素に進める
            count++;
        }
    }

    

    if (ImGui::Button("Create")) {
        Vector3 position = {};
        uint32_t type = 0;
        Create(position, type);
    }
    if (ImGui::Button("Save")) {
        // CSVからデータの読み込み
        std::unique_ptr<CSV> csv = std::make_unique<CSV>();
        std::vector<CSV::Data> datas;
        for (auto& enemy : enemies_) {
            datas.emplace_back(enemy->GetWorldTransform().translation_, enemy->GetType());
        }
        csv->WritingData(datas);
    }

    ImGui::End();
#endif // _DEBUG
}

void EnemyManager::Draw(const ViewProjection& viewProjection) {
	for (auto& enemy : enemies_) {
		if (enemy->GetIsDrawing()) {
			enemy->Draw(viewProjection);
		}
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
	//enemy->Initialize(model_, position, type);
    enemy->Initialize(models_type0_, models_type1_, position, type);
	enemy->SetViewProjection(viewProjection_);
	enemy->SetEnemyBulletManager(enemyBulletManager_);
	enemy->SetPlayer(player_);
	enemies_.emplace_back(enemy);
}

