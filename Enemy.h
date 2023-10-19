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
		kSpike
	};

	enum OctopusParts {
		kHead,
		kLeg
	};
	enum SpikeParts {
		kBoll,
		ks
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
	void Initialize(const std::vector<Model*>& type0,const std::vector<Model*>&type1, const Vector3& position, uint32_t type);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	bool GetIsAlive() { return isAlive_; }
	bool GetIsDrawing() { return isDrawing_; }
	uint32_t GetType() { return type_; }
	WorldTransform GetWorldTransform() { return worldTransform_; }
	void SetPlayer(Player* player) { player_ = player; }
	void SetViewProjection(ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
	void SetEnemyBulletManager(EnemyBulletManager* enemyBulletManager) { enemyBulletManager_ = enemyBulletManager; }
	void SetTranslation(const Vector3& translation) { worldTransform_.translation_ = translation; }
	void SetRotate(const Vector3& rotate) { worldTransform_.rotation_ = rotate; }
	void SetType(uint32_t type) { type_ = type; }
	void SetMatWorld() { worldTransform_.UpdateMatrix(); }
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
	float radius_ = 1.0f;
	bool isAlive_ = true;
	bool isDrawing_ = true;

	// 演出用
	std::vector<Model*> models_type0_;
	std::vector<Model*> models_type1_;
	WorldTransform worldTransform_type0_[2];
	WorldTransform worldTransform_type1_[2];
	

	// 振る舞い
	Behavior behavior_ = Behavior::kStandby;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;



	// 時間管理用の変数
	const uint32_t ShotTime_ = 180;
	const uint32_t DamageTime_ = 60;
	std::vector<uint32_t> times_;

	// 弾発生用の変数
	EnemyBulletManager* enemyBulletManager_;

	// 分裂用の変数
	bool EnemyCreateFlag = false;
	Vector3 splitPos_;

	// 仮に動かすため
	Input* input_;
};