#pragma once

#include "IScene.h"
#include "Input.h"
#include "Fade.h"
#include "Audio.h"

#include "BackGround.h"
#include "Frame.h"
#include "FollowCamera.h"
#include "TitleBoss.h"
#include "TitlePlayer.h"

class TitleScene : public IScene {
public:
	~TitleScene() override;

	void Initialize() override;
	void Update() override;
	void Draw() override;

	void Finalize() override;

private:
	std::unique_ptr<Fade> fade_;

	Input* input_ = nullptr;
	ViewProjection viewProjection_;
	Audio* audio_ = nullptr;
	size_t soundHandle_;
	size_t selectSoundHandle_;

	int count_;

	std::unique_ptr<BackGround> backGround_;
	std::vector<uint32_t> backGroundTextureHandles_;
	std::unique_ptr<Frame> frame_;
	std::vector<Model*>frameModel_;
	std::unique_ptr<FollowCamera> followCamera_;
	std::unique_ptr<TitleBoss> titleBoss_;
	std::vector<Model*> titleBossModel_;
	std::unique_ptr<TitlePlayer> player_;
	std::vector<Model*> playerModel_;
};