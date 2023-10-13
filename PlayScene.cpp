#include "PlayScene.h"

#include "CSV.h"
#include "DirectXCommon.h"
#include "MyMath.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Draw.h"

void PlayScene::Initialize()
{
	// デバックカメラ
	debugCamera_ = new DebugCamera();
	IsDebugCamera_ = false;
	// 入力
	input_ = Input::GetInstance();
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
	//LoadCSVData("Resources/CSV/Spaw.csv", &enemyPopCommands_);
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
	//UpdateEnemyPopCommands();

}

void PlayScene::Update()
{
	
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
	enemyEditor_->Update(enemy_, enemyModel_.get());
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
		//LoadCSVData("Resources/CSV/Spaw.csv", &enemyPopCommands_);
		//UpdateEnemyPopCommands();
	}
}

void PlayScene::ForeDraw()
{
}

void PlayScene::Draw()
{
	backGround_->Draw(*viewProjection_);
	frame_->Draw(*viewProjection_);
	player_->Draw(*viewProjection_);

	for (Enemy* enemy : enemy_) {
		enemy->Draw(*viewProjection_);
	}
}

void PlayScene::BackDraw()
{
}

void PlayScene::Release()
{
	SafeDelete(debugCamera_);
	for (Enemy* enemy : enemy_) {
		delete enemy;
	}
	enemy_.clear();
}

void PlayScene::SetViewProjection(ViewProjection& viewProjection)
{
	viewProjection_ = &viewProjection;
}

