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

	const char* GetDescription()
	{
		return description.GetString();
	}

protected:

	virtual void Interact() = 0;

protected:

	SString description;

};

#endif // __INTERACTIVE_H__