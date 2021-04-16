#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"

#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "Teleport.h"
#include "NPC.h"
#include "Notifier.h"

class BattleSystem;
class GuiManager;
class GuiButton;
class Window;
class DialogSystem;

enum GameState
{
    ROAMING,
    PAUSE,
    SETTINGS,
    BATTLE,
    INVENTORY,
    MAP
};

class SceneGameplay : public Scene
{
public:

    SceneGameplay();
    virtual ~SceneGameplay();

    bool Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityManager, DialogSystem* dialogSystem, Easing* easing);

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

    void EnableBattleButtons();

    void ToggleHUDButtons();

    void TogglePauseButtons();

    // HUD
    void UpdateHud(Input* input);
    void DrawHud(Render* render);

    // Pause
    void UpdatePause(Input* input);
    void DrawPause(Render* render);

private:

    // Needed modules
    EntityManager* entityManager;
    GuiManager* guiManager;
    Window* win;
    DialogSystem* dialogSystem;

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
    SDL_Rect pauseTitleRect;

    // Fonts
    Font* titleFont;
    Font* buttonFont;

    // Audio Fx for buttons
    int hoverFx, clickFx;

    // Buttons to manage the battle system
    GuiButton* btnAttack;
    GuiButton* btnDefend;
    GuiButton* btnItem;
    GuiButton* btnRun;
    GuiButton* btnNone;

    // Hud buttons
    GuiButton* btnPause;
    GuiButton* btnInventory;
    GuiButton* btnPhone;

    // Pause buttons
    GuiButton* btnReturn;
    GuiButton* btnSettings;
    GuiButton* btnExit;

    // Gamepad's menu focused button
    uint focusedButtonId;

    Notifier* notifier;
    MapType currentMap = MapType::CEMETERY;
};

#endif // __SCENEGAMEPLAY_H__