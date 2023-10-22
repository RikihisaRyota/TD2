#include "EnemyEditor.h"

#include "CSV.h"
#include "EnemyManager.h"
#include "Model.h"
#include "ImGuiManager.h"

void EnemyEditor::Update(EnemyManager* enemyManager) {
#ifdef _DEBUG
    uint32_t count = 0;
    ImGui::Begin("Enemy");
    for (auto it = enemyManager->GetEnemies().begin(); it != enemyManager->GetEnemies().end(); ) {
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
            enemy->SetMatWorld();
            if (ImGui::Button("Delete")) {
                delete enemy;
                it = enemyManager->GetEnemies().erase(it); // イテレータを更新
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
        enemyManager->Create(position, type);
    }
    if (ImGui::Button("Save")) {
        // CSVからデータの読み込み
        std::unique_ptr<CSV> csv = std::make_unique<CSV>();
        std::vector<CSV::Data> datas;
        for (auto& enemy : enemyManager->GetEnemies()) {
            datas.emplace_back(enemy->GetWorldTransform().translation_, enemy->GetType());
        }
        if (spawn0_) {
            csv->WritingData("spawn", datas);
        }
        else if (spawn1_) {
            csv->WritingData("spawn", datas);
        }
        else if (spawn2_) {
            csv->WritingData("spawn", datas);
        }
    }

    ImGui::Checkbox("SpawnPattern : 0", &spawn0_);
    if (spawn0_) {
        spawn1_ = false;
        spawn2_ = false;
    }
    ImGui::Checkbox("SpawnPattern : 1", &spawn1_);
    if (spawn1_) {
        spawn0_ = false;
        spawn2_ = false;
    }
    ImGui::Checkbox("SpawnPattern : 2", &spawn2_);
    if (spawn2_) {
        spawn0_ = false;
        spawn1_ = false;
    }

    enemyManager->SetSpawn0(spawn0_);
    enemyManager->SetSpawn1(spawn1_);
    enemyManager->SetSpawn2(spawn2_);

    if (ImGui::Button("Reset")) {
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
        enemyManager->Reset();
        std::vector<CSV::Data> datas = csv->UpdateDataCommands();
        // 読み込んだデータから生成
        for (CSV::Data data : datas) {
            enemyManager->Create(data.position, data.type);
        }
    }


    ImGui::End();
#endif // _DEBUG
}
