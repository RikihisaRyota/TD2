#pragma once
#include "IScene.h"
class InGameScene :public IScene
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �w�i�X�v���C�g
	/// </summary>
	void BackDraw() override;

	/// <summary>
	/// 3D���f��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �O�i�X�v���C�g
	/// </summary>
	void foreDraw() override;
private:

};

