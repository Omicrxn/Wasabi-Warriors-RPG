#ifndef __SCENEBATTLE_H__
#define __SCENEBATTLE_H__

#include "Screen.h"

#include "Animation.h"

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

    bool Load(int minIndex, int maxIndex, Scene* currentScene, BattleSystem* battleSystem, Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityManager, SDL_Texture* charactersSpritesheet, SDL_Texture* uiSpritesheet, Font* titleFont, Font* buttonFont, Font* menuFont, int hoverFx, int clickFx, int returnFx);

    bool Update(Input* input, float dt, uint& focusedButtonId);

    bool Draw(Render* render);

    bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager);

    // To return to the gameplay and reset the battle elements and buttons
    void ExitBattle();

    void ResetOneTimeAnimations();

    void EnableBattleButtons();

private:

    // Needed modules
    EntityManager* entityManager;
    GuiManager* guiManager;
    Window* win;
    AudioManager* audio;

    // Pointer to scene gameplay to get its info.
    SceneGameplay* sceneGameplay;

    // Scene gameplay textures
    SDL_Texture* charactersSpritesheet;

    // Battle system module
    BattleSystem* battleSystem;

    // Battle system textures
    SDL_Texture* backgroundTex;
    SDL_Texture* guiAtlasTex;
    SDL_Texture* aura;
    SDL_Texture* cast1;
    SDL_Texture* enemyCast;
    SDL_Texture* indicator;

    // Battle screen animations
    Animation auraAnim;
    Animation cast1Anim;
    Animation enemyCastAnim;
    Animation indicatorAnim;

    SDL_Rect backgroundRect;

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
};

#endif // __SCENEBATTLE_H__