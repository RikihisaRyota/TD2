#pragma once
#include "IScene.h"

#include "Audio.h"
#include "CubeRenderer.h"
#include "DebugCamera.h"
#include "Input.h"
#include "PrimitiveDrawer.h"
#include "Model.h"
#include "OBJ.h"
#include "SafeDelete.h"
#include "PlaneRenderer.h"
#include "SphereRenderer.h"
#include "Sprite.h"

class TitleScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void ForeDraw() override;
	void Draw() override;
	void BackDraw() override;
	void Release() override;
private:

};

