#pragma once

#include "WorldTransform.h"
#include "ViewProjection.h"

class Input;
class Player;
class PlayerLanding {
public:
	PlayerLanding();
	~PlayerLanding();
	void Initialize();
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void SetPlayer(Player* player) { player_ = player; }
	void Debug();
private:
	Player* player_;
	Input* input_;
};

