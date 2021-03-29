#include "SceneBattle.h"

#include "Textures.h"
#include "Render.h"

SceneBattle::SceneBattle()
{
	type = SceneType::BATTLE;
}

SceneBattle::~SceneBattle()
{
}

bool SceneBattle::Load(Textures* tex, Window* win, AudioManager* audio, GuiManager* guiManager, EntityManager* entityman)
{
	return true;
}

bool SceneBattle::Update(Input* input, float dt)
{
	return false;
}

bool SceneBattle::Draw(Render* render)
{
	render->DrawRectangle({ 0, 0, 1280, 720 }, { 255, 255, 255, (unsigned char)255.0f }, true, false);
	return true;
}

bool SceneBattle::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
	return false;
}
