#include "GameScene.h"

#include "DirectXCommon.h"
#include "MyMath.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Draw.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	// デバックカメラ
	debugCamera_ = new DebugCamera();
	// 入力
	input_ = Input::GetInstance();
	// カメラの初期化
	viewProjection_.Initialize();
#pragma region 生成
	csv_ = std::make_unique<CSV>();
	floor_ = std::make_unique<CubeRenderer>();
	followCamera_ = std::make_unique<FollowCamera>();
	player_ = std::make_unique<Player>();
	playerModel_ = std::make_unique<Model>();
#pragma endregion
#pragma region 初期化
	// CSV
	csv_->LoadCSV("Resources/CSV/Spaw.csv");
	std::vector<CSV::Date> date = csv_->UpdataDateCommands();
	// 床
	floor_.reset(CubeRenderer::Create());
	floorWorldTransform_.Initialize();
	floorWorldTransform_.translation_ = { 0.0f,-2.0f,0.0f };
	floorWorldTransform_.scale_ = { 20.0f,1.0f,1.0f };
	floorWorldTransform_.UpdateMatrix();
	// カメラ
	followCamera_->Initialize();
	followCamera_->SetTarget(&player_->GetWorldTransform());
	// プレイヤー
	playerModel_.reset(Model::Create("Player"));
	player_->Initialize(playerModel_.get());
#pragma endregion

}

void GameScene::Update() {
	// デバックカメラ
	debugCamera_->Update(&viewProjection_);
	
	player_->Update();

	//followCamera_->Update();
	//viewProjection_ = followCamera_->GetViewProjection();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	dxCommon_->ClearDepthBuffer();
#pragma endregion
#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	CubeRenderer::PreDraw(commandList);
	SphereRenderer::PreDraw(commandList);
	OBJ::PreDraw(commandList);
	Model::PreDraw(commandList);
	PrimitiveDrawer::PreDraw(commandList);
	PlaneRenderer::PreDraw(commandList);
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	floor_->Draw(floorWorldTransform_,viewProjection_);
	player_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	PlaneRenderer::PostDraw();
	PrimitiveDrawer::PostDraw();
	Model::PostDraw();
	SphereRenderer::PostDraw();
	OBJ::PostDraw();
	CubeRenderer::PostDraw();
#pragma endregion



#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	Sprite::SetBlendState(Sprite::BlendState::kNormal);
	

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::Release() {
	SafeDelete(debugCamera_);
}
