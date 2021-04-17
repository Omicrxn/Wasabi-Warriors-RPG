#pragma once
#ifndef __BEING_H__
#define __BEING_H__

#include "Entity.h"
#include "Animation.h"

enum class Animations
{
	IDLE,
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
		name = {};
		level = 0;
		damage = 0;
		maxHP = 0;
		currentHP = 0;
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

	SString name;       // Being name
	int level;          // General being level
	int damage;         // Damage (attack) points
	int maxHP;          // Maximum HP (in order not to get over it when healing the being)
	int currentHP;      // Current HP points
	int strength;       // Base attack damage
	int defense;        // Base defense against attack
	int attackSpeed;    // Attack Speed determines the turn order between party members and enemies
	float criticalRate; // Chance of an attack to be critical
};

class Being : public Entity
{
public:

	Being() : Entity(EntityType::UNKNOWN) {};

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
	SDL_Rect animRec;
	SString classType;
};

#endif //__BEING_H__