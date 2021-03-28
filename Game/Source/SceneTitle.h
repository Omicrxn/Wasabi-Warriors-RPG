#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Scene.h"
#include "Animation.h"
#include "Point.h"

class GuiButton;
class Font;
class Window;

#include "SDL/include/SDL.h"

class SceneTitle : public Scene
{
public:

    SceneTitle();
    virtual ~SceneTitle();

    bool Load(Textures* tex, Window* win, GuiManager* guiManager);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload(Textures* tex, GuiManager* guiManager);

    // Declare on mouse click event
    bool OnGuiMouseClickEvent(GuiControl* control);

private:

    GuiButton* btnStart;
    GuiButton* btnContinue;
    GuiButton* btnCredits;
    GuiButton* btnOptions;
    GuiButton* btnExit;

    SDL_Texture* backgroundTex;
    SDL_Rect backgroundRect;

    SDL_Texture* guiAtlasTex;

    // Mouse Cursor attributes
    SDL_Rect mouseRect[2];
    iPoint mousePos;
    bool clicking;

    Animation backgroundAnim;

    Font* titleFont;
    Font* buttonFont;

    Window* win;
};

#endif // __SCENETITLE_H__
