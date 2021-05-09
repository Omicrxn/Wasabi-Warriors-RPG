#pragma once
#ifndef __COLLISION_H__
#define __COLLISION_H__

#define MAX_COLLIDERS 50

#include "Module.h"
#include "SDL/include/SDL_rect.h"

class Input;
class Render;

struct Collider
{
	enum Type
	{
		NONE = -1,
		PLAYER,
		NPC,
		ENEMY,
		TELEPORT,
		SIGN,
		ITEM,
		ACTIVATOR,
		MAX
	};

	//Methods
	Collider(SDL_Rect rectangle, Type type, Module* listener = nullptr);

	void SetPos(int x, int y);

	bool Intersects(const SDL_Rect& r) const;

	//Variables
	SDL_Rect rect;
	bool pendingToDelete = false;
	Type type;
	Module* listener = nullptr;
};


class Collisions : public Module
{
public:
	// Constructor
	// Fills all collision matrix data
	Collisions(Input* input, Render* render);

	// Destructor
	~Collisions();

	// Called at the beginning of the application loop
	// Removes all colliders pending to delete
	// Checks for new collisions and calls its listeners
	bool PreUpdate();

	// Called at the middle of the application loop
	// Switches the debug mode on/off
	bool Update(float dt);

	// Called at the end of the application loop
	// Draw all colliders (if debug mode is enabled)
	bool PostUpdate();

	// Removes all existing colliders
	bool CleanUp();

	// Adds a new collider to the list
	Collider* AddCollider(SDL_Rect rect, Collider::Type type, Module* listener = nullptr);

	//Delete collider to the list
	bool DeleteCollider(Collider* collider);

	// Draws all existing colliders with some transparency
	void DebugDraw();

	inline uint GetColliderCount() const { return colliderCount; };

	// Load / Save
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	bool godMode = false;

private:
	// All existing colliders in the scene
	Collider* colliders[MAX_COLLIDERS] = { nullptr };

	// The collision matrix. Defines the interaction for two collider types
	// If set two false, collider 1 will ignore collider 2
	bool matrix[Collider::Type::MAX][Collider::Type::MAX];

	// Simple debugging flag to draw all colliders
	bool debug = false;
	// The amount of colliders loaded into the array
	uint colliderCount = 0;

	// Needed modules
	Input* input;
	Render* render;
};

#endif // __COLLISION_H__
