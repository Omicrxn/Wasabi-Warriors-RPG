#include "Teleport.h"
Teleport::Teleport(Collisions* collisions, EntityManager* entityManager)
{
	texture = NULL;
	position = iPoint(10 * 16, 27 * 16);
	width = 32;
	height = 32;
	collider = collisions->AddCollider({ position.x + 86,position.y + 43,width,height }, Collider::Type::TELEPORT, (Module*)entityManager);
	active = true;
}

Teleport::~Teleport()
{
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
	Notifier::GetInstance()->NotifyMapChange(destination);
}

void Teleport::SetUpDestination(MapType destination)
{
	this->destination = destination;
}

void Teleport::OnCollision(Collider* collider)
{
	Interact();
}
