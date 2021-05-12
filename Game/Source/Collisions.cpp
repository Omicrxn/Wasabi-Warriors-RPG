#include "Collisions.h"

#include "Render.h"
#include "Input.h"
#include "SDL/include/SDL_scancode.h"
#include "Map.h"
#include "Log.h"

Collisions::Collisions(Input* input, Render* render) : Module()
{
	name = "collisions";
	this->input = input;
	this->render = render;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::NPC] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::TELEPORT] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::SIGN] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::ITEM] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::ACTIVATOR] = false;

	matrix[Collider::Type::ENEMY][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::NPC] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::TELEPORT] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::SIGN] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::ITEM] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::ACTIVATOR] = false;

	matrix[Collider::Type::NPC][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::NPC][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::NPC][Collider::Type::NPC] = false;
	matrix[Collider::Type::NPC][Collider::Type::TELEPORT] = false;
	matrix[Collider::Type::NPC][Collider::Type::SIGN] = false;
	matrix[Collider::Type::NPC][Collider::Type::ITEM] = false;
	matrix[Collider::Type::NPC][Collider::Type::ACTIVATOR] = false;

	matrix[Collider::Type::TELEPORT][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::TELEPORT][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::TELEPORT][Collider::Type::NPC] = false;
	matrix[Collider::Type::TELEPORT][Collider::Type::TELEPORT] = false;
	matrix[Collider::Type::TELEPORT][Collider::Type::SIGN] = false;
	matrix[Collider::Type::TELEPORT][Collider::Type::ITEM] = false;
	matrix[Collider::Type::TELEPORT][Collider::Type::ACTIVATOR] = false;

	matrix[Collider::Type::ITEM][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::ITEM][Collider::Type::NPC] = false;
	matrix[Collider::Type::ITEM][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::ITEM][Collider::Type::TELEPORT] = false;
	matrix[Collider::Type::ITEM][Collider::Type::SIGN] = false;
	matrix[Collider::Type::ITEM][Collider::Type::ITEM] = false;
	matrix[Collider::Type::ITEM][Collider::Type::ACTIVATOR] = false;

	matrix[Collider::Type::SIGN][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::SIGN][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::SIGN][Collider::Type::NPC] = false;
	matrix[Collider::Type::SIGN][Collider::Type::TELEPORT] = false;
	matrix[Collider::Type::SIGN][Collider::Type::SIGN] = false;
	matrix[Collider::Type::SIGN][Collider::Type::ITEM] = false;
	matrix[Collider::Type::SIGN][Collider::Type::ACTIVATOR] = false;

	matrix[Collider::Type::ACTIVATOR][Collider::Type::ACTIVATOR] = false;
	matrix[Collider::Type::ACTIVATOR][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::ACTIVATOR][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::ACTIVATOR][Collider::Type::NPC] = false;
	matrix[Collider::Type::ACTIVATOR][Collider::Type::TELEPORT] = false;
	matrix[Collider::Type::ACTIVATOR][Collider::Type::SIGN] = false;
	matrix[Collider::Type::ACTIVATOR][Collider::Type::ITEM] = false;
}

Collisions::~Collisions() {}

bool Collisions::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
			--colliderCount;
		}
	}

	Collider* c1;
	Collider* c2;

	if (godMode)
		return true;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// Skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// Avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// Skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c2->type == Collider::Type::ITEM) c1->rect = { c1->rect.x * 3, c1->rect.y * 3, c1->rect.w * 3, c1->rect.h * 3 };
			if (c1->Intersects(c2->rect))
			{
				if (matrix[c1->type][c2->type] && c1->listener)
					c1->listener->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->listener)
					c2->listener->OnCollision(c2, c1);
			}
			if (c2->type == Collider::Type::ITEM) c1->rect = { c1->rect.x / 3, c1->rect.y / 3, c1->rect.w / 3, c1->rect.h / 3 };
		}
	}

	return true;
}

bool Collisions::Update(float dt)
{
	if (input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		debug = !debug;
	if (input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		godMode = !godMode;

	return true;
}

bool Collisions::PostUpdate()
{
	if (debug)
		DebugDraw();

	return true;
}

// Debug draw
void Collisions::DebugDraw()
{
	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		if (colliders[i] == nullptr)
			continue;
		render->scale = 3;
		switch (colliders[i]->type) {
		case Collider::Type::NONE:
			render->DrawRectangle(colliders[i]->rect, { 255, 255, 255, alpha });
			break;
		case Collider::Type::NPC:
			render->DrawRectangle(colliders[i]->rect, { 0, 0, 255, alpha });
			break;
		case Collider::Type::PLAYER:
			render->DrawRectangle(colliders[i]->rect, { 0, 255, 0, alpha });
			break;
		case Collider::Type::ENEMY:
			render->DrawRectangle(colliders[i]->rect, { 255, 0, 0, alpha });
			break;
		case Collider::Type::TELEPORT:
			render->DrawRectangle(colliders[i]->rect, { 255, 255, 0, alpha });
			break;
		case Collider::Type::SIGN:
			render->DrawRectangle(colliders[i]->rect, { 255, 0, 255, alpha });
			break;
		case Collider::Type::ITEM:
			render->scale = 1;
			render->DrawRectangle(colliders[i]->rect, { 127, 0, 255, alpha });
			render->scale = 3;
			break;
		case Collider::Type::ACTIVATOR:
			render->DrawRectangle(colliders[i]->rect, { 255, 127, 80, alpha });
			break;
		}
		render->scale = 1;
	}
}

bool Collisions::LoadState(pugi::xml_node& collisionsNode)
{
	return true;
}

bool Collisions::SaveState(pugi::xml_node& collisionsNode) const
{
	/* ---------- CHECKS IF THE Attribute WE WANT OVERWRITE EXISTS OR NOT  ----------*/
	SString tempName = collisionsNode.attribute("collisionsCount").name();
	if (tempName == "collisionsCount")
	{
		// Attribute currentMap exists
		collisionsNode.attribute("collisionsCount").set_value(this->GetColliderCount());
	}
	else
	{
		// Attribute currentMap does not exist
		collisionsNode.append_attribute("collisionsCount").set_value(this->GetColliderCount());
	}
	return true;
}

bool Collisions::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		if (colliders[i] != nullptr) {
			delete colliders[i];
			colliders[i] = nullptr;
			--colliderCount;
		}
	}

	return true;
}

Collider* Collisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			++colliderCount;
			break;
		}
	}

	return ret;
}

bool Collisions::DeleteCollider(Collider* collider)
{
	if (collider != nullptr)
	{
		for (uint i = 0; i < MAX_COLLIDERS; ++i)
		{
			if (colliders[i] == collider) {
				collider->pendingToDelete = true;
				break;
			}
		}
	}

	return false;
}


inline bool CheckCollision(SDL_Rect rec1, SDL_Rect rec2)
{
	if ((rec1.x < (rec2.x + rec2.w) && (rec1.x + rec1.w) > rec2.x) &&
		(rec1.y < (rec2.y + rec2.h) && (rec1.y + rec1.h) > rec2.y)) return true;
	else return false;
}

void Collisions::DetectTilemapCollision(Collider* collider,Map* map, iPoint tempPosition, iPoint& entityPosition)
{
	// Check if updated player position collides with next tile
	// IMPROVEMENT: Just check adyacent tiles to player
		for (int y = 0; y < map->data.height; y++)
		{
			for (int x = 0; x < map->data.width; x++)
			{
				//Check ground
				if ((map->data.layers[4]->Get(x, y) >= 1051) &&
					CheckCollision(map->GetTilemapRec(x, y), collider->rect))
				{
						entityPosition = tempPosition;
					break;
				}
			}
		}
}

// Collider class
Collider::Collider(SDL_Rect rectangle, Type type, Module* listener) : rect(rectangle), type(type), listener(listener) {}

void Collider::SetPos(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

bool Collider::Intersects(const SDL_Rect& r) const
{
	// Return true if there is an overlap
	// between argument "r" and property "rect"
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.y + rect.h > r.y);
}
