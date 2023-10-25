#pragma once
#include <vector>

#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Random.h"

class TitlePlayer;
class TitleBoss {
public:
	enum class Parts {
		kSharkHead,
		kSharkJaw,
		kSharkBody,

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

	void SetTitlePlayer(TitlePlayer* titlePlayer) {titlePlayer_ = titlePlayer;}
	Matrix4x4 GetWorldMat() { return  worldTransform_.matWorld_; }
	bool GetSceneChangeFlag() {	return sceneChange_; }
private:
	TitlePlayer* titlePlayer_;
	std::vector<Model*> models_;
	// ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransform motion_;
	std::vector<WorldTransform> parts_;
	float animationTime_;
	float animationMax_ = 90.0f;

	float limitMax_ = 300.0f;
	bool sceneChange_;
	Random::RandomNumberGenerator rnd;
};

