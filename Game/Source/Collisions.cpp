#include "Collisions.h"

#include "Render.h"
#include "Input.h"
#include "SDL/include/SDL_scancode.h"

#include "Log.h"

Collisions::Collisions(Input* input, Render* render) : Module()
{
	name = "collisions";
	this->input = input;
	this->render = render;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[Collider::Type::WALL][Collider::Type::WALL] = false;
	matrix[Collider::Type::WALL][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::WALL][Collider::Type::ENEMY] = true;

	matrix[Collider::Type::PLAYER][Collider::Type::WALL] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::ENEMY] = true;

	matrix[Collider::Type::ENEMY][Collider::Type::WALL] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::ENEMY] = false;
}

Collisions::~Collisions() {}

bool Collisions::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true) {
			delete colliders[i];
			colliders[i] = nullptr;
			--colliderCount;
		}
	}

	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		// Skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// Avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k) {
			// Skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->Intersects(c2->rect)) {
				if (matrix[c1->type][c2->type] && c1->listener)
					c1->listener->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->listener)
					c2->listener->OnCollision(c2, c1);
			}
		}
	}

	return true;
}

bool Collisions::Update() {
	if (input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	return true;
}

bool Collisions::PostUpdate() {
	if (debug)
		DebugDraw();

	return true;
}

// Debud draw
void Collisions::DebugDraw()
{
	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type) {
		case Collider::Type::NONE:
			render->DrawRectangle(colliders[i]->rect, { 255, 255, 255, alpha });
			break;
		case Collider::Type::WALL:
			render->DrawRectangle(colliders[i]->rect, { 0, 0, 255, alpha });
			break;
		case Collider::Type::PLAYER:
			render->DrawRectangle(colliders[i]->rect, { 0, 255, 0, alpha });
			break;
		case Collider::Type::ENEMY:
			render->DrawRectangle(colliders[i]->rect, { 255, 0, 0, alpha });
			break;
		}
	}
}

bool Collisions::CleanUp() {
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

Collider* Collisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener) {
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		if (colliders[i] == nullptr) {
			ret = colliders[i] = new Collider(rect, type, listener);
			++colliderCount;
			break;
		}
	}

	return ret;
}

bool Collisions::DeleteCollider(Collider* collider) {
	if (collider != nullptr) {
		for (uint i = 0; i < MAX_COLLIDERS; ++i) {
			if (colliders[i] == collider) {
				collider->pendingToDelete = true;
				break;
			}
		}
	}

	return false;
}

// Collider class
Collider::Collider(SDL_Rect rectangle, Type type, Module* listener) : rect(rectangle), type(type), listener(listener) {}

void Collider::SetPos(int x, int y) {
	rect.x = x;
	rect.y = y;
}

bool Collider::Intersects(const SDL_Rect& r) const {
	// Return true if there is an overlap
	// between argument "r" and property "rect"
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.y + rect.h > r.y);
}
