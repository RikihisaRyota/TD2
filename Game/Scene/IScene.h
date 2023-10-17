#pragma once

enum Scene {
	TITLE_SCENE,
	GAME_SCENE,
	CLEAR_SCENE,
	OVER_SCENE,
};

class IScene {
public:
	virtual ~IScene() {};

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void Finalize() = 0;

	int GetSceneNumber() { return sceneNumber_; }

protected:
	static int sceneNumber_;
};