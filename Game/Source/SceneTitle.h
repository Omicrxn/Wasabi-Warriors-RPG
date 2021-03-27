#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Scene.h"
#include "Animation.h"

#include "GuiButton.h"

class Font;

class SceneTitle : public Scene
{
public:

    SceneTitle();
    virtual ~SceneTitle();

    bool Load(Textures* tex, GuiManager* guiman);

    bool Update(Input* input, float dt);

    bool Draw(Render* render);

    bool Unload(Textures* tex);

    // Declare on mouse click event
    bool OnGuiMouseClickEvent(GuiControl* control);

private:

    GuiButton* btnStart;
    GuiButton* btnExit;

private:
    SDL_Texture* backgroundTex;

private:
    Animation backgroundAnim;

private:
    Font* font;
};

#endif // __SCENETITLE_H__
