#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Being.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "EntityManager.h"

#include "Point.h"
#include "SString.h"

#include "SDL/include/SDL.h"

class SceneManager;

class Player: public Being
{
public:

    Player(SString name,Textures* tex, Collisions* collisions, EntityManager* entityManager,EntityType type, EntitySubtype subtype, iPoint position);
    ~Player();

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    void SetUpTexture();

    SDL_Rect GetBounds();

    // To set the player name
    void SetName(SString name);

    // To setup the player class type
    bool SetUpClass(SString name);

private:

    void Walk(iPoint direction, float dt);

public:

    Animations currentAnim;

    bool stopPlayer = false;
    bool transitioning = false;
    bool isGod = false;
};

#endif // __PLAYER_H__
