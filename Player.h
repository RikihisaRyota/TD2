#pragma once

#include <cmath>
#include <math.h>
#include <memory>
#include <optional>

#include "Collider.h"
#include "Model.h"
#include "OBB.h"
#include "PlayerBulletManager.h"
#include "PlayerJump.h"
#include "PlayerMove.h"
#include "PlayerPullingMove.h"
#include "PlayerString.h"
#include "WorldTransform.h"
#include "ViewProjection.h"


/// <summary>
/// 自キャラ
/// </summary>
class Player : public Collider{
public:
	enum Behavior {
		kMove,
		kPullingMove,
		kString,
		kJump,
		kLanding,

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

	void Reset();

	void Debug();

	void OBJtoOBB(); // WorldTransformをOBBへ変換
	// 当たり判定
	void OnCollision(uint32_t type, Sphere* sphere)override;
	void HitBoxInitialize() override;
	void HitBoxUpdate() override;
	void HitBoxDraw(const ViewProjection& viewProjection) override;

	uint32_t GetWeightNum() { return weightCount_; }
	uint32_t GetWeightMax() { return kWeightMax_; }
	bool GetIsPulling() { return isPulling_; }
	bool GetIsLanding() { return isLanding_; }
	void SetPlayerBulletManager(PlayerBulletManager* PlayerBulletManager) { playerBulletManager_ = PlayerBulletManager;	}
	PlayerBulletManager* GetPlayerBulletManager() { return playerBulletManager_; }
	PlayerJump* GetPlayerJump() { return playerJump_.get(); }
	PlayerPullingMove* GetPlayerMove() { return playerPullingMove.get(); }
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
	void SetHeight(float height) { kHeight_ = height; }
	float GetHeight() { return kHeight_; }
	void SetWidth(float width) { kWidth_ = width; }
	float GetWidth() {return kWidth_; }
private:
	void BehaviorInitialize();
	void MoveLimit();

	uint32_t kWeightMax_ = 20;

	float radius_ = 1.0f;
	PlayerBulletManager* playerBulletManager_;
	Input* input_;
	// ワールド変換データ
	WorldTransform worldTransform_;
	OBB obb_; // 当たり判定用
	// モデル
	Model* model_ = nullptr;
	// ふるまい
	Behavior behavior_ = Behavior::kMove;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	// プレイヤーの動き
	std::unique_ptr<PlayerJump> playerJump_;
	std::unique_ptr<PlayerMove> playerMove_;
	std::unique_ptr<PlayerPullingMove> playerPullingMove;
	std::unique_ptr<PlayerString> playerString_;
	// プレイヤーの行動範囲
	float kWidth_ = 100.0f;
	float kHeight_ = 50.0f;
	// 引っ張られている
	bool isPulling_;
	bool isLanding_;
	// プレイヤーについているおもりの数
	uint32_t weightCount_;
};