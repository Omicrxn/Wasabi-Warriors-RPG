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

private:

    void Walk(iPoint direction, float dt);

protected:

    DynArray<iPoint>* path;
};

#endif // __ENEMY_H__
