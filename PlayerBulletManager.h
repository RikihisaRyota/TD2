#pragma once
#include "PlayerBullet.h"
class PlayerBulletManager {
public:
	~PlayerBulletManager();
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void CreateBullet(const Vector3& position);
	void Reset();
	void SetViewProjection(ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
	std::vector<PlayerBullet*>& GetPlayerBullets() { return bullets_; }
private:
	Model* model_;
	std::vector<PlayerBullet*> bullets_;
	ViewProjection* viewProjection_;
};

