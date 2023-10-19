#pragma once

#pragma region エンジン
#include "BasicGraphicsPipline.h"
#include "Input.h"
#include "TextureManager.h"
#include "Audio.h"
#include "ImGuiManager.h"
#include "DirectXCommon.h"
#pragma endregion

#pragma region シーン
#include <IScene.h>
#include <TitleScene.h>
#include <GameScene.h>
#include <GameClear.h>
#include <GameOver.h>
#pragma endregion

class SceneManager {
public:
	SceneManager();
	~SceneManager();

	void Run();
	void Initialize();

private:
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;

	GameScene* gameScene = nullptr;
	ImGuiManager* imguiManager;
	Audio* audio;

	std::unique_ptr<IScene> sceneArray_[4];

	int sceneNumber_;
	int preSceneNumber_;
 };