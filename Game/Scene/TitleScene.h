#pragma once

#include "IScene.h"
#include "Input.h"
#include "Fade.h"

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
	int count_;
};