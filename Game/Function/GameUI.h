#pragma once

#include "Sprite.h"
#include "TextureManager.h"

#include <memory>

class GameUI {
public:
	void Initialize();
	void Update();
	void Draw();
	
	~GameUI();

private:
#pragma region スプライト
	Sprite* shellIcon_;

	Sprite* bossIcon_;
	Sprite* bossHP_;
	Sprite* bossHPUnder_;

	Sprite* ikaHP_[3];
	Sprite* ikaMove_;
	Sprite* ikaPower_;
	Sprite* numberSheet_;
	Sprite* ikaMoveSheet_;

	Sprite* route_;
#pragma endregion

#pragma region リソース
	uint32_t shellIconHandle_;

	uint32_t bossIconHandle_;
	uint32_t bossHPHandle_;
	uint32_t bossHPUnderHandle_;

	uint32_t ikaHPHandle_;
	uint32_t ikaMoveHandle_;
	uint32_t ikaPowerHandle_;

	uint32_t ikaMoveSheetHandle_;
	uint32_t numberSheetHandle_;
	uint32_t routeHandle_;
#pragma endregion

#pragma region 変数群
	// Press Space Move の変数
	Vector2 moveSheetSize_;
	float moveSheetAnimation_;

	// アニメーション関連の変数
	int animationTimer_;
	bool isAnimation_;
#pragma endregion
};