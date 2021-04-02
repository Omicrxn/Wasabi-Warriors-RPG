#include "Enemy.h"

#include "Pathfinding.h"

#define DEFAULT_PATH_LENGTH 50

Enemy::Enemy() : Being()
{
    path = PathFinding::GetInstance()->CreatePath(iPoint(0, 0), iPoint(0, 0));

	// Define enemy parameters
	this->stats.name = "Enemy";
	this->stats.level = 1;
	this->stats.damage = 10;
	this->stats.maxHP = 120;
	this->stats.currentHP = 100;
	this->stats.strength = 10;
	this->stats.defense = 10;
	this->stats.attackSpeed = 5;
	this->stats.criticalRate = 5;
}

Enemy::~Enemy()
{

}

void Enemy::Walk(iPoint direction, float dt)
{
    //if (direction.x != 0 && direction.y != 0)
    //{
    //    // limit movement speed diagonally, so you move at 70% speed
    //    //velocity.x *= 0.7;
    //    //velocity.y *= 0.7;

    //    velocity = { 0,0 };
    //}
    //else
    //{
    //    velocity = { 150.0f, 150.0f };
    //}

    //position.x = position.x + direction.x * (velocity.x * dt);
    //position.y = position.y + direction.y * (velocity.y * dt);
}