#include "SceneEnding.h"

#include "Render.h"

#include "SDL/include/SDL.h"

SceneEnding::SceneEnding()
{
    this->name = "sceneending";
}

SceneEnding::~SceneEnding()
{
}

bool SceneEnding::Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager)
{
    return false;
}

bool SceneEnding::Update(Input* input, float dt)
{
    return false;
}

bool SceneEnding::Draw(Render* render)
{
    SDL_Rect rec = { 0, 0, 1280, 720 };
    SDL_Color color = { 0, 0, 255, 255 };
    render->DrawRectangle(rec, color);

    return false;
}

bool SceneEnding::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
    return false;
}

