#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"

struct SDL_Texture;
struct Render;
struct Input;

enum class EntityType
{
    PLAYER,
    ENEMY,
    ITEM,
    MAP,
    NPC,
    UNKNOWN
};

class Entity
{
public:

    Entity(EntityType type) : type(type), active(true) {}

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
    void SetTexture(SDL_Texture* texture, int spritePos = -1) {
        this->texture = texture;
        this->spritePos = spritePos;
        if (spritePos > -1)
            SetUpTexture();
    }
    virtual void SetUpTexture() {}

public:

    EntityType type;
    SString name;         // Entity name identifier?
    uint32 id;            // Entity identifier?

    // Possible properties, it depends on how generic we
    // want our Entity class, maybe it's not renderable...
    iPoint position;        // Use a float instead?
    bool renderable = false;
    SDL_Texture* texture;

    bool destroy = false;

protected:

    bool active = true;
    int spritePos;
};

#endif // __ENTITY_H__