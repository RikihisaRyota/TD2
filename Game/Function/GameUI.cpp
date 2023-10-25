#include "GameUI.h"
#include "Player.h"
#include "Boss.h"

GameUI::~GameUI() {
	delete shellIcon_;
	delete bossIcon_;
	delete bossHP_;
	delete bossHPUnder_;

	for (int i = 0; i < 3; i++) {
		delete ikaHP_[i];
	}
	delete ikaMove_;
	delete ikaPower_;

	delete numberSheet_;
	
	delete ikaMoveSheet_;

	delete route_;
}

void GameUI::Initialize() {
	moveSheetSize_ = { 256.0f,256.0f };
	moveSheetAnimation_ = 5.0f;

	animationTimer_ = 0;

	eatSize_ = { 96.0f,96.0f };
	eatAnimation_ = 1;
	eatDigit = 10;

	shellIconHandle_ = TextureManager::Load("Resources/UI/shellIcon.png");

	bossIconHandle_ = TextureManager::Load("Resources/UI/bossIcon.png");
	bossHPHandle_ = TextureManager::Load("Resources/UI/hpbar.png");
	bossHPUnderHandle_ = TextureManager::Load("Resources/UI/hpbarsitaji.png");

	ikaHPHandle_ = TextureManager::Load("Resources/UI/ikaHP.png");
	ikaMoveHandle_ = TextureManager::Load("Resources/UI/ikamove.png");
	ikaPowerHandle_ = TextureManager::Load("Resources/UI/ikaPower.png");
	ikaMoveSheetHandle_ = TextureManager::Load("Resources/UI/ikamoveSheet.png");

	numberSheetHandle_ = TextureManager::Load("Resources/UI/numberSheet.png");
	routeHandle_ = TextureManager::Load("Resources/UI/ruto.png");

	shellIcon_ = Sprite::Create(shellIconHandle_, { 570,670 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	
	bossIcon_ = Sprite::Create(bossIconHandle_, { 750,670 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	bossHP_ = Sprite::Create(bossHPHandle_, { 640,50 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	bossHPUnder_ = Sprite::Create(bossHPUnderHandle_, { 640,50 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	
	ikaHP_[0] = Sprite::Create(ikaHPHandle_, { 50,660 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	ikaHP_[1] = Sprite::Create(ikaHPHandle_, { 120,660 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	ikaHP_[2] = Sprite::Create(ikaHPHandle_, { 190,660 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	ikaMove_ = Sprite::Create(ikaMoveHandle_, { 0,0 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	ikaPower_ = Sprite::Create(ikaPowerHandle_, { 1200,70 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	
	ikaMoveSheet_ = Sprite::Create(ikaMoveSheetHandle_, { 1170,620 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	ikaMoveSheet_->SetSize(Vector2(120.0f, 120.0f));

	numberSheet_ = Sprite::Create(numberSheetHandle_, {1220,80}, {1.0f,1.0f,1.0f,1.0f}, {0.5f,0.5f}, false, false);
	numberSheet_->SetSize(Vector2(60.0f, 60.0f));

	route_ = Sprite::Create(routeHandle_, { 640,660 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
}

void GameUI::Update() {
	animationTimer_++;

	if (animationTimer_ % 30 == 0) {
		moveSheetAnimation_ += 1.0f;
	}

	if (animationTimer_ >= 120) {
		animationTimer_ = 0;
	}

	if (moveSheetAnimation_ >= 5.0f) {
		moveSheetAnimation_ = 0.0f;
	}
}

void GameUI::Draw() {
	route_->Draw();

	shellIcon_->Draw();

	bossIcon_->Draw();
	bossHPUnder_->Draw();
	bossHP_->Draw();
	
	if (player_->GetPlayerHP() == 3) {
		for (int i = 0; i < 3; i++) {
			ikaHP_[i]->Draw();
		}
	}
	else if (player_->GetPlayerHP() == 2) {
		for (int i = 0; i < 2; i++) {
			ikaHP_[i]->Draw();
		}
	}
	else {
		ikaHP_[0]->Draw();
	}
	
	ikaPower_->Draw();

	ikaMoveSheet_->SetTextureRect({ moveSheetSize_.x * moveSheetAnimation_,0.0f }, { moveSheetSize_ });
	ikaMoveSheet_->Draw();
	
	numberSheet_->SetTextureRect({ eatSize_.x * eatAnimation_,0.0f }, { eatSize_ });
	numberSheet_->Draw();
}