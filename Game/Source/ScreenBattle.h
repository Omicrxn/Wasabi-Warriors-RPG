#ifndef __SCENEBATTLE_H__
#define __SCENEBATTLE_H__

#include "Screen.h"

#include "Animation.h"
#include "Timer.h"

class BattleSystem;
class SceneGameplay;

//class Window;

//class GuiManager;
//class GuiButton;

class ScreenBattle : public Screen
{
public:

    ScreenBattle();
    virtual ~ScreenBattle();

    bool Load(int minIndex, int maxIndex, Scene* currentScene, BattleSystem* battleSystem, Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityManager, SDL_Texture* charactersSpritesheet, SDL_Texture* guiAtlasTex, SDL_Texture* guiAtlasTex2, Font* titleFont, Font* buttonFont, Font* menuFont, Font* menuFont2, int hoverFx, int clickFx, int returnFx);

    bool Update(Input* input, float dt, uint& focusedButtonId);

    bool Draw(Render* render);

    bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

    void ResetOneTimeAnimations();

    void EnableBattleButtons();
    void DisableBattleButtons();

    BattleSystem* GetBattleSystem();

    bool HoverUpdate(Input* input);

private:

    // Pointer to scene gameplay to get its info.
    SceneGameplay* sceneGameplay;

    // Scene gameplay textures
    SDL_Texture* charactersSpritesheet;

    // Battle system textures
    SDL_Texture* backgroundTex;
    SDL_Texture* guiAtlasTex;
    SDL_Texture* guiAtlasTex2;
    SDL_Texture* optionsBackgroundTex;
    SDL_Texture* cast1;
    SDL_Texture* enemyCast;
    SDL_Texture* indicator;
    SDL_Texture* playerDefense;
    SDL_Texture* enemyDefense;

    // Battle screen animations
    Animation cast1Anim;
    Animation enemyCastAnim;
    Animation indicatorAnim;
    Animation playerDefenseAnim;
    Animation enemyDefenseAnim;

    SDL_Rect backgroundRect;
    SDL_Rect optionsBackgroundRect;
    SDL_Rect backRamen1;
    SDL_Rect backRamen2;
    SDL_Rect frontRamen1;
    SDL_Rect frontRamen2;
    SDL_Rect leftSticks;
    SDL_Rect rightSticks;

    // Fonts
    Font* titleFont;
    Font* buttonFont;
    Font* menuFont;
    Font* menuFont2;

    // Audio Fx for buttons
    int hoverFx, clickFx, returnFx;

    // Buttons to manage the battle system
    GuiButton* btnAttack;
    GuiButton* btnDefend;
    GuiButton* btnItem;
    GuiButton* btnRun;

    // Gamepad's menu focused button
    uint focusedButtonId;

    Timer timer;

    // Variables to manage attack/defense menus
    iPoint hover;
    bool pressed = false;
};

#endif // __SCENEBATTLE_H__