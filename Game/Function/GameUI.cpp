#include "GameUI.h"
#include "Player.h"
#include "Boss.h"

GameUI::~GameUI() {
	delete shellIcon_;
	delete bossIcon_;
	delete sharkIcon_;

	delete bossHP_;
	delete bossHPUnder_;

	for (int i = 0; i < 3; i++) {
		delete ikaHP_[i];
	}
	delete ikaMove_;
	delete ikaPower_;

	for (int i = 0; i < 2; i++) {
		delete numberSheet_[i];
	}

	delete ikaMoveSheet_;

	delete route_;
}

void GameUI::Initialize() {
	input_ = Input::GetInstance();

	moveSheetSize_ = { 256.0f,256.0f };
	moveSheetAnimation_ = 5.0f;

	animationTimer_ = 0;
	bossDigit_ = 0;

	bossLifeRect_ = { 0.0f,64.0f };

	eatSize_ = { 96.0f,96.0f };

	shellIconHandle_ = TextureManager::Load("Resources/UI/shellIcon.png");
	sharkIconHandle_ = TextureManager::Load("Resources/UI/sharkIcon.png");

	bossIconHandle_ = TextureManager::Load("Resources/UI/bossIcon.png");
	bossHPHandle_ = TextureManager::Load("Resources/UI/hpbar.png");
	bossHPUnderHandle_ = TextureManager::Load("Resources/UI/hpbarsitaji.png");

	ikaHPHandle_ = TextureManager::Load("Resources/UI/ikaHP.png");
	ikaMoveHandle_ = TextureManager::Load("Resources/UI/ikamove.png");
	ikaPowerHandle_ = TextureManager::Load("Resources/UI/ikaPower.png");
	ikaMoveSheetHandle_ = TextureManager::Load("Resources/UI/ikamoveSheet.png");

	numberSheetHandle_ = TextureManager::Load("Resources/UI/numberSheet.png");
	routeHandle_ = TextureManager::Load("Resources/UI/ruto.png");

	shellIcon_ = Sprite::Create(shellIconHandle_, { 470,670 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	sharkIcon_ = Sprite::Create(sharkIconHandle_, { 810,670 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	bossIcon_ = Sprite::Create(bossIconHandle_, { 640,670 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	bossHP_ = Sprite::Create(bossHPHandle_, { 640,50 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	bossHPUnder_ = Sprite::Create(bossHPUnderHandle_, { 640,50 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	
	ikaHP_[0] = Sprite::Create(ikaHPHandle_, { 70,650 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	ikaHP_[1] = Sprite::Create(ikaHPHandle_, { 140,650 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	ikaHP_[2] = Sprite::Create(ikaHPHandle_, { 210,650 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	ikaMove_ = Sprite::Create(ikaMoveHandle_, { 0,0 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	ikaPower_ = Sprite::Create(ikaPowerHandle_, { 1200,70 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	
	ikaMoveSheet_ = Sprite::Create(ikaMoveSheetHandle_, { 1150,600 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	ikaMoveSheet_->SetSize(Vector2(150.0f, 150.0f));

	numberSheet_[0] = Sprite::Create(numberSheetHandle_, {1220,80}, {1.0f,1.0f,1.0f,1.0f}, {0.5f,0.5f}, false, false);
	numberSheet_[0]->SetSize(Vector2(60.0f, 60.0f));
	numberSheet_[1] = Sprite::Create(numberSheetHandle_, { 1180,80 }, { 1.0f,1.0f,1.0f,1.0f }, { 0.5f,0.5f }, false, false);
	numberSheet_[1]->SetSize(Vector2(60.0f, 60.0f));

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

	bossHP_->SetSize({ ((float)640 * boss_->GetBossHP() / 5),64 });
}

void GameUI::Draw() {
	// 道すべ
	route_->Draw();

	// アイコン
	shellIcon_->Draw();
	bossIcon_->Draw();
	sharkIcon_->Draw();

	// ボス体力
	bossHPUnder_->Draw();

	bossHP_->Draw();
	
	// プレイヤー体力
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
	
	// 右上の食べた数
	ikaPower_->Draw();

	// プレススペース
	ikaMoveSheet_->SetTextureRect({ moveSheetSize_.x * moveSheetAnimation_,0.0f }, { moveSheetSize_ });
	ikaMoveSheet_->Draw();
	
	// 食べた数用の数字連番
	eatDigit_ = player_->GetWeightNum();
	eatAnimation_ = SeparateDigits(eatDigit_);

	numberSheet_[0]->SetTextureRect({eatSize_.x * eatAnimation_[0],0.0f}, {eatSize_});
	numberSheet_[0]->Draw();
	numberSheet_[1]->SetTextureRect({ eatSize_.x * eatAnimation_[1],0.0f}, {eatSize_});
	numberSheet_[1]->Draw();
}

std::vector<int> GameUI::SeparateDigits(int number) {
	std::vector<int> digits;

	int tensDigits = number % 10;
	int onesDigits = number / 10;

	digits.push_back(tensDigits);
	digits.push_back(onesDigits);

	return digits;
}