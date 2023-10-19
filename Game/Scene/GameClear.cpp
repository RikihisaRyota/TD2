#include "GameClear.h"
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
	// コマンドリスト取得
	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

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

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void GameClear::Finalize() {

}