#pragma once

#include "IScene.h"
#include "Input.h"
#include "Fade.h"
#include "Audio.h"

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
	
	Audio* audio_ = nullptr;
	size_t soundHandle_;
	size_t selectSoundHandle_;

	int count_;
};