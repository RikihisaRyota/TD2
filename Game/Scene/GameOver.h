#pragma once

#include "Audio.h"
#include "IScene.h"
#include "Input.h"
#include "TextureManager.h"
#include "Fade.h"

#include "BackGround.h"
#include "ClearSprite.h"
#include "Frame.h"
#include "FollowCamera.h"
#include "TreasureBox.h"

class GameOver : public IScene {
public:
	~GameOver();
	void Initialize() override;
	void Update() override;
	void Draw() override;

	void Finalize() override;

private:
	std::unique_ptr<Fade> fade_;

	Input* input_ = nullptr;

	Audio* audio_ = nullptr;
	
	size_t soundHandle_;
	size_t playHandle_;

	size_t selectSoundHandle_;
	size_t selectPlayHandle_;

	bool isStart_ = true;
	bool easingFlag_ = true;
	bool floatFlag_ = false;

	float easeingCount_;
	float easeingSecond_ = 0.01f;

	uint32_t floatTimer_;
	float floatValue_ = -0.5f;
	Vector4 colorValue_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	float alphaValue_ = -0.01f;

	ViewProjection viewProjection_;

	Vector2 gameOverSpritePosition_;
	Vector2 pressSpacePosition_;

	Vector2 gameOverStartPosition_;
	Vector2 pressSpaceStartPosition_;
	Vector2 gameOverEndPosition_;
	Vector2 pressSpaceEndPosition_;

	std::unique_ptr<BackGround> backGround_;
	std::vector<uint32_t> backGroundTextureHandles_;
	std::unique_ptr<Frame> frame_;
	std::vector<Model*>frameModel_;
	Sprite* gameOverSprite_;
	Sprite* pressSpaceSprite_;
};