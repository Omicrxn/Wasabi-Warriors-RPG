#include "Teleport.h"

#include "SceneGameplay.h"

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
	if (collider != nullptr)
	{
		collider->pendingToDelete = true;
		collider = nullptr;
	}
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
	cooldown.Start();
	if (simpleTP)
	{
		if ((name == "floor2_2" && !notifier->GetSceneGameplay()->GetGameProgress()->hasKilledOfficers) ||
			(name == "floor3_2" && !notifier->GetSceneGameplay()->GetGameProgress()->hasActivated))
		{
			notifier->SetTeleportMode(true);
			notifier->SetTeleport(this);
		}
		else
		{
			if (!notifier->OnPositionChange())
			{
				notifier->NotifyPositionChange();
				notifier->SetNextPosition(this->nextPosition);
			}
		}
	}
	else
	{
		if ((destination == MapType::SECOND_RESTAURANT && !notifier->GetSceneGameplay()->GetGameProgress()->hasVisitedErikaTombstone) ||
			(destination == MapType::THIRD_RESTAURANT && !notifier->GetSceneGameplay()->GetGameProgress()->hasSavedFirstApprentice) ||
			(destination == MapType::SKYSCRAPER && !notifier->GetSceneGameplay()->GetGameProgress()->hasSavedLastApprentice))
		{
			notifier->SetTeleportMode(true);
			notifier->SetTeleport(this);
		}
		else
			notifier->NotifyMapChange(destination);

		hasInteracted = true;
	}
}

void Teleport::SetUpDestination(MapType destination)
{
	this->destination = destination;
}

void Teleport::SetAsSimpleTP()
{
	simpleTP = true;
}

void Teleport::SetNextPosition(int nextPosX, int nextPosY)
{
	nextPosition.x = nextPosX;
	nextPosition.y = nextPosY;
}

iPoint Teleport::GetNextPosition()
{
	return nextPosition;
}

void Teleport::OnCollision(Collider* collider)
{
	if (!hasInteracted && cooldown.ReadSec() >= 2.0f)
	{
		Interact();
	}
	/*if (!simpleTP && this->collider != nullptr)
	{
		this->collider->pendingToDelete = true;
		this->collider = nullptr;
	}*/
}
