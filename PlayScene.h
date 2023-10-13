#pragma once
#include "IScene.h"

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

class PlayScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void ForeDraw() override;
	void Draw() override;
	void BackDraw() override;
	void Release() override;
	void SetViewProjection(ViewProjection& viewProjection) override;
private:
	Input* input_ = nullptr;
	DebugCamera* debugCamera_ = nullptr;
	ViewProjection* viewProjection_;

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

};

