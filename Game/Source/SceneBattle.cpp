#include "SceneBattle.h"

#include "Textures.h"
#include "Render.h"
#include "Font.h"
#include "Window.h"

#include "BattleSystem.h"

#include "GuiManager.h"
#include "GuiButton.h"

#include <string.h>

#define TITLE_FADE_SPEED 0.05f

SceneBattle::SceneBattle()
{
	type = SceneType::BATTLE;

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

    menuCurrentSelection = ActionSelection::NONE;

    hoverFx = -1;
    clickFx = -1;

    controllerFocus = 0;
}

SceneBattle::~SceneBattle()
{
}

bool SceneBattle::Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityman)
{
    this->win = win;

    audio->PlayMusic("Assets/Audio/Music/menu.ogg");

    titleFont = new Font("Assets/Fonts/shojumaru.xml", tex);
    buttonFont = new Font("Assets/Fonts/showg.xml", tex);

    //backgroundTex = tex->Load("Assets/Textures/Scenes/main_menu.png");
    backgroundRect = { 0, 0, 1280, 720 };

    guiAtlasTex = tex->Load("Assets/Textures/UI/Elements/ui_spritesheet.png");

    hoverFx = audio->LoadFx("Assets/Audio/Fx/bong.ogg");
    clickFx = audio->LoadFx("Assets/Audio/Fx/click.ogg");

    uint width, height;
    win->GetWindowSize(width, height);

    this->guiManager = guiManager;

    btnStart = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, { (int)width / 2 - (int)((float)width / 12), 200, 190, 49 }, "ATTACK");
    btnStart->SetObserver(this);
    btnStart->SetTexture(guiAtlasTex);
    btnStart->SetFont(buttonFont);
    btnStart->SetButtonAudioFx(hoverFx, clickFx);

    btnContinue = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, { (int)width / 2 - (int)((float)width / 12), 300, 190, 49 }, "DEFEND");
    btnContinue->SetObserver(this);
    btnContinue->SetTexture(guiAtlasTex);
    btnContinue->SetFont(buttonFont);
    btnContinue->SetButtonAudioFx(hoverFx, clickFx);

    btnOptions = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, { (int)width / 2 - (int)((float)width / 12), 400, 190, 49 }, "ITEM");
    btnOptions->SetObserver(this);
    btnOptions->SetTexture(guiAtlasTex);
    btnOptions->SetFont(buttonFont);
    btnOptions->SetButtonAudioFx(hoverFx, clickFx);

    btnCredits = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, { (int)width / 2 - (int)((float)width / 12), 500, 190, 49 }, "RUN");
    btnCredits->SetObserver(this);
    btnCredits->SetTexture(guiAtlasTex);
    btnCredits->SetFont(buttonFont);
    btnCredits->SetButtonAudioFx(hoverFx, clickFx);

    btnExit = (GuiButton*)guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, { (int)width / 2 - (int)((float)width / 12), 600, 190, 49 }, "NONE");
    btnExit->SetObserver(this);
    btnExit->SetTexture(guiAtlasTex);
    btnExit->SetFont(buttonFont);
    btnExit->SetButtonAudioFx(hoverFx, clickFx);

	return true;
}

bool SceneBattle::Update(Input* input, float dt)
{
    if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) TransitionToScene(SceneType::GAMEPLAY);

    if (input->pads[0].enabled)
    {
            /* Input */
        if (((input->pads[0].up || input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)) && controllerFocus >= 1)
            --controllerFocus;
        else if (((input->pads[0].down || input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)) && controllerFocus <= 3)
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
    }

    if (menuCurrentSelection == ActionSelection::ATTACK)
        BattleSystem::GetInstance()->battleGUIState = BattleGUIState::ATTACK;
    else if (menuCurrentSelection == ActionSelection::DEFEND)
        BattleSystem::GetInstance()->battleGUIState = BattleGUIState::DEFEND;
    
    BattleSystem::GetInstance()->Update(input);

	return true;
}

bool SceneBattle::Draw(Render* render)
{
	render->DrawRectangle({ 0, 0, 1280, 720 }, { 255, 255, 255, (unsigned char)255.0f }, true, false);

    /*render->DrawTexture(backgroundTex, 0, 0, &backgroundRect);*/

    uint width, height;
    win->GetWindowSize(width, height);

    render->DrawText(titleFont, "Fight!", width / 2 - width / 2.5f + 3, height / 2 - height / 2.5f + 3, 125, 0, { 105, 105, 105, 255 });
    render->DrawText(titleFont, "Fight!", width / 2 - width / 2.5f, height / 2 - height / 2.5f, 125, 0, { 255, 255, 255, 255 });

    // Player name
    render->DrawText(titleFont, BattleSystem::GetInstance()->GetPlayer()->stats.name.GetString(), 100 + 3, 300 + 3, 75, 0, { 105, 105, 105, 255 });
    render->DrawText(titleFont, BattleSystem::GetInstance()->GetPlayer()->stats.name.GetString(), 100, 300, 75, 0, { 255, 255, 255, 255 });

    // Player life
    char HP[8] = { 0 };
    sprintf_s(HP, 8, "HP: %03i", BattleSystem::GetInstance()->GetPlayer()->stats.currentHP);
    render->DrawText(titleFont, HP, 100 + 3, 375 + 3, 75, 0, { 105, 105, 105, 255 });
    render->DrawText(titleFont, HP, 100, 375, 75, 0, { 255, 255, 255, 255 });

    // Enemy name
    render->DrawText(titleFont, BattleSystem::GetInstance()->GetEnemy()->stats.name.GetString(), 800 + 3, 100 + 3, 75, 0, { 105, 105, 105, 255 });
    render->DrawText(titleFont, BattleSystem::GetInstance()->GetEnemy()->stats.name.GetString(), 800, 100, 75, 0, { 255, 255, 255, 255 });

    sprintf_s(HP, 8, "HP: %03i", BattleSystem::GetInstance()->GetEnemy()->stats.currentHP);
    render->DrawText(titleFont, HP, 800 + 3, 175 + 3, 75, 0, { 105, 105, 105, 255 });
    render->DrawText(titleFont, HP, 800, 175, 75, 0, { 255, 255, 255, 255 });

	return true;
}

bool SceneBattle::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
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
bool SceneBattle::OnGuiMouseClickEvent(GuiControl* control)
{
    switch (control->type)
    {
    case GuiControlType::BUTTON:
    {
        if (control->id == 1) menuCurrentSelection = ActionSelection::ATTACK;
        else if (control->id == 2) menuCurrentSelection = ActionSelection::DEFEND;
        else if (control->id == 3) menuCurrentSelection = ActionSelection::ITEM;
        else if (control->id == 4) menuCurrentSelection = ActionSelection::RUN;
        else if (control->id == 5) menuCurrentSelection = ActionSelection::NONE;
        break;
    }
    default: break;
    }

    return true;
}
