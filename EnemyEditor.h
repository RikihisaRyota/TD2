#pragma once

#include <list>

#include "Model.h"

class Enemy;
class EnemyEditor {
public:
	void Update(std::list<Enemy*>& enemies,Model* model);
};

