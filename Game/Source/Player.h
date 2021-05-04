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

    Player(Textures* tex, Collisions* collisions, EntityManager* entityManager, EntitySubtype subtype);
    ~Player();

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    void SetUpTexture(SString texPath);

    SDL_Rect GetBounds();

    // To set the player name
    void SetName(SString name);

    // To setup the player class type
    bool SetUpClass(SString name);

private:

    void Walk(iPoint direction, float dt);

public:

    Animations currentAnim;

    int width, height;
    bool stopPlayer = false;
    bool transitioning = false;
    bool isGod = false;
};

#endif // __PLAYER_H__
