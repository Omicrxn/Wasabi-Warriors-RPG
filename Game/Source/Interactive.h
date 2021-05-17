#pragma once
#ifndef __INTERACTIVE_H__
#define __INTERACTIVE_H__
#include "Entity.h"
#include "SString.h"

class Interactive : public Entity
{
public:

	Interactive() : Entity(EntityType::UNKNOWN) {}
	~Interactive(){}

	void SetDescription(const char* string)
	{
		description = string;
	}

	const char* GetDescription()
	{
		return description.GetString();
	}

	void ChangeInteraction()
	{
		hasInteracted = !hasInteracted;
	}

protected:

	virtual void Interact() = 0;

protected:

	SString description;

	bool hasInteracted = false;
};

#endif // __INTERACTIVE_H__