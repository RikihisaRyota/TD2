#pragma once

#include "Sprite.h"
#include "TextureManager.h"
#include "ImGuiManager.h"

class Fade {
public:
	~Fade();

	void Initialize();

	void FadeInUpdate();
	void FadeOutUpdate();
	
	void FadeInDraw();
	void FadeOutDraw();

	void Finalize();

	bool FadeInFlagSet(bool flag) { return isCount_[0] = flag; }
	bool FadeOutFlagSet(bool flag) { return isCount_[1] = flag; }

	float GetColor(int num) { return isCount_[num]; }

private:
	Sprite* sprite_[2];
	uint32_t textureHandle_;

	float fadeColor_[2];
	bool isCount_[2];
};