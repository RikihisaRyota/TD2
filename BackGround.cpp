#include "BackGround.h"

#include "MyMath.h"
#include "Player.h"

BackGround::~BackGround() {
	for (auto& sprite : backSprite_) {
		delete sprite;
	}
	backSprite_.clear();
	for (auto& sprite : middleSprite_) {
		delete sprite;
	}
	middleSprite_.clear();
	for (auto& sprite : frontSprite_) {
		delete sprite;
	}
	frontSprite_.clear();
}

void BackGround::Initialize(std::vector<uint32_t> textureHandle) {

	for (size_t i = 0; i < static_cast<size_t>(Type::kCount); i++) {
		Vector2 pos = { 1280.0f * 0.5f ,720.0f * 0.5f };
		position_.emplace_back(pos);
		switch (i) {
		case BackGround::kBack:
			pos = { 1280.0f * 0.5f - 1280.0f,720.0f * 0.5f };
			backSprite_.emplace_back(Sprite::Create(textureHandle.at(i), pos, Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f)));
			pos = { 1280.0f * 0.5f ,720.0f * 0.5f };
			backSprite_.emplace_back(Sprite::Create(textureHandle.at(i), pos, Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f)));
			pos = { 1280.0f * 0.5f + 1280.0f ,720.0f * 0.5f };
			backSprite_.emplace_back(Sprite::Create(textureHandle.at(i), pos, Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f)));
			break;
		case BackGround::kMiddle:
			pos = { 1280.0f * 0.5f - 1280.0f,720.0f * 0.5f };
			middleSprite_.emplace_back(Sprite::Create(textureHandle.at(i), pos, Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f)));
			pos = { 1280.0f * 0.5f ,720.0f * 0.5f };
			middleSprite_.emplace_back(Sprite::Create(textureHandle.at(i), pos, Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f)));
			pos = { 1280.0f * 0.5f + 1280.0f ,720.0f * 0.5f };
			middleSprite_.emplace_back(Sprite::Create(textureHandle.at(i), pos, Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f)));
			break;
		case BackGround::kFront:
			pos = { 1280.0f * 0.5f - 1280.0f,720.0f * 0.5f };
			frontSprite_.emplace_back(Sprite::Create(textureHandle.at(i), pos, Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f)));
			pos = { 1280.0f * 0.5f ,720.0f * 0.5f };
			frontSprite_.emplace_back(Sprite::Create(textureHandle.at(i), pos, Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f)));
			pos = { 1280.0f * 0.5f + 1280.0f ,720.0f * 0.5f };
			frontSprite_.emplace_back(Sprite::Create(textureHandle.at(i), pos, Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f)));
			break;
		}
	}
}

void BackGround::Update() {
	Vector3 playerPos = player_->GetTranslation();
	float const screenWidth = 1280.0f;
	for (size_t i = 0; i < static_cast<size_t>(Type::kCount); i++) {
		switch (i) {
		case BackGround::kBack:
			break;
		case BackGround::kMiddle:
			position_.at(i).x = std::fmodf(playerPos.x * -1.5f, screenWidth);
			if (position_.at(i).x < 0.0f) {
				position_.at(i).x += screenWidth;
			}
			middleSprite_.at(0)->SetPosition({ position_.at(i).x - 1280.0f,position_.at(i).y });
			middleSprite_.at(1)->SetPosition(position_.at(i));
			middleSprite_.at(2)->SetPosition({ position_.at(i).x + 1280.0f,position_.at(i).y });
			break;
		case BackGround::kFront:
			position_.at(i).x = std::fmodf(playerPos.x * -3.5f, screenWidth);
			if (position_.at(i).x < 0.0f) {
				position_.at(i).x += screenWidth;
			}
			frontSprite_.at(0)->SetPosition({ position_.at(i).x - 1280.0f,position_.at(i).y });
			frontSprite_.at(1)->SetPosition(position_.at(i));
			frontSprite_.at(2)->SetPosition({ position_.at(i).x + 1280.0f,position_.at(i).y });
			break;
		}
	}
}

void BackGround::Draw() {
	for (auto& back : backSprite_) {
		back->Draw();
	}
	for (auto& middle : middleSprite_) {
		middle->Draw();
	}
	for (auto& front : frontSprite_) {
		front->Draw();
	}
}
