#include "Teleport.h"
Teleport::Teleport(SString name, Collisions* collisions, EntityManager* entityManager, EntityType type, iPoint position)
{
	texture = NULL;
	this->position = position;
	this->type = type;
	this->name = name;
	width = 32;
	height = 32;
	collider = collisions->AddCollider({ position.x ,position.y,width,height }, Collider::Type::TELEPORT, (Module*)entityManager);
	active = true;
	notifier = Notifier::GetInstance();
	hasInteracted = false;
}

Teleport::~Teleport()
{
	collider->pendingToDelete = true;
}

bool Teleport::Update(Input* input, float dt)
{
	// Update collider position
	if (collider != nullptr)
	{
		collider->SetPos(position.x, position.y);
		collider->rect.w = width;
		collider->rect.h = height;
	}
	return true;
}


void Teleport::Interact()
{
	notifier->NotifyMapChange(destination);
	hasInteracted = true;
}

void Teleport::SetUpDestination(MapType destination)
{
	this->destination = destination;
}

void Teleport::OnCollision(Collider* collider)
{
	if (!hasInteracted)
	{
		Interact();
	}
	this->collider->pendingToDelete = true;
}
