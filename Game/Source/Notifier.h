#pragma once
#include "Defs.h"

#include "SString.h"

enum class MapType
{
    NONE = -1,
    CEMETERY = 0,
    HOUSE,
    MEDIUM_CITY,
    RESTAURANT,
    TOWN

};
class Notifier
{
public:
    static Notifier* GetInstance();
    ~Notifier() {RELEASE(instance)}
    void NotifyBattle() { battle = !battle; }
    bool GetBattle() { return battle; }

    void NotifyMapChange(MapType maptype)
    {
        nextMap = maptype;
        requestMapChange = true;
    }


    bool OnMapChange() { return requestMapChange; }
    MapType ChangeMap() { 
        requestMapChange = false;
        return nextMap;
    }
    MapType GetNextMap() { return nextMap; }

    void NotifyDialog()
    {
            requestDialog = true;
    }

    bool OnDialog() {

            return requestDialog;
    }

    void SetDialogMode(bool dialogMode)
    {
        this->requestDialog = dialogMode;
    }

    SString GetEnemy()
    {
        return enemy;
    }

    void SetEnemy(SString enemy)
    {
        this->enemy = enemy;
    }

private:
    Notifier() : battle(false) {}
    bool battle = false;
    MapType nextMap = MapType::CEMETERY;
    bool requestMapChange = false;
    // Singleton instance
    static Notifier* instance;
    bool requestDialog = false;
    SString enemy;
};