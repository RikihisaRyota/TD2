#include "SceneManager.h"
#include "ImGuiManager.h"

SceneManager::SceneManager() {

}

SceneManager::~SceneManager() {
	
}

void SceneManager::Run() {
	Initialize();

	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		imguiManager->Begin();
		input->Update();
		//audio->Update();

		TextureManager::GetInstance()->PreDraw();
		dxCommon->PreDraw();

		// ステートパターン化
		preSceneNumber_ = sceneNumber_;
		sceneNumber_ = sceneArray_[sceneNumber_]->GetSceneNumber();

		if (sceneNumber_ != preSceneNumber_) {
			sceneArray_[preSceneNumber_]->Finalize();
			sceneArray_[sceneNumber_]->Initialize();
		}

		sceneArray_[sceneNumber_]->Update();
		
		sceneArray_[sceneNumber_]->Draw();
		
		imguiManager->End();
		imguiManager->Draw();

		dxCommon->PostDraw();
	}

	// スプライト
	Sprite::Release();

	// ImGui解放
	imguiManager->Finalize();

	// Line解放
	PrimitiveDrawer::Release();

	// テクスチャマネージャーの解放
	TextureManager::Release();

	// DirectXの解放
	dxCommon->Release();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();
}

void SceneManager::Initialize() {
	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"DirectXClass");

	// DirectXの初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon);

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// BasicGraphicsPipelineの静的初期化
	BasicGraphicsPipline::SetDevice(dxCommon->GetDevice());

	// ToonGraphicsPipelineの静的初期化
	ToonGraphicsPipline::SetDevice(dxCommon->GetDevice());

	// LineGraphicsPipelineの静的初期化
	LineGraphicsPipline::SetDevice(dxCommon->GetDevice());

	// Planeの静的初期化
	PlaneRenderer::SetDevice(dxCommon->GetDevice());

	// OBJの静的初期化
	OBJ::SetDevice(dxCommon->GetDevice());

	// Cubeのデバイスセット
	CubeRenderer::SetDevice(dxCommon->GetDevice());

	// Sphereのデバイスセット
	SphereRenderer::SetDevice(dxCommon->GetDevice());

	// Material
	Material::SetDevice(dxCommon->GetDevice());

	// Mesh
	Mesh::SetDevice(dxCommon->GetDevice());

	// Model
	Model::SetDevice(dxCommon->GetDevice());

	// Line
	PrimitiveDrawer::SetDevice(dxCommon->GetDevice());
	PrimitiveDrawer::GetInstance()->Initialize();

	// ImGuiの初期化
	imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// Audio
	Audio* audio = Audio::GetInstance();
	audio->Initialize();

	sceneArray_[TITLE_SCENE] = std::make_unique<TitleScene>();
	sceneArray_[GAME_SCENE] = std::make_unique<GameScene>();
	sceneArray_[CLEAR_SCENE] = std::make_unique<GameClear>();
	sceneArray_[OVER_SCENE] = std::make_unique<GameOver>();

	sceneNumber_ = TITLE_SCENE;
	sceneArray_[sceneNumber_]->Initialize();
}