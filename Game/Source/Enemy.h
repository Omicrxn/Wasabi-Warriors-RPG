#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Being.h"
#include "Render.h"
#include "Textures.h"
#include "EntityManager.h"

#include "Point.h"
#include "SString.h"

class Enemy : public Being
{
public:

    Enemy(Collisions* collisions, EntityManager* entityManager);
    virtual ~Enemy();

public:

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    void SetUpTexture();

    void Walk(iPoint direction, float dt);

    void SetName(SString name);

    void OnCollision(Collider* collider) override;

public:

    Animations currentAnim;

    int width, height;

    bool inCombat = false;
    bool readyForCombat = false;
};

#endif // __ENEMY_H__
