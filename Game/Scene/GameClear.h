#pragma once

#include "Audio.h"
#include "IScene.h"
#include "Input.h"
#include "Fade.h"

#include "BackGround.h"
#include "ClearSprite.h"
#include "Frame.h"
#include "FollowCamera.h"
#include "TreasureBox.h"

class GameClear : public IScene {
public:
	~GameClear();
	void Initialize() override;
	void Update() override;
	void Draw() override;

	void Finalize() override;

private:
	std::unique_ptr<Fade> fade_;

	Input* input_ = nullptr;

	Audio* audio_ = nullptr;
	size_t soundHandle_;
	size_t selectSoundHandle_;

	bool isStart_ = true;

	ViewProjection viewProjection_;

	std::unique_ptr<BackGround> backGround_;
	std::vector<uint32_t> backGroundTextureHandles_;
	std::unique_ptr<ClearSprite> clearSprite_;
	std::unique_ptr<Frame> frame_;
	std::vector<Model*>frameModel_;
	std::unique_ptr<FollowCamera> followCamera_;
	std::unique_ptr<TreasureBox> treasureBox_;
	std::vector<Model*> modelTreasureBox_;
};