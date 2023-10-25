#pragma once

#include "IScene.h"
#include "Input.h"
#include "Fade.h"

#include "BackGround.h"
#include "Frame.h"

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

	bool isStart_ = true;

	ViewProjection viewProjection_;

	std::unique_ptr<BackGround> backGround_;
	std::vector<uint32_t> backGroundTextureHandles_;
	std::unique_ptr<Frame> frame_;
	std::vector<Model*>frameModel_;
};