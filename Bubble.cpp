#include "Bubble.h"

#include "TextureManager.h"
#include "MyMath.h"
#include "WinApp.h"

Bubble* Bubble::GetInstance()
{
	static Bubble instance;
	return &instance;
}

void Bubble::Create(const Vector3& position, const Vector2& velocity, const ViewProjection& viewProjection)
{
	if (time_ == 0) {
		Vector3 position2D = position;

		// ビューポート行列
		Matrix4x4 matViewport =
			MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
		// ビュー行列とプロジェクション行列、ビューポート行列を合成する
		Matrix4x4 matViewProjectionViewport =
			Mul(Mul(viewProjection.matView_, viewProjection.matProjection_), matViewport);
		// ワールド→スクリーン座標変換(ここで3Dから2Dになる)
		position2D = Transform(position2D, matViewProjectionViewport);

		BubleStatus* newBubble = new BubleStatus;
		newBubble->position = Vector2(position2D.x, position2D.y);
		newBubble->velocity = velocity;
		newBubble->velocity.y *= 10.0f;
		newBubble->velocity.x *= 10.0f;
		newBubble->sprite = Sprite::Create(
			textureHandle_, newBubble->position, newBubble->color, newBubble->anchorpoint);
		newBubble->size = newBubble->sprite->GetSize();
		bubbles_.push_back(newBubble);
		time_ = 5;
	}
	
	time_--;
}

void Bubble::Initialize()
{
	bubbles_.clear();
	textureHandle_ = TextureManager::Load("Resources/Images/bubble.png");
	
}

void Bubble::Update()
{
	for (auto& bubble : bubbles_) {
		// 移動
		bubble->position.x += bubble->velocity.x;
		bubble->position.y += bubble->velocity.y;

		
		bubble->velocity.y -= 0.1f;
		
		// サイズ(小さくなる)
		bubble->size.x -= 0.2f;
		bubble->size.y -= 0.2f;
		if (bubble->size.x <= 0.0f) {
			bubble->isDead = true; // 0になったらフラグが立つ
		}

		

		// スプライトに反映
		bubble->sprite->SetPosition(bubble->position);
		bubble->sprite->SetSize(bubble->size);
	}

	bubbles_.remove_if([](BubleStatus* bubble) {
		if (bubble->isDead) {
			delete bubble->sprite;
			delete bubble;
			return true;
		}
		return false;
		});

}

void Bubble::Draw()
{
	for (auto& bubble : bubbles_) {
		bubble->sprite->Draw();
	}

}

void Bubble::IsDeat()
{
}

void Bubble::Finalize()
{
	for (auto& bubble : bubbles_) {
		delete bubble->sprite;
		delete bubble;
		bubble = nullptr;
	}

	bubbles_.clear();
}
