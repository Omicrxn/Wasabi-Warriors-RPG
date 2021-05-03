#pragma once
#ifndef __BEING_H__
#define __BEING_H__

#include "Entity.h"
#include "Stats.h"


enum class Animations
{
	IDLE,
	UP,
	DOWN,
	LEFT,
	RIGHT
};



class Being : public Entity
{
public:

	Being() : Entity(EntityType::UNKNOWN) {};
	virtual ~Being() {}

	virtual void Walk(iPoint direction, float dt) = 0;
	virtual void SetUpTexture() {}
	virtual bool SetUpClass(SString name) { return true; }

	fPoint velocity;
	iPoint direction;
	Stats stats;
	Animation idleAnim;
	Animation walkRightAnim;
	Animation walkLeftAnim;
	Animation walkUpAnim;
	Animation walkDownAnim;

	SString classType;
};

#endif //__BEING_H__