#pragma once
#include "Defs.h"
class Notifier
{
public:
    static Notifier* GetInstance();
    ~Notifier() {RELEASE(instance)}
    void NotifyBattle() { battle = !battle; }
    bool GetBattle() { return battle; }

private:
    Notifier() : battle(false) {}
    bool battle = false;

    // Singleton instance
    static Notifier* instance;
};