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

GameOver::~GameOver()
{
	for (auto& model : frameModel_) {
		delete model;
	}
}

void GameOver::Initialize() {
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	soundHandle_ = audio_->SoundLoadWave("Resources/Audios/title.wav");
	selectSoundHandle_ = audio_->SoundLoadWave("Resources/Audios/selectSound.wav");
	audio_->SoundPlayLoopStart(soundHandle_);

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();

	isStart_ = true;

	viewProjection_.Initialize();
#pragma region 生成
	backGround_ = std::make_unique<BackGround>();
	frame_ = std::make_unique<Frame>();
#pragma endregion

	backGroundTextureHandles_.emplace_back(TextureManager::Load("Resources/Images/backGround.png"));
	backGroundTextureHandles_.emplace_back(TextureManager::Load("Resources/Images/backGround2.png"));
	backGroundTextureHandles_.emplace_back(TextureManager::Load("Resources/Images/backGround1.png"));
	backGround_->Initialize(backGroundTextureHandles_,BackGround::kGameOver);
	// 枠組み
	frameModel_.emplace_back(Model::Create("rockBlock", true));
	frameModel_.emplace_back(Model::Create("rockBlock2", true));
	frame_->SetViewProjection(&viewProjection_);
	frame_->Initialize(frameModel_, Frame::kGameOver);
	// テクスチャ
	auto texture = TextureManager::Load("Resources/Images/GameOver.png");
	auto texture2 = TextureManager::Load("Resources/Images/pressSpace.png");

	// イージング用のスタートとエンドのポジション
	gameOverStartPosition_ = { 50.0f, -200.0f };
	gameOverEndPosition_ = { 50.0f, 48.0f };
	pressSpaceStartPosition_ = { 285.0f, -200.0f };
	pressSpaceEndPosition_ = { 285.0f, 590.0f };

	gameOverSprite_ = Sprite::Create(texture, gameOverSpritePosition_);
	pressSpaceSprite_ = Sprite::Create(texture2, pressSpacePosition_);
	gameOverSprite_->SetSize({1200.0f, 150.0f});
	pressSpaceSprite_->SetSize({720.0f, 100.0f});
	gameOverSpritePosition_ = gameOverStartPosition_;
	pressSpacePosition_ = pressSpaceStartPosition_;
}

void GameOver::Update() {
	

	frame_->Update();
	fade_->FadeOutUpdate();

	if (easingFlag_) {
		float n1 = 7.5625f;
		float d1 = 2.75f;
		float easedT = 0.0f;
		if (easeingCount_ < (1.0f / d1)) {
			easedT = n1 * easeingCount_ * easeingCount_;
		}
		else if (easeingCount_ < (2.0f / d1)) {
			float x = easeingCount_ - (1.5f / d1);
			easedT = n1 * x * x + 0.75f;
		}
		else if (easeingCount_ < (2.5f / d1)) {
			float x = easeingCount_ - (2.25f / d1);
			easedT = n1 * x * x + 0.9375f;
		}
		else {
			float x = easeingCount_ - (2.625f / d1);
			easedT = n1 * x * x + 0.984375f;
		}

		gameOverSpritePosition_.y = (1.0f - easedT) * gameOverStartPosition_.y + easedT * gameOverEndPosition_.y;
		pressSpacePosition_.y = (1.0f - easedT) * pressSpaceStartPosition_.y + easedT * pressSpaceEndPosition_.y;
	}
	else {

		if (floatTimer_ > 60) {
			if (!floatFlag_) {
				floatFlag_ = true;
			}
			else {
				floatValue_ *= -1.0f;
			}
			floatTimer_ = 0;

		}
		floatTimer_++;
		if (floatFlag_) {
			if (colorValue_.w > 1.0f) {
				alphaValue_ *= -1.0f;
				colorValue_.w = 1.0f;
			}
			else if (colorValue_.w < 0.0f) {
				alphaValue_ *= -1.0f;
				colorValue_.w = 0.0f;
			}
			colorValue_.w += alphaValue_;
			gameOverSpritePosition_.y += floatValue_;
			pressSpaceSprite_->SetColor(colorValue_);
		}
	}

	if (easeingCount_ < 1.0f) {
		easeingCount_ += easeingSecond_;
	}
	else {
		easingFlag_ = false;
	}

	gameOverSprite_->SetPosition(gameOverSpritePosition_);
	pressSpaceSprite_->SetPosition(pressSpacePosition_);

	viewProjection_.UpdateMatrix();

	if (fade_->GetColor(1) < 0.0f) {
		//audio_->SoundPlayLoopEnd(soundHandle_);
		isStart_ = false;
	}

	if (sceneNumber_ > 4) {
		sceneNumber_ = 0;
	}

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
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
	//gameOverSprite_->Draw(viewProjection_);

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

	gameOverSprite_->Draw();
	pressSpaceSprite_->Draw();
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