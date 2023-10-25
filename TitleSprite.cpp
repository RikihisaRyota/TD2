#include "TitleSprite.h"

#include "TitleBoss.h"
#include "ImGuiManager.h"
#include "MyMath.h"

void TitleSprite::Initialize(std::vector<Sprite*> sprite) {
	for (size_t i = 0; i < static_cast<size_t>(Type::kCount); i++) {
		sprite_.emplace_back(sprite.at(i));
	}
	moveBaseSize_ = { 256.0f,256.0f };
	animationTime_ = 0.0f;
	animationCount_ = 0.0f;
	animationMax_ = 5.0f;
	arrowAngle_ = 0.0f;
	titleAngle_ = 0.0f;
	moveAngle_ = 0.0f;
	interval_ = 146.0f;
	titlePos_ = { 640.0f,190.0f };
	arrowPos_ = { 1170.0f,370.0f };
	arrowSize_ = { 128.0f,128.0f };
	movePos_ = {640.0f,520.0f};
	moveSize_ = { 200.0f,200.0f };
	sprite_.at(static_cast<size_t>(Type::kTitle))->SetPosition(titlePos_);
	sprite_.at(static_cast<size_t>(Type::kTitle))->SetRotation(0.0f);
	sprite_.at(static_cast<size_t>(Type::kArrow0))->SetSize(arrowSize_);
	sprite_.at(static_cast<size_t>(Type::kArrow0))->SetPosition({ arrowPos_.x,arrowPos_.y + interval_ });
	sprite_.at(static_cast<size_t>(Type::kArrow0))->SetRotation(0.0f);
	sprite_.at(static_cast<size_t>(Type::kArrow1))->SetSize(arrowSize_);
	sprite_.at(static_cast<size_t>(Type::kArrow1))->SetPosition({ arrowPos_.x,arrowPos_.y });
	sprite_.at(static_cast<size_t>(Type::kArrow1))->SetRotation(0.0f);
	sprite_.at(static_cast<size_t>(Type::kArrow2))->SetSize(arrowSize_);
	sprite_.at(static_cast<size_t>(Type::kArrow2))->SetPosition({ arrowPos_.x,arrowPos_.y - interval_ });
	sprite_.at(static_cast<size_t>(Type::kArrow2))->SetRotation(0.0f);
	sprite_.at(static_cast<size_t>(Type::kMove))->SetSize(moveSize_);
	sprite_.at(static_cast<size_t>(Type::kMove))->SetPosition(movePos_);
	sprite_.at(static_cast<size_t>(Type::kMove))->SetRotation(0.0f);
}

void TitleSprite::Update(const ViewProjection& viewProjection) {

	ViewProjection view = viewProjection;
	Vector3 worldPos = { titleBoss_->GetWorldMat().m[3][0] + 20.0f ,titleBoss_->GetWorldMat().m[3][1] ,titleBoss_->GetWorldMat().m[3][2] };
	Matrix4x4 matViewport = MakeViewportMatrix(0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 1.0f);
	Matrix4x4 matViewProjectionViewport = view.matView_ * view.matProjection_ * matViewport;
	Vector3 screenPos = Transform(worldPos, matViewProjectionViewport);

	// title
	if (sprite_.at(static_cast<size_t>(Type::kTitle))->GetPosition().x <= screenPos.x) {
		titleAngle_ += 0.5f;
		Vector2 titleTranslate = sprite_.at(static_cast<size_t>(Type::kTitle))->GetPosition();
		sprite_.at(static_cast<size_t>(Type::kTitle))->SetRotation(titleAngle_);
		Vector2 vector = { std::cosf(DegToRad(-30.0f)),std::sinf(DegToRad(-30.0f)) };
		titleTranslate += vector * 15.0f;
		sprite_.at(static_cast<size_t>(Type::kTitle))->SetPosition(titleTranslate);

	}
	else {
		Vector2 titleTranslate = sprite_.at(static_cast<size_t>(Type::kTitle))->GetPosition();
		titleAngle_ += 0.05f;
		titleTranslate.y += std::sinf(titleAngle_) * 0.5f;
		if (DegToRad(titleAngle_) >= 360.0f) {
			titleAngle_ = 0.0f;
		}
		sprite_.at(static_cast<size_t>(Type::kTitle))->SetPosition(titleTranslate);
	}
	// arrow
	if (sprite_.at(static_cast<size_t>(Type::kArrow1))->GetPosition().x <= screenPos.x) {
		arrowAngle_ += 0.5f;
		sprite_.at(static_cast<size_t>(Type::kArrow0))->SetRotation(titleAngle_);
		sprite_.at(static_cast<size_t>(Type::kArrow1))->SetRotation(titleAngle_);
		sprite_.at(static_cast<size_t>(Type::kArrow2))->SetRotation(titleAngle_);
		Vector2 vector = { std::cosf(DegToRad(-10.0f)),std::sinf(DegToRad(-10.0f)) };
		arrowPos_ += vector * 15.0f;
		sprite_.at(static_cast<size_t>(Type::kArrow0))->SetPosition({ arrowPos_.x,arrowPos_.y + interval_ });
		vector = { std::cosf(DegToRad(0.0f)),std::sinf(DegToRad(0.0f)) };
		arrowPos_ += vector * 15.0f;
		sprite_.at(static_cast<size_t>(Type::kArrow1))->SetPosition(arrowPos_);
		vector = { std::cosf(DegToRad(10.0f)),std::sinf(DegToRad(10.0f)) };
		arrowPos_ += vector * 15.0f;
		sprite_.at(static_cast<size_t>(Type::kArrow2))->SetPosition({ arrowPos_.x,arrowPos_.y - interval_ });

	}
	else {
		arrowAngle_ += 0.05f;
		arrowPos_.x += std::cosf(arrowAngle_) * 0.5f;
		if (DegToRad(titleAngle_) >= 360.0f) {
			titleAngle_ = 0.0f;
		}
		sprite_.at(static_cast<size_t>(Type::kArrow0))->SetPosition({ arrowPos_.x,arrowPos_.y + interval_ });
		sprite_.at(static_cast<size_t>(Type::kArrow1))->SetPosition(arrowPos_);
		sprite_.at(static_cast<size_t>(Type::kArrow2))->SetPosition({ arrowPos_.x,arrowPos_.y - interval_ });
	}

	// move
	if (sprite_.at(static_cast<size_t>(Type::kMove))->GetPosition().x <= screenPos.x) {
		moveAngle_ += 0.5f;
		sprite_.at(static_cast<size_t>(Type::kMove))->SetRotation(titleAngle_);
		Vector2 vector = { std::cosf(DegToRad(30.0f)),std::sinf(DegToRad(30.0f)) };
		movePos_ += vector * 15.0f;
		sprite_.at(static_cast<size_t>(Type::kMove))->SetPosition(movePos_ );
	}
	else {
		if (animationTime_ >= animationMax_) {
			animationTime_ = 0.0f;
			animationCount_ += 1.0f;
			if (animationCount_ >= 5.0f) {
				animationCount_ = 0.0f;
			}
		}
		sprite_.at(static_cast<size_t>(Type::kMove))->SetTextureRect({moveBaseSize_.x* animationCount_,0.0f},  moveBaseSize_);
		animationTime_ += 1.0f;
	}
}

void TitleSprite::Draw() {
	for (auto& sprite : sprite_) {
		sprite->Draw();
	}
}
