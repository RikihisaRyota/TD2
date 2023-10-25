#include "GameOver.h"
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

void GameOver::Initialize() {
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	soundHandle_ = audio_->SoundLoadWave("Resources/Audios/over.wav");
	selectSoundHandle_ = audio_->SoundLoadWave("Resources/Audios/selectSound.wav");
	//audio_->SoundPlayLoopStart(soundHandle_);

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();

	isStart_ = true;
}

void GameOver::Update() {
	ImGui::Begin("SceneManage");
	ImGui::InputInt("SceneNumber", &sceneNumber_);
	ImGui::Text("GameOver Scene");
	ImGui::End();

	fade_->FadeOutUpdate();

	if (fade_->GetColor(1) < 0.0f) {
		isStart_ = false;
	}

	if (input_->PushKey(DIK_SPACE)) {
		sceneNumber_ = TITLE_SCENE;
		audio_->SoundPlayLoopEnd(soundHandle_);
		audio_->SoundPlayWave(selectSoundHandle_);
	}
}

void GameOver::Draw() {
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

	if (isStart_ == true) {
		fade_->FadeOutFlagSet(true);
		fade_->FadeOutDraw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void GameOver::Finalize() {

}