#ifndef __ITEM_H__
#define __ITEM_H__

#include "Interactive.h"

#include "Point.h"
#include "Textures.h"
#include "SString.h"
#include "Stats.h"

class EntityManager;
class AudioManager;

class Item : public Interactive
{
public:

    Item() {}
    Item(SString name, Textures* tex, AudioManager* audio, Collisions* collisions, EntityManager* entityManager, EntityType type, EntitySubtype subtype, iPoint position);
    virtual ~Item();
    bool Update(float dt) { return true; };
    bool Draw(Render* render);
    void OnCollision(Collider* collider) override;
    void Interact() {}
    void ConvertToInvItem();
    Stats Interact(Stats stats);
    bool SetUpClass(SString name);
    SDL_Rect GetSection();
public:

    Stats stats;
    bool usable = true;

    // Bool to keep track of where the item is (on the map or in the inventory)
    bool onMap;

    int consumeFx = -1;
    int pickUpFx = -1;
private:

    int width, height;

    // Needed modules
    EntityManager* entityManager;
    AudioManager* audio;

    SDL_Rect section;

    bool hasInteracted = false;
};

struct InvItem
{
    Item* item;
    uint count;
};

#endif // __ITEM_H__
