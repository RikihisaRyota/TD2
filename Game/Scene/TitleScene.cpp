#include "TitleScene.h"
#include "ImGuiManager.h"

#pragma region 描画関連インクルード
#include "DirectXCommon.h"
#include "CubeRenderer.h"
#include "SphereRenderer.h"
#include "OBJ.h"
#include "Model.h"
#include "PrimitiveDrawer.h"
#include "PlaneRenderer.h"
#pragma endregion

TitleScene::~TitleScene() {

}

void TitleScene::Initialize() {
	input_ = Input::GetInstance();

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
}

void TitleScene::Update() {
	ImGui::Begin("SceneManage");
	ImGui::InputInt("SceneNumber", &sceneNumber_);
	ImGui::Text("GameTitle Scene");
	ImGui::End();

	fade_->FadeInUpdate();

	if (fade_->GetColor(0) == 1.2f) {
		sceneNumber_ = GAME_SCENE;
	}

	if (sceneNumber_ < 0) {
		sceneNumber_ = 0;
	}
}

void TitleScene::Draw() {
	// コマンドリスト取得
	ID3D12GraphicsCommandList* commandList =DirectXCommon::GetInstance()->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion


#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	CubeRenderer::PreDraw(commandList);
	SphereRenderer::PreDraw(commandList);
	OBJ::PreDraw(commandList);
	Model::PreDraw(commandList);
	PrimitiveDrawer::PreDraw(commandList);
	PlaneRenderer::PreDraw(commandList);
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	PlaneRenderer::PostDraw();
	PrimitiveDrawer::PostDraw();
	Model::PostDraw();
	SphereRenderer::PostDraw();
	OBJ::PostDraw();
	CubeRenderer::PostDraw();
#pragma endregion


#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	Sprite::SetBlendState(Sprite::BlendState::kNormal);
	
	if (input_->PushKey(DIK_SPACE)) {
		fade_->FadeInFlagSet(true);
	}

	fade_->FadeInDraw();
	
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void TitleScene::Finalize() {

}