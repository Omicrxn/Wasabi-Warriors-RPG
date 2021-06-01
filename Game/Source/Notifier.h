#pragma once
#include "Defs.h"
#include "SString.h"
#include "Point.h"

class Item;
class Activator;
class Entity;

enum class MapType
{
    NONE = -1,
    CEMETERY = 0,
    HOUSE,
    MEDIUM_CITY,
    RESTAURANT,
    TOWN,
    BIG_CITY,
    SKYSCRAPER,
    SECRET_ROOM
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
    void NotifyActivator()
    {
        requestActivator = !requestActivator;
    }
    void SetActivator(Activator* activator)
    {
        this->activator = activator;
    }
    Activator* GetActivator()
    {
        return activator;
    }
    bool OnActivator()
    {
        return requestActivator;
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

private:

    Notifier() : battle(false) {}
    bool battle = false;
    bool isBattleActive = false;

    int dialogIndex = -1;

    MapType nextMap = MapType::CEMETERY;
    bool requestMapChange = false;

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
};