#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"

#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "Teleport.h"
#include "NPC.h"
#include "Notifier.h"
#include "Timer.h"

class BattleSystem;
class GuiManager;
class Window;
class DialogSystem;

class GuiButton;
class GuiIcon;
class Screen;

enum GameState
{
    ROAMING,
    INVENTORY,
    PHONE,
    PAUSE,
    SETTINGS,
    EXIT,
    BATTLE
};

class SceneGameplay : public Scene
{
public:

    SceneGameplay(bool hasStartedFromContinue = false);
    virtual ~SceneGameplay();

    bool Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityManager, DialogSystem* dialogSystem, Easing* easing, Transitions* transitions, App* app);

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

    void SetUpTp();

private:

    void EnableBattleButtons();

private:

    // Needed modules
    EntityManager* entityManager;
    GuiManager* guiManager;
    Window* win;
    DialogSystem* dialogSystem;
    AudioManager* audio;
    Transitions* transitions;

    // The screen for the HUD
    Screen* screenRoaming;
    // The screen for the Pause
    Screen* screenPause;

    // Scene gameplay textures
    SDL_Texture* spritesheet;

    // Camera
    SDL_Rect camera;

    // Entities
    Map* map;
    Player* currentPlayer;

    // Battle system module and bool to activate or deactivate it
    BattleSystem* battleSystem;
    bool battle;

    // Enum to know at what state the game encounters ( in order to show HUD, INVENTORY, or MAP)
    GameState currentState;

    // Battle system textures
    SDL_Texture* backgroundTex;
    SDL_Texture* titlesTex;
    SDL_Texture* guiAtlasTex;

    SDL_Rect backgroundRect;
    SDL_Rect pauseBackgroundRect;
   
    SDL_Rect settingsTitleRect;

    // Fonts
    Font* titleFont;
    Font* buttonFont;

    // Audio Fx for buttons
    int hoverFx, clickFx, returnFx;

    // Buttons to manage the battle system
    GuiButton* btnAttack;
    GuiButton* btnDefend;
    GuiButton* btnItem;
    GuiButton* btnRun;

    // Gamepad's menu focused button
    uint focusedButtonId;

    Notifier* notifier;
    MapType currentMap = MapType::CEMETERY;

    // To Know if this scene was created through a load
    bool hasStartedFromContinue;

    Timer pauseTimer;
};

#endif // __SCENEGAMEPLAY_H__