#include "GameScene.h"

#include "CSV.h"
#include "DirectXCommon.h"
#include "MyMath.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Draw.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	for (Enemy* enemy : enemy_) {
		delete enemy;
	}
	enemy_.clear();
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
	enemyEditor_ = std::make_unique<EnemyEditor>();
	frame_ = std::make_unique<Frame>();
	followCamera_ = std::make_unique<FollowCamera>();
	player_ = std::make_unique<Player>();
	playerModel_ = std::make_unique<Model>();
#pragma endregion
#pragma region 初期化
	// CSV
	LoadCSVData("Resources/CSV/Spaw.csv", &enemyPopCommands_);
	auto textureHandle = TextureManager::Load("Resources/Images/back.png");
	backGround_->Initialize(textureHandle);
	// 枠組み
	frame_->SetPlayer(player_.get());
	frame_->Initialize();
	// カメラ
	followCamera_->SetTarget(&player_->GetWorldTransform());
	followCamera_->SetPlayer(player_.get());
	followCamera_->Initialize();
	// プレイヤー
	playerModel_.reset(Model::Create("Player"));
	player_->Initialize(playerModel_.get());
	enemyModel_.reset(Model::Create("Enemy"));
	UpdateEnemyPopCommands();
#pragma endregion
}

void GameScene::Update() {
	frame_->Update();
	player_->Update();

	//for (Enemy* enemy : enemy_) {
	//	enemy->Update();
	//	// 当たり判定
	//	if (IsCollision(player_->GetOBB(), enemy->GetObb())) {
	//		// 落下中ではない
	//		if (!player_->GetIsDrop()) {
	//			if (!enemy->GetFlag()) {
	//				enemy->SetPosition(player_->GetWorldPosition());
	//				/// プレイヤーに当たったか
	//				enemy->SetFlag(true);
	//				// プレイヤーにおもりを追加
	//				player_->SetWeight(1);
	//			}
	//			else {
	//				enemy->SetPosition(player_->GetWorldPosition());
	//			}
	//		}
	//		else {
	//			if (!enemy->GetFlag()) {
	//				enemy->SetPosition(player_->GetWorldPosition());
	//				enemy->SetFlag(true);
	//				player_->SetIsHitStop(true);
	//			}
	//			else {
	//				enemy->SetPosition(player_->GetWorldPosition());
	//			}
	//		}
	//	}
	//}
	// 敵生成
	enemyEditor_->Update(enemy_,enemyModel_.get());
	// 0を押すとカメラを切り替える
	if (input_->TriggerKey(DIK_0)) {
		IsDebugCamera_ ^= true;
	}
	if (IsDebugCamera_) {
		// デバックカメラ
		debugCamera_->Update(&viewProjection_);
	}
	else {
		followCamera_->Update();
		viewProjection_ = followCamera_->GetViewProjection();
	}
	// リセット
	if (input_->TriggerKey(DIK_R)) {
		for (Enemy* enemy : enemy_) {
			delete enemy;
		}
		enemy_.clear();
		LoadCSVData("Resources/CSV/Spaw.csv", &enemyPopCommands_);
		UpdateEnemyPopCommands();
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
	backGround_->Draw(viewProjection_);
	frame_->Draw(viewProjection_);
	player_->Draw(viewProjection_);

	/*for (Enemy* enemy : enemy_) {
		enemy->Draw(viewProjection_);
	}*/

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

void GameScene::LoadCSVData(const char* csvName, std::stringstream* popCommands) {
	std::ifstream file;

	file.open(csvName);
	assert(file.is_open());

	*popCommands << file.rdbuf();

	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	std::string line;
	enemyPopCommands_.clear();
	enemyPopCommands_.seekg(0, std::ios_base::beg);

	while (getline(enemyPopCommands_, line)) {
		std::istringstream line_stream(line);

		std::string word;

		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		float x = 0, y = 0, z = 0;
		uint32_t type = 0u;

		if (word.find("Position") == 0) {
			getline(line_stream, word, ',');
			x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			z = (float)std::atof(word.c_str());

		}

		if (word.find("Type") == 0) {
			getline(line_stream, word, ',');
			type = (uint32_t)std::atof(word.c_str());
		}
		SpawnEnemy(Vector3(x, y, z), type);

	}
}

void GameScene::SpawnEnemy(const Vector3& position, uint32_t type) {
	Enemy* enemy = new Enemy();

	enemy->Initialize(enemyModel_.get(), position, type);

	enemy_.push_back(enemy);
}