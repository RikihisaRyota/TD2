#include "TitleScene.h"

TitleScene::TitleScene()
{
	input_ = Input::GetInstance();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
}

void TitleScene::Update()
{
	if (input_->PushKey(DIK_I)) {
		sceneNo = InGame;
	}
}

void TitleScene::BackDraw()
{

}

void TitleScene::Draw()
{

}

void TitleScene::foreDraw()
{

}

void TitleScene::Release()
{
}
