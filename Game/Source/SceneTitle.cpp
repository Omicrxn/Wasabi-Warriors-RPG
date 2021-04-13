#include "SceneTitle.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "GuiManager.h"
#include "Window.h"

#include "GuiButton.h"
#include "Font.h"

#include "SDL/include/SDL.h"

#define TITLE_FADE_SPEED 0.05f

SceneTitle::SceneTitle()
{
    type = SceneType::TITLE;
    
    backgroundTex = nullptr;
    backgroundRect = { 0, 0, 0, 0 };

    guiAtlasTex = nullptr;
    miscTex = nullptr;

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

    titleFont = nullptr;
    buttonFont = nullptr;

    win = nullptr;

    menuCurrentSelection = MenuSelection::NONE;
    //settingsCurrentSelection = SettingsSelection::NONE;

    hoverFx = -1;
    clickFx = -1;
    
    focusedButtonId = 0;
}

SceneTitle::~SceneTitle()
{
}

bool SceneTitle::Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager)
{
    this->win = win;

    audio->PlayMusic("Assets/Audio/Music/menu.ogg");

    titleFont = new Font("Assets/Fonts/shojumaru.xml", tex);
    buttonFont = new Font("Assets/Fonts/showg.xml", tex);

    backgroundTex = tex->Load("Assets/Textures/Scenes/main_menu.png");
    backgroundRect = { 0, 0, 1280, 720 };

    guiAtlasTex = tex->Load("Assets/Textures/UI/Elements/ui_spritesheet.png");
    miscTex = tex->Load("Assets/Textures/UI/Custom Sprite/Misc.png");

    hoverFx = audio->LoadFx("Assets/Audio/Fx/bong.ogg");
    clickFx = audio->LoadFx("Assets/Audio/Fx/click.ogg");

    uint width, height;
    win->GetWindowSize(width, height);

    this->guiManager = guiManager;
    settingsScene = false;

    /* THE MENU BUTTONS */
    btnStart = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, { (int)width / 2 - (int)((float)width / 12), 200, 190, 49 }, "START");
    btnStart->SetObserver(this);
    btnStart->SetTexture(guiAtlasTex);
    btnStart->SetFont(buttonFont);
    btnStart->SetButtonAudioFx(hoverFx, clickFx);

    btnContinue = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, { (int)width / 2 - (int)((float)width / 12), 300, 190, 49 }, "CONTINUE");
    btnContinue->SetObserver(this);
    btnContinue->SetTexture(guiAtlasTex);
    btnContinue->SetFont(buttonFont);
    btnContinue->SetButtonAudioFx(hoverFx, clickFx);

    btnOptions = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, { (int)width / 2 - (int)((float)width / 12), 400, 190, 49 }, "OPTIONS");
    btnOptions->SetObserver(this);
    btnOptions->SetTexture(guiAtlasTex);
    btnOptions->SetFont(buttonFont);
    btnOptions->SetButtonAudioFx(hoverFx, clickFx);

    btnCredits = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, { (int)width / 2 - (int)((float)width / 12), 500, 190, 49 }, "CREDITS");
    btnCredits->SetObserver(this);
    btnCredits->SetTexture(guiAtlasTex);
    btnCredits->SetFont(buttonFont);
    btnCredits->SetButtonAudioFx(hoverFx, clickFx);

    btnExit = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, { (int)width / 2 - (int)((float)width / 12), 600, 190, 49 }, "EXIT");
    btnExit->SetObserver(this);
    btnExit->SetTexture(guiAtlasTex);
    btnExit->SetFont(buttonFont);
    btnExit->SetButtonAudioFx(hoverFx, clickFx);

    /* THE SETTINGS BUTTONS */
    btnFullScreen = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, { (int)width / 2 - (int)((float)width / 12), 200, 190, 49 }, "FULLSCREEN");
    btnFullScreen->SetObserver(this);
    btnFullScreen->SetTexture(guiAtlasTex);
    btnFullScreen->SetFont(buttonFont);
    btnFullScreen->SetButtonAudioFx(hoverFx, clickFx);
    btnFullScreen->state = GuiControlState::DISABLED;

    btnVsync = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, { (int)width / 2 - (int)((float)width / 12), 300, 190, 49 }, "VSYNC");
    btnVsync->SetObserver(this);
    btnVsync->SetTexture(guiAtlasTex);
    btnVsync->SetFont(buttonFont);
    btnVsync->SetButtonAudioFx(hoverFx, clickFx);
    btnVsync->state = GuiControlState::DISABLED;

    btnMusicVolume = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 8, { (int)width / 2 - (int)((float)width / 12), 400, 190, 49 }, "MUSIC VOLUME");
    btnMusicVolume->SetObserver(this);
    btnMusicVolume->SetTexture(guiAtlasTex);
    btnMusicVolume->SetFont(buttonFont);
    btnMusicVolume->SetButtonAudioFx(hoverFx, clickFx);
    btnMusicVolume->state = GuiControlState::DISABLED;

    btnFXVolume = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 9, { (int)width / 2 - (int)((float)width / 12), 500, 190, 49 }, "FX VOLUME");
    btnFXVolume->SetObserver(this);
    btnFXVolume->SetTexture(guiAtlasTex);
    btnFXVolume->SetFont(buttonFont);
    btnFXVolume->SetButtonAudioFx(hoverFx, clickFx);
    btnFXVolume->state = GuiControlState::DISABLED;

    btnReturnTitle = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, { (int)width / 2 - (int)((float)width / 12), 600, 190, 49 }, "RETURN");
    btnReturnTitle->SetObserver(this);
    btnReturnTitle->SetTexture(guiAtlasTex);
    btnReturnTitle->SetFont(buttonFont);
    btnReturnTitle->SetButtonAudioFx(hoverFx, clickFx);
    btnReturnTitle->state = GuiControlState::DISABLED;

    focusedButtonId = 0;

    return true;
}

bool SceneTitle::Update(Input* input, float dt)
{
    if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) TransitionToScene(SceneType::GAMEPLAY);

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
        DisableSettingsButtons();
        EnableTitleButtons();
    }

    if (menuCurrentSelection == MenuSelection::START)
    {
        DisableTitleButtons();
        TransitionToScene(SceneType::GAMEPLAY);
    }
    else if (menuCurrentSelection == MenuSelection::CONTINUE)
    {
        DisableTitleButtons();
        TransitionToScene(SceneType::GAMEPLAY);
    }
    else if (menuCurrentSelection == MenuSelection::SETTINGS)
    {
        focusedButtonId = 5;
        // Disable main title buttons and enable the settings buttons and slider
        DisableTitleButtons();
        EnableSettingsButtons();
        
        // Create splines for settings buttons
        // app->easing->CreateSpline(&rect.x, -rect.x + 400, 6000, SplineType::EASE)
    }
    //else if (menuCurrentSelection == MenuSelection::EXIT)

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
        settingsBackgroundRect = SDL_Rect({ 12, 292, 300, 200 });
        render->scale = 3;
        render->DrawTexture(miscTex, 60, 25, &settingsBackgroundRect);
        render->scale = 1;
    }
    
    
    return true;
}

bool SceneTitle::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
    focusedButtonId = 0;

    tex->UnLoad(backgroundTex);
    tex->UnLoad(guiAtlasTex);
    tex->UnLoad(miscTex);

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

    RELEASE(titleFont);
    RELEASE(buttonFont);

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

void SceneTitle::DisableTitleButtons()
{
    for (int i = 0; i < 5; ++i)
    {
        guiManager->controls.At(i)->data->state = GuiControlState::DISABLED;
    }
}

void SceneTitle::DisableSettingsButtons()
{
    for (int i = 5; i < 10; ++i)
    {
        guiManager->controls.At(i)->data->state = GuiControlState::DISABLED;
    }
}
