#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class BackGround {
public:
	enum Type {
		kBack,
		kMiddle,
		kFront,

		kCount,
	};
public:
	~BackGround();
	void Initialize(std::vector<uint32_t> textureHandle);
	void Update();
	void Draw(const ViewProjection& viewProjection);
private:
	Vector2 spritePosition_;
	std::vector<Sprite*>sprite_;
	uint32_t textureHandle_;
};

