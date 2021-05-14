#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Collisions.h"
#include "Notifier.h"
#include "Animation.h"

struct SDL_Texture;
struct Render;
struct Input;
struct Textures;
class EntityManager;


enum class EntityType
{
    UNKNOWN = -1,
    PLAYER,
    ENEMY,
    ITEM,
    MAP,
    NPC,
    TELEPORT,
    ACTIVATOR
};
 
enum class EntitySubtype
{
    UNKNOWN = -1,
    PLAYER_HUNTER,
    PLAYER_WIZARD,
    ENEMY_HENCHMAN,
    ENEMY_BRUISER,
    ENEMY_BOSS,
    ITEM_POTION,
    ITEM_BPOTION,
    ITEM_MPOTION,
    ITEM_WASABI,
    ITEM_PAN,
    ITEM_FORK

};
class Entity
{
public:

    Entity(EntityType type) : type(type), active(true), name("") {}
    virtual ~Entity() {}

    virtual bool Update(float dt)
    {
        return true;
    }

    virtual bool Update(Input* input, float dt)
    {
        return true;
    }

    virtual bool Draw(Render* render)
    {
        return true;
    }

    bool IsActive()
    {
        return active;
    }

    void SetState(bool state)
    {
        active = state;
    }
    void SetTexture(int spritePos = -1) {
        this->spritePos = spritePos;
        if (spritePos > -1)
            SetUpTexture();
    }
    virtual void SetUpTexture() {}

    const Collider* GetCollider() const { return collider; }

    virtual void OnCollision(Collider* collider) {};
    virtual SDL_Rect GetBounds() const { return { collider->rect.x, collider->rect.y, width, height }; };

public:

    EntityType type;
    EntitySubtype subtype;
    SString name;         // Entity name identifier?
    uint32 id;            // Entity identifier?
    Collider* collider = nullptr;
    // Possible properties, it depends on how generic we
    // want our Entity class, maybe it's not renderable...
    iPoint position;        // Use a float instead?
    bool renderable = false;
    SDL_Texture* texture = nullptr;
    SString texPath;
    int width, height;
    bool destroy = false;
    int spritePos;
    Collisions* collisions;
protected:

    bool active = true;
    Notifier* notifier;
    Textures* tex = nullptr;
    Input* input = nullptr;
    EntityManager* entityManager = nullptr;

public:

    SDL_Rect animRec;
};

#endif // __ENTITY_H__