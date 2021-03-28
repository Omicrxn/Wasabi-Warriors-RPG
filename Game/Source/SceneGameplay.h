#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"

#include "Map.h"
#include "Player.h"

class SceneGameplay : public Scene
{
public:

    SceneGameplay();
    virtual ~SceneGameplay();

    bool Load(Textures* tex, Window* win, GuiManager* guiManager, EntityManager* entityManager);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload(Textures* tex);

private:

    Map* map;
    Player* currentPlayer;

    SDL_Rect camera;

    SDL_Texture* texture;
    List<Player*> players;
};

#endif // __SCENEGAMEPLAY_H__