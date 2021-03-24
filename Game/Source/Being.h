#pragma once
#ifndef __BEING_H__
#define __BEING_H__

#include "Entity.h"
enum class Direction 
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};
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
private:
	void Walk();
private:
	int hp;
	fPoint velocity;
	Direction direction;
	Stats stats;
};
#endif //__BEING_H__