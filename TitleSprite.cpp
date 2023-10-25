#include "TitleSprite.h"

void TitleSprite::Initialize(std::vector<Sprite*> sprite) {
	for (size_t i = 0; i < static_cast<size_t>(Type::kCount); i++) {
		sprite_.emplace_back(sprite.at(i));
	}
}

void TitleSprite::Update() {}

void TitleSprite::Draw(const ViewProjection& viewProjection) {
	for (auto& sprite : sprite_) {
		sprite->Draw();
	}
}
