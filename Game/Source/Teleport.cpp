#include "Teleport.h"
Teleport::Teleport(Collisions* collisions, EntityManager* entityManager)
{
	texture = NULL;
	position = iPoint(10 * 16, 27 * 16);
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
