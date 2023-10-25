#pragma once

#include "Sprite.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "TextureManager.h"

#include <memory>
#include <iostream>
#include <vector>

class Player;
class Boss;

class GameUI {
public:
	~GameUI();

	void Initialize();
	void Update();
	void Draw();
	
	void SetObject(Player* player, Boss* boss) { player_ = player; boss_ = boss; }
	
	std::vector<int> SeparateDigits(int number);

private:
#pragma region インクルード
	Player* player_;
	Boss* boss_;
	Input* input_ = nullptr;
#pragma endregion

#pragma region スプライト
	Sprite* shellIcon_;
	Sprite* sharkIcon_;
	Sprite* bossIcon_;
	Sprite* bossHP_;
	Sprite* bossSheet_[2];
	Sprite* bossHPUnder_;

	Sprite* ikaHP_[3];
	Sprite* ikaMove_;
	Sprite* ikaPower_;
	Sprite* numberSheet_[2];
	Sprite* ikaMoveSheet_;

	Sprite* route_;
#pragma endregion

#pragma region リソース
	uint32_t shellIconHandle_;
	uint32_t sharkIconHandle_;
	uint32_t bossIconHandle_;
	uint32_t bossHPHandle_;
	uint32_t bossSheetHandle_;
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

	// 食べた数のアニメーション
	Vector2 eatSize_;
	std::vector<int> eatAnimation_;
	int eatDigit_;

	// ボスの体力バー
	Vector2 bossSize_;
	std::vector<int> bossAnimation_;
	int bossDigit_;
#pragma endregion
};