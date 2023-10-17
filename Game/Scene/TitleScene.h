#pragma once

#include "IScene.h"
#include "Input.h"

class TitleScene : public IScene {
public:
	~TitleScene() override;

	void Initialize() override;
	void Update() override;
	void Draw() override;

	void Finalize() override;

private:
	Input* input_ = nullptr;
	int count_;
};