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
#include "CSV.h"
#include "Enemy.h"
#include "FollowCamera.h"
#include "Player.h"

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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DebugCamera* debugCamera_ = nullptr;
	ViewProjection viewProjection_;

	bool IsDebugCamera_;
	/// <summary>
	/// ゲーム用
	/// </summary>
	std::unique_ptr<CSV> csv_;
	std::unique_ptr<CubeRenderer> floor_;
	WorldTransform floorWorldTransform_;
	std::unique_ptr<FollowCamera> followCamera_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> playerModel_;
	std::unique_ptr<Enemy> enemy_;
	std::unique_ptr<Model> enemyModel_;
};