#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"

#include "Map.h"
#include "Player.h"
#include "Enemy.h"

class SceneGameplay : public Scene
{
public:

    SceneGameplay();
    virtual ~SceneGameplay();

    bool Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityManager);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

private:

    Map* map;
    Player* currentPlayer;

    SDL_Rect camera;

    SDL_Texture* texture;
    List<Player*> players;
    
    Enemy* enemy1;
};

#endif // __SCENEGAMEPLAY_H__