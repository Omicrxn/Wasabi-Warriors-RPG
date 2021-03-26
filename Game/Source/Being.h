#pragma once
#ifndef __BEING_H__
#define __BEING_H__

#include "Entity.h"
#include "Animation.h"
class Stats
{
public:
	Stats() 
	{
		strength = 0;
		defense = 0;
		attackSpeed = 0;
		criticalRate = 0;
	}
	Stats(int strength, int defense, int attackSpeed, float criticalRate) 
	{
		strength = strength;
		defense = defense;
		attackSpeed = attackSpeed;
		criticalRate = criticalRate;

	}
	int strength; // Base attack damage
	int defense; // Base defense against attack
	int attackSpeed; // Attack Speed determines the turn order between party members and enemies
	float criticalRate; //Chance of an attack to be critical
};
class Being : public Entity
{
public:
	Being() : Entity(EntityType::UNKNOWN) {};
public:
	virtual void Walk(iPoint direction, float dt) = 0;
protected:
	int hp;
	fPoint velocity;
	iPoint direction;
	Stats stats;
	Animation idleAnim;
	Animation walkingAnim;
};
#endif //__BEING_H__