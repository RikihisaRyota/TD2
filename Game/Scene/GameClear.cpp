#include "GameClear.h"
#include "ImGuiManager.h"

void GameClear::Initialize() {
	input_ = Input::GetInstance();
}

void GameClear::Update() {
	ImGui::Begin("SceneManage");
	ImGui::InputInt("SceneNumber", &sceneNumber_);
	ImGui::Text("GameClear Scene");
	ImGui::End();

	if (input_->PushKey(DIK_0)&& input_->PrePushKey(DIK_0)) {
		sceneNumber_ = OVER_SCENE;
	}

	if (sceneNumber_ > 4) {
		sceneNumber_ = 0;
	}
}

void GameClear::Draw() {

}

void GameClear::Finalize() {

}