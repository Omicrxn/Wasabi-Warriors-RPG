#ifndef __ITEM_H__
#define __ITEM_H__

#include "Interactive.h"

#include "Point.h"
#include "Textures.h"
#include "SString.h"
#include "Stats.h"

class EntityManager;

class Item : public Interactive
{
public:

    Item(SString name,Textures* tex, Collisions* collisions, EntityManager* entityManager,EntityType type, EntitySubtype subtype, iPoint position);
    virtual ~Item();
    bool Draw(Render* render);
    void OnCollision(Collider* collider) override;
    void Interact() {}
    Stats Interact(Stats stats);
    bool SetUpClass(SString name);

public:

    Stats stats;
    bool usable = true;
    SString classType;
    // Bool to keep track of where the item is (on the map or in the inventory)
    bool onMap;

private:

    // Needed modules
    EntityManager* entityManager;

    SDL_Rect section;
};

struct InvItem
{
    Item* item;
    uint count;
};

#endif // __ITEM_H__
