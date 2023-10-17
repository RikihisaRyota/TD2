#pragma once
#include "IScene.h"
#include "Input.h"

class TitleScene :public IScene
{
public:
	TitleScene();
	~TitleScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 背景スプライト
	/// </summary>
	void BackDraw() override;

	/// <summary>
	/// 3Dモデル
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 前景スプライト
	/// </summary>
	void foreDraw() override;

	/// <summary>
	/// 前景スプライト
	/// </summary>
	void Release() override;

private:
	Input* input_;

};

