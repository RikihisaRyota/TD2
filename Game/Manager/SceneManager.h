#pragma once

#include "IScene.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "GameOver.h"
#include "GameClear.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "WinApp.h"
#include "DirectXCommon.h"

class SceneManager {
public:
	SceneManager();
	~SceneManager();

	void Run();
	void Init();

private:
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;

	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	Audio* audio = Audio::GetInstance();
	GameScene* gameScene = nullptr;

	std::unique_ptr<IScene> sceneArray_[6];

	int sceneNumber_;
	int preSceneNumber_;
 };