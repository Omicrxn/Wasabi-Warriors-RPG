#pragma once
#include "Defs.h"
#include "SString.h"
#include "Point.h"
#include "Timer.h"

class Item;
class Activator;
class Entity;
class SceneGameplay;
class Teleport;

enum class MapType
{
    NONE = -1,
    CEMETERY = 0,
    HOUSE,
    KANAGAWA,
    RESTAURANT,
    TOWN,
    DOTONBORI,
    SKYSCRAPER,
    SECRET_ROOM,
    OSAKA,
    SECOND_RESTAURANT,
    THIRD_RESTAURANT,
    SECOND_SECRET_ROOM,
    THIRD_SECRET_ROOM
};

class Notifier
{
public:

    static Notifier* GetInstance();
    ~Notifier() {RELEASE(instance)}

    // Battle management
    void NotifyBattle()
    {
        battle = !battle;
    }
    bool GetBattle() { return battle; }
    SString GetEnemy() { return enemy; }
    void SetEnemy(SString enemy)
    {
        this->enemy = enemy;
    }
    void SetBattleState(bool isBattleActive)
    {
        this->isBattleActive = isBattleActive;
    }
    bool OnBattle()
    {
        return isBattleActive;
    }

    // Teleport functions
    void NotifyMapChange(MapType maptype)
    {
        nextMap = maptype;
        requestMapChange = true;
    }
    bool OnMapChange() { return requestMapChange; }
    MapType ChangeMap()
    { 
        requestMapChange = false;
        return nextMap;
    }
    MapType GetNextMap() { return nextMap; }

    void SetCurrentMap(MapType mapType) { currentMap = mapType; }
    MapType GetCurrentMap() {  return currentMap; }

    void NotifyPositionChange()
    {
        requestPositionChange = !requestPositionChange;
    }
    void SetNextPosition(iPoint nextPosition)
    {
        this->nextPosition = nextPosition;
    }
    bool OnPositionChange() { return requestPositionChange; }
    iPoint GetNextPosition() { return nextPosition; }

    // Dialog functions
    void NotifyDialog(int dialogIndex)
    {
        requestDialog = true;
        this->dialogIndex = dialogIndex;
    }
    bool OnDialog()
    {
        return requestDialog;
    }
    void SetDialogMode(bool dialogMode)
    {
        this->requestDialog = dialogMode;
    }
    int GetDialogIndex()
    {
        return dialogIndex;
    }

    // Items management
    void SetItemAddition(bool itemAddition)
    {
        this->itemAddition = itemAddition;
    }
    bool GetItemAddition()
    {
        return itemAddition;
    }
    void SetItem(Item* item)
    {
        this->item = item;
    }
    Item* GetItem()
    {
        return item;
    }

    // Activators management
    void SetActivatorMode(bool mode)
    {
        requestActivator = mode;
    }
    bool OnActivator()
    {
        return requestActivator;
    }
    void SetActivator(Activator* activator)
    {
        this->activator = activator;
    }
    Activator* GetActivator()
    {
        return activator;
    }

    // Interaction notifier
    void NotifyInteraction()
    {
        interactionNotifier = !interactionNotifier;
        if(!interactionNotifier)
            this->interactingEntity = nullptr;
    }
    void SetInteractingEntity(Entity* interactingEntity)
    {
        this->interactingEntity = interactingEntity;
    }
    bool GetInteractionNotifier()
    {
        return interactionNotifier;
    }
    Entity* GetInteractingEntity()
    {
        return interactingEntity;
    }

    // Mobile Notification
    void SendMobileNotification(const char* text)
    {
        notificationTimer.Start();
        newMobileNotification = true;
        this->textStr = text;
    }

    void CloseMobileNotification()
    {
        newMobileNotification = false;
    }

    const char* GetMobileNotification()
    {
        if (newMobileNotification)
            return this->textStr;
        else
            return nullptr;
    }

    Timer& GetNotificationTimer()
    {
        return this->notificationTimer;
    }

    void SetSceneGameplay(SceneGameplay* sceneGameplay)
    {
        this->sceneGameplay = sceneGameplay;
    }

    SceneGameplay* GetSceneGameplay()
    {
        return sceneGameplay;
    }

    // Teleports management
    void SetTeleportMode(bool mode)
    {
        requestTeleport = mode;
    }
    bool OnTeleport()
    {
        return requestTeleport;
    }
    void SetTeleport(Teleport* activator)
    {
        this->teleport = activator;
    }
    Teleport* GetTeleport()
    {
        return teleport;
    }

private:

    Notifier() : battle(false) {}
    bool battle = false;
    bool isBattleActive = false;

    int dialogIndex = -1;

    MapType nextMap = MapType::CEMETERY;
    bool requestMapChange = false;

    MapType currentMap;

    iPoint nextPosition = {};
    bool requestPositionChange = false;

    // Singleton instance
    static Notifier* instance;
    bool requestDialog = false;
    SString enemy;

    // Items
    bool itemAddition = false;
    Item* item = nullptr;

    // Activators
    bool requestActivator = false;
    Activator* activator = nullptr;

    // Interaction notifier
    bool interactionNotifier = false;
    Entity* interactingEntity = nullptr;
    Timer notificationTimer;

    // Mobile notification
    bool newMobileNotification = false;
    const char* textStr = nullptr;

    // An instance of the gameplay scene
    // in order to have it available from different places
    SceneGameplay* sceneGameplay = sceneGameplay;

    // Teleports
    bool requestTeleport = false;
    Teleport* teleport = nullptr;
};