#include "BackGround.h"

#include "MyMath.h"

BackGround::~BackGround() {
	for (auto& sprite : sprite_) {
		delete sprite;
	}
	sprite_.clear();
}

void BackGround::Initialize(std::vector<uint32_t> textureHandle) {
	spritePosition_ = { 1280.0f * 0.5f,720.0f * 0.5f };
	for (size_t i = 0; i < static_cast<size_t>(Type::kCount); i++) {
		sprite_.emplace_back(Sprite::Create(textureHandle.at(i), spritePosition_, Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f)));
	}
}

void BackGround::Update() {
}

void BackGround::Draw(const ViewProjection& viewProjection) {
	for (auto& sprite : sprite_) {
		sprite->Draw();
	}
}
