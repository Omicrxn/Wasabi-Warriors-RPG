#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"

#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "NPC.h"

class BattleSystem;
class GuiManager;
class GuiButton;
class Window;
class DialogSystem;
class Notifier;

class SceneGameplay : public Scene
{
public:

    SceneGameplay();
    virtual ~SceneGameplay();

    bool Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityManager, DialogSystem* dialogSystem);

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

    // To return to the gameplay and reset the battle elements and buttons
    void ExitBattle();

private:

    // Needed modules
    EntityManager* entityManager;
    GuiManager* guiManager;
    Window* win;
    DialogSystem* dialogSystem;

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

    Notifier* notifier = nullptr;
};

#endif // __SCENEGAMEPLAY_H__