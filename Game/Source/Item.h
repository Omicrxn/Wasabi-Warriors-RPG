#ifndef __ITEM_H__
#define __ITEM_H__

#include "Interactive.h"

#include "Point.h"
#include "SString.h"
#include "Stats.h"

class Item : public Interactive
{
public:

    Item();
    virtual ~Item();
    void Interact() {}
    Stats Interact(Stats stats);
    bool SetUpClass(SString name);
public:
    Stats stats;
    bool usable = true;
    SString classType;
};

#endif // __ITEM_H__
