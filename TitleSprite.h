#pragma once

#include <vector>

#include "Sprite.h"
#include "WorldTransform.h"
#include "ViewProjection.h"


class TitleBoss;
class TitleSprite {
public:

	enum Type {
		kTitle,
		kArrow0,
		kArrow1,
		kArrow2,
		kMove,

		kCount,
	};
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name= "model">モデル</param>
	/// <param name= "textureHandle">テクスチャハンドル</param>
	void Initialize(std::vector<Sprite*> sprite);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const ViewProjection& view);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name= "viewProjection">ビュープロジェクション（参照渡し）</param>
	void Draw();

	void SetTitleBoss(TitleBoss* titleBoss) { titleBoss_ = titleBoss; }
private:
	TitleBoss* titleBoss_;
	std::vector<Sprite*> sprite_;
	Vector2 titlePos_;
	Vector2 arrowPos_;
	Vector2 arrowSize_;
	Vector2 movePos_;
	Vector2 moveSize_;
	Vector2 moveBaseSize_;
	float animationTime_;
	float animationCount_;
	float animationMax_;
	float interval_;
	float titleAngle_;
	float arrowAngle_;
	float moveAngle_;
};

