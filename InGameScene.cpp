#include "InGameScene.h"

#include "CSV.h"
#include "DirectXCommon.h"
#include "MyMath.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Draw.h"

InGameScene::InGameScene()
{
	// 入力
	input_ = Input::GetInstance();
}

InGameScene::~InGameScene()
{
	viewProjection_ = new ViewProjection();
	delete viewProjection_;
	for (Enemy* enemy : enemy_) {
		delete enemy;
	}
	enemy_.clear();
}

void InGameScene::Initialize()
{
	// デバックカメラ
	debugCamera_ = new DebugCamera();
	IsDebugCamera_ = false;
	// カメラの初期化
	viewProjection_ = new ViewProjection();
	viewProjection_->Initialize();
#pragma region 生成
	/*backGround_ = std::make_unique<BackGround>();
	enemyEditor_ = std::make_unique<EnemyEditor>();
	frame_ = std::make_unique<Frame>();
	followCamera_ = std::make_unique<FollowCamera>();
	player_ = std::make_unique<Player>();
	playerModel_ = std::make_unique<Model>();*/
	backGround_ = new BackGround();
	enemyEditor_ = new EnemyEditor();
	frame_ = new Frame();
	followCamera_ = new FollowCamera();
	player_ = new Player();
	playerModel_ = new Model();
#pragma endregion
#pragma region 初期化
	auto textureHandle = TextureManager::Load("Resources/Images/back.png");
	backGround_->Initialize(textureHandle);
	// 枠組み
	frame_->SetPlayer(player_);
	frame_->Initialize();
	// カメラ
	followCamera_->SetTarget(&player_->GetWorldTransform());
	followCamera_->SetPlayer(player_);
	followCamera_->Initialize();
	// プレイヤー
	playerModel_ = Model::Create("Player");
	player_->Initialize(playerModel_);
	enemyModel_ = Model::Create("Enemy");
	// CSVからデータの読み込み
	std::unique_ptr<CSV> csv = std::make_unique<CSV>();
	csv->LoadCSV("Resources/CSV/Spaw.csv");
	std::vector<CSV::Data> datas = csv->UpdateDataCommands();
	// 読み込んだデータから生成
	for (CSV::Data data : datas) {
		SpawnEnemy(data.position, data.type);
	}
}

void InGameScene::Update()
{
	if (input_->PushKey(DIK_T)) {
		sceneNo = TITLE;
	}

	frame_->Update();
		player_->Update();

		uint32_t enemyCount = 0;
		for (Enemy* enemy : enemy_) {
			enemy->Update();
			// 当たり判定
			if (IsCollision(player_->GetOBB(), enemy->GetObb())) {
				// 落下中ではない
				if (!player_->GetIsDrop()) {
					if (!enemy->GetFlag()) {
						enemy->SetPosition(player_->GetWorldPosition());
						/// プレイヤーに当たったか
						enemy->SetFlag(true);
						// プレイヤーにおもりを追加
						player_->SetWeight(1);
					}
					else {
						enemy->SetPosition(player_->GetWorldPosition());
					}
				}
				else {
					if (!enemy->GetFlag()) {
						enemy->SetPosition(player_->GetWorldPosition());
						enemy->SetFlag(true);
						player_->SetIsHitStop(true);
					}
					else {
						enemy->SetPosition(player_->GetWorldPosition());
					}
				}
			}
			enemyCount++;
		}
		// 敵生成
		enemyEditor_->Update(enemy_,enemyModel_);
		// 0を押すとカメラを切り替える
		if (input_->TriggerKey(DIK_0)) {
			IsDebugCamera_ ^= true;
		}
		if (IsDebugCamera_) {
			// デバックカメラ
			debugCamera_->Update(viewProjection_);
		}
		else {
			followCamera_->Update();
			*viewProjection_ = followCamera_->GetViewProjection();
		}
		// リセット
		if (input_->TriggerKey(DIK_R)) {
			for (Enemy* enemy : enemy_) {
				delete enemy;
			}
			enemy_.clear();
			// CSVからデータの読み込み
			std::unique_ptr<CSV> csv = std::make_unique<CSV>();
			csv->LoadCSV("Resources/CSV/Spaw.csv");
			std::vector<CSV::Data> datas = csv->UpdateDataCommands();
			// 読み込んだデータから生成
			for (CSV::Data data : datas) {
				SpawnEnemy(data.position, data.type);
			}
		}
}

void InGameScene::BackDraw()
{
}

void InGameScene::Draw()
{
	backGround_->Draw(*viewProjection_);
	frame_->Draw(*viewProjection_);
	player_->Draw(*viewProjection_);

	for (Enemy* enemy : enemy_) {
		enemy->Draw(*viewProjection_);
	}
}

void InGameScene::foreDraw()
{

}

void InGameScene::SpawnEnemy(const Vector3& position, uint32_t type)
{
	Enemy* enemy = new Enemy();

	enemy->Initialize(enemyModel_, position, type);

	enemy_.push_back(enemy);
}

void InGameScene::Release()
{
	
	delete viewProjection_;
	delete backGround_;
	delete followCamera_;
	delete frame_;
	delete player_;
	delete playerModel_;
	delete enemyEditor_;
	delete enemyModel_;
	for (Enemy* enemy : enemy_) {
		delete enemy;
	}
	enemy_.clear();

}


