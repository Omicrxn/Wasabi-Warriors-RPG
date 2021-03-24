#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"

#include "DynArray.h"
#include "Point.h"
#include "SString.h"

class Enemy : public Entity
{
public:

    Enemy();
    virtual ~Enemy();

private:

    DynArray<iPoint>* path;
};

#endif // __ENEMY_H__
