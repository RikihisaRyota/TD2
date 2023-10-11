#include "EnemyEditor.h"

#include "Enemy.h"
#include "Model.h"
#include "ImGuiManager.h"

void EnemyEditor::Update(std::list<Enemy*>& enemies, Model* model) {
    uint32_t count = 0;
    ImGui::Begin("Enemy");
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        auto& enemy = *it;

        if (ImGui::TreeNode((std::to_string(count)).c_str())) {
            Vector3 translation = enemy->GetWorldTransform().translation_;
            Vector3 rotation = enemy->GetWorldTransform().rotation_;
            ImGui::DragFloat3("position", &translation.x, 0.1f);
            ImGui::DragFloat3("rotation", &rotation.x, 0.01f);
            enemy->SetTranslation(translation);
            enemy->SetRotate(rotation);
            int flag = enemy->GetFlag();
            ImGui::DragInt("flag", &flag, 1);
            enemy->SetFlag(flag);

            if (ImGui::Button("Delete")) {
                delete enemy;
                it = enemies.erase(it); // イテレータを更新
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
        Enemy* enemy = new Enemy();
        enemy->Initialize(model, Vector3(0.0f, 10.0f, 0.0f), 0);
        enemies.emplace_back(enemy);
    }
    if (ImGui::Button("Save")) {

    }

    ImGui::End();
}
