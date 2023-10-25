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
	void Initialize(const std::vector<Model*>& type0, const std::vector<Model*>& type1, const std::vector<Model*>& type2, const Vector3& max, const Vector3& min, uint32_t type);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	bool GetIsAlive() { return isAlive_; }
	bool GetIsDrawing() { return isDrawing_; }
	bool GetIsCreateFlag() { return EnemyCreateFlag; }
	uint32_t GetType() { return type_; }
	uint32_t GetShotTime() { return shotTime_; }
	uint32_t GetBulletShotCount() { return bulletShotCount_; }
	WorldTransform GetWorldTransform() { return worldTransform_; }
	float GetMaxSize() { return maxSize_; }
	float GetEaseSecond_Grow() { return easeSecond_Grow_; }
	float GetEaseSecond_Shot() { return easeSecond_Shot_; }
	float GetOnceUpSize() { return onceUpSize_; }
	float GetInitialRadius() { return initialRadius_; }
	float GetScaleUpValue_Shot() { return scaleUpValue_Shot_; }
	float GetRotateValue_Shot() { return rotateValue_Shot_; }
	Vector3 GetMax() { return splitPos_Max_; }
	Vector3 GetMin() { return splitPos_Min_; }


	void SetPlayer(Player* player) { player_ = player; }
	void SetViewProjection(ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
	void SetEnemyBulletManager(EnemyBulletManager* enemyBulletManager) { enemyBulletManager_ = enemyBulletManager; }
	void SetTranslation(const Vector3& translation) { worldTransform_.translation_ = translation; }
	void SetRotate(const Vector3& rotate) { worldTransform_.rotation_ = rotate; }
	void SetType(uint32_t type) { type_ = type; }
	void SetMatWorld();
	void SetWidth(float width) { width_ = width; }
	void SetHeight(float height) { height_ = height; }
	
	void SetShotTime(uint32_t time) { shotTime_ = time; }
	void SetMaxSize(float maxSize) { maxSize_ = maxSize; }
	void SetEaseSecond_Grow(float easeSecond_Grow) { easeSecond_Grow_ = easeSecond_Grow; }
	void SetEaseSecond_Shot(float easeSecond_Shot) { easeSecond_Shot_ = easeSecond_Shot; }
	void SetOnceUpSize(float onceUpSize) { onceUpSize_ = onceUpSize; }
	void SetInitialRadius(float InitialRadius) {initialRadius_ = InitialRadius;}
	void SetBulletShotCount(uint32_t bulletShotCount) { bulletShotCount_ = bulletShotCount; }
	void SetScaleUpValue_Shot(float scaleUpValue_Shot) { scaleUpValue_Shot_ = scaleUpValue_Shot; }
	void SetRotateValue_Shot(float rotateValue_Shot) { rotateValue_Shot_ = rotateValue_Shot; }

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
	ViewProjection* viewProjection_;
	Model* model_ = nullptr;
	Player* player_;
	uint32_t type_;
	float initialRadius_ = 7.0f;
	float radius_;
	bool isAlive_ = true;
	bool isDrawing_ = true;

	float width_;
	float height_;

	// 演出用
	std::vector<Model*> models_type0_;
	std::vector<Model*> models_type1_;
	std::vector<Model*> models_type2_;
	WorldTransform worldTransform_type0_Head_;
	WorldTransform worldTransform_type0_Leg_;
	WorldTransform worldTransform_type1_Body_;
	WorldTransform worldTransform_type1_Prick_;
	WorldTransform worldTrasnform_type2_;
	

	// 振る舞い
	Behavior behavior_ = Behavior::kStandby;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// イージング
	float easeTime_ = 0;
	float easeMin_[2] = {0};
	float easeMax_[2] = {0};

	// 時間管理用の変数
	int shotTime_ = 60;
	const uint32_t DamageTime_ = 60;
	std::vector<uint32_t> times_;

	// 弾発生用の変数
	EnemyBulletManager* enemyBulletManager_;
	float easeSecond_Shot_ = 0.05f;
	uint32_t bulletShotCount_ = 20;
	float scaleUpValue_Shot_ = 0.01f;
	float rotateValue_Shot_ = 1.0f;
	
	// 分裂用の変数
	bool EnemyCreateFlag = false;
	bool splitFlag_ = false;
	Vector3 splitPos_Max_;
	Vector3 splitPos_Min_;
	float easeSecond_Split_ = 0.1f;
	Vector3 easeMin_Vector3_;
	Vector3 easeMax_Vector3_;
	float distance_Split_;

	// 成長用の変数
	float maxSize_;
	float onceUpSize_ = 1.0f;
	float easeSecond_Grow_ = 0.1f;

	// 仮に動かすため
	Input* input_;
};