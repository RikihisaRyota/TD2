#pragma once

#include <cmath>
#include <math.h>
#include <memory>
#include <optional>

#include "Audio.h"
#include "Collider.h"
#include "Model.h"
#include "OBB.h"
#include "PlayerBulletManager.h"
#include "PlayerJump.h"
#include "PlayerMove.h"
#include "PlayerPullingMove.h"
#include "PlayerString.h"
#include "PlayerStun.h"
#include "PlayerLanding.h"
#include "WorldTransform.h"
#include "ViewProjection.h"


/// <summary>
/// 自キャラ
/// </summary>
class Player : public Collider{
public:
	enum Parts {
		kBody,
		kLegLeft,
		kLegRight,

		kPartsCount,
	};
	enum Behavior {
		kMove,
		kPullingMove,
		kString,
		kJump,
		kLanding,
		kStun,
		kDoNothing,

		kCount
	};
public:
	~Player();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name= "model">モデル</param>
	/// <param name= "textureHandle">テクスチャハンドル</param>
	void Initialize(std::vector<Model*> models);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name= "viewProjection">ビュープロジェクション（参照渡し）</param>
	void Draw(const ViewProjection& viewProjection);

	void UpdateMatrix();
	void Reset();

	// 当たり判定
	void OnCollision(uint32_t type, Sphere* sphere)override;
	void HitBoxInitialize() override;
	void HitBoxUpdate() override;
	void HitBoxDraw(const ViewProjection& viewProjection) override;
#pragma region getter,setter
	bool GetIsGameOver() { return isGameOver_; }
	uint32_t GetPlayerHP() { return Hp_; }
	void SubtractionPlayerHP();
	void SetEnemyEatSoundHandle(size_t handle) { enemyEatSoundHandle_ = handle; }
	float GetSize() { return radius_; }
	Vector3 GetInitialPosition() { return kInitialPosition_; }
	uint32_t GetWeightNum() { return weightCount_; }
	void SetWeightNum(uint32_t num) { weightCount_ = num; }
	uint32_t GetWeightMax() { return kWeightMax_; }
	bool GetIsPulling() { return isPulling_; }
	void SetIsPulling(bool flg) { isPulling_= flg; }
	bool GetIsLanding() { return isLanding_; }
	void SetIsLanding(bool flg) { isLanding_= flg; }
	bool GetInvincible() { return isInvincible_; }
	void SetInvincible(bool flg) { isInvincible_ = flg; }
	void SetPlayerBulletManager(PlayerBulletManager* PlayerBulletManager) { playerBulletManager_ = PlayerBulletManager; }
	PlayerBulletManager* GetPlayerBulletManager() { return playerBulletManager_; }
	PlayerJump* GetPlayerJump() { return playerJump_.get(); }
	PlayerPullingMove* GetPlayerPullingMove() { return playerPullingMove_.get(); }
	PlayerMove* GetPlayerMove() { return playerMove_.get(); }
	PlayerString* GetPlayerString() { return playerString_.get(); }
	PlayerStun* GetPlayerStun() { return playerStun_.get(); }
	void SetScale(const Vector3& scale);
	const Vector3 GetScale() const { return worldTransform_.scale_; }
	void SetRotation(const Vector3& rotation);
	const Vector3 GetRotation() const { return worldTransform_.rotation_; }
	void SetTranslation(const Vector3& translation);
	const Vector3 GetTranslation() const { return worldTransform_.translation_; }
	void SetWorldTransform(const WorldTransform& worldTransform);
	WorldTransform& GetWorldTransform() { return worldTransform_; }

	void SetMotionScale(const Vector3& scale);
	const Vector3 GetMotionScale() const { return motion_.scale_; }
	void SetMotionRotation(const Vector3& rotation);
	const Vector3 GetMotionRotation() const { return motion_.rotation_; }
	void SetMotionTranslation(const Vector3& translation);
	const Vector3 GetMotionTranslation() const { return motion_.translation_; }
	void SetMotionWorldTransform(const WorldTransform& worldTransform);
	const WorldTransform& GetMotionWorldTransform() const { return motion_; }

	void SetPartsScale(const Vector3& scale,size_t num);
	const Vector3 GetPartsScale(size_t num) const { return parts_.at(num).scale_; }
	void SetPartsRotation(const Vector3& rotation, size_t num);
	const Vector3 GetPartsRotation(size_t num) const { return parts_.at(num).rotation_; }
	void SetPartsTranslation(const Vector3& translation, size_t num);
	const Vector3 GetPartsTranslation(size_t num) const { return parts_.at(num).translation_; }
	void SetPartsWorldTransform(const WorldTransform& worldTransform, size_t num);
	const WorldTransform& GetPartsWorldTransform(size_t num) const { return parts_.at(num); }

	void SetBehavior(const std::optional<Behavior>& behaviorRequest);
	Behavior GetBehavior() const { return behavior_; }
	void SetHeight(float height) { kHeight_ = height; }
	float GetHeight() { return kHeight_; }
	void SetWidth(float width) { kWidth_ = width; }
	float GetWidth() { return kWidth_; }
	void SetViewProjection(ViewProjection* viewProjection) { viewProjection_ = viewProjection;}
	bool GetIsHitStop() { return isHitStop_; }
	void SetIsHitStop(bool flg) { isHitStop_ = flg; }
#pragma endregion
private:
	void Debug();
	void BehaviorInitialize();
	void MoveLimit();
	void InvincibleUpdate();

	uint32_t kWeightMax_ = 10;
	Vector3 kInitialPosition_ = { 80.0f,-10.0f,0.0f };
	float kRadiusMax_ = 15.0f;
	float kRadiusMin_ = 5.0f;
	float radius_ = kRadiusMin_;
	PlayerBulletManager* playerBulletManager_;
	Input* input_;
	Audio* audio_;
	ViewProjection* viewProjection_;
	// ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransform motion_;
	std::vector<WorldTransform> parts_;
	OBB obb_; // 当たり判定用
	// モデル
	std::vector<Model*> models_;
	// ふるまい
	Behavior behavior_ = Behavior::kMove;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	// プレイヤーの動き
	std::unique_ptr<PlayerJump> playerJump_;
	std::unique_ptr<PlayerMove> playerMove_;
	std::unique_ptr<PlayerPullingMove> playerPullingMove_;
	std::unique_ptr<PlayerString> playerString_;
	std::unique_ptr<PlayerStun> playerStun_;
	std::unique_ptr<PlayerLanding> playerLanding_;
	// プレイヤーのHP
	uint32_t Hp_;
	// プレイヤーの行動範囲
	float kWidth_ = 100.0f;
	float kHeight_ = 50.0f;
	// 引っ張られている
	bool isPulling_;
	bool isLanding_;
	// プレイヤーについているおもりの数
	uint32_t weightCount_;
	// 無敵かどうか
	bool isInvincible_;
	uint32_t invincibleCount_;
	uint32_t kInvincibleMax_ = 60;
	// ヒットストップ
	bool isHitStop_;
	// 音
	size_t enemyEatSoundHandle_;
	// ゲームオーバー
	bool isGameOver_;
};