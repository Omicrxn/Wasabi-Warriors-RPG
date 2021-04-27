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

typedef unsigned int uint;

class Screen
{
public:
    Screen(): minIndex(0), maxIndex(0), currentScene(nullptr), win(nullptr), guiManager(nullptr), font(nullptr), isActive(false)
    {
        for (int i = 0; i < 5; ++i)
        {
            atlas[i] = nullptr;
        }
    }
    ~Screen(){}

    virtual bool Load(Scene* currentScene, Window* win, GuiManager* guiManager, Easing* easing, SDL_Texture* atlas0, Font* font, int hoverFx, int clickFx) {
        return true;
    }
    virtual bool Load(Scene* currentScene, Window* win, GuiManager* guiManager, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int hoverFx, int clickFx) {
        return true;
    }
    virtual bool Load(Scene* currentScene, Window* win, GuiManager* guiManager, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, SDL_Texture* atlas2, Font* font, int hoverFx, int clickFx) {
        return true;
    }
    virtual bool Load(Scene* currentScene, Window* win, GuiManager* guiManager, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, SDL_Texture* atlas2, SDL_Texture* atlas3, Font* font, int hoverFx, int clickFx) {
        return true;
    }
    virtual bool Load(Scene* currentScene, Window* win, GuiManager* guiManager, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, SDL_Texture* atlas2, SDL_Texture* atlas3, SDL_Texture* atlas4, Font* font, int hoverFx, int clickFx) {
        return true;
    }

    virtual bool Update(Input* input, float dt, uint& focusedButtonId) {
        return true;
    }
    virtual bool Draw(Render* render){
        return true;
    }
    virtual bool Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager) {
        return true;
    }

    void HideButtons() 
    {
        for (int i = minIndex; i <= maxIndex; ++i)
        {
            this->guiManager->controls.At(i)->data->state = GuiControlState::HIDDEN;
        }
    }
    void ShowButtons() 
    {
        for (int i = minIndex; i <= maxIndex; ++i)
        {
            this->guiManager->controls.At(i)->data->state = GuiControlState::NORMAL;
        }
    }
    void UpdateControllerSelection(int focusedButtonId)
    {
        for (int i = minIndex; i <= maxIndex; ++i)
        {
            if (i != focusedButtonId)
            {
                // SET GAMEPAD FOCUS TO FALSE
                this->guiManager->controls.At(i)->data->gamepadFocus = false;
            }
            else
            {
                // SET GAMEPAD FOCUS TO TRUE
                this->guiManager->controls.At(i)->data->gamepadFocus = true;
            }
        }
    }

    virtual bool LoadState(pugi::xml_node&){
        return true;
    }
    virtual bool SaveState(pugi::xml_node&) const {
        return true;
    }

    bool isActive;

    int minIndex; 
    int maxIndex;

    Scene* currentScene;
    GuiManager* guiManager;
    Window* win;
    Font* font;

    SDL_Texture* atlas[5];
};
#endif // __SCENE_H__