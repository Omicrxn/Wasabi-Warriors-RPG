#include "SceneTitle.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "GuiManager.h"
#include "Window.h"

#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

#include "Font.h"

#include "SDL/include/SDL.h"

#define TITLE_FADE_SPEED 0.05f

SceneTitle::SceneTitle()
{
    type = SceneType::TITLE;
    
    guiManager = nullptr;
    win = nullptr;

    backgroundTex = nullptr;
    backgroundRect = { 0, 0, 0, 0 };

    guiAtlasTex = nullptr;

    settingsBackgroundRect = { 1228, 295, 300, 200 };

    titleFont = nullptr;
    buttonFont = nullptr;

    hoverFx = -1;
    clickFx = -1;

    btnStart = nullptr;
    btnContinue = nullptr;
    btnOptions = nullptr;
    btnCredits = nullptr;
    btnExit = nullptr;

    btnFullScreen = nullptr;
    btnVsync = nullptr;
    btnMusicVolume = nullptr;
    btnFXVolume = nullptr;
    btnReturnTitle = nullptr;

    menuCurrentSelection = MenuSelection::NONE;
    //settingsCurrentSelection = SettingsSelection::NONE;

    settingsScene = false;
    
    focusedButtonId = 0;
}

SceneTitle::~SceneTitle()
{
}

bool SceneTitle::Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager)
{
    this->guiManager = guiManager;
    this->win = win;

    uint width, height;
    win->GetWindowSize(width, height);

    backgroundTex = tex->Load("Assets/Textures/Scenes/main_menu.png");
    backgroundRect = { 0, 0, 1280, 720 };

    guiAtlasTex = tex->Load("Assets/Textures/UI/Elements/ui_spritesheet.png");

    titleFont = new Font("Assets/Fonts/shojumaru.xml", tex);
    buttonFont = new Font("Assets/Fonts/showg.xml", tex);

    hoverFx = audio->LoadFx("Assets/Audio/Fx/bong.ogg");
    clickFx = audio->LoadFx("Assets/Audio/Fx/click.ogg");

    /* MENU BUTTONS */
    btnStart = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, { (int)width / 2 - (int)((float)width / 12), 200, 190, 49 }, "START");
    btnStart->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, ButtonColour::WHITE);

    btnContinue = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, { (int)width / 2 - (int)((float)width / 12), 300, 190, 49 }, "CONTINUE");
    btnContinue->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, ButtonColour::WHITE);

    btnOptions = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, { (int)width / 2 - (int)((float)width / 12), 400, 190, 49 }, "OPTIONS");
    btnOptions->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, ButtonColour::WHITE);

    btnCredits = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, { (int)width / 2 - (int)((float)width / 12), 500, 190, 49 }, "CREDITS");
    btnCredits->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, ButtonColour::WHITE);

    btnExit = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, { (int)width / 2 - (int)((float)width / 12), 600, 190, 49 }, "EXIT");
    btnExit->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, ButtonColour::WHITE);

    /* SETTINGS BUTTONS */
    btnFullScreen = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, { (int)width / 2 - (int)((float)width / 12), 200, 190, 49 }, "FULLSCREEN");
    btnFullScreen->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, ButtonColour::WHITE);

    btnVsync = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, { (int)width / 2 - (int)((float)width / 12), 300, 190, 49 }, "VSYNC");
    btnVsync->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, ButtonColour::WHITE);

    btnMusicVolume = (GuiSlider*)guiManager->CreateGuiControl(GuiControlType::SLIDER, 8, { (int)width / 2 - (int)((float)width / 12), 400, 190, 49 }, "MUSIC VOLUME");
    btnMusicVolume->SetSliderProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, ButtonColour::WHITE);

    btnFXVolume = (GuiSlider*)guiManager->CreateGuiControl(GuiControlType::SLIDER, 9, { (int)width / 2 - (int)((float)width / 12), 500, 190, 49 }, "FX VOLUME");
    btnFXVolume->SetSliderProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, ButtonColour::WHITE);

    btnReturnTitle = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, { (int)width / 2 - (int)((float)width / 12), 600, 190, 49 }, "RETURN");
    btnReturnTitle->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, ButtonColour::WHITE);

    HideSettingsButtons();

    audio->PlayMusic("Assets/Audio/Music/menu.ogg");

    return true;
}

bool SceneTitle::Update(Input* input, float dt)
{
    // Debug purposes
    if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) TransitionToScene(SceneType::GAMEPLAY);
    // ---

    if (input->GetControllerState())
    {
        if (((input->GetControllerButton(CONTROLLER_BUTTON_UP) == KEY_DOWN) == KEY_DOWN) && focusedButtonId >= 1)
            --focusedButtonId;
        else if (((input->GetControllerButton(CONTROLLER_BUTTON_DOWN) == KEY_DOWN) == KEY_DOWN) && focusedButtonId <= 3)
            ++focusedButtonId;

        if (settingsScene == false)
        {
            for (int i = 0; i < 5; ++i)
            {
                if (i != focusedButtonId)
                {
                    // SET GAMEPAD FOCUS TO FALSE
                    guiManager->controls.At(i)->data->gamepadFocus = false;
                }
                else
                {
                    // SET GAMEPAD FOCUS TO TRUE
                    guiManager->controls.At(i)->data->gamepadFocus = true;
                }
            }
        }
        else
        {
            for (int i = 5; i < 10; ++i)
            {
                if (i != focusedButtonId)
                {
                    // SET GAMEPAD FOCUS TO FALSE
                    guiManager->controls.At(i)->data->gamepadFocus = false;
                }
                else
                {
                    // SET GAMEPAD FOCUS TO TRUE
                    guiManager->controls.At(i)->data->gamepadFocus = true;
                }
            }
        }
    }

    if (menuCurrentSelection == MenuSelection::SETTINGS && settingsScene == false)
    {
        menuCurrentSelection = MenuSelection::NONE;

        focusedButtonId = 0;

        // Disable settings buttons and sliders and enable main title buttons
        HideSettingsButtons();
        EnableTitleButtons();
    }

    if (menuCurrentSelection == MenuSelection::START)
    {
        HideTitleButtons();
        TransitionToScene(SceneType::GAMEPLAY);
    }
    else if (menuCurrentSelection == MenuSelection::CONTINUE)
    {
        HideTitleButtons();
        TransitionToScene(SceneType::GAMEPLAY);
    }
    else if (menuCurrentSelection == MenuSelection::SETTINGS)
    {
        focusedButtonId = 5;
        // Disable main title buttons and enable the settings buttons and slider
        HideTitleButtons();
        EnableSettingsButtons();
        
        // Create splines for settings buttons
        // app->easing->CreateSpline(&rect.x, -rect.x + 400, 6000, SplineType::EASE)
    }
    else if (menuCurrentSelection == MenuSelection::EXIT)
    {
        // TODO
    }

    return true;
}

bool SceneTitle::Draw(Render* render)
{
    render->DrawTexture(backgroundTex, 0, 0, &backgroundRect);

    uint width, height;
    win->GetWindowSize(width, height);

    if (settingsScene == false)
    {
        render->DrawText(titleFont, "Wasabi Warriors", width / 2 - width / 2.5f + 3, height / 2 - height / 2.5f + 3, 125, 0, { 105, 105, 105, 255 });
        render->DrawText(titleFont, "Wasabi Warriors", width / 2 - width / 2.5f, height / 2 - height / 2.5f, 125, 0, { 255, 255, 255, 255 });
    }
    else
    {
        render->scale = 3;
        render->DrawTexture(guiAtlasTex, 60, 25, &settingsBackgroundRect);
        render->scale = 1;
    }
    
    return true;
}

bool SceneTitle::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
    tex->UnLoad(backgroundTex);
    tex->UnLoad(guiAtlasTex);

    RELEASE(titleFont);
    RELEASE(buttonFont);

    guiManager->DestroyGuiControl(btnStart);
    guiManager->DestroyGuiControl(btnContinue);
    guiManager->DestroyGuiControl(btnOptions);
    guiManager->DestroyGuiControl(btnCredits);
    guiManager->DestroyGuiControl(btnExit);

    guiManager->DestroyGuiControl(btnFullScreen);
    guiManager->DestroyGuiControl(btnVsync);
    guiManager->DestroyGuiControl(btnMusicVolume);
    guiManager->DestroyGuiControl(btnFXVolume);
    guiManager->DestroyGuiControl(btnReturnTitle);

    menuCurrentSelection = MenuSelection::NONE;

    settingsScene = false;

    focusedButtonId = 0;

    return true;
}

//----------------------------------------------------------
// Manage GUI events for this screen
//----------------------------------------------------------
bool SceneTitle::OnGuiMouseClickEvent(GuiControl* control)
{
    switch (control->type)
    {
    case GuiControlType::BUTTON:
    {
        if (control->id == 1) menuCurrentSelection = MenuSelection::START;
        else if (control->id == 2) menuCurrentSelection = MenuSelection::CONTINUE;
        else if (control->id == 3)
        {
            menuCurrentSelection = MenuSelection::SETTINGS;
            settingsScene = true;
        }
        else if (control->id == 4) menuCurrentSelection = MenuSelection::CREDITS;
        else if (control->id == 5) menuCurrentSelection = MenuSelection::EXIT;
        else if (control->id == 10) settingsScene = false;
        break;
    }
    default: break;
    }

    return true;
}

void SceneTitle::EnableTitleButtons()
{
    for (int i = 0; i < 5; ++i)
    {
        guiManager->controls.At(i)->data->state = GuiControlState::NORMAL;
    }
}

void SceneTitle::EnableSettingsButtons()
{
    for (int i = 5; i < 10; ++i)
    {
        guiManager->controls.At(i)->data->state = GuiControlState::NORMAL;
    }
}

void SceneTitle::HideTitleButtons()
{
    for (int i = 0; i < 5; ++i)
    {
        guiManager->controls.At(i)->data->state = GuiControlState::HIDDEN;
    }
}

void SceneTitle::HideSettingsButtons()
{
    for (int i = 5; i < 10; ++i)
    {
        guiManager->controls.At(i)->data->state = GuiControlState::HIDDEN;
    }
}
