#pragma once

#include "Collider.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Random.h"

class Player;
class Boss : public Collider {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name= "model">モデル</param>
	/// <param name= "textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name= "viewProjection">ビュープロジェクション（参照渡し）</param>
	void Draw(const ViewProjection& viewProjection);

	void SetPlayer(Player* player) { player_ = player; }
	
	void Reset();

	void Debug();
	// 当たり判定
	void OnCollision(uint32_t type, Sphere* sphere)override;
	void HitBoxInitialize() override;
	void HitBoxUpdate() override;
	void HitBoxDraw(const ViewProjection& viewProjection) override;
private:
	uint32_t kSakeMax_ = 30;

	// プレイヤー
	Player* player_;
	// モデル
	Model* model_;
	// ワールド変換データ
	WorldTransform worldTransform_;
	// 半径
	float radius_ = 3.0f;
	// ランダム
	Random::RandomNumberGenerator rnd;
	
	uint32_t shakeCount_;
};

