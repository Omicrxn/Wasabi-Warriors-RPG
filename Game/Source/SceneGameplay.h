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

struct InvItem;

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

struct GameProgress
{
    int xp = 0;
    int gold = 0;

    // Quests bools (for the quest manager to keep track of the quests state)
    bool hasSpoken = false;
    int numKilledOfficers = 0;
    bool hasKilledOfficers = false;
    bool hasActivated = false;
};

class SceneGameplay : public Scene
{
public:

    SceneGameplay(bool hasStartedFromContinue = false);
    virtual ~SceneGameplay();

    bool Load(Input* input, Render* render, Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityManager, DialogSystem* dialogSystem, Easing* easing, Transitions* transitions, App* app);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

    // Load / Save
    bool LoadState(pugi::xml_node&);
    bool SaveState(pugi::xml_node&) const;

    void SaveGameProgress(pugi::xml_node&) const;
    void LoadGameProgress(pugi::xml_node&);

    // Returns the current player
    Player* GetCurrentPlayer();

    // Returns the current map
    MapType GetCurrentMap();

    // Declare on mouse click event
    bool OnGuiMouseClickEvent(GuiControl* control);

    // To return to the gameplay and reset the battle elements and buttons
    void ExitBattle();

    void SetUpTp();

    // Adds an item to the inventory items list
    void AddItemToInvItemsList(Item* item);

    void PlayMapMusic();

    // Functions to keep track of the game progress
    GameProgress* GetGameProgress();
    void RewardXP(int xp);
    void RewardGold(int gold);

    void CollisionHandler();
private:

    // Needed modules
    EntityManager* entityManager;
    GuiManager* guiManager;
    Window* win;
    DialogSystem* dialogSystem;
    AudioManager* audio;
    Transitions* transitions;
    QuestManager* questManager;

    // The screen for the HUD
    Screen* screenRoaming;
    // The screen for the Pause
    Screen* screenPause;
    // The screen for the settings
    Screen* screenSettings;
    // The screen for the battle
    Screen* screenBattle;
    // The screen for the inventory
    Screen* screenInventory;

    // Scene gameplay textures
    SDL_Texture* charactersSpritesheet;

    // Camera
    SDL_Rect camera;

    // Entities
    Map* map;
    Player* currentPlayer;

    // Battle system module
    BattleSystem* battleSystem;

    // Enum to know at what state the game encounters ( in order to show HUD, INVENTORY, or MAP)
    GameState currentState;

    // Battle system textures
    SDL_Texture* backgroundTex;
    SDL_Texture* titlesTex;
    SDL_Texture* guiAtlasTex;
    SDL_Texture* aura;
    SDL_Texture* cast1;
    SDL_Texture* enemyCast;
    SDL_Texture* indicator;
    SDL_Texture* signal;

    Animation auraAnim;
    Animation cast1Anim;
    Animation enemyCastAnim;
    Animation indicatorAnim;
    Animation signalAnim;

    SDL_Rect backgroundRect;
    SDL_Rect pauseBackgroundRect;
   
    SDL_Rect settingsTitleRect;

    // Fonts
    Font* titleFont;
    Font* buttonFont;
    Font* menuFont;

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

    // Invenory items list
    List<InvItem*> invItemsList;

    // Struct to keep track of the game progress
    GameProgress gameProgress;
};

#endif // __SCENEGAMEPLAY_H__