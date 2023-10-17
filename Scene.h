#pragma once
class Scene {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 背景スプライト
	/// </summary>
	virtual void BackDraw() = 0;

	/// <summary>
	/// 3Dモデル
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 前景スプライト
	/// </summary>
	virtual void foreDraw() = 0;

	/// <summary>
	/// 前景スプライト
	/// </summary>
	virtual void Release() = 0;

	int GetSceneNo() { return sceneNo; }
	static int sceneNo;
};

