#pragma once
#ifndef __BEING_H__
#define __BEING_H__

#include "Entity.h"

class Being : public Entity
{
public:
	Being() : Entity(EntityType::UNKNOWN) {};
private:
	fPoint velocity;
};
#endif //__BEING_H__