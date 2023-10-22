#include "EnemyEditor.h"

#include "CSV.h"
#include "EnemyManager.h"
#include "Model.h"
#include "ImGuiManager.h"

void EnemyEditor::Update(EnemyManager* enemyManager) {
#ifdef _DEBUG
    uint32_t count = 0;
    ImGui::Begin("Enemy");
    int shotTime = enemyManager->GetShotTime();
    float sizeMax = enemyManager->GetMaxSize();
    float easeSecond_Shot = enemyManager->GetEaseSecond_Shot();
    float easeSecond_Grow = enemyManager->GetEaseSecond_Grow();
    float onceUpSize = enemyManager->GetOnceUpSize();
    float initialRadius = enemyManager->GetInitialRadius();
    for (auto it = enemyManager->GetEnemies().begin(); it != enemyManager->GetEnemies().end(); ) {
        auto& enemy = *it;

        if (ImGui::TreeNode((std::to_string(count)).c_str())) {
            Vector3 translation = enemy->GetWorldTransform().translation_;
            Vector3 rotation = enemy->GetWorldTransform().rotation_;

            bool type0 = false;
            bool type1 = false;
            bool type2 = false;

            if (enemy->GetType() == static_cast<uint32_t>(Enemy::EnemyType::kOctopus)) {
                type0 = true;
            }
            else if (enemy->GetType() == static_cast<uint32_t>(Enemy::EnemyType::kSpike)) {
                type1 = true;
            }
            else if (enemy->GetType() == static_cast<uint32_t>(Enemy::EnemyType::kfeed)) {
                type2 = true;
            }
            ImGui::DragFloat3("position", &translation.x, 0.1f);
            ImGui::DragFloat3("rotation", &rotation.x, 0.01f);
            ImGui::Checkbox("Type0", &type0);
            ImGui::Checkbox("Type1", &type1);
            ImGui::Checkbox("Type2", &type2);
            if (type0) {
                enemy->SetType(static_cast<uint32_t>(Enemy::EnemyType::kOctopus));
            }
            else if (type1) {
                enemy->SetType(static_cast<uint32_t>(Enemy::EnemyType::kSpike));
            }
            else if (type2) {
                enemy->SetType(static_cast<uint32_t>(Enemy::EnemyType::kfeed));
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

    ImGui::DragInt("ShotSpeed", &shotTime, 1, 0, 1000);
    ImGui::DragFloat("MaxSize", &sizeMax);
    ImGui::DragFloat("onceUpSize", &onceUpSize);
    ImGui::DragFloat("easeSecond_Shot", &easeSecond_Shot);
    ImGui::DragFloat("easeSecond_Grow", &easeSecond_Grow);
    ImGui::DragFloat("InitialRadius", &initialRadius);

    enemyManager->SetShotTime(shotTime);
    enemyManager->SetEaseSecond_Grow(easeSecond_Grow);
    enemyManager->SetEaseSecond_Shot(easeSecond_Shot);
    enemyManager->SetMaxSize(sizeMax);
    enemyManager->SetOnceUpSize(onceUpSize);
    enemyManager->SetInitialRadius(initialRadius);


    ImGui::End();
#endif // _DEBUG
}
