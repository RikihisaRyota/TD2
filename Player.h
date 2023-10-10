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

	void OBJtoOBB();
	void Move();

	Vector3 GetWorldPosition();
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

	OBB GetOBB() { return obb_; }

private:
	void Debug();
	float kDropMaxSpeed_ = 0.05f;
	float kDropSpeed_ = 0.01f;
	float kDropHorizontalSpeed_ = 0.4f;
	float kGravity_ = 0.001f;
	float kInertia_ = 0.9f;
	float kSpeed_ = 0.01f;
	float kRightAngle_= 45.0f;
	float kLeftAngle_= 135.0f;
	// ワールド変換データ
	WorldTransform worldTransform_;
	const ViewProjection* viewProjection_ = nullptr;
	Input* input_ = nullptr;
	// モデル
	Model* model_ = nullptr;

	Vector3 velocity_;
	Vector3 acceleration_;

	OBB obb_;
};