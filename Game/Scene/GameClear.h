#pragma once

#include "IScene.h"
#include "Input.h"

class GameClear : public IScene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;

	void Finalize() override;

private:
	Input* input_ = nullptr;
};