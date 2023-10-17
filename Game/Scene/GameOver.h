#pragma once

#include "IScene.h"
#include "Input.h"
#include "TextureManager.h"
#include "Sprite.h"

class GameOver : public IScene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;

	void Finalize() override;

private:
	Input* input_ = nullptr;
};