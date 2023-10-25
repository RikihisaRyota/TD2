#pragma once
#include <vector>

#include "Audio.h"
#include "Model.h"
#include "Input.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class TitlePlayer {
public:
	enum Parts {
		kBody,
		kLegLeft,
		kLegRight,

		kCount,
	};
public:
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

	void SetSoundHandle(size_t handle) { moveSoundHandle_ = handle; }
	bool GetSceneFlag() { return isSceneChange_; }
private:
	void MoveLimit();
	Vector3 kInitialPosition_ = { 100.0f,-10.0f,0.0f };
	float kRadius_ = 5.0f;
	float kPower_ = 0.3f;
	float kInertia_ = 0.98f;
	float kAngle_ = 60.0f;
	float LimitMin_ = 90.0f;
	float LimitMax_ = 230.0f;
	float kGravity = 0.003f;
	float kLegAngle_ = 35.0f;

	Input* input_;
	Audio* audio_;
	std::vector<Model*> models_;
	// ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransform motion_;
	std::vector<WorldTransform> parts_;

	// サウンドハンドル
	size_t moveSoundHandle_;

	Vector3 acceleration_;
	Vector3 velocity_;
	bool direction_;
	// プレイヤーの体の向き
	float playerCurrentRotate_;
	float playerNextRotate_;
	// プレイヤーの脚の向き
	bool isSwell_;
	uint32_t swellCount_;
	float playerCurrentLegLeftRotate_;
	float playerCurrentLegRightRotate_;

	bool isSceneChange_;
};

