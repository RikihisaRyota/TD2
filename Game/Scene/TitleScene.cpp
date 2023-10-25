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
	for (auto& model : frameModel_) {
		delete model;
	}
	for (auto& model : titleBossModel_) {
		delete model;
	}
	for (auto& model : playerModel_) {
		delete model;
	}
}

void TitleScene::Initialize() {
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	soundHandle_ = audio_->SoundLoadWave("Resources/Audios/title.wav");
	selectSoundHandle_ = audio_->SoundLoadWave("Resources/Audios/selectSound.wav");
	audio_->SoundPlayLoopStart(soundHandle_);

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();

	viewProjection_.Initialize();

#pragma region 生成
	backGround_ = std::make_unique<BackGround>();
	frame_ = std::make_unique<Frame>();
	followCamera_ = std::make_unique<FollowCamera>();
	titleBoss_ = std::make_unique<TitleBoss>();
	player_ = std::make_unique<TitlePlayer>();
#pragma endregion

	backGroundTextureHandles_.emplace_back(TextureManager::Load("Resources/Images/backGround.png"));
	backGroundTextureHandles_.emplace_back(TextureManager::Load("Resources/Images/backGround2.png"));
	backGroundTextureHandles_.emplace_back(TextureManager::Load("Resources/Images/backGround1.png"));
	backGround_->Initialize(backGroundTextureHandles_, BackGround::kTitle);
	// 枠組み
	frameModel_.emplace_back(Model::Create("rockBlock", true));
	frameModel_.emplace_back(Model::Create("rockBlock2", true));
	frame_->SetViewProjection(&viewProjection_);
	frame_->Initialize(frameModel_, Frame::kTitle);
	// カメラ
	followCamera_->SetAnimationMax(60.0f);
	followCamera_->Initialize(false);
	viewProjection_ = followCamera_->GetViewProjection();
	viewProjection_.UpdateMatrix();

	titleBossModel_.emplace_back(Model::Create("SharkHead", true));
	titleBossModel_.emplace_back(Model::Create("SharkJaw", true));
	titleBossModel_.emplace_back(Model::Create("SharkBody", true));
	titleBoss_->SetTitlePlayer(player_.get());
	titleBoss_->Initialize(titleBossModel_);

	// プレイヤー
	player_->SetSoundHandle(audio_->SoundLoadWave("Resources/Audios/playerMove.wav"));
	playerModel_.emplace_back(Model::Create("playerBody", true));
	playerModel_.emplace_back(Model::Create("playerLegLeft", true));
	playerModel_.emplace_back(Model::Create("playerLegRight", true));
	player_->Initialize(playerModel_);
}

void TitleScene::Update() {
	ImGui::Begin("SceneManage");
	ImGui::InputInt("SceneNumber", &sceneNumber_);
	ImGui::Text("GameTitle Scene");
	ImGui::End();

	if (titleBoss_->GetSceneChangeFlag()) {
		fade_->FadeInFlagSet(true);
		audio_->SoundPlayWave(selectSoundHandle_);
	}
	backGround_->Update();
	fade_->FadeInUpdate();
	frame_->Update();
	player_->Update();
	titleBoss_->Update();

	if (fade_->GetColor(0) > 1.0f) {
		sceneNumber_ = GAME_SCENE;
		audio_->SoundPlayLoopEnd(soundHandle_);
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
	backGround_->Draw();

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
	frame_->Draw(viewProjection_);
	titleBoss_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
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

	fade_->FadeInDraw();
	
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void TitleScene::Finalize() {
	
}