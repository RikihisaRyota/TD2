#include "ParticleManager.h"

#include "Bubble.h"

void ParticleManager::Initialize()
{
	
	Bubble::GetInstance()->Initialize();
}

void ParticleManager::Update()
{
	Bubble::GetInstance()->Update();
}

void ParticleManager::ForeDraw()
{
	//Bubble::GetInstance()->Draw();
}

void ParticleManager::Draw()
{

}

void ParticleManager::BackDraw()
{
	//Sprite::SetBlendState(Sprite::BlendState::kNormal);
	Bubble::GetInstance()->Draw();
}
