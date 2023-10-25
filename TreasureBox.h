#pragma once

#include <vector>

#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class TreasureBox {
public:
	enum class State {
		kCome,
		kOpen,

		kCount,
	};
	enum class Parts {
		kHuta,
		kUnder,
		
		kCount,
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

	Vector3 boxWorldTranslateStart_;
	Vector3 boxWorldTranslateEnd_;

	Vector3 upperRotateStart_;
	Vector3 upperRotateEnd_;

	float animationTime_;
	float animationMax_;
	
	State state_ = State::kCome;
};

