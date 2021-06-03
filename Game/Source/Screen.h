#ifndef __SCREEN_H__
#define __SCREEN_H__
#include "SDL/include/SDL.h"
#include "PugiXml/src/pugixml.hpp"

#include "GuiManager.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiIcon.h"
#include "GuiSlider.h"

class Render;
class Input;
class Textures;
class Window;
class AudioManager;
class GuiManager;
class Easing;
class Scene;
class Font;

class GuiButton;
class GuiSlider;
class GuiCheckBox;
class GuiIcon;
class Screen;
class BattleSystem;
class SceneGameplay;

typedef unsigned int uint;

class Screen
{
public:
    Screen(): minIndex(0), maxIndex(0), currentScene(nullptr), win(nullptr), guiManager(nullptr),entityManager(nullptr), font(nullptr), audio(nullptr), easing(nullptr), battleSystem(nullptr), isActive(false)
    {
        for (int i = 0; i < 5; ++i)
        {
            atlas[i] = nullptr;
        }
    }
    ~Screen() {}

    // Many overloads if we need more textures in the screen 
    // Screen Pause
    virtual bool Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, EntityManager* entityManager, AudioManager* audio, Easing* easing, SDL_Texture* atlas0, Font* font, int hoverFx, int clickFx)
    {
        return true;
    }
    // Screen Inventory
    virtual bool Load(int minIndex, int maxIndex, Scene* currentScene, BattleSystem* battleSystem, Window* win, GuiManager* guiManager, EntityManager* entityManager, AudioManager* audio, Easing* easing, SDL_Texture* atlas0, Font* font, int hoverFx, int clickFx)
    {
        return true;
    }
    // Screen Battle
    virtual bool Load(int minIndex, int maxIndex, Scene* currentScene, BattleSystem* battleSystem, Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityManager, SDL_Texture* atlas0, SDL_Texture* atlas1, SDL_Texture* atlas2, Font* font0, Font* font1, Font* font2, Font* font3, int hoverFx, int clickFx, int returnFx)
    {
        return true;
    }
    // Screen Main Menu
    virtual bool Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int hoverFx, int clickFx)
    {
        return true;
    }
    // Screen Credits
    virtual bool Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, SDL_Texture* atlas0, Font* font, int hoverFx, int returnFx)
    {
        return true;
    }
    // Screen Settings
    virtual bool Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, SDL_Texture* atlas0, Font* buttonFont, int hoverFx, int clickFx, int returnFx)
    {
        return true;
    }
    // Screen Roaming
    virtual bool Load(SceneGameplay* gameplayScene, GuiManager* guiManager, EntityManager* entityManager, AudioManager* audio, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int clickFx)
    {
        return true;
    }

    virtual bool Update(Input* input, float dt, uint& focusedButtonId) 
    {
        return true;
    }
    virtual bool Update(Input* input, float dt)
    {
        return true;
    }

    virtual bool Draw(Render* render)
    {
        return true;
    }

    virtual bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
    {
        return true;
    }

    void HideButtons()
    {
        for (int i = minIndex; i <= maxIndex; ++i)
        {
            /*this->guiManager->controls.At(i)->data->state = GuiControlState::HIDDEN;*/
            this->guiManager->FindById(i)->state = GuiControlState::HIDDEN;
        }
    }
    void ShowButtons()
    {
        for (int i = minIndex; i <= maxIndex; ++i)
        {
            /*this->guiManager->controls.At(i)->data->state = GuiControlState::NORMAL;*/
            this->guiManager->FindById(i)->state = GuiControlState::NORMAL;
        }
    }

    virtual void Enable()
    {
        this->isActive = true;
        ShowButtons();
    }

    virtual void Disable()
    {
        this->isActive = false;
        HideButtons();
    }

    void UpdateControllerSelection(int focusedButtonId)
    {
        for (int i = minIndex; i <= maxIndex; ++i)
        {
            if (i != focusedButtonId)
            {
                // SET GAMEPAD FOCUS TO FALSE
                /* this->guiManager->controls.At(i)->data->gamepadFocus = false;*/
                this->guiManager->FindById(i)->gamepadFocus = false;
            }
            else
            {
                // SET GAMEPAD FOCUS TO TRUE
                /* this->guiManager->controls.At(i)->data->gamepadFocus = true;*/
                this->guiManager->FindById(i)->gamepadFocus = true;
            }
        }
    }

    virtual bool LoadState(pugi::xml_node&)
    {
        return true;
    }
    virtual bool SaveState(pugi::xml_node&) const
    {
        return true;
    }

    bool isActive;

    int minIndex; 
    int maxIndex;

    Scene* currentScene;
    GuiManager* guiManager;
    Window* win;
    Font* font;
    EntityManager* entityManager;
    Easing* easing;
    AudioManager* audio;
    BattleSystem* battleSystem;

    SDL_Texture* atlas[5];
};
#endif // __SCREEN_H__