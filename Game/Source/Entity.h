#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Collisions.h"
#include "Notifier.h"
#include "Animation.h"
#include "Textures.h"

struct SDL_Texture;
struct Render;
struct Input;
//class Textures;
class EntityManager;
class AudioManager;

enum class EntityType
{
    UNKNOWN = -1,
    PLAYER,
    ENEMY,
    ITEM,
    MAP,
    NPC,
    TELEPORT,
    ACTIVATOR,
    SECRET_WALL,
    LEVER,
    STATIC
};
 
enum class EntitySubtype
{
    UNKNOWN = -1,
    PLAYER_KENZO,
    PLAYER_EIKEN,
    ENEMY_HENCHMAN,
    ENEMY_BRUISER,
    ENEMY_BOSS,
    ITEM_POTION,
    ITEM_BPOTION,
    ITEM_MPOTION,
    ITEM_WASABI,
    ITEM_PAN,
    ITEM_FORK,
    ATTACK,
    DEFENSE,
    PLAYER_REI,
    PLAYER_DOG
};

struct TileSetEntity
{
    //Functions to help loading data in xml-------------------------------------
    //Get the rect info of an id of tileset
    SDL_Rect TileSetEntity::GetTileRect(int id) const
    {
        SDL_Rect rect;
        rect.w = tileWidth;
        rect.h = tileHeight;
        rect.x = margin + ((rect.w + spacing) * (id % columns));
        rect.y = margin + ((rect.h + spacing) * (id / columns));
        return rect;
    }

    std::string name;
    uint tileWidth = 0;
    uint tileHeight = 0;
    uint spacing = 0;
    uint margin = 0;
    uint tileCount = 0;
    uint columns = 0;
    std::string imagePath;
    SDL_Texture* texture = nullptr;
    uint width = 0;
    uint height = 0;
};

enum class EntityState
{
	IDLE = 0,
	WALKING,

	UNKNOWN
};

struct EntityAnim
{
    uint id = 0;
    uint numFrames = 0;
    SDL_Rect* frames = nullptr;
    EntityState animType;

    //Return how many frames are in one animation
    uint EntityAnim::FrameCount(pugi::xml_node& n)
    {
        numFrames = 0;
        pugi::xml_node node = n;
        for (; node != NULL; node = node.next_sibling("frame"))
        {
            numFrames++;
        }

        return numFrames;
    }
};

struct EntityInfo
{
    TileSetEntity tileset;
    EntityAnim* animations = nullptr;
    uint numAnimations = 0;
};

struct ColliderInfo
{
    Collider* collider = nullptr;
    iPoint offset;
    int width = 0;
    int height = 0;
    Collider::Type type;
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
    void SetId(uint32 newId)
    {
        id = newId;
    }

    // Set ivot for the sprite sorting
    void Entity::SetPivot(const int& x, const int& y)
    {
        pivot.Create(x, y);
    }

    bool CleanUp()
    {
        bool ret = false;

        if (data.tileset.texture != nullptr)
            ret = tex->UnLoad(data.tileset.texture);
        if (/*collider.collider*/collider != nullptr)
            /*collider.collider*/collider->pendingToDelete = true;
        currentAnim = nullptr;

        return ret;
    }

public:

    EntityType type;
    EntitySubtype subtype;
    EntityInfo data;
    /*ColliderInfo collider;*/
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
    iPoint pivot = { 0,0 };
    iPoint size = { 0,0 };
    Animation* currentAnim = nullptr;

protected:

    bool active = true;
    Notifier* notifier;
    Render* render;
    Textures* tex = nullptr;
    Input* input = nullptr;
    EntityManager* entityManager = nullptr;
    AudioManager* audio = nullptr;

public:

    SDL_Rect animRec;
};

#endif // __ENTITY_H__