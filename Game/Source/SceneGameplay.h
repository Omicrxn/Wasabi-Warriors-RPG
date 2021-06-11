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

class AssetsManager;
class BattleSystem;
class GuiManager;
class Window;
class DialogSystem;
class Textures;

class GuiButton;
class GuiIcon;

class ScreenRoaming;
class ScreenPause;
class ScreenSettings;
class ScreenBattle;
class ScreenInventory;

struct InvItem;

enum class GameState
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
    bool hasCalledCook = false; //For quest id: 1
    bool hasVisitedRestaurantQuest = false; //For quest id: 2
    bool hasSavedFirstApprentice = false; //For quest id: 4
    bool hasSavedLastApprentice = false; //For quest id: 5
    bool hasVisitedErikaTombstone = false; //For quest id: 3
    bool hasSpoken = false; //For quest id: 6
    int numKilledOfficers = 0; //For quest id: 7
    bool hasKilledOfficers = false;//For quest id: 7
    bool hasActivated = false; //For quest id: 8
    bool hasKilledBoss = false; //For quest id: 9

    // Puzzles bools
    /*bool hasPickedKey = false;*/
    bool hasFinishedPuzzle1 = false;

    // Map bools
    bool hasVisitedCemetery = false;
    bool hasVisitedHouse = false;
    bool hasVisitedMediumCity = false;
    bool hasVisitedRestaurant = false; 
    bool hasVisitedSecondRestaurant = false; 
    bool hasVisitedThirdRestaurant = false; 
    bool hasVisitedTown = false;
    bool hasVisitedBigCity = false;
    bool hasVisitedSkyScraper = false;
    bool hasVisitedSecretRoom = false;
    bool hasVisitedOsaka = false;

    //Dungeon Dialog Bools
    bool hasTriggeredDialogDungeon1 = false;
    bool hasTriggeredDialogDungeon2 = false;
    bool hasTriggeredDialogDungeon3 = false;
    bool hasTriggeredDialogDungeon4 = false;
    bool hasTriggeredDialogDungeon5 = false;
};

class SceneGameplay : public Scene
{
public:

    SceneGameplay(bool hasStartedFromContinue = false);
    virtual ~SceneGameplay();

    bool Load(Input* input, Render* render, Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityManager, DialogSystem* dialogSystem, Easing* easing, Transitions* transitions, AssetsManager* assetsManager, App* app);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

    // Loads / Saves
    // Called from app
    bool LoadState(pugi::xml_node&);
    bool SaveState(pugi::xml_node&) const;

    // Called to save / load booleans of progress
    void SaveGameProgress(pugi::xml_node&) const;
    void LoadGameProgress(pugi::xml_node&);

    // Called to save / load map entities from save_game.xml
    void SaveEntities(pugi::xml_node&) const;
    void LoadEntities(pugi::xml_node&);

    // Called to load inventory items from save_game.xml
    void LoadInventory(pugi::xml_node&);

    // Returns the current player
    Player* GetCurrentPlayer();

    // Returns the current map
    MapType GetCurrentMap();

    // Declare on mouse click event
    bool OnGuiMouseClickEvent(GuiControl* control);

    // Logic of Pause and Inventory Controls
    void OpenPause();
    void OpenInventory();
    
    // To return to the gameplay and reset the battle elements and buttons
    void ExitBattle();

    // Loads map
    void SetUpTp();

    // Adds an item to the inventory items list
    void AddItemToInvItemsList(Item* item);

    // Plays map music
    void PlayMapMusic();

    // Functions to keep track of the game progress
    GameProgress* GetGameProgress();
    void RewardXP(int xp);
    void RewardGold(int gold);

    void SetMapTransitionState();

    // Create entities from the map (For now ww are only creating static entities)
    void CreateEntities();

private:

    // Needed modules
    AssetsManager* assetsManager;
    EntityManager* entityManager;
    GuiManager* guiManager;
    Window* win;
    DialogSystem* dialogSystem;
    AudioManager* audio;
    Transitions* transitions;
    QuestManager* questManager;
    App* app;
    Textures* tex;

    // The screen for the HUD
    ScreenRoaming* screenRoaming;
    // The screen for the Pause
    ScreenPause* screenPause;
    // The screen for the settings
    ScreenSettings* screenSettings;
    // The screen for the battle
    ScreenBattle* screenBattle;
    // The screen for the inventory
    ScreenInventory* screenInventory;

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
    SDL_Texture* guiAtlasTex2;
    SDL_Texture* guiAtlasOut;
    SDL_Texture* cast1;
    SDL_Texture* enemyCast;
    SDL_Texture* indicator;
    SDL_Texture* signal;

    // Particle Animations
    Animation auraAnim;
    Animation cast1Anim;
    Animation enemyCastAnim;
    Animation indicatorAnim;
    Animation signalAnim;

    SDL_Rect backgroundRect;
   
    SDL_Rect settingsTitleRect;

    // Fonts
    Font* titleFont;
    Font* battleFont;
    Font* menuFont;
    Font* buttonFont;

    // Audio Fx for buttons
    int hoverFx, clickFx, returnFx;
    int doorOpenFx;
    int bagOpenFx;

    // Buttons to manage the battle system
    GuiButton* btnAttack;
    GuiButton* btnDefend;
    GuiButton* btnItem;
    GuiButton* btnRun;

    // Gamepad's menu focused button ID
    uint focusedButtonId;

    Notifier* notifier;

    MapType currentMap;

    // To Know if this scene was created through a load
    bool hasStartedFromContinue;

    Timer pauseTimer;

    // Invenory items list
    List<InvItem*> invItemsList;

    // Struct to keep track of the game progress
    GameProgress gameProgress;

    // Bool to keep track of the map change specific moment
    bool readyToChangeMap;

    bool onDialog = false;

    bool controller;
};

#endif // __SCENEGAMEPLAY_H__