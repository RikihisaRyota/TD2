#pragma once

#include <vector>

#include "Model.h"

class EnemyManager;
class CSV;
class EnemyEditor {
public:
	void Update(EnemyManager* enemyManager);
private:
	bool spawn0_ = true;
	bool spawn1_ = false;
	bool spawn2_ = false;
};

