#include "SceneTitle.h"

#include "Input.h"
#include "Render.h"
#include "AssetsManager.h"
#include "Textures.h"
#include "GuiManager.h"
#include "Window.h"

#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "GuiIcon.h"

#include "Font.h"
#include "Easing.h"
#include "Transitions.h"

#include "ScreenSettings.h"
#include "ScreenMainMenu.h"
#include "ScreenCredits.h"

#include "SDL/include/SDL.h"
#include "Log.h"

#define TITLE_FADE_SPEED 0.05f

SceneTitle::SceneTitle()
{
    type = SceneType::TITLE;

    this->name = "scenetitle";

    // The pointers
    assetsManager = nullptr;
    guiManager = nullptr;
    win = nullptr;
    render = nullptr;
    easing = nullptr;
    audio = nullptr;
    transitions = nullptr;

    // Background
    backgroundTex = nullptr;
    backgroundRect = { 0, 0, 1280, 720 };

    // Textures
    guiAtlasTex = nullptr;
    guiAtlasTex2 = nullptr;
    titlesTex = nullptr;

    // Background Rects
    settingsTitleRect = { 0, 149, 530, 81 };
    settingsBackgroundRect = { 1228, 295, 300, 200 };
    creditsTitleRect = { 0, 238, 511, 84 };

    // Font
    buttonFont = nullptr;

    // Fx
    hoverFx = -1;
    clickFx = -1;
    titleFx = -1;
    returnFx = -1;

    // Selections
    menuCurrentSelection = MenuSelection::NONE;
    focusedButtonId = 0;

    // Screens
    screenCredits = nullptr;
    screenSettings = nullptr;
    screenMainMenu = nullptr;
}

SceneTitle::~SceneTitle()
{
}

bool SceneTitle::Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, Easing* easing, Render* render, Transitions* transitions, AssetsManager* assetsManager)
{
    LOG("Loading Scene Title");

    this->assetsManager = assetsManager;
    this->guiManager = guiManager;
    this->win = win;
    this->render = render;
    this->easing = easing;
    this->audio = audio;
    this->transitions = transitions;

    uint width, height;
    win->GetWindowSize(width, height);

    backgroundTex = tex->Load("Textures/Scenes/main_menu.png");

    guiAtlasTex = tex->Load("Textures/UI/ui_spritesheet.png");
    guiAtlasTex2 = tex->Load("Textures/UI/guiTextureSpritesheet.png");
    titlesTex = tex->Load("Textures/Scenes/titles.png");

    buttonFont = new Font("Fonts/comic_serif.xml", tex, assetsManager);

    hoverFx = audio->LoadFx("Audio/Fx/bong.ogg");
    clickFx = audio->LoadFx("Audio/Fx/click.ogg");
    titleFx = audio->LoadFx("Audio/Fx/title.ogg");
    returnFx = audio->LoadFx("Audio/Fx/back.ogg");

    screenMainMenu = new ScreenMainMenu();
    screenMainMenu->Load(0, 4, this, win, guiManager, guiAtlasTex2, titlesTex, buttonFont, hoverFx, clickFx);
    screenMainMenu->isActive = true;

    screenSettings = new ScreenSettings();
    screenSettings->Load(5, 9, this, win, guiManager, guiAtlasTex2, buttonFont, hoverFx, clickFx, returnFx);

    pugi::xml_document docData;
    pugi::xml_node screenNode;
    pugi::xml_parse_result result = docData.load_file("save_game.xml");
    // Check result for loading errors
    if (result == NULL)
    {
        LOG("Could not load map info xml file map_info.xml. pugi error: %s", result.description());
    }
    else
    {
        screenNode = docData.child("game_state").child("screen");
        screenSettings->LoadState(screenNode);
    }

    screenCredits = new ScreenCredits();
    screenCredits->Load(10, 10, this, win, guiManager, guiAtlasTex2, buttonFont, hoverFx, returnFx);

    audio->PlayMusic("Audio/Music/menu.ogg", 0.5f);

    ScreenMainMenu* tempTitle = screenMainMenu;
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

    screenMainMenu->ShowButtons();
    screenSettings->HideButtons();
    screenCredits->HideButtons();

    guiManager->ToggleMouse();

    return true;
}

bool SceneTitle::Update(Input* input, float dt)
{
    if (input->GetControllerState())
    {
        if (menuCurrentSelection == MenuSelection::NONE)
            screenMainMenu->Update(input, dt, focusedButtonId);
        else if (menuCurrentSelection == MenuSelection::SETTINGS)
            screenSettings->Update(input, dt, focusedButtonId);
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

    if (menuCurrentSelection == MenuSelection::NONE)
    {
        screenMainMenu->Draw(render);
    }

    if (menuCurrentSelection == MenuSelection::SETTINGS)
    {
        screenSettings->Draw(render);
    }

    if (menuCurrentSelection == MenuSelection::CREDITS)
    {
        screenCredits->Draw(render);
    }

    return true;
}

bool SceneTitle::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
    audio->StopMusic();

    easing->CleanUp();

    // Unload textures
    tex->UnLoad(backgroundTex);
    tex->UnLoad(guiAtlasTex);
    tex->UnLoad(guiAtlasTex2);
    tex->UnLoad(titlesTex);

    // Unload Fx
    audio->UnloadFx(clickFx);
    audio->UnloadFx(hoverFx);
    audio->UnloadFx(titleFx);
    audio->UnloadFx(returnFx);

    // Unload Font
    RELEASE(buttonFont);

    // Unload Screens
    screenSettings->Unload(tex, audio, guiManager);
    screenMainMenu->Unload(tex, audio, guiManager);
    screenCredits->Unload(tex, audio, guiManager);

    RELEASE(screenSettings);
    RELEASE(screenMainMenu);
    RELEASE(screenCredits);

    this->assetsManager = nullptr;
    this->guiManager = nullptr;
    this->win = nullptr;
    this->render = nullptr;
    this->easing = nullptr;
    this->audio = nullptr;

    return true;
}

bool SceneTitle::LoadState(pugi::xml_node& scenetitle)
{
    // Pass the screensettings node to the screen settings
    pugi::xml_node screenNode = scenetitle.parent().parent().child("screen");
    screenSettings->LoadState(screenNode);
    return true;
}

bool SceneTitle::SaveState(pugi::xml_node& scenetitle) const
{
    // Pass the audio node to the screen settings
    pugi::xml_node screenNode = scenetitle.parent().parent().child("screen");
    screenSettings->SaveState(screenNode);
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
        if (control->id == 0)
        {
            menuCurrentSelection = MenuSelection::CONTINUE;

            screenMainMenu->Disable();

            audio->StopMusic();

            transitions->Transition(WhichAnimation::FADE_TO_BLACK, (Scene*)this, SceneType::GAMEPLAY_LOAD, 2);
        }
        else if (control->id == 1)
        {
            menuCurrentSelection = MenuSelection::START;

            screenMainMenu->Disable();

            audio->StopMusic();

            transitions->Transition(WhichAnimation::FADE_TO_BLACK, (Scene*)this, SceneType::GAMEPLAY, 2);
        }
        else if (control->id == 2)
        {
            menuCurrentSelection = MenuSelection::SETTINGS;

            screenMainMenu->Disable();
            screenSettings->Enable();

            audio->StopMusic();
            audio->PlayMusic("Audio/Music/menu_settings.ogg");

            focusedButtonId = 5;
        }
        else if (control->id == 3)
        {
            menuCurrentSelection = MenuSelection::CREDITS;

            screenMainMenu->Disable();
            screenCredits->Enable();

            focusedButtonId = 10;
        }
        else if (control->id == 4)
        {
            menuCurrentSelection = MenuSelection::EXIT;

            audio->StopMusic();

            guiManager->ExitGame();
        }
        break;
    }
    case GuiControlType::ICON:
    {
        if (control->id == 9)
        {
            menuCurrentSelection = MenuSelection::NONE;

            // Hide settings buttons and sliders and enable main title buttons
            screenSettings->Disable();
            screenMainMenu->Enable();

            audio->StopMusic();
            audio->PlayMusic("Audio/Music/menu.ogg");

            focusedButtonId = 2;
        }
        else if (control->id == 10)
        {
            menuCurrentSelection = MenuSelection::NONE;

            // Hide credits icon and enable main title buttons
            screenCredits->Disable();
            screenMainMenu->Enable();

            audio->StopMusic();
            audio->PlayMusic("Audio/Music/menu.ogg");

            focusedButtonId = 3;
        }
        break;
    }
    case GuiControlType::CHECKBOX:
    {
        // Fullscreen
        if (control->id == 7)
        {
            if (screenSettings->checkFullScreen->GetCheck())
                SDL_SetWindowFullscreen(win->window, 1);
            else
                SDL_SetWindowFullscreen(win->window, 0);
        }
        // Vsync
        if (control->id == 8)
        {
            Uint32 windowFlags = SDL_GetWindowFlags(win->window);
            Uint32 rendererFlags = SDL_RENDERER_ACCELERATED;

            if (screenSettings->checkFullScreen->GetCheck())
            {
            }

            if (screenSettings->checkVsync->GetCheck())
                rendererFlags |= SDL_RENDERER_PRESENTVSYNC;
            else
                rendererFlags = rendererFlags ^ SDL_RENDERER_PRESENTVSYNC;

            SDL_DestroyWindow(win->window);
            win->window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, windowFlags);

            SDL_DestroyRenderer(render->renderer);
            render->renderer = SDL_CreateRenderer(win->window, -1, rendererFlags);
            guiManager->Start();

            transitions->Transition(WhichAnimation::NONE, this, SceneType::TITLE);
        }

        pugi::xml_document docData;
        pugi::xml_node screenNode;

        pugi::xml_parse_result result = docData.load_file("save_game.xml");

        // Check result for loading errors
        if (result == NULL)
        {
            LOG("Could not load map info xml file map_info.xml. pugi error: %s", result.description());
        }
        else
        {
            screenNode = docData.child("game_state").child("screen");
            screenSettings->SaveState(screenNode);
            docData.save_file("save_game.xml");
        }
        break;
    }
    case GuiControlType::SLIDER:
    {
        if (control->id == 5)
        {
            GuiSlider* tempSlider = (GuiSlider*)this->guiManager->controls.At(7)->data;
            int value = tempSlider->GetValue();
            audio->ChangeMusicVolume(value);
        }
        else if (control->id == 6)
        {
            GuiSlider* tempSlider = (GuiSlider*)this->guiManager->controls.At(8)->data;
            int value = tempSlider->GetValue();
            audio->ChangeFxVolume(value);
        }
        break;
    }
    default: 
        break;
    }

    return true;
}