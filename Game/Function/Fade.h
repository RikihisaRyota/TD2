#pragma once

#include "Sprite.h"
#include "TextureManager.h"
#include "ImGuiManager.h"

class Fade {
public:
	~Fade();

	void Initialize();
	void Update();
	void Draw();

	void Finalize();

	// 黒から徐々に明るくする場合は1 明るい状態から徐々に暗くする場合は2
	int SetFlag(int isCount) { return isCount_ = isCount; }

	float GetColor() { return fadeColor_; }

private:
	Sprite* sprite_;
	uint32_t textureHandle_;

	float fadeColor_;
	int isCount_ = 0;
};