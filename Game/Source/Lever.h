#pragma once
#include "Interactive.h"
#include "Textures.h"
#include "EntityManager.h"
#include "Render.h"
class SecretWall;
class Lever : public Interactive
{
public:

	Lever(SString name, Textures* tex, EntityManager* entityManager, EntityType type, iPoint position);
	~Lever();

	bool Update(float dt);
	bool Draw(Render* render);
	void SetName(SString name);
	void SetSecretWall(SecretWall* secretWall);
	void SetNumber(uint number);
	void Reset();
private:

	void Interact();
	void OnCollision(Collider* collider) override;


private:

	SDL_Rect rect;
	SecretWall* secretWall;
	uint number = 0;
};