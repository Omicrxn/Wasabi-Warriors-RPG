#include "SceneEnding.h"

#include "Render.h"
#include "AssetsManager.h"
#include "Font.h"

#include "SDL/include/SDL.h"
#include "Easing.h"
#include "Audio.h"
#include "Window.h"
#include "Textures.h"
#include "Transitions.h"

#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "GuiIcon.h"
#include "GuiManager.h"


SceneEnding::SceneEnding(bool isVictory)
{
    this->name = "sceneending";
    // Put a boolean in the constructor to know if it is a victory or a defeat
    this->isVictory = isVictory;

    textFont = nullptr;

    // The pointers
    assetsManager = nullptr;
    guiManager = nullptr;
    win = nullptr;
    easing = nullptr;
    audio = nullptr;
    transitions = nullptr;

    guiAtlasTex = nullptr;

    positionX = 2000;
    hoverFx = -1;
    clickFx = -1;

    iconReturnTitle = nullptr;
}

SceneEnding::~SceneEnding()
{
}

bool SceneEnding::Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, Easing* easing, AssetsManager* assetsManager)
{
    audio->StopMusic();
    if (isVictory)
        audio->PlayMusic("Audio/Music/victory.ogg");
    else
        audio->PlayMusic("Audio/Music/defeat.ogg");

    this->assetsManager = assetsManager;
    this->guiManager = guiManager;
    this->win = win;
    this->easing = easing;
    this->audio = audio;

    // Load font
    textFont = new Font("Fonts/comic_serif.xml", tex, assetsManager);
    buttonFont = new Font("Fonts/SHOWG.xml", tex, assetsManager);

    uint width, height;
    win->GetWindowSize(width, height);

    guiAtlasTex = tex->Load("Textures/UI/ui_spritesheet.png");
    hoverFx = audio->LoadFx("Audio/Fx/bong.ogg");
    clickFx = audio->LoadFx("Audio/Fx/click.ogg");

    // Load textures
    backgroundTex = tex->Load("Textures/Scenes/bg_dungeon.jpg");

    /*iconReturnTitle = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, 1, { 2000, 570, 70, 55 });
    iconReturnTitle->SetIconProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, IconType::ICON_RETURN);*/

    /*easing->CreateSpline(&positionX, 200, 5000, SplineType::QUINT);
    easing->CreateSpline(&iconReturnTitle->bounds.x, (int)width / 2 + (int)((float)width / 4), 10000, SplineType::BACK);*/

    return false;
}

bool SceneEnding::Update(Input* input, float dt)
{
    controllerState = input->GetControllerState();

    if (!controllerState)
    {
        if (input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
            TransitionToScene(SceneType::TITLE);
    }
    else
    {
        if (input->GetControllerButton(CONTROLLER_BUTTON_A) == KEY_DOWN)
            TransitionToScene(SceneType::TITLE);
    }

    return false;
}

bool SceneEnding::Draw(Render* render)
{
    render->camera.x = 0;
    render->camera.y = 0;

    // Background
    SDL_Rect backgroundRect = { 0,0,1280,720 };
    render->DrawTexture(backgroundTex, 0, 0, &backgroundRect, 0);

    // Text
    if (isVictory)
    {
        // Victory message
        render->DrawText(textFont, "You win!", 470 + 3, 330 + 3, 100, 5, { 0,128,0,255 });
        render->DrawText(textFont, "You win!", 470, 330, 100, 5, { 0,255,0,255 });
    }
    else
    {
        // Defeat message
        render->DrawText(textFont, "You lose...", 470 + 3, 330 + 3, 100, 5, { 128,0,0,255 });
        render->DrawText(textFont, "You lose...", 470, 330, 100, 5, { 255,0,0,255 });
    }
    if (!controllerState)
    {
        render->DrawText(textFont, "Press SPACE to continue", 368 + 3, 600 + 3, 50, 5, { 128,128,128,255 });
        render->DrawText(textFont, "Press SPACE to continue", 368, 600, 50, 5, { 255,255,255,255 });
    }
    else
    {
        render->DrawText(textFont, "Press A to continue", 420 + 3, 600 + 3, 50, 5, { 128,128,128,255 });
        render->DrawText(textFont, "Press A to continue", 420, 600, 50, 5, { 255,255,255,255 });
    }

    // Background
    /*SDL_Rect rec = { 0, 0, 1280, 720 };
    SDL_Color color = { 0,0,0, 255 };
    render->DrawRectangle(rec, color);*/

    //if (isVictory)
    //{
    //    //Put victory message
    //    render->DrawText(textFont, "Congratulations! You WON!!!", positionX, 200, 50, 10, { 255, 255, 255, 255 });
    //    render->DrawText(textFont, "                         Press ---->     To return", positionX, 570, 50, 10, { 255, 255, 255, 255 });
    //}
    //else
    //{
    //    //Put defeat message
    //    render->DrawText(textFont, "That's too bad! You LOST!!!", positionX, 200, 50, 10, { 255, 255, 255, 255 });
    //    render->DrawText(textFont, "                         Press ---->     To return", positionX, 570, 50, 10, { 255, 255, 255, 255 });
    //}

    return false;
}

bool SceneEnding::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
    // Unloading textures
    tex->UnLoad(guiAtlasTex);
    guiAtlasTex = nullptr;
    tex->UnLoad(backgroundTex);
    backgroundTex = nullptr;
 
    guiManager->DestroyGuiControl(iconReturnTitle);
    iconReturnTitle = nullptr;

    // Release fonts
    RELEASE(textFont);
    textFont = nullptr;
    RELEASE(buttonFont);
    buttonFont = nullptr;

    audio->UnloadFx(clickFx);
    audio->UnloadFx(hoverFx);

    return false;
}

bool SceneEnding::OnGuiMouseClickEvent(GuiControl* control)
{
    switch (control->type)
    {
        case GuiControlType::ICON:
        {
            if (control->id == 1)
            {
                TransitionToScene(SceneType::TITLE);
            }
            break;
        }
    }
    return false;
}

