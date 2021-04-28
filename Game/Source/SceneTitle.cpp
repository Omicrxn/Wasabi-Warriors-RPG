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

#include "ScreenSettings.h"
#include "ScreenTitle.h"
#include "ScreenCredits.h"

#include "SDL/include/SDL.h"

#define TITLE_FADE_SPEED 0.05f

SceneTitle::SceneTitle()
{
    type = SceneType::TITLE;

    guiManager = nullptr;
    win = nullptr;
    easing = nullptr;
    audio = nullptr;

    backgroundTex = nullptr;
    backgroundRect = { 0, 0, 1280, 720 };

    guiAtlasTex = nullptr;
    titlesTex = nullptr;

    settingsTitleRect = { 0, 149, 530, 81 };
    settingsBackgroundRect = { 1228, 295, 300, 200 };
    creditsTitleRect = { 0, 238, 511, 84 };

    titleFont = nullptr;
    buttonFont = nullptr;

    hoverFx = -1;
    clickFx = -1;
    titleFx = -1;
    returnFx = -1;

    menuCurrentSelection = MenuSelection::NONE;

    focusedButtonId = 0;
    prevFocusedButtonId = 0;
}

SceneTitle::~SceneTitle()
{
}

bool SceneTitle::Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, Easing* easing, Render* render)
{
    this->guiManager = guiManager;
    this->win = win;
    this->easing = easing;
    this->audio = audio;

    uint width, height;
    win->GetWindowSize(width, height);

    backgroundTex = tex->Load("Assets/Textures/Scenes/main_menu.png");

    guiAtlasTex = tex->Load("Assets/Textures/UI/ui_spritesheet.png");
    titlesTex = tex->Load("Assets/Textures/Scenes/titles.png");

    titleFont = new Font("Assets/Fonts/shojumaru.xml", tex);
    buttonFont = new Font("Assets/Fonts/showg.xml", tex);

    hoverFx = audio->LoadFx("Assets/Audio/Fx/bong.ogg");
    clickFx = audio->LoadFx("Assets/Audio/Fx/click.ogg");
    titleFx = audio->LoadFx("Assets/Audio/Fx/title.ogg");
    returnFx = audio->LoadFx("Assets/Audio/Fx/back.ogg");

    screenTitle = new ScreenTitle();
    screenTitle->Load(this, win, guiManager, easing, guiAtlasTex, titlesTex, buttonFont, hoverFx, clickFx);
    screenTitle->isActive = true;

    screenSettings = new ScreenSettings();
    screenSettings->Load(this, win, guiManager, easing, guiAtlasTex, titlesTex, buttonFont, hoverFx, clickFx);

    screenCredits = new ScreenCredits();
    screenCredits->Load(this, win, guiManager, easing, guiAtlasTex, titlesTex, buttonFont, hoverFx, clickFx);

    audio->PlayMusic("Assets/Audio/Music/menu.ogg", 40.0f);

    ScreenTitle* tempTitle = (ScreenTitle*)screenTitle;
    tempTitle->titlePosition = { (int)width + tempTitle->mainTitlesRect.w * 2, (int)((float)height / 2) - (int)((float)height / 2.5f) };

   
    titleFxTimer.Start();
    easing->CreateSpline(&tempTitle->titlePosition.x, width / 2 - tempTitle->mainTitlesRect.w / 2, 3000, SplineType::BACK);
    easing->CreateSpline(&tempTitle->btnExit->bounds.x, (int)width / 2 - 190 / 2, 3000, SplineType::EXPO);
    easing->CreateSpline(&tempTitle->btnCredits->bounds.x, (int)width / 2 - 190 / 2, 3000, SplineType::EXPO);
    easing->CreateSpline(&tempTitle->btnOptions->bounds.x, (int)width / 2 - 190 / 2, 3000, SplineType::EXPO);
    easing->CreateSpline(&tempTitle->btnStart->bounds.x, (int)width / 2 - 190 / 2, 3000, SplineType::EXPO);
    easing->CreateSpline(&tempTitle->btnContinue->bounds.x, (int)width / 2 - 190 / 2, 3000, SplineType::EXPO);

    render->camera.x = 0;
    render->camera.y = 0;

    screenTitle->ShowButtons();
    screenSettings->HideButtons();
    return true;
}

bool SceneTitle::Update(Input* input, float dt)
{
    // Debug purposes
    //if (input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN) TransitionToScene(SceneType::GAMEPLAY);
    // ---

    if (input->GetControllerState())
    {
        if (screenTitle->isActive)
        {
            screenTitle->Update(input, dt, focusedButtonId);
            screenTitle->UpdateControllerSelection(focusedButtonId);
        }

        if (screenSettings->isActive)
        {
            screenSettings->Update(input, dt, focusedButtonId);
            screenSettings->UpdateControllerSelection(focusedButtonId);
        }
    }

    if (menuCurrentSelection == MenuSelection::SETTINGS && screenSettings->isActive == false)
    {
        menuCurrentSelection = MenuSelection::NONE;

        focusedButtonId = prevFocusedButtonId;

        // Hide settings buttons and sliders and enable main title buttons
        screenSettings->HideButtons();
        screenTitle->ShowButtons();
    }
    if (menuCurrentSelection == MenuSelection::CREDITS && screenCredits->isActive == false)
    {
        menuCurrentSelection = MenuSelection::NONE;

        // Hide credits icon and enable main title buttons
        guiManager->controls.At(9)->data->state = GuiControlState::HIDDEN;
        screenTitle->ShowButtons();
    }

    if (menuCurrentSelection == MenuSelection::START)
    {
        screenTitle->HideButtons();
        TransitionToScene(SceneType::GAMEPLAY);
    }
    else if (menuCurrentSelection == MenuSelection::CONTINUE)
    {
        screenTitle->HideButtons();
        TransitionToScene(SceneType::GAMEPLAY_LOAD);
    }
    else if (menuCurrentSelection == MenuSelection::SETTINGS)
    {
        // Hide main title buttons and enable the settings buttons and slider
        screenTitle->HideButtons();
        screenSettings->ShowButtons();
    }
    else if (menuCurrentSelection == MenuSelection::CREDITS)
    {
        // Hide main title buttons and enable credits return icon
        screenTitle->HideButtons();
        guiManager->controls.At(9)->data->state = GuiControlState::NORMAL;
    }
    else if (menuCurrentSelection == MenuSelection::EXIT)
    {
        guiManager->ExitGame();
    }

    return true;
}

bool SceneTitle::Draw(Render* render)
{
    uint width, height;
    win->GetWindowSize(width, height);

    render->DrawTexture(backgroundTex, 0, 0, &backgroundRect, 0.0f);

    // Main title FX sounds just at title appearing
    if (titleFxTimer.ReadSec() >= 1.9f && titleFxTimer.ReadSec() < 2.0f)
    {
        audio->PlayFx(titleFx);
    }

    if (screenTitle->isActive)
    {
        screenTitle->Draw(render);
    }

    if (screenSettings->isActive)
    {
        screenSettings->Draw(render);
    }

    if (screenCredits->isActive)
    {
        screenCredits->Draw(render);
    }

    return true;
}

bool SceneTitle::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{

    easing->CleanUp();
    tex->UnLoad(backgroundTex);
    backgroundTex = nullptr;
    tex->UnLoad(guiAtlasTex);
    guiAtlasTex = nullptr;
    tex->UnLoad(titlesTex);
    titlesTex = nullptr;

    audio->UnloadFx(clickFx);
    audio->UnloadFx(hoverFx);
    audio->UnloadFx(titleFx);
    audio->UnloadFx(returnFx);

    RELEASE(titleFont);
    titleFont = nullptr;
    RELEASE(buttonFont);
    buttonFont = nullptr;

    screenSettings->Unload(tex, audio, guiManager);
    screenTitle->Unload(tex, audio, guiManager);
    screenCredits->Unload(tex, audio, guiManager);

    RELEASE(screenSettings);
    RELEASE(screenTitle);
    RELEASE(screenCredits);

    this->guiManager = nullptr;
    this->win = nullptr;
    this->easing = nullptr;
    this->audio = nullptr;
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
            prevFocusedButtonId = focusedButtonId;
            focusedButtonId = 5;
            menuCurrentSelection = MenuSelection::SETTINGS;

            screenSettings->isActive = true;
            screenTitle->isActive = false;
        }
        else if (control->id == 3)
        {
            menuCurrentSelection = MenuSelection::CREDITS;
            screenTitle->isActive = false;
            screenCredits->isActive = true;
        }
        else if (control->id == 4) menuCurrentSelection = MenuSelection::EXIT;
        break;
    }
    case GuiControlType::ICON:
    {
        if (control->id == 9)
        {
            if (screenSettings->isActive)
            {
                screenSettings->isActive = false;
                screenTitle->isActive = true;
            }
            else if (screenCredits->isActive)
            {
                screenCredits->isActive = false;
                screenTitle->isActive = true;
            }
               
        }
        break;
    }
    case GuiControlType::CHECKBOX:
    {
        if (control->id == 5)
        {
            ScreenSettings* temp = (ScreenSettings*)screenSettings;

            if (temp->checkFullScreen->GetCheck())
                SDL_SetWindowFullscreen(win->window, 1);
            else
                SDL_SetWindowFullscreen(win->window, 0);
        }
    }
    default: break;
    }

    return true;
}
