#pragma once

#include <list>

#include "Sprite.h"
#include "ViewProjection.h"

class Bubble
{
public:
	struct BubleStatus {
		Vector2 position;
		Vector2 velocity;
		Vector4 color = {1,1,1,1};
		Vector2 anchorpoint = {0.5f, 0.5f};
		Vector2 size;
		bool isDead = false;
		Sprite* sprite;
	};

public:
	static Bubble* GetInstance();

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Create(const Vector3& position, const Vector2& velocity, const ViewProjection& viewProjection);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="ViewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

private:
	/// <summary>
	/// 
	/// </summary>
	void IsDeat();
private:
	Bubble() = default;
	~Bubble() = default;
	Bubble(const Bubble&) = delete;
	const Bubble& operator=(const Bubble&) = delete;

private:
	std::list<BubleStatus*> bubbles_;
	uint32_t textureHandle_;
};

