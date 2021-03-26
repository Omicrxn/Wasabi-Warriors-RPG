#include "Enemy.h"

#include "Pathfinding.h"

#define DEFAULT_PATH_LENGTH 50

Enemy::Enemy(SString name, int level, int damage, int maxHP, int currentHP) : Entity(EntityType::ENEMY)
{
    path = PathFinding::GetInstance()->CreatePath(iPoint(0, 0), iPoint(0, 0));

    this->name = name;
    this->level = level;

    this->damage = damage;

    this->maxHP = maxHP;
    this->currentHP = currentHP;
}

Enemy::~Enemy()
{

}