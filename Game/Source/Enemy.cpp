#include "Enemy.h"

#include "Pathfinding.h"

#define DEFAULT_PATH_LENGTH 50

Enemy::Enemy() : Being()
{
    path = PathFinding::GetInstance()->CreatePath(iPoint(0, 0), iPoint(0, 0));
}

Enemy::~Enemy()
{

}

void Enemy::SetParameters(SDL_Texture* tex, SString name, int level, int damage, int maxHP, int currentHP, int strength, int defense, int attackSpeed, float criticalRate)
{
	this->stats.name = name;
	this->stats.level = level;
	this->stats.damage = damage;
	this->stats.maxHP = maxHP;
	this->stats.currentHP = currentHP;
	this->stats.strength = strength;
	this->stats.defense = defense;
	this->stats.attackSpeed = attackSpeed;
	this->stats.criticalRate = criticalRate;
}