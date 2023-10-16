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
#include "CSV.h"
#include "Enemy.h"
#include "EnemyEditor.h"
#include "FollowCamera.h"
#include "Frame.h"
#include "Player.h"

// シーンクラス
#include "IScene.h"
#include "TitleScene.h"
#include "InGameScene.h"

class DirectXCommon;
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
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
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 解放
	/// </summary>
	void Release();

	// エネミーリスポーン用
	void SpawnEnemy(const Vector3& position, uint32_t type);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DebugCamera* debugCamera_ = nullptr;
	ViewProjection viewProjection_;

	bool IsDebugCamera_;
	/// <summary>
	/// ゲーム用
	/// </summary>
	std::unique_ptr<BackGround> backGround_;
	std::unique_ptr<FollowCamera> followCamera_;
	std::unique_ptr<Frame> frame_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> playerModel_;
	std::list<Enemy*> enemy_;
	std::unique_ptr<EnemyEditor> enemyEditor_;
	std::unique_ptr<Model> enemyModel_;

	std::stringstream enemyPopCommands_;

private:
	std::unique_ptr<IScene> sceneArr_[3];

	// どのステージを呼び出すかを管理する変数
	int sceneNo_;
	int prevSceneNo_;
};