#include "TitleScene.h"
#include "ImGuiManager.h"

TitleScene::~TitleScene() {

}

void TitleScene::Initialize() {
	input_ = Input::GetInstance();
	count_ = 0;
}

void TitleScene::Update() {
	ImGui::Begin("SceneManage");
	ImGui::InputInt("SceneNumber", &sceneNumber_);
	ImGui::Text("GameTitle Scene");
	ImGui::End();

	if (input_->PushKey(DIK_SPACE)) {
		sceneNumber_ = GAME_SCENE;
		count_++;
	}

	if (sceneNumber_ < 0) {
		sceneNumber_ = 0;
	}
}

void TitleScene::Draw() {

}

void TitleScene::Finalize() {

}