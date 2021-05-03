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
    guiManager = nullptr;
    win = nullptr;
    easing = nullptr;
    audio = nullptr;
    this->transitions = transitions;

    // Background
    backgroundTex = nullptr;
    backgroundRect = { 0, 0, 1280, 720 };

    // Texture
    guiAtlasTex = nullptr;
    titlesTex = nullptr;

    // More backgound
    settingsTitleRect = { 0, 149, 530, 81 };
    settingsBackgroundRect = { 1228, 295, 300, 200 };
    creditsTitleRect = { 0, 238, 511, 84 };

    // Fonts
    titleFont = nullptr;
    buttonFont = nullptr;

    // Fx
    hoverFx = -1;
    clickFx = -1;
    titleFx = -1;
    returnFx = -1;

    // Selections
    menuCurrentSelection = MenuSelection::NONE;
    focusedButtonId = 0;
    prevFocusedButtonId = 0;

    // Screens
    screenCredits = nullptr;
    screenSettings = nullptr;
    screenMainMenu = nullptr;
}

SceneTitle::~SceneTitle()
{
}

bool SceneTitle::Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, Easing* easing, Render* render, Transitions* transitions)
{
    this->guiManager = guiManager;
    this->win = win;
    this->render = render;
    this->easing = easing;
    this->audio = audio;
    this->transitions = transitions;

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

    screenMainMenu = new ScreenMainMenu();
    screenMainMenu->Load(0, 4, this, win, guiManager, NULL, easing, guiAtlasTex, titlesTex, buttonFont, hoverFx, clickFx);
    screenMainMenu->isActive = true;

    screenSettings = new ScreenSettings();
    screenSettings->Load(5, 9, this, win, guiManager, NULL, easing, guiAtlasTex, titlesTex, buttonFont, hoverFx, clickFx);

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
    screenCredits->Load(0, 0, this, win, guiManager, NULL, easing, guiAtlasTex, titlesTex, buttonFont, hoverFx, clickFx);

    audio->PlayMusic("Assets/Audio/Music/menu.ogg", 40.0f);

    ScreenMainMenu* tempTitle = (ScreenMainMenu*)screenMainMenu;
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

    return true;
}

bool SceneTitle::Update(Input* input, float dt)
{
    // Debug purposes
    //if (input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN) TransitionToScene(SceneType::GAMEPLAY);
    // ---

    if (input->GetControllerState())
    {
        if (screenMainMenu->isActive)
        {
            screenMainMenu->Update(input, dt, focusedButtonId);
            screenMainMenu->UpdateControllerSelection(focusedButtonId);
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
        screenMainMenu->ShowButtons();
    }
    if (menuCurrentSelection == MenuSelection::CREDITS && screenCredits->isActive == false)
    {
        menuCurrentSelection = MenuSelection::NONE;

        // Hide credits icon and enable main title buttons
        guiManager->controls.At(9)->data->state = GuiControlState::HIDDEN;
        screenMainMenu->ShowButtons();
    }

    if (menuCurrentSelection == MenuSelection::START)
    {
        screenMainMenu->HideButtons();
        /*TransitionToScene(SceneType::GAMEPLAY);*/
        transitions->Transition(WhichAnimation::FADE_TO_BLACK, (Scene*)this, SceneType::GAMEPLAY, 2);
    }
    else if (menuCurrentSelection == MenuSelection::CONTINUE)
    {
        screenMainMenu->HideButtons();
        TransitionToScene(SceneType::GAMEPLAY_LOAD);
    }
    else if (menuCurrentSelection == MenuSelection::SETTINGS)
    {
        // Hide main title buttons and enable the settings buttons and slider
        screenMainMenu->HideButtons();
        screenSettings->ShowButtons();
    }
    else if (menuCurrentSelection == MenuSelection::CREDITS)
    {
        // Hide main title buttons and enable credits return icon
        screenMainMenu->HideButtons();
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

    if (screenMainMenu->isActive)
    {
        screenMainMenu->Draw(render);
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
    // Unload textures
    easing->CleanUp();
    tex->UnLoad(backgroundTex);
    backgroundTex = nullptr;
    tex->UnLoad(guiAtlasTex);
    guiAtlasTex = nullptr;
    tex->UnLoad(titlesTex);
    titlesTex = nullptr;

    // Unload Fx
    audio->UnloadFx(clickFx);
    audio->UnloadFx(hoverFx);
    audio->UnloadFx(titleFx);
    audio->UnloadFx(returnFx);

    // Unload Fonts
    RELEASE(titleFont);
    titleFont = nullptr;
    RELEASE(buttonFont);
    buttonFont = nullptr;

    // Unload Screens
    screenSettings->Unload(tex, audio, guiManager);
    screenMainMenu->Unload(tex, audio, guiManager);
    screenCredits->Unload(tex, audio, guiManager);

    RELEASE(screenSettings);
    RELEASE(screenMainMenu);
    RELEASE(screenCredits);

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
        if (control->id == 0) menuCurrentSelection = MenuSelection::CONTINUE;
        else if (control->id == 1) menuCurrentSelection = MenuSelection::START;
        else if (control->id == 2)
        {
            prevFocusedButtonId = focusedButtonId;
            focusedButtonId = 5;
            menuCurrentSelection = MenuSelection::SETTINGS;

            screenSettings->isActive = true;
            screenMainMenu->isActive = false;
        }
        else if (control->id == 3)
        {
            menuCurrentSelection = MenuSelection::CREDITS;
            screenMainMenu->isActive = false;
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
                screenMainMenu->isActive = true;
            }
            else if (screenCredits->isActive)
            {
                screenCredits->isActive = false;
                screenMainMenu->isActive = true;
            }
               
        }
        break;
    }
    case GuiControlType::CHECKBOX:
    {
        // Fullscreen
        if (control->id == 5)
        {
            ScreenSettings* temp = (ScreenSettings*)screenSettings;

            if (temp->checkFullScreen->GetCheck())
                SDL_SetWindowFullscreen(win->window, 1);
            else
                SDL_SetWindowFullscreen(win->window, 0);
        }
        // Vsync
        if (control->id == 6)
        {
            ScreenSettings* temp = (ScreenSettings*)screenSettings;
            Uint32 windowFlags = SDL_GetWindowFlags(win->window);
            Uint32 rendererFlags = SDL_RENDERER_ACCELERATED;
            
            if (temp->checkFullScreen->GetCheck())


            if (temp->checkVsync->GetCheck())
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


    }
    case GuiControlType::SLIDER:
    {
        if (control->id == 7)
        {
            GuiSlider* tempSlider = (GuiSlider*)this->guiManager->controls.At(7)->data;
            int value = tempSlider->GetValue();
            audio->ChangeMusicVolume(value);
        }
        else if (control->id == 8)
        {
            GuiSlider* tempSlider = (GuiSlider*)this->guiManager->controls.At(8)->data;
            int value = tempSlider->GetValue();
            audio->ChangeFxVolume(value);
        }
            
    }
    default: break;
    }

    return true;
}
