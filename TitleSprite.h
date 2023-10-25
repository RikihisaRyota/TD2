#pragma once

#include <vector>

#include "Sprite.h"
#include "WorldTransform.h"
#include "ViewProjection.h"


class TitleSprite {
public:

	enum Type {
		kTitle,
		kBoard,

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
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name= "viewProjection">ビュープロジェクション（参照渡し）</param>
	void Draw(const ViewProjection& viewProjection);
private:
	std::vector<Sprite*> sprite_;
	std::vector<Vector2> pos_;
};

