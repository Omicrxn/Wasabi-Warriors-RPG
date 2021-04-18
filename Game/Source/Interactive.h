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

protected:

	virtual void Interact() = 0;

private:

	SString description;
};

#endif // __INTERACTIVE_H__