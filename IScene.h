#pragma once
#include "Input.h"

#include "ViewProjection.h"
// 状態
enum SCENE {TITLE, PLAY, CLEAR};

class IScene
{
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void ForeDraw() = 0;
	virtual void Draw() = 0;
	virtual void BackDraw() = 0;
	virtual void Release() = 0;
	virtual void SetViewProjection(ViewProjection& viewProjection) = 0;
	int GetSceneNo() { return sceneNo_; }

protected:
	static int sceneNo_;
	//Input* input_;
};

