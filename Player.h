#pragma once

#include "Model.h"
#include "Input.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "OBB.h"
#include <cmath>
#include <math.h>

#include <memory>

/// <summary>
/// 自キャラ
/// </summary>
class Player {
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

	void OBJtoOBB(); // WorldTransformをOBBへ変換

	void Move(); // プレイヤーの移動処理

	Vector3 GetWorldPosition();
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

	OBB GetOBB() { return obb_; } // OBBの取得用
	bool GetIsDrop() { return isDrop_; }
	void SetWeight(uint32_t count) { weightCount_ += count; }
	void SetIsHitStop(bool flag) { isHitStop_ = flag; }

	void SetWidth(float width) { kWidth_ = width; }
	void SetHeight(float height) { kHeight_ = height; }
private:
	void Debug();
	float kDropMaxSpeed_ = 0.05f;
	float kDropSpeed_ = 0.01f;
	float kDropHorizontalSpeed_ = 0.4f;
	float kGravity_ = 0.1f;
	float kInertia_ = 0.9f;
	float kSpeed_ = 0.5f;
	float kRightAngle_= 45.0f;
	float kLeftAngle_= 135.0f;
	float kWidth_ = 40.0f;
	float kHeight_ = 300.0f;
	uint32_t kLifeTimeMax_ = 360;
	uint32_t kWeightMax_ = 5;

	// ワールド変換データ
	WorldTransform worldTransform_;
	const ViewProjection* viewProjection_ = nullptr;
	Input* input_ = nullptr;
	
	// モデル
	Model* model_ = nullptr;
	Vector3 velocity_;
	Vector3 acceleration_;
  
	OBB obb_; // 当たり判定用
	// 何体敵が引っ付いているかカウント用
	uint32_t weightCount_;
	// 酸素ゲージ
	uint32_t lifeTimeCount_;
	// 落下中か
	bool isDrop_;
	// ヒットストップ
	bool isHitStop_;
	uint32_t hitStopCount_;
	uint32_t hitStopMax_ = 5;

	float angle_ = 90.0f;
	float addAngle_ = 5.0f;
	float kDashSpeed_ = 0.5f;
	float kJumpSpeed_ = 0.6f;
};