#pragma once

// シーン内での処理を行う基底クラス
enum SCENE{TITLE, GAME_STAGE, CLEAR};

class IScene
{
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
	

	int GetSceneNo() { return sceneNo; }
protected:
	static int sceneNo;

};

