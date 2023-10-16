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
	void SetViewProjection(const ViewProjection& viewProjection) { viewProjection_ = viewProjection; }

private:
	Model* model_;
	std::vector<PlayerBullet*> bullets_;
	ViewProjection viewProjection_;
};

