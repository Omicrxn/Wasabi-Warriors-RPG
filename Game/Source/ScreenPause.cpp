#include "ScreenPause.h"

#include "Easing.h"
#include "Window.h"

#include "Font.h"
#include "EntityManager.h"
#include "Player.h"
#include "QuestManager.h"

ScreenPause::ScreenPause()
{
	iconResume = nullptr;
	iconSettings = nullptr;
	iconExit = nullptr;

	pauseTitleRect = { 0, 73, 440, 75 };
	mobileRect = { 375, 339, 392, 603 };

	position = 700;

	currentAnimation = nullptr;
	menuFont = nullptr;
	questManager = nullptr;

	kenzoAnim.PushBack({ 795, 519, 162, 186 });
	kenzoAnim.PushBack({ 987, 519, 162, 186 });
	kenzoAnim.loop = true;
	kenzoAnim.speed = 0.004f;

	eikenAnim.PushBack({ 795, 732, 162, 192 });
	eikenAnim.PushBack({ 987, 732, 162, 192 });
	eikenAnim.loop = true;
	eikenAnim.speed = 0.004f;

	reiAnim.PushBack({ 795, 308, 162, 188 });
	reiAnim.PushBack({ 987, 308, 162, 188 });
	reiAnim.loop = true;
	reiAnim.speed = 0.004f;

	state = MobileState::MAIN;
}

ScreenPause::~ScreenPause()
{
}

bool ScreenPause::Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, EntityManager* entityManager, AudioManager* audio, Easing* easing, SDL_Texture* atlas0, SDL_Texture* atlas1, Font* font, int hoverFx, int clickFx)
{
	this->currentScene = currentScene;
	this->atlas[0] = atlas0;
	this->atlas[1] = atlas1;
	this->font = font;
	this->easing = easing;
	this->entityManager = entityManager;

	this->guiManager = guiManager;
	this->win = win;

	this->minIndex = minIndex;
	this->maxIndex = maxIndex;
	int counterId = minIndex;

	iconResume = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 609, position + 580, 65, 55 });
	iconResume->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, CONTROLLER_BUTTON_START, IconType::ICON_RESUME);
	iconResume->state = GuiControlState::HIDDEN;
	++counterId;

	iconSettings = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 490, position + 108, 58, 55 });
	iconSettings->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, CONTROLLER_BUTTON_X, IconType::ICON_SETTINGS);
	iconSettings->state = GuiControlState::HIDDEN;
	++counterId;

	iconExit = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 750, position + 108, 46, 55 });
	iconExit->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, CONTROLLER_BUTTON_B, IconType::ICON_EXIT);
	iconExit->state = GuiControlState::HIDDEN;
	++counterId;

	iconQuest = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 490, position + 435, 90, 90 });
	iconQuest->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, CONTROLLER_BUTTON_START, IconType::ICON_QUEST);
	iconQuest->state = GuiControlState::HIDDEN;
	++counterId;

	iconTeam = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 595, position + 435, 90, 90 });
	iconTeam->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, CONTROLLER_BUTTON_START, IconType::ICON_TEAM);
	iconTeam->state = GuiControlState::HIDDEN;
	++counterId;

	iconMap = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 710, position + 435, 90, 90 });
	iconMap->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, CONTROLLER_BUTTON_START, IconType::ICON_MAP);
	iconMap->state = GuiControlState::HIDDEN;

	return true;
}

bool ScreenPause::Update(Input* input, float dt, uint& focusedButtonId)
{
	return true;
}

bool ScreenPause::Draw(Render* render)
{
	// Draw pause background & title
	uint width, height;
	win->GetWindowSize(width, height);
	render->DrawRectangle({ 0,0, (int)width,(int)height }, { 0, 0, 0, 100 }, true, false);	
	//render->DrawTexture(guiAtlasTex, 1280 / 2 - pauseBackgroundRect.w / 2, 720 / 2 - pauseBackgroundRect.h / 2, &pauseBackgroundRect, 0.0f);
	// title : render->DrawTexture(atlas[1], 1280 / 2 - pauseTitleRect.w / 2, 100, &pauseTitleRect, 0.0f);
	render->DrawTexture(atlas[0], (1280 / 2 - mobileRect.w / 2) / render->scale, position / render->scale , &mobileRect, 0.0f);

	// Draw player profile anim
	if (state == MobileState::MAIN)
	{
		for (int i = 0; i < entityManager->playerList.Count(); ++i)
		{
			if (entityManager->playerList.At(i)->data->IsActive())
			{
				if (entityManager->playerList.At(i)->data->name == "Kenzo" && currentAnimation != &kenzoAnim)
				{
					currentAnimation = &kenzoAnim;
					currentAnimation->Reset();
				}
				else if (entityManager->playerList.At(i)->data->name == "Eiken" && currentAnimation != &eikenAnim)
				{
					currentAnimation = &eikenAnim;
					currentAnimation->Reset();
				}
				else if (entityManager->playerList.At(i)->data->name == "Rei" && currentAnimation != &reiAnim)
				{
					currentAnimation = &reiAnim;
					currentAnimation->Reset();
				}
			}

			if (currentAnimation != nullptr)
			{
				currentAnimation->Update();
				render->DrawTexture(atlas[0], 500, position + 135, &currentAnimation->GetCurrentFrame(), 0.0f);
			}
		}
	}
	
	pugi::xml_document docData;
	pugi::xml_parse_result result = docData.load_file("save_game.xml");
	pugi::xml_node docNode = docData.first_child().child("scenemanager").child("scenegameplay").child("gameProgress");

	if (state == MobileState::MAIN)
	{
		/*Draw gold and hud*/
		char s[24] = { 0 };

		// Gold HUD
		render->DrawText(menuFont, "$:", 688, position + 143, 50, 3, { 0,0,0,255 });
		render->DrawText(menuFont, "$:", 685, position + 140, 50, 3, { 249,215,28,255 });
		sprintf_s(s, 24, "%i", docNode.attribute("gold").as_int());
		render->DrawText(menuFont, s, 763, position + 143, 50, 3, { 0,0,0,255 });
		render->DrawText(menuFont, s, 760, position + 140, 50, 3, { 255,255,255,255 });

		// XP HUD
		render->DrawText(menuFont, "XP:", 688, position + 188, 50, 3, { 0,0,0,255 });
		render->DrawText(menuFont, "XP:", 685, position + 185, 50, 3, { 60,179,113,255 });
		sprintf_s(s, 24, "%i", docNode.attribute("xp").as_int());
		render->DrawText(menuFont, s, 763, position + 188, 50, 3, { 0,0,0,255 });
		render->DrawText(menuFont, s, 760, position + 185, 50, 3, { 255,255,255,255 });
	}
	else if (state == MobileState::QUEST)
	{
		questManager->PostUpdate();
	}
	else if (state == MobileState::TEAM)
	{
		for (int i = 0; i < entityManager->playerList.Count(); ++i)
		{
			if (entityManager->playerList.At(i)->data->IsActive())
			{
				// Draw player name
				render->DrawText(menuFont, entityManager->playerList.At(i)->data->name.GetString(), 588, position + 53, 50, 3, { 255,165,0,255 });

				// Draw player description from the wiki
				int offsetA = 20;
				if (entityManager->playerList.At(i)->data->name == "Kenzo")
				{
					render->DrawText(menuFont, "Young man from Osaka with passion", 490, position + 90 + offsetA, 19, 3, { 0,0,0,255 });
					render->DrawText(menuFont, "for cooking, but no resources.", 490, position + 105 + offsetA, 19, 3, { 0,0,0,255 });
					render->DrawText(menuFont, "Then he found Erika, the owner of the", 490, position + 120 + offsetA, 19, 3, { 0,0,0,255 });
					render->DrawText(menuFont, "best restaurant in the region, who ", 490, position + 135 + offsetA, 19, 3, { 0,0,0,255 });
					render->DrawText(menuFont, "gave Kenzo an opportunity.", 490, position + 150 + offsetA, 19, 3, { 0,0,0,255 });
					render->DrawText(menuFont, "Now Kenzo is a grateful cook in her", 490, position + 165 + offsetA, 19, 3, { 0,0,0,255 });
					render->DrawText(menuFont, "restaurant in Otaru.", 490, position + 180 + offsetA, 19, 3, { 0,0,0,255 });
				}
				else if (entityManager->playerList.At(i)->data->name == "Eiken")
				{
					render->DrawText(menuFont, "blablablabalbal", 500, position + 90, 15, 3, { 0,0,0,255 });
				}
				else if (entityManager->playerList.At(i)->data->name == "Rei")
				{
					render->DrawText(menuFont, "blablablabalbal", 500, position + 90, 15, 3, { 0,0,0,255 });
				}

				// Draw Player Stats
				int offsetB = 50;
				render->DrawText(font, "LVL:", 500, 290 + offsetB, 15, 2, { 255,165,0,255 });
				render->DrawText(font, "Damage:", 500, 310 + offsetB, 15, 2, { 255,165,0,255 });
				render->DrawText(font, "Max HP:", 500, 330 + offsetB, 15, 2, { 255,165,0,255 });
				render->DrawText(font, "HP:", 500, 350 + offsetB, 15, 2, { 255,165,0,255 });
				render->DrawText(font, "Strength:", 500, 370 + offsetB, 15, 2, { 255,165,0,255 });
				render->DrawText(font, "Defense:", 500, 390 + offsetB, 15, 2, { 255,165,0,255 });
				render->DrawText(font, "Atk Speed:", 500, 410 + offsetB, 15, 2, { 255,165,0,255 });
				render->DrawText(font, "CritRate:", 500, 430 + offsetB, 15, 2, { 255,165,0,255 });

				int posStatsNum = 630;
				char statsString[30] = { 0 };
				sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.level);
				render->DrawText(font, statsString, posStatsNum, 290 + offsetB, 15, 2, { 0,0,0,255 });

				sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.damage);
				render->DrawText(font, statsString, posStatsNum, 310 + offsetB, 15, 2, { 0,0,0,255 });

				sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.maxHP);
				render->DrawText(font, statsString, posStatsNum, 330 + offsetB, 15, 2, { 0,0,0,255 });

				sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.currentHP);
				render->DrawText(font, statsString, posStatsNum, 350 + offsetB, 15, 2, { 0,0,0,255 });

				sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.strength);
				render->DrawText(font, statsString, posStatsNum, 370 + offsetB, 15, 2, { 0,0,0,255 });

				sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.defense);
				render->DrawText(font, statsString, posStatsNum, 390 + offsetB, 15, 2, { 0,0,0,255 });

				sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.attackSpeed);
				render->DrawText(font, statsString, posStatsNum, 410 + offsetB, 15, 2, { 0,0,0,255 });

				sprintf_s(statsString, 30, "%.1f  ", entityManager->playerList.At(i)->data->stats.criticalRate);
				render->DrawText(font, statsString, posStatsNum, 430 + offsetB, 15, 2, { 0,0,0,255 });
			}
		}
	}
	
	return true;
}

bool ScreenPause::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
	guiManager->DestroyGuiControl(iconResume);
	iconResume = nullptr;
	guiManager->DestroyGuiControl(iconSettings);
	iconSettings = nullptr;
	guiManager->DestroyGuiControl(iconExit);
	iconExit = nullptr;

	guiManager->DestroyGuiControl(iconQuest);
	iconQuest = nullptr;
	guiManager->DestroyGuiControl(iconTeam);
	iconTeam = nullptr;
	guiManager->DestroyGuiControl(iconMap);
	iconMap = nullptr;

	return true;
}

void ScreenPause::Enable(bool isFromSettings)
{
	/* The position when you open the mobile*/
	if (isFromSettings == false)
	{
		easing->CreateSpline(&position, 50, 1500, SplineType::QUINT);

		easing->CreateSpline(&iconSettings->bounds.y, 108, 1500, SplineType::QUINT);
		easing->CreateSpline(&iconExit->bounds.y, 108, 1500, SplineType::QUINT);
		easing->CreateSpline(&iconResume->bounds.y, 580, 1500, SplineType::QUINT);

		easing->CreateSpline(&iconQuest->bounds.y, 435, 1500, SplineType::QUINT);
		easing->CreateSpline(&iconTeam->bounds.y, 435, 1500, SplineType::QUINT);
		easing->CreateSpline(&iconMap->bounds.y, 435, 1500, SplineType::QUINT);
	}
	else
	{
		position = 50;
		iconSettings->bounds.y = 108;
		iconExit->bounds.y = 108;
		iconResume->bounds.y = 580;
		iconQuest->bounds.y = 435;
		iconTeam->bounds.y = 435;
		iconMap->bounds.y = 435;
	}
	
	Screen::Enable();
}

void ScreenPause::Disable()
{
	/* The position when you close the mobile*/
	position = 700;
	iconSettings->bounds.y = position + 108;
	iconExit->bounds.y = position + 108;
	iconResume->bounds.y = position + 580;

	iconQuest->bounds.y = position + 435;
	iconTeam->bounds.y = position + 435;
	iconMap->bounds.y = position + 435;
	Screen::Disable();
}

void ScreenPause::SetMenuFont(Font* font)
{
	this->menuFont = font;
}

void ScreenPause::SetQuestManager(QuestManager* questManager)
{
	this->questManager = questManager;
}
