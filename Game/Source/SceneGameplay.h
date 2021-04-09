#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"

#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "NPC.h"

class BattleSystem;
class GuiButton;
class Window;

class SceneGameplay : public Scene
{
public:

    SceneGameplay();
    virtual ~SceneGameplay();

    bool Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityManager);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

    // Load / Save
    bool LoadState(pugi::xml_node&);
    bool SaveState(pugi::xml_node&) const;

    // Returns the current player
    Player* GetCurrentPlayer();

    // Declare on mouse click event
    bool OnGuiMouseClickEvent(GuiControl* control);

private:

    // Needed modules
    EntityManager* entityManager;
    GuiManager* guiManager;
    Window* win;

    // Entitites
    Map* map;
    Player* currentPlayer;

    // Camera
    SDL_Rect camera;

    // Scene gameplay textures
    SDL_Texture* spritesheet;

    // Battle system module and bool to activate or deactivate it
    BattleSystem* battleSystem;
    bool battle;

    // Battle system textures
    SDL_Texture* backgroundTex;
    SDL_Rect backgroundRect;
    SDL_Texture* guiAtlasTex;

    // Buttons to manage the battle system
    GuiButton* btnAttack;
    GuiButton* btnDefend;
    GuiButton* btnItem;
    GuiButton* btnRun;
    GuiButton* btnNone;

    // Audio Fx for buttons
    int hoverFx, clickFx;

    // Fonts
    Font* titleFont;
    Font* buttonFont;

    // Gamepad's menu focused button
    uint controllerFocus;
};

#endif // __SCENEGAMEPLAY_H__