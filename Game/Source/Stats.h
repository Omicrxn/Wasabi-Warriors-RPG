#pragma once
class Stats
{
public:

	Stats()
	{
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
	Stats operator +(const Stats &s)const
	{
		Stats returnStats;
		returnStats.level = level + s.level;
		returnStats.damage = damage + s.damage;
		returnStats.maxHP = maxHP + s.maxHP;
		returnStats.currentHP = currentHP + s.currentHP;
		returnStats.strength = strength + s.strength;
		returnStats.defense = defense + s.defense;
		returnStats.attackSpeed = attackSpeed + s.attackSpeed;
		returnStats.criticalRate = criticalRate + s.criticalRate;
		return returnStats;
	}

public:

	int level;          // General being level
	int damage;         // Damage (attack) points
	int maxHP;          // Maximum HP (in order not to get over it when healing the being)
	int currentHP;      // Current HP points
	int strength;       // Base attack damage
	int defense;        // Base defense against attack
	int attackSpeed;    // Attack Speed determines the turn order between party members and enemies
	float criticalRate; // Chance of an attack to be critical
};