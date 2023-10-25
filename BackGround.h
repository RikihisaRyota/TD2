#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class Player;
class BackGround {
public:
	enum State {
		kTitle,
		kInGame,
		kGameClear,
		kGameOver,
	};
	enum Type {
		kBack,
		kMiddle,
		kFront,

		kCount,
	};
public:
	~BackGround();
	void Initialize(std::vector<uint32_t> textureHandle, State state);
	void Update();
	void Draw();
	void SetPlayer(Player* player) { player_=player; }
private:
	Player* player_;
	std::vector<Vector2> position_;
	std::vector<Sprite*> backSprite_;
	std::vector<Sprite*> middleSprite_;
	std::vector<Sprite*> frontSprite_;
	uint32_t textureHandle_;

	Vector3 titlePosition_;

	State state_;
};

