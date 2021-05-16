#include "SceneLogo.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"

#include "GuiManager.h"
#include "Easing.h"

#define LOGO_FADE_SPEED 1.0f

SceneLogo::SceneLogo()
{
    type = SceneType::LOGO;

    this->name = "scenelogo";

    logoTex = nullptr;
    backgroundTex = nullptr;
    logo = { 0, 0, 1280, 720 };

    logoFx = -1;
    state = 0;
    timeCounter = 0.0f;
    logoAlpha = 0.0f;

    logoFxTimer.Start();

    posX = 2000;

    width = 0;
    height = 0;

    audio = nullptr;
    easing = nullptr;
}

SceneLogo::~SceneLogo()
{
}

bool SceneLogo::Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, Easing* easing)
{
    this->audio = audio;
    this->easing = easing;

    logoTex = tex->Load("Textures/Scenes/logo_logo.png");
    backgroundTex = tex->Load("Textures/Scenes/logo_background.png");
    logoFx = audio->LoadFx("Audio/Fx/logo.wav");

    logoFxTimer.Start();
    win->GetWindowSize(width, height);

    easing->CreateSpline(&posX, width / 2 - 861 / 2, 3000, SplineType::QUART);
    return true;
}

bool SceneLogo::Update(Input* input, float dt)
{
    if (state == 0)
    {
        state = 1;
    }
    else if (state == 1)
    {
        logoAlpha += (LOGO_FADE_SPEED * dt);

        if (logoAlpha > 1.0f)
        {
            logoAlpha = 1.0f;
            state = 2;
        }
    }
    else if (state == 2)
    {
        // Waiting for 3 seconds
        timeCounter += dt;
        if (timeCounter >= 3.0f) state = 3;
    }
    else if (state == 3)
    {
        logoAlpha -= (LOGO_FADE_SPEED * dt);

        if (logoAlpha < 0.0f)
        {
            logoAlpha = 0.0f;
            TransitionToScene(SceneType::TITLE);
        }
    }

    if (input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN || input->GetControllerButton(CONTROLLER_BUTTON_A) == KeyState::KEY_DOWN)
        TransitionToScene(SceneType::TITLE);

    // Main title FX sounds just at title appearing
    if (logoFxTimer.ReadSec() >= 1.0f && logoFxTimer.ReadSec() < 1.1f)
    {
        audio->PlayFx(logoFx);
    }

    if (logoFxTimer.ReadSec() >= 2.0f && logoFxTimer.ReadSec() < 2.1f)
    {
        easing->CreateSpline(&posX, 2000, 6000, SplineType::BACK);
    }
    


    return true;
}

bool SceneLogo::Draw(Render* render)
{
    // Set texture alpha with the updated logoAlpha to accomplish fade in / fade out
    //SDL_SetTextureAlphaMod(logoTex, logoAlpha);
    render->DrawTexture(backgroundTex, 0, 0);
    render->DrawTexture(logoTex, posX, 200);
    return true;
}

bool SceneLogo::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
    tex->UnLoad(logoTex);
    tex->UnLoad(backgroundTex);
    audio->UnloadFx(logoFx);
    return true;
}