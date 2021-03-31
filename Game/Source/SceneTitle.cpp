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

    btnStart = nullptr;
    btnContinue = nullptr;
    btnOptions = nullptr;
    btnCredits = nullptr;
    btnExit = nullptr;

    titleFont = nullptr;
    buttonFont = nullptr;

    win = nullptr;

    menuCurrentSelection = MenuSelection::NONE;
    //settingsCurrentSelection = SettingsSelection::NONE;

    hoverFx = -1;
    clickFx = -1;

    controllerFocus = 0;
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

    hoverFx = audio->LoadFx("Assets/Audio/Fx/bong.ogg");
    clickFx = audio->LoadFx("Assets/Audio/Fx/click.ogg");

    uint width, height;
    win->GetWindowSize(width, height);

    this->guiManager = guiManager;
    
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

    return true;
}

bool SceneTitle::Update(Input* input, float dt)
{
    if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) TransitionToScene(SceneType::GAMEPLAY);

    //backgroundAnim.Update();

    if (input->pads[0].enabled)
    {
        /*if (input->pads[0].up && controllerFocus >= 1 || input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
            --controllerFocus;
        else if (input->pads[0].down && controllerFocus <= 3 || input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
            ++controllerFocus;*/

        /* Input */ 
        if ( ((input->pads[0].up || input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)) && controllerFocus >= 1)
            --controllerFocus;
        else if ( ((input->pads[0].down|| input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)) && controllerFocus <= 3)
            ++controllerFocus;

        for (int i = 0; i < 5; ++i)
        {
            if (i != controllerFocus)
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

        /*switch (controllerFocus)
        {
        case 0:
            btnContinue->gamepadFocus = false;

            btnStart->gamepadFocus = true;
            break;
        case 1:
            btnStart->gamepadFocus = false;
            btnOptions->gamepadFocus = false;

            btnContinue->gamepadFocus = true;
            break;
        case 2:
            btnContinue->gamepadFocus = false;
            btnCredits->gamepadFocus = false;

            btnOptions->gamepadFocus = true;
            break;
        case 3:
            btnOptions->gamepadFocus = false;
            btnExit->gamepadFocus = false;

            btnCredits->gamepadFocus = true;
            break;
        case 4:
            btnCredits->gamepadFocus = false;

            btnExit->gamepadFocus = true;
            break;
        default:
            break;
        }*/
    }

    if (menuCurrentSelection == MenuSelection::START)
        TransitionToScene(SceneType::GAMEPLAY);
    else if (menuCurrentSelection == MenuSelection::CONTINUE)
        TransitionToScene(SceneType::GAMEPLAY);
    //else if (menuCurrentSelection == MenuSelection::EXIT)

    return true;
}

bool SceneTitle::Draw(Render* render)
{
    render->DrawTexture(backgroundTex, 0, 0, &backgroundRect);

    uint width, height;
    win->GetWindowSize(width, height);

    render->DrawText(titleFont, "Wasabi Warriors", width / 2 - width / 2.5f + 3, height / 2 - height / 2.5f + 3, 125, 0, { 105, 105, 105, 255 });
    render->DrawText(titleFont, "Wasabi Warriors", width / 2 - width / 2.5f, height / 2 - height / 2.5f, 125, 0, { 255, 255, 255, 255 });
    
    return true;
}

bool SceneTitle::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
    tex->UnLoad(backgroundTex);
    tex->UnLoad(guiAtlasTex);

    guiManager->DestroyGuiControl(btnStart);
    guiManager->DestroyGuiControl(btnContinue);
    guiManager->DestroyGuiControl(btnOptions);
    guiManager->DestroyGuiControl(btnCredits);
    guiManager->DestroyGuiControl(btnExit);

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
        else if (control->id == 3) menuCurrentSelection = MenuSelection::SETTINGS;
        else if (control->id == 4) menuCurrentSelection = MenuSelection::CREDITS;
        else if (control->id == 5) menuCurrentSelection = MenuSelection::EXIT;
        break;
    }
    default: break;
    }

    return true;
}