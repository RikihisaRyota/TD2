#include "GameOver.h"
#include "ImGuiManager.h"

void GameOver::Initialize() {
	input_ = Input::GetInstance();
}

void GameOver::Update() {
	ImGui::Begin("SceneManage");
	ImGui::InputInt("SceneNumber", &sceneNumber_);
	ImGui::Text("GameOver Scene");
	ImGui::End();

	if (input_->PushKey(DIK_0)) {
		sceneNumber_ = TITLE_SCENE;
	}
}

void GameOver::Draw() {

}

void GameOver::Finalize() {

}