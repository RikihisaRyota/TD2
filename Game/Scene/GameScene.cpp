#include "GameScene.h"

#include "CSV.h"
#include "DirectXCommon.h"
#include "MyMath.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Draw.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	for (auto& model : enemyModels_Type0_) {
		delete model;
	}
	for (auto& model : enemyModels_Type1_) {
		delete model;
	}
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	// デバックカメラ
	debugCamera_ = new DebugCamera();
	IsDebugCamera_ = false;
	// 入力
	input_ = Input::GetInstance();
	// カメラの初期化
	viewProjection_.Initialize();
#pragma region 生成
	backGround_ = std::make_unique<BackGround>();
	boss_ = std::make_unique<Boss>();
	collisionManager_ = std::make_unique<CollisionManager>();
	enemyEditor_ = std::make_unique<EnemyEditor>();
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyBulletManager_ = std::make_unique<EnemyBulletManager>();
	frame_ = std::make_unique<Frame>();
	followCamera_ = std::make_unique<FollowCamera>();
	player_ = std::make_unique<Player>();
	playerBulletManager_ = std::make_unique<PlayerBulletManager>();
	playerBulletModel_ = std::make_unique<Model>();
	uvula_ = std::make_unique<Uvula>();
	uvulaHead_ = std::make_unique<Model>();
	uvulaBody_= std::make_unique<Model>();
	fade_ = std::make_unique<Fade>();
#pragma endregion
#pragma region 初期化
	auto textureHandle = TextureManager::Load("Resources/Images/back.png");
	backGround_->Initialize(textureHandle);
	// 枠組み
	frame_->SetPlayer(player_.get());
	frame_->SetUvula(uvula_.get());
	frame_->Initialize();
	// カメラ
	followCamera_->SetTarget(&player_->GetWorldTransform());
	followCamera_->SetPlayer(player_.get());
	followCamera_->Initialize();
	// プレイヤー
	playerModel_.emplace_back(Model::Create("playerBody"));
	playerModel_.emplace_back(Model::Create("playerLegLeft"));
	playerModel_.emplace_back(Model::Create("playerLegRight"));
	playerBulletModel_.reset(Model::Create("playerBullet"));
	player_->SetViewProjection(&viewProjection_);
	player_->SetPlayerBulletManager(playerBulletManager_.get());
	player_->Initialize(playerModel_);
	playerBulletManager_->SetViewProjection(&viewProjection_);
	playerBulletManager_->Initialize(playerBulletModel_.get());

	// 敵
	enemyModel_.reset(Model::Create("Enemy"));
	//enemyModels_.clear();
	enemyModels_Type0_ = {
		Model::Create("octopusHead"), Model::Create("octopusLeg")
	};
	enemyModels_Type1_ = {
		Model::Create("toge")
	};
	enemyBulletManager_->SetViewProjection(&viewProjection_);
	enemyBulletManager_->SetPlayer(player_.get());
	enemyBulletManager_->Initialize(enemyModel_.get());
	enemyManager_->SetViewProjection(&viewProjection_);
	enemyManager_->SetPlayer(player_.get());
	//enemyManager_->Initialize(enemyModel_.get());
	enemyManager_->Initialize(enemyModels_Type0_, enemyModels_Type1_);
	enemyManager_->SetEnemyBulletManager(enemyBulletManager_.get());
	// CSVからデータの読み込み
	std::unique_ptr<CSV> csv = std::make_unique<CSV>();
	csv->LoadCSV("Spaw");
	std::vector<CSV::Data> datas = csv->UpdateDataCommands();
	// 読み込んだデータから生成
	for (CSV::Data data : datas) {
		SpawnEnemy(data.position, data.type);
	}
	// ベロ
	uvulaHead_.reset(Model::Create("uvulaHead"));
	uvulaBody_.reset(Model::Create("uvulaBody"));
	uvula_->SetPlayer(player_.get());
	uvula_->Initialize(uvulaHead_.get(), uvulaBody_.get());
	// ボス
	bossModel_.emplace_back(Model::Create("bossOnJaw"));
	bossModel_.emplace_back(Model::Create("bossLowerJaw"));
	boss_->SetPlayer(player_.get());
	boss_->Initialize(bossModel_);

	fade_->Initialize();

	isGameStart_ = false;
	isGameEnd_ = false;
	isGameOver_ = false;
#pragma endregion
}

void GameScene::Update() {
	fade_->FadeOutUpdate();
	if (fade_->GetColor(1) < 0.0f) {
		isGameStart_ = true;
	}

	if (isGameStart_ == true) {
		frame_->Update();
		player_->Update();
		enemyManager_->Update();
		playerBulletManager_->Update();
		enemyBulletManager_->Update();
		uvula_->Update();
		boss_->Update();
		fade_->FadeInUpdate();

		// 敵生成
		//enemyEditor_->Update(enemyManager_.get(), enemyModel_.get());
		collisionManager_->Update(player_.get(), playerBulletManager_.get(), enemyManager_.get(), enemyBulletManager_.get(), uvula_.get());

		if (!IsDebugCamera_) {
			frame_->Update();
			player_->Update();
			// デバック
			enemyManager_->SetIsDebug(IsDebugCamera_);
			//
			enemyManager_->Update();
			playerBulletManager_->Update();
			enemyBulletManager_->Update();
			uvula_->Update();
			boss_->Update();
			// 敵生成
			collisionManager_->Update(player_.get(), playerBulletManager_.get(), enemyManager_.get(), enemyBulletManager_.get(), uvula_.get());
			// shiftを押すとカメラを切り替える
			if (input_->TriggerKey(DIK_LSHIFT)) {
				IsDebugCamera_ ^= true;
			}
			followCamera_->Update();
			viewProjection_ = followCamera_->GetViewProjection();
		}
		else {
			// shiftを押すとカメラを切り替える
			if (input_->TriggerKey(DIK_LSHIFT)) {
				IsDebugCamera_ ^= true;
			}
			enemyEditor_->Update(enemyManager_.get());
			// デバックカメラ
			debugCamera_->Update(&viewProjection_);
			enemyManager_->SetIsDebug(IsDebugCamera_);
		}

	}

	ImGui::Begin("SceneManage");
	ImGui::InputInt("SceneNumber", &sceneNumber_);
	ImGui::Text("Game Scene");
	ImGui::End();


	if (input_->PushKey(DIK_8)) {
		isGameEnd_ = true;
	}

	if (input_->PushKey(DIK_9)) {
		isGameOver_ = true;
	}

	if (fade_->GetColor(0) > 1.0f && isGameEnd_ == true) {
		sceneNumber_ = CLEAR_SCENE;
	}
	else if (fade_->GetColor(0) > 1.0f && isGameOver_ == true) {
		sceneNumber_ = OVER_SCENE;
	}
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
	player_->Draw(viewProjection_);
	backGround_->Draw(viewProjection_);
	frame_->Draw(viewProjection_);
	uvula_->Draw(viewProjection_);
	enemyManager_->Draw(viewProjection_);
	enemyBulletManager_->Draw(viewProjection_);
	playerBulletManager_->Draw(viewProjection_);
	boss_->Draw(viewProjection_);

	PrimitiveDrawer::GetInstance()->Draw();
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

	if (isGameStart_ == false) {
		fade_->FadeOutFlagSet(true);
		fade_->FadeOutDraw();
	}

	if (isGameEnd_ == true || isGameOver_ == true) {
		fade_->FadeInFlagSet(true);
		fade_->FadeInDraw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::Finalize() {
	SafeDelete(debugCamera_);
}

void GameScene::Release() {
	
}

void GameScene::SpawnEnemy(const Vector3& position, uint32_t type) {
	enemyManager_->Create(position, type);
}