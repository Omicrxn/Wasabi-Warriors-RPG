#pragma once
#include "Interactive.h"
#include "EntityManager.h"
#include "Timer.h"

class Teleport : public Interactive
{
public:

	Teleport(SString name, Collisions* collisions, EntityManager* entityManager, EntityType type, iPoint position);
	~Teleport();

	bool Update(Input* input, float dt);
	void SetName(SString name){ this->nameTeleport = name; }
	SString GetName() { return this->nameTeleport; }
	SDL_Rect GetBounds() { return { position.x,position.y,width,height }; }
	void SetUpDestination(MapType destination);
	MapType GetDestination() { return destination; }
	void SetAsSimpleTP();
	void SetNextPosition(int nextPosX, int nextPosY);
	iPoint GetNextPosition();

private:

	void Interact();
	void OnCollision(Collider* collider) override;

	MapType destination = MapType::NONE;
	SString nameTeleport; // NPC name

	uint stepsCounter;

	// Bool to set the TP as simple (without map change)
	bool simpleTP = false;
	iPoint nextPosition;

	Timer cooldown;
};