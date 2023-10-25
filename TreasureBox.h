#pragma once

#include <vector>

#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class TreasureBox {
public:
	enum class Parts {
		kLid,
	};
public:
	void Initialize(std::vector<Model*> models);
	void Update();
	void Draw(const ViewProjection& viewProjection);
private:
	std::vector<Model*> models_;
	WorldTransform worldTransfrom_;
	WorldTransform motion_;
	std::vector<WorldTransform> parts_;
};

