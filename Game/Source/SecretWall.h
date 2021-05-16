#pragma once
#include "Interactive.h"
#include "Textures.h"
#include "EntityManager.h"
#include "Render.h"
class SecretWall : public Interactive
{
public:

	SecretWall(SString name,Collisions* collisions, Textures* tex, EntityManager* entityManager, EntityType type, iPoint position);
	~SecretWall();

	bool Update(float dt);
	bool Draw(Render* render);
	void SetName(SString name);
	void SetLever(int leverNumber);

private:

	void Interact() {};
	void OnCollision(Collider* collider) override;
	

private:

	SDL_Rect rect;
	List<uint> sequence;
public:
	bool lever1 = false;
	bool lever2 = false;
	bool lever3 = false;
	bool reset = false;
	Input* input = nullptr;
};