#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Being.h"

#include "DynArray.h"
#include "Point.h"
#include "SString.h"

class Enemy : public Being
{
public:

    Enemy();
    virtual ~Enemy();

    void SetParameters(SDL_Texture* tex, SString name, int level, int damage, int maxHP, int currentHP, int strength, int defense, int attackSpeed, float criticalRate);

protected:

    DynArray<iPoint>* path;
};

#endif // __ENEMY_H__
