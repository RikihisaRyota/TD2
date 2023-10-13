#pragma once

#include <cmath>
#include <math.h>
#include <memory>
#include <optional>

#include "Model.h"
#include "OBB.h"
#include "PlayerJump.h"
#include "PlayerMove.h"
#include "PlayerString.h"
#include "WorldTransform.h"
#include "ViewProjection.h"


/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	enum Behavior {
		kMove,
		kString,
		kJump,

		kCount
	};
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

	void Debug();

	void OBJtoOBB(); // WorldTransformをOBBへ変換

	PlayerJump* GetPlayerJump() { return playerJump_.get(); }
	PlayerMove* GetPlayerMove() { return playerMove_.get(); }
	PlayerString* GetPlayerString() { return playerString_.get(); }
	void SetScale(const Vector3& scale);
	const Vector3 GetScale() const { return worldTransform_.scale_; }
	void SetRotation(const Vector3& rotation);
	const Vector3 GetRotation() const { return worldTransform_.rotation_; }
	void SetTranslation(const Vector3& translation);
	const Vector3 GetTranslation() const { return worldTransform_.translation_; }
	void SetWorldTransform(const WorldTransform& worldTransform);
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	void SetBehavior(const std::optional<Behavior>& behaviorRequest);
	Behavior GetBehavior() const { return behavior_; }
private:
	void BehaviorInitialize();
	// ワールド変換データ
	WorldTransform worldTransform_;
	OBB obb_; // 当たり判定用
	// モデル
	Model* model_ = nullptr;
	// ふるまい
	Behavior behavior_ = Behavior::kMove;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	std::unique_ptr<PlayerJump> playerJump_;
	std::unique_ptr<PlayerMove> playerMove_;
	std::unique_ptr<PlayerString> playerString_;
};