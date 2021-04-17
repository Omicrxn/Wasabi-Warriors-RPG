#include "SceneTitle.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "GuiManager.h"
#include "Window.h"

#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "GuiIcon.h"

#include "Font.h"
#include "Easing.h"

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

    mainTitlesRect = { 0, 0, 1073, 73 };
    settingsTitleRect = { 0, 149, 530, 81 };
    settingsBackgroundRect = { 1228, 295, 300, 200 };

    titleFont = nullptr;
    buttonFont = nullptr;

    hoverFx = -1;
    clickFx = -1;
    titleFx = -1;

    btnStart = nullptr;
    btnContinue = nullptr;
    btnOptions = nullptr;
    btnCredits = nullptr;
    btnExit = nullptr;

    checkFullScreen = nullptr;
    checkVsync = nullptr;
    sliderMusicVolume = nullptr;
    sliderFXVolume = nullptr;

    iconReturnTitle = nullptr;

    menuCurrentSelection = MenuSelection::NONE;
    //settingsCurrentSelection = SettingsSelection::NONE;

    settingsScene = false;
    
    focusedButtonId = 0;
}

SceneTitle::~SceneTitle()
{
}

bool SceneTitle::Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, Easing* easing)
{
    this->guiManager = guiManager;
    this->win = win;
    this->easing = easing;
    this->audio = audio;

    uint width, height;
    win->GetWindowSize(width, height);

    backgroundTex = tex->Load("Assets/Textures/Scenes/main_menu.png");
    backgroundRect = { 0, 0, 1280, 720 };

    guiAtlasTex = tex->Load("Assets/Textures/UI/Elements/ui_spritesheet.png");
    titlesTex = tex->Load("Assets/Textures/Scenes/titles.png");

    titleFont = new Font("Assets/Fonts/shojumaru.xml", tex);
    buttonFont = new Font("Assets/Fonts/showg.xml", tex);

    hoverFx = audio->LoadFx("Assets/Audio/Fx/bong.ogg");
    clickFx = audio->LoadFx("Assets/Audio/Fx/click.ogg");
    titleFx = audio->LoadFx("Assets/Audio/Fx/confirmation.ogg");

    /* MENU BUTTONS */
    btnContinue = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, { 0, 200, 190, 49 }, "CONTINUE");
    btnContinue->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::WHITE);
    easing->CreateSpline(&btnContinue->bounds.x, (int)width / 2 - (int)((float)width / 12), 4000, SplineType::EXPO);

    btnStart = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, { 0, 300, 190, 49 }, "START");
    btnStart->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::WHITE);
    easing->CreateSpline(&btnStart->bounds.x, (int)width / 2 - (int)((float)width / 12), 4000, SplineType::EXPO);

    btnOptions = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, { 0, 400, 190, 49 }, "OPTIONS");
    btnOptions->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::WHITE);
    easing->CreateSpline(&btnOptions->bounds.x, (int)width / 2 - (int)((float)width / 12), 4000, SplineType::EXPO);

    btnCredits = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, { 0, 500, 190, 49 }, "CREDITS");
    btnCredits->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::WHITE);
    easing->CreateSpline(&btnCredits->bounds.x, (int)width / 2 - (int)((float)width / 12), 4000, SplineType::EXPO);

    btnExit = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, { 0, 600, 190, 49 }, "EXIT");
    btnExit->SetButtonProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, Style::WHITE);
    easing->CreateSpline(&btnExit->bounds.x, (int)width / 2 - (int)((float)width / 12), 4000, SplineType::EXPO);

    /* SETTINGS BUTTONS */
    checkFullScreen = (GuiCheckBox*)guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 5, { (int)width / 2 - (int)((float)width / 3.5f) - 20, 194, 45, 49 }, "FULLSCREEN");
    checkFullScreen->SetCheckBoxProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx);

    checkVsync = (GuiCheckBox*)guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 6, { (int)width / 2 - (int)((float)width / 3.5f) - 20, 294, 45, 49 }, "VSYNC");
    checkVsync->SetCheckBoxProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx);

    sliderMusicVolume = (GuiSlider*)guiManager->CreateGuiControl(GuiControlType::SLIDER, 7, { (int)width / 2 - (int)((float)width / 12) + 5, 200, 300, 30 }, "MUSIC VOLUME");
    sliderMusicVolume->SetSliderProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx);

    sliderFXVolume = (GuiSlider*)guiManager->CreateGuiControl(GuiControlType::SLIDER, 8, { (int)width / 2 - (int)((float)width / 12) + 5, 300, 300, 30 }, "FX VOLUME");
    sliderFXVolume->SetSliderProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx);

    iconReturnTitle = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, 9, { (int)width / 2 + (int)((float)width / 4), 570, 70, 55 });
    iconReturnTitle->SetIconProperties(this, guiAtlasTex, buttonFont, hoverFx, clickFx, CONTROLLER_BUTTON_B, IconType::ICON_RETURN);

    HideSettingsButtons();

    audio->PlayMusic("Assets/Audio/Music/menu.ogg", 40.0f);

    titlePosition = { (int)width + mainTitlesRect.w * 2, (int)((float)height / 2) - (int)((float)height / 2.5f) };
 
    titleFxTimer.Start();
    easing->CreateSpline(&titlePosition.x, width / 2 - mainTitlesRect.w / 2, 4000, SplineType::BACK);

    return true;
}

bool SceneTitle::Update(Input* input, float dt)
{
    // Debug purposes
    if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) TransitionToScene(SceneType::GAMEPLAY);
    // ---

    if (input->GetControllerState())
    {
        if (settingsScene == false)
        {
            if ((input->GetControllerButton(CONTROLLER_BUTTON_UP) == KEY_DOWN) && focusedButtonId > 0)
                --focusedButtonId;
            else if ((input->GetControllerButton(CONTROLLER_BUTTON_DOWN) == KEY_DOWN) && focusedButtonId < 4)
                ++focusedButtonId;

            UpdateControllerSelection(0, 4);
        }
        else
        {
            if (focusedButtonId == 5)
            {
                if (input->GetControllerButton(CONTROLLER_BUTTON_DOWN) == KEY_DOWN)
                    focusedButtonId = 6;
                else if (input->GetControllerButton(CONTROLLER_BUTTON_RIGHT) == KEY_DOWN)
                    focusedButtonId = 7;
            }
            else if (focusedButtonId == 6)
            {
                if (input->GetControllerButton(CONTROLLER_BUTTON_UP) == KEY_DOWN)
                    focusedButtonId = 5;
                else if (input->GetControllerButton(CONTROLLER_BUTTON_RIGHT) == KEY_DOWN)
                    focusedButtonId = 8;
            }
            else if (focusedButtonId == 7)
            {
                if (input->GetControllerButton(CONTROLLER_BUTTON_DOWN) == KEY_DOWN)
                    focusedButtonId = 8;
                else if (input->GetControllerButton(CONTROLLER_BUTTON_LEFT) == KEY_DOWN)
                    focusedButtonId = 5;
            }
            else if (focusedButtonId == 8)
            {
                if (input->GetControllerButton(CONTROLLER_BUTTON_UP) == KEY_DOWN)
                    focusedButtonId = 7;
                else if (input->GetControllerButton(CONTROLLER_BUTTON_LEFT) == KEY_DOWN)
                    focusedButtonId = 6;
            }
            UpdateControllerSelection(5, 8);
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
        // Disable main title buttons and enable the settings buttons and slider
        HideTitleButtons();
        EnableSettingsButtons();
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

    // Main title FX sounds just at title appearing
    if (titleFxTimer.ReadSec() >= 3.0f && titleFxTimer.ReadSec() < 3.1f)
    {
        audio->PlayFx(titleFx);
    }

    if (settingsScene == false)
    {
        /* render->DrawText(titleFont, "Wasabi Warriors", width / 2 - width / 2.5f + 3, height / 2 - height / 2.5f + 3, 125, 0, { 105, 105, 105, 255 });
        render->DrawText(titleFont, "Wasabi Warriors", width / 2 - width / 2.5f, height / 2 - height / 2.5f, 125, 0, { 255, 255, 255, 255 });*/
        /*render->DrawTexture(titlesTex, width / 2 - mainTitlesRect.w / 2, height / 2 - height / 2.5f, &mainTitlesRect, 0.0f);*/
        render->DrawTexture(titlesTex, titlePosition.x, titlePosition.y, &mainTitlesRect, 0.0f);
    }
    else
    {
        render->scale = 3;
        render->DrawTexture(guiAtlasTex, 60, 25, &settingsBackgroundRect, 0.0f);
        render->scale = 1;
    }

    return true;
}

bool SceneTitle::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
    tex->UnLoad(backgroundTex);
    tex->UnLoad(guiAtlasTex);
    tex->UnLoad(titlesTex);

    audio->UnloadFx(clickFx);
    audio->UnloadFx(hoverFx);
    audio->UnloadFx(titleFx);

    RELEASE(titleFont);
    RELEASE(buttonFont);

    guiManager->DestroyGuiControl(btnStart);
    guiManager->DestroyGuiControl(btnContinue);
    guiManager->DestroyGuiControl(btnOptions);
    guiManager->DestroyGuiControl(btnCredits);
    guiManager->DestroyGuiControl(btnExit);

    guiManager->DestroyGuiControl(checkFullScreen);
    guiManager->DestroyGuiControl(checkVsync);
    guiManager->DestroyGuiControl(sliderMusicVolume);
    guiManager->DestroyGuiControl(sliderFXVolume);

    guiManager->DestroyGuiControl(iconReturnTitle);

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
        if (control->id == 0) menuCurrentSelection = MenuSelection::CONTINUE;
        else if (control->id == 1) menuCurrentSelection = MenuSelection::START;
        else if (control->id == 2)
        {
            focusedButtonId = 5;
            menuCurrentSelection = MenuSelection::SETTINGS;
            settingsScene = true;
        }
        else if (control->id == 3) menuCurrentSelection = MenuSelection::CREDITS;
        else if (control->id == 4) menuCurrentSelection = MenuSelection::EXIT;
        break;
    }
    case GuiControlType::ICON:
    {
        if (control->id == 9) settingsScene = false;
        break;
    }
    default: break;
    }

    return true;
}

void SceneTitle::UpdateControllerSelection(int idStart, int idEnd)
{
    bool isHovering = false;

    for (int i = idStart; i <= idEnd; ++i)
    {
        if (guiManager->controls.At(i)->data->mouseFocus)
            isHovering = true;
    }

    for (int i = idStart; i <= idEnd; ++i)
    {
        if (i != focusedButtonId || isHovering)
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