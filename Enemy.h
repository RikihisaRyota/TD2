#pragma once
#include <optional>

#include "Collider.h"
#include "Model.h"
#include "Player.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "EnemyBulletManager.h"
#include "Input.h"

class EnemyManager;
class Enemy :public Collider {
public:
	enum class EnemyType {
		kOctopus,
		kSpike,
		kfeed
	};

	enum OctopusParts {
		kHead,
		kLeg
	};
	enum SpikeParts {
		kBody,
		kPrick
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
	void Initialize(const std::vector<Model*>& type0,const std::vector<Model*>&type1,const std::vector<Model*> &type2, const Vector3& position, uint32_t type);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	bool GetIsAlive() { return isAlive_; }
	bool GetIsDrawing() { return isDrawing_; }
	bool GetIsCreateFlag() { return EnemyCreateFlag; }
	uint32_t GetType() { return type_; }
	uint32_t GetShotTime() { return shotTime_; }
	WorldTransform GetWorldTransform() { return worldTransform_; }
	Vector3 GetSplitPos() { return splitPos_; }
	float GetMaxSize() { return maxSize_; }
	float GetEaseSecond_Grow() { return easeSecond_Grow_; }
	float GetEaseSecond_Shot() { return easeSecond_Shot_; }
	float GetOnceUpSize() { return onceUpSize_; }
	float GetInitialRadius() { return initialRadius_; }
	void SetPlayer(Player* player) { player_ = player; }
	void SetViewProjection(ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
	void SetEnemyBulletManager(EnemyBulletManager* enemyBulletManager) { enemyBulletManager_ = enemyBulletManager; }
	void SetTranslation(const Vector3& translation) { worldTransform_.translation_ = translation; }
	void SetRotate(const Vector3& rotate) { worldTransform_.rotation_ = rotate; }
	void SetType(uint32_t type) { type_ = type; }
	void SetMatWorld();
	void SetShotTime(uint32_t time) { shotTime_ = time; }
	void SetMaxSize(float maxSize) { maxSize_ = maxSize; }
	void SetEaseSecond_Grow(float easeSecond_Grow) { easeSecond_Grow_ = easeSecond_Grow; }
	void SetEaseSecond_Shot(float easeSecond_Shot) { easeSecond_Shot_ = easeSecond_Shot; }
	void SetOnceUpSize(float onceUpSize) { onceUpSize_ = onceUpSize; }
	void SetInitialRadius(float InitialRadius) {initialRadius_ = InitialRadius;}
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

	void Debug();

	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	Model* model_ = nullptr;
	Player* player_;
	uint32_t type_;
	float initialRadius_ = 2.0f;
	float radius_ = 2.0;
	bool isAlive_ = true;
	bool isDrawing_ = true;

	// 演出用
	std::vector<Model*> models_type0_;
	std::vector<Model*> models_type1_;
	std::vector<Model*> models_type2_;
	WorldTransform worldTransform_type0_[2];
	WorldTransform worldTransform_type1_[2];
	WorldTransform worldTrasnform_type2_;
	

	// 振る舞い
	Behavior behavior_ = Behavior::kStandby;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// イージング
	float easeTime_ = 0;
	float easeMin_ = 0;
	float easeMax_ = 0;

	// 時間管理用の変数
	int shotTime_ = 180;
	const uint32_t DamageTime_ = 60;
	std::vector<uint32_t> times_;

	// 弾発生用の変数
	EnemyBulletManager* enemyBulletManager_;
	float easeSecond_Shot_ = 0.01f;

	// 分裂用の変数
	bool EnemyCreateFlag = false;
	Vector3 splitPos_;

	// 成長用の変数
	float maxSize_ = 5.0f;
	float onceUpSize_ = 1.0f;
	float easeSecond_Grow_ = 0.01f;

	// 仮に動かすため
	Input* input_;
};