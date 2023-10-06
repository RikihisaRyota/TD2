#pragma once

#include "Model.h"
#include "Input.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
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

	void Move();

	Vector3 GetWorldPosition();
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
private:
	void Debug();
	float kSpeed_ = 5.0f;
	float kGravity_ = 0.2f;
	float kDropSpeed_ = 10.0f;
	// ワールド変換データ
	WorldTransform worldTransform_;
	const ViewProjection* viewProjection_ = nullptr;
	Input* input_ = nullptr;
	// モデル
	Model* model_ = nullptr;
};