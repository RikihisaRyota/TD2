#pragma once

#include "Collider.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Random.h"

class Player;
class Boss : public Collider {
public:
	enum Type {
		kFirstBoss,
		kMiddleBoss,
		kLastBoss,

		kCount,
	};
	enum class Parts {
		kShellfishUp,
		kShellfishDown,
		kOnJaw,
		kLowerJaw,
		kNeck,
		kSharkHead,
		kSharkJaw,
		kSharkBody,

		kCount,
	};
public:
	Boss();
	~Boss();
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

	void SetPlayer(Player* player) { player_ = player; }
	
	void Reset();

	void Debug();

	bool GetIsClear() { return isClear_; }
	// 当たり判定
	void OnCollision(uint32_t type, Sphere* sphere)override;
	void HitBoxInitialize() override;
	void HitBoxUpdate() override;
	void HitBoxDraw(const ViewProjection& viewProjection) override;

	uint32_t GetBossHP() {return HP_;}
	Type GetBossType() { return bossType_; }
private:
	void UpdateMatrix();
	void DeathAnimation();
	void AttackAnimation();
	uint32_t kFirstBossHP_ = 10;
	uint32_t kMiddleBossHP_ = 15;
	uint32_t kLastBossHP_ = 20;
	uint32_t HP_;
	uint32_t kAnimationMax_ = 120;

	uint32_t animationCount_;

	// プレイヤー
	Player* player_;
	// モデル
	std::vector<Model*> models_;
	// ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransform motion_;
	std::vector<WorldTransform> parts_;
	// 半径
	float radius_ = 3.0f;
	// ランダム
	Random::RandomNumberGenerator rnd;
	
	uint32_t shakeCount_;

	Type bossType_ = Type::kFirstBoss;
	// ボスのリスポーン
	bool isRespawn_;
	// アニメーションが終わったかどうか
	bool isAnimation_;
	// クリア
	bool isClear_;
};

