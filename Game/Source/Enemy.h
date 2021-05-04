#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Being.h"
#include "Render.h"
#include "Textures.h"
#include "EntityManager.h"

#include "Point.h"
#include "SString.h"

enum class EnemyType 
{
    HENCHMAN,
    BRUISER,
    BOSS
};

class Enemy : public Being
{
public:

    Enemy(Textures* tex,Collisions* collisions, EntityManager* entityManager, Transitions* transitions, EntitySubtype subtype);
    ~Enemy();

public:

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    void SetUpTexture(SString texPath);

    void Walk(iPoint direction, float dt);

    void SetName(SString name);

    void OnCollision(Collider* collider) override;

    // To setup the enemy class type
    bool SetUpClass(SString name);

    void StartCombat();

public:

    Animations currentAnim;

    int width, height;

    bool readyForCombat = true;

    // Needed modules
    Transitions* transitions = nullptr;
};

#endif // __ENEMY_H__
