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

GameClear::~GameClear() {
	for (auto& model : frameModel_) {
		delete model;
	}
}

void GameClear::Initialize() {
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	soundHandle_ = audio_->SoundLoadWave("Resources/Audios/clear.wav");
	selectSoundHandle_ = audio_->SoundLoadWave("Resources/Audios/selectSound.wav");
	//audio_->SoundPlayLoopStart(soundHandle_);
	
	fade_ = std::make_unique<Fade>();
	fade_->Initialize();

	isStart_ = true;

	viewProjection_.Initialize();
#pragma region 生成
	backGround_ = std::make_unique<BackGround>();
	frame_ = std::make_unique<Frame>();
	followCamera_ = std::make_unique<FollowCamera>();
	treasureBox_ = std::make_unique<TreasureBox>();
	clearSprite_ = std::make_unique<ClearSprite>();
#pragma endregion

	backGroundTextureHandles_.emplace_back(TextureManager::Load("Resources/Images/backGround.png"));
	backGroundTextureHandles_.emplace_back(TextureManager::Load("Resources/Images/backGround2.png"));
	backGroundTextureHandles_.emplace_back(TextureManager::Load("Resources/Images/backGround1.png"));
	backGround_->Initialize(backGroundTextureHandles_,BackGround::kGameClear);
	// 枠組み
	frameModel_.emplace_back(Model::Create("rockBlock", true));
	frameModel_.emplace_back(Model::Create("rockBlock2", true));
	frame_->SetViewProjection(&viewProjection_);
	frame_->Initialize(frameModel_,Frame::kGameClear);
	// カメラ
	followCamera_->SetAnimationMax(60.0f);
	followCamera_->SetTreasureBox(treasureBox_.get());
	followCamera_->Initialize(false);
	viewProjection_ = followCamera_->GetViewProjection();
	viewProjection_.UpdateMatrix();
	// 宝箱
	modelTreasureBox_.emplace_back(Model::Create("treasureBoxhuta",true));
	modelTreasureBox_.emplace_back(Model::Create("treasureboxUnder",true));
	treasureBox_->Initialize(modelTreasureBox_);
	treasureBox_->SetComeAnimationMax(120.0f);
	treasureBox_->SetOpenAnimationMax(60.0f);
	// テクスチャ
	auto texture = TextureManager::Load("Resources/Images/gameclear.png");
	clearSprite_->SetTreasureBox(treasureBox_.get());
	clearSprite_->Initialize(texture);
	clearSprite_->SetAnimationMax(120.0f);
}

void GameClear::Update() {

	frame_->Update();
	followCamera_->Update();
	fade_->FadeOutUpdate();
	treasureBox_->Update();
	clearSprite_->Update();
	
	viewProjection_ = followCamera_->GetViewProjection();
	viewProjection_.UpdateMatrix();

	if (fade_->GetColor(1) < 0.0f) {
		isStart_ = false;
	}

	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNumber_ = TITLE_SCENE;
		audio_->SoundPlayLoopEnd(soundHandle_);
		audio_->SoundPlayWave(selectSoundHandle_);
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
	clearSprite_->Draw(viewProjection_);
	treasureBox_->Draw(viewProjection_);

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

void GameClear::Finalize() {

}