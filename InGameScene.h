#pragma once
#include "IScene.h"
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

class InGameScene :public IScene
{
public:
	InGameScene();
	~InGameScene();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 背景スプライト
	/// </summary>
	void BackDraw() override;

	/// <summary>
	/// 3Dモデル
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 前景スプライト
	/// </summary>
	void foreDraw() override;

	// エネミーリスポーン用
	void SpawnEnemy(const Vector3& position, uint32_t type);

	void Release() override;
private:
	Input* input_ = nullptr;
	DebugCamera* debugCamera_ = nullptr;
	ViewProjection* viewProjection_;

	bool IsDebugCamera_;
	/// <summary>
	/// ゲーム用
	/// </summary>
	/*std::unique_ptr<BackGround> backGround_;
	std::unique_ptr<FollowCamera> followCamera_;
	std::unique_ptr<Frame> frame_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> playerModel_;
	std::list<Enemy*> enemy_;
	std::unique_ptr<EnemyEditor> enemyEditor_;
	std::unique_ptr<Model> enemyModel_;*/
	BackGround* backGround_;
	FollowCamera* followCamera_;
	Frame* frame_;
	Player* player_;
	Model* playerModel_;
	std::list<Enemy*> enemy_;
	EnemyEditor* enemyEditor_;
	Model* enemyModel_;

	std::stringstream enemyPopCommands_;
};

