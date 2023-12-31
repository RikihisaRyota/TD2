#pragma once
#include <cstdlib>
#include <memory>

#include "Audio.h"
#include "CubeRenderer.h"
#include "DebugCamera.h"
#include "Input.h"
#include "PrimitiveDrawer.h"
#include "Model.h"
#include "OBJ.h"
#include "SafeDelete.h"
#include "PlaneRenderer.h"
#include "SphereRenderer.h"
#include "Sprite.h"

// ここから下になるべくアルファベット順で
#include "BackGround.h"
#include "Boss.h"
#include "CollisionManager.h"
#include "CSV.h"
#include "Enemy.h"
#include "EnemyEditor.h"
#include "EnemyManager.h"
#include "EnemyBulletManager.h"
#include "Fade.h"
#include "FollowCamera.h"
#include "Frame.h"
#include "GameUI.h"
#include "IScene.h"
#include "Player.h"
#include "PlayerBulletManager.h"
#include "Uvula.h"

class DirectXCommon;
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public IScene{
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ファイナライズ
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 解放
	/// </summary>
	void Release();

	// エネミーリスポーン用
	void SpawnEnemy(const Vector3& position, uint32_t type);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugCamera* debugCamera_ = nullptr;
	ViewProjection viewProjection_;

	bool IsDebugCamera_;

	/// <summary>
	/// ゲーム用
	/// </summary>
	std::unique_ptr<BackGround> backGround_;
	std::vector<uint32_t> backGroundTextureHandles_;
	std::unique_ptr<Boss> boss_;
	std::vector<Model*> bossModel_;
	std::unique_ptr<CollisionManager> collisionManager_;
	std::unique_ptr<FollowCamera> followCamera_;
	std::unique_ptr<Frame> frame_;
	std::vector<Model*>frameModel_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<PlayerBulletManager> playerBulletManager_;
	std::vector<Model*> playerModel_;
	std::unique_ptr<Model> playerBulletModel_;
	std::unique_ptr<EnemyEditor> enemyEditor_;
	std::unique_ptr<EnemyManager> enemyManager_;
	std::unique_ptr<EnemyBulletManager> enemyBulletManager_;
	std::unique_ptr<Model> enemyModel_;
	std::unique_ptr<Uvula> uvula_;
	std::unique_ptr<Model> uvulaHead_;
	std::unique_ptr<Model> uvulaBody_;
	std::unique_ptr<GameUI> gameUI_;

	std::vector<Model*> enemyModels_Type0_;
	std::vector<Model*> enemyModels_Type1_;
	std::vector<Model*> enemyModels_Type2_;

	std::stringstream enemyPopCommands_;

	size_t inGameSoundHandle_;

	// ヒットストップ用
	uint32_t hitStopCount_;

	std::unique_ptr<Fade> fade_;

	// フェード管理用
	bool isGameStart_ = false;
	bool isClear_ = false;
	bool isGameOver_ = false;
};