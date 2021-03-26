#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"

#include "DynArray.h"
#include "Point.h"
#include "SString.h"

class Enemy : public Entity
{
public:

    Enemy(SString name, int level, int damage, int maxHP, int currentHP);
    virtual ~Enemy();

    SString name;
    int level = 0;

    int damage = 0;

    int maxHP = 0;
    int currentHP = 0;

protected:

    DynArray<iPoint>* path;
};

#endif // __ENEMY_H__
