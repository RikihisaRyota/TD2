#pragma once

// �V�[�����ł̏������s�����N���X
enum SCENE{TITLE, GAME_STAGE, CLEAR};

class IScene
{
public:
	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �w�i�X�v���C�g
	/// </summary>
	virtual void BackDraw() = 0;

	/// <summary>
	/// 3D���f��
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// �O�i�X�v���C�g
	/// </summary>
	virtual void foreDraw() = 0;
	

	int GetSceneNo() { return sceneNo; }
protected:
	static int sceneNo;

};

