#pragma once

#include <cstdint>
#include <memory>

#include "PlaneRenderer.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class BackGround {
public:
	void Initialize(uint32_t textureHandle);
	void Draw(const ViewProjection& viewProjection);
private:
	std::unique_ptr<PlaneRenderer> plane_;
	WorldTransform worldTransform_;
	uint32_t textureHandle_;
};

