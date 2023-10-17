#pragma once
#include <optional>

#include "Collider.h"
#include "Model.h"
#include "WorldTransform.h"
#include "EnemyBulletManager.h"

#include "Input.h"

class Enemy :public Collider {
public:
	enum class EnemyType {
		kOctopus,
		kSpike
	};

	enum Behavior {
		kStandby, // 待機状態
		kShot,    // 発射
		kSplit,   // 分裂
		kDamage,  // ダメージ
		kCling,   // くっつく
		kGrow,    // 大きくなる
		kCount    // 最大数
	};
public:
	void Initialize(Model* model, const Vector3& position, uint32_t type);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void SetEnemyBulletManager(EnemyBulletManager* enemyBulletManager) { enemyBulletManager_ = enemyBulletManager; }
private:
	// 当たり判定
	// 衝突したら呼び出される処理
	void OnCollision(uint32_t type, Sphere* sphere)override;
	void HitBoxInitialize() override;
	void HitBoxUpdate() override;
	void HitBoxDraw(const ViewProjection& viewProjection) override;

	// 振る舞いリクエストチェック
	void BehaviorRequestCheck();
	// 振る舞いごとの初期化
	void StandbyInitialize();
	void ShotInitialize();
	void SplitInitialize();
	void DamageInitialize();
	void ClingInitialize();
	void GrowInitialize();
	// 振る舞いごとの更新
	void StandbyUpdate();
	void ShotUpdate();
	void SplitUpdate();
	void DamageUpdate();
	void ClingUpdate();
	void GrowUpdate();

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t type_;
	float radius_ = 1.0f;

	// 振る舞い
	Behavior behavior_ = Behavior::kStandby;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// 弾を発射を管理するための変数
	EnemyBulletManager* enemyBulletManager_;

	// 時間管理用の変数
	const uint32_t ShotTime_ = 60;
	const uint32_t DamageTime_ = 60;
	std::vector<uint32_t> times_;

	// 仮に動かすため
	Input* input_;
};