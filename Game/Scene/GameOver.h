#pragma once

#include "IScene.h"
#include "Input.h"
#include "TextureManager.h"
#include "Fade.h"

class GameOver : public IScene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;

	void Finalize() override;

private:
	std::unique_ptr<Fade> fade_;

	Input* input_ = nullptr;

	bool isStart_ = true;
};