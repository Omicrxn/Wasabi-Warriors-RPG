#include "ScreenPause.h"

#include "Easing.h"
#include "Window.h"

#include "Font.h"
#include "EntityManager.h"
#include "Player.h"
#include "QuestManager.h"
#include "SceneGameplay.h"

ScreenPause::ScreenPause()
{
	iconReturn = nullptr;
	iconSettings = nullptr;
	iconExit = nullptr;
	iconQuest = nullptr;
	iconTeam = nullptr;
	iconMap = nullptr;

	mobileRect = { 375, 339, 392, 603 };

	mapRect = { 24, 1602, 330, 477 };
	redDot = { 756, 288, 24, 24 };

	position = 700;
	mobRelativePos = (1280 / 2 - mobileRect.w / 2);
	currentAnimation = nullptr;
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
	
	makiAnim.PushBack({ 1428, 1305, 138, 156 });
	makiAnim.PushBack({ 1581, 1305, 138, 156 });
	makiAnim.PushBack({ 1734, 1305, 138, 156 });
	makiAnim.loop = true;
	makiAnim.speed = 0.004f;

	mapHoverPos[0] = {139, 77};
	mapHoverPos[1] = {147, 273};
	mapHoverPos[2] = {117, 406};
	mapHoverPos[3] = {253, 406 };
	mapHoverPos[4] = {276, 309 };

	for (int i = 0; i < 5; ++i)
		isMapHover[i] = false;
	isMapHover[0] = true;

	currentMap = MapType::NONE;
	state = MobileState::MAIN;
}

ScreenPause::~ScreenPause()
{
}

bool ScreenPause::Load(int minIndex, int maxIndex, Scene* currentScene, Window* win, GuiManager* guiManager, EntityManager* entityManager, AudioManager* audio, Easing* easing, SDL_Texture* atlas0, Font* font, int hoverFx, int clickFx)
{
	this->currentScene = currentScene;
	this->atlas[0] = atlas0;
	this->font = font;
	this->easing = easing;

	this->entityManager = entityManager;
	this->audio = audio;
	this->guiManager = guiManager;
	this->win = win;

	this->hoverFx = hoverFx;

	this->minIndex = minIndex;
	this->maxIndex = maxIndex;
	int counterId = minIndex;

	iconSettings = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 490, position + 108, 58, 55 });
	iconSettings->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, IconType::ICON_SETTINGS);
	iconSettings->state = GuiControlState::HIDDEN;
	++counterId;

	iconExit = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 750, position + 108, 46, 55 });
	iconExit->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, IconType::ICON_EXIT);
	iconExit->state = GuiControlState::HIDDEN;
	++counterId;

	iconQuest = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 490, position + 435, 90, 90 });
	iconQuest->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, IconType::ICON_QUEST);
	iconQuest->state = GuiControlState::HIDDEN;
	++counterId;

	iconTeam = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 590, position + 435, 90, 90 });
	iconTeam->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, IconType::ICON_TEAM);
	iconTeam->state = GuiControlState::HIDDEN;
	++counterId;

	iconMap = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 710, position + 435, 90, 90 });
	iconMap->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, IconType::ICON_MAP);
	iconMap->state = GuiControlState::HIDDEN;
	++counterId;

	iconReturn = (GuiIcon*)guiManager->CreateGuiControl(GuiControlType::ICON, counterId, { 609, position + 580, 54, 54 });
	iconReturn->SetIconProperties(currentScene, atlas[0], font, hoverFx, clickFx, IconType::ICON_RETURN);
	iconReturn->state = GuiControlState::HIDDEN;

	return true;
}

bool ScreenPause::Update(Input* input, float dt, uint& focusedButtonId)
{
	if (state == MobileState::MAP)
	{
		int mouseX, mouseY;
		input->GetMousePosition(mouseX, mouseY);

		isMapHover[hovering] = true;

		for (int i = 0; i < 5; ++i)
		{
			if (PointCircleCollCheck(mouseX, mouseY, mobRelativePos + 30 + mapHoverPos[i].x, position + 39 + mapHoverPos[i].y, 40))
			{
				if (isMapHover[i] == false)
					audio->PlayFx(hoverFx);
				isMapHover[i] = true;
				hovering = i;
			}
			else
			{
				if (hovering == i)
					continue;
				isMapHover[i] = false;
			}

		}

		if ((input->GetKey(SDL_SCANCODE_UP) == KeyState::KEY_DOWN || input->GetControllerButton(CONTROLLER_BUTTON_UP) == KeyState::KEY_DOWN) && hovering > 0)
		{
			--hovering;
			audio->PlayFx(hoverFx);
		}
		else if ((input->GetKey(SDL_SCANCODE_DOWN) == KeyState::KEY_DOWN || input->GetControllerButton(CONTROLLER_BUTTON_DOWN) == KeyState::KEY_DOWN) && hovering < 4)
		{
			++hovering;
			audio->PlayFx(hoverFx);
		}
	}

	return true;
}

bool ScreenPause::Draw(Render* render)
{
	// Draw pause background & title
	uint width, height;
	win->GetWindowSize(width, height);

	render->DrawRectangle({ 0,0, (int)width,(int)height }, { 0, 0, 0, 70 }, true, false);

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
				else if (entityManager->playerList.At(i)->data->name == "Maki" && currentAnimation != &makiAnim)
				{
					currentAnimation = &makiAnim;
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
		/* Draw gold and hud */
		char s[24] = { 0 };

		// Gold HUD
		render->DrawText(font, "$:", 683, position + 142, 50, 3, { 0,0,0,255 });
		render->DrawText(font, "$:", 680, position + 140, 50, 3, { 249,215,28,255 });
		sprintf_s(s, 24, "%i", docNode.attribute("gold").as_int());
		render->DrawText(font, s, 743, position + 142, 50, 3, { 0,0,0,255 });
		render->DrawText(font, s, 740, position + 140, 50, 3, { 255,255,255,255 });

		// XP HUD
		render->DrawText(font, "XP:", 673, position + 187, 50, 3, { 0,0,0,255 });
		render->DrawText(font, "XP:", 670, position + 185, 50, 3, { 60,179,113,255 });
		sprintf_s(s, 24, "%i", docNode.attribute("xp").as_int());
		render->DrawText(font, s, 740, position + 187, 50, 3, { 0,0,0,255 });
		render->DrawText(font, s, 737, position + 185, 50, 3, { 255,255,255,255 });
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
				render->DrawText(font, entityManager->playerList.At(i)->data->name.GetString(), 588, position + 53, 50, 3, { 255,165,0,255 });

				// Draw player description from the wiki
				int offsetA = 20;
				if (entityManager->playerList.At(i)->data->name == "Kenzo")
				{
					render->DrawText(font, "Young man from Osaka with", 490, position + 90 + offsetA, 20, 3, { 0,0,0,255 });
					render->DrawText(font, "passion for cooking, but", 490, position + 105 + offsetA, 20, 3, { 0,0,0,255 });
					render->DrawText(font, "no resources. Then he found", 490, position + 120 + offsetA, 20, 3, { 0,0,0,255 });
					render->DrawText(font, "Erika, the owner of the best", 490, position + 135 + offsetA, 20, 3, { 0,0,0,255 });
					render->DrawText(font, "restaurant in the region, who", 490, position + 150 + offsetA, 20, 3, { 0,0,0,255 });
					render->DrawText(font, "gave Kenzo an opportunity. ", 490, position + 165 + offsetA, 20, 3, { 0,0,0,255 });
					render->DrawText(font, "Now Kenzo is a grateful cook", 490, position + 180 + offsetA, 20, 3, { 0,0,0,255 });
					render->DrawText(font, "in her Kanafawa restaurant.", 490, position + 195 + offsetA, 20, 3, { 0,0,0,255 });
				}
				else if (entityManager->playerList.At(i)->data->name == "Eiken")
				{
					render->DrawText(font, "Skilled and faithful chef", 490, position + 90 + offsetA, 20, 3, { 0,0,0,255 });
					render->DrawText(font, "who was Erika's apprentice.", 490, position + 105 + offsetA, 20, 3, { 0,0,0,255 });
					render->DrawText(font, "He keeps a key to open the", 490, position + 120 + offsetA, 20, 3, { 0,0,0,255 });
					render->DrawText(font, "vault of the recipe that", 490, position + 135 + offsetA, 20, 3, { 0,0,0,255 });
					render->DrawText(font, "Kenzo is searching for.", 490, position + 150 + offsetA, 20, 3, { 0,0,0,255 });
				}
				else if (entityManager->playerList.At(i)->data->name == "Rei")
				{
					render->DrawText(font, "Professional and brilliant previous", 490, position + 90 + offsetA, 20, 3, { 0,0,0,255 });
					render->DrawText(font, "Erika's apprentice and head chef ", 490, position + 105 + offsetA, 20, 3, { 0,0,0,255 });
					render->DrawText(font, "of the restaurant in Dotonbori.", 490, position + 120 + offsetA, 20, 3, { 0,0,0,255 });
					render->DrawText(font, "He has the last key of the", 490, position + 135 + offsetA, 20, 3, { 0,0,0,255 });
					render->DrawText(font, "receipe vault", 490, position + 150 + offsetA, 20, 3, { 0,0,0,255 });
				}
				else if (entityManager->playerList.At(i)->data->name == "Maki")
				{
					render->DrawText(font, "Kenzo's dog and best friend.", 490, position + 90 + offsetA, 20, 3, { 0,0,0,255 });
					render->DrawText(font, "A very smart and faithful dog", 490, position + 105 + offsetA, 20, 3, { 0,0,0,255 });
					render->DrawText(font, "that will follow Kenzo everywhere.", 490, position + 120 + offsetA, 20, 3, { 0,0,0,255 });
					render->DrawText(font, "He is very well trained and loves", 490, position + 135 + offsetA, 20, 3, { 0,0,0,255 });
					render->DrawText(font, "Kenzo like crazy.", 490, position + 150 + offsetA, 20, 3, { 0,0,0,255 });
				}

				// Draw Player Stats
				int offsetB = 50;
				render->DrawText(font, "LVL:", 500, 290 + offsetB, 25, 2, { 255,165,0,255 });
				render->DrawText(font, "Damage:", 500, 310 + offsetB, 25, 2, { 255,165,0,255 });
				render->DrawText(font, "Max HP:", 500, 330 + offsetB, 25, 2, { 255,165,0,255 });
				render->DrawText(font, "HP:", 500, 350 + offsetB, 25, 2, { 255,165,0,255 });
				render->DrawText(font, "Strength:", 500, 370 + offsetB, 25, 2, { 255,165,0,255 });
				render->DrawText(font, "Defense:", 500, 390 + offsetB, 25, 2, { 255,165,0,255 });
				render->DrawText(font, "Atk Speed:", 500, 410 + offsetB, 25, 2, { 255,165,0,255 });
				render->DrawText(font, "CritRate:", 500, 430 + offsetB, 25, 2, { 255,165,0,255 });

				int posStatsNum = 630;
				char statsString[30] = { 0 };
				sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.level);
				render->DrawText(font, statsString, posStatsNum, 290 + offsetB, 25, 2, { 0,0,0,255 });

				sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.damage);
				render->DrawText(font, statsString, posStatsNum, 310 + offsetB, 25, 2, { 0,0,0,255 });

				sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.maxHP);
				render->DrawText(font, statsString, posStatsNum, 330 + offsetB, 25, 2, { 0,0,0,255 });

				sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.currentHP);
				render->DrawText(font, statsString, posStatsNum, 350 + offsetB, 25, 2, { 0,0,0,255 });

				sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.strength);
				render->DrawText(font, statsString, posStatsNum, 370 + offsetB, 25, 2, { 0,0,0,255 });

				sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.defense);
				render->DrawText(font, statsString, posStatsNum, 390 + offsetB, 25, 2, { 0,0,0,255 });

				sprintf_s(statsString, 30, "%i  ", entityManager->playerList.At(i)->data->stats.attackSpeed);
				render->DrawText(font, statsString, posStatsNum, 410 + offsetB, 25, 2, { 0,0,0,255 });

				sprintf_s(statsString, 30, "%.1f  ", entityManager->playerList.At(i)->data->stats.criticalRate);
				render->DrawText(font, statsString, posStatsNum, 430 + offsetB, 25, 2, { 0,0,0,255 });
			}
		}
	}
	else if (state == MobileState::MAP)
	{
		render->DrawTexture(atlas[0], mobRelativePos + 30 / render->scale, position + 39 / render->scale, &mapRect, 0.0f);
		render->DrawText(font, "RESTAURANT", mobRelativePos + 155, position + 39 + 95, 10, 2, { 255,255,255,255 });
		render->DrawText(font, "RESTAURANT", mobRelativePos + 209, position + 39 + 263, 10, 2, { 255,255,255,255 });
		render->DrawText(font, "RESTAURANT", mobRelativePos + 205, position + 39 + 404, 10, 2, { 0,0,0,255 });

		switch (currentMap)
		{
		case MapType::NONE:
			break;
		case MapType::CEMETERY: // CEMETERY
			render->DrawTexture(atlas[0], mobRelativePos + mapHoverPos[4].x - redDot.w / 2 + 30, position + mapHoverPos[4].y - redDot.h / 2 + 30, &redDot, 0.0f);
			break;
		case MapType::HOUSE: // SHINSEKAI
			render->DrawTexture(atlas[0], mobRelativePos + mapHoverPos[2].x - redDot.w / 2 + 30, position + mapHoverPos[2].y - redDot.h / 2 + 30, &redDot, 0.0f);
			break;
		case MapType::KANAGAWA: // KANAGAWA
			render->DrawTexture(atlas[0], mobRelativePos + mapHoverPos[3].x - redDot.w / 2 + 30, position + mapHoverPos[3].y - redDot.h / 2 + 30, &redDot, 0.0f);
			break;
		case MapType::RESTAURANT:
			break;
		case MapType::SECOND_RESTAURANT:
			break;
		case MapType::THIRD_RESTAURANT:
			break;
		case MapType::TOWN: // SHINSEKAI
			render->DrawTexture(atlas[0], mobRelativePos + mapHoverPos[2].x - redDot.w / 2 + 30, position + mapHoverPos[2].y - redDot.h / 2 + 30, &redDot, 0.0f);
			break;
		case MapType::DOTONBORI: // DOTONBORI
			render->DrawTexture(atlas[0], mobRelativePos + mapHoverPos[0].x - redDot.w / 2 + 30, position + mapHoverPos[0].y - redDot.h / 2 + 30, &redDot, 0.0f);
			break;
		case MapType::SKYSCRAPER: // DOTONBORI
			render->DrawTexture(atlas[0], mobRelativePos + mapHoverPos[0].x - redDot.w / 2 + 30, position + mapHoverPos[0].y - redDot.h / 2 + 30, &redDot, 0.0f);
			break;
		case MapType::SECRET_ROOM:
			break;
		case MapType::OSAKA: // OSAKA
			render->DrawTexture(atlas[0], mobRelativePos + mapHoverPos[1].x - redDot.w / 2 + 30, position + mapHoverPos[1].y - redDot.h / 2 + 30, &redDot, 0.0f);
			break;
		default:
			break;
		}

		for (int i = 0; i < 5; ++i)
		{
			if (isMapHover[i])
			{
				/*render->DrawCircle(mobRelativePos + 30 + mapHoverPos[i].x, position + 39 + mapHoverPos[i].y, 40, { 255, 0, 0, 255 });*/
				switch (i)
				{
				case 0:
					render->DrawText(font, "DOTONBORI", mobRelativePos - 70 + mapHoverPos[i].x, position + mapHoverPos[i].y, 30, 2, { 255,255,255,255 });
					break;
				case 1:
					render->DrawText(font, "OSAKA", mobRelativePos - 70 + mapHoverPos[i].x, position + 15 + mapHoverPos[i].y, 30, 2, { 255,255,255,255 });
					break;
				case 2:
					render->DrawText(font, "SHINSEKAI", mobRelativePos - 70 + mapHoverPos[i].x, position + 39 + mapHoverPos[i].y, 30, 2, { 255,255,255,255 });
					break;
				case 3:
					render->DrawText(font, "KANAGAWA", mobRelativePos - 70 + mapHoverPos[i].x, position + 50 + mapHoverPos[i].y, 30, 2, { 255,255,255,255 });
					break;
				case 4:
					render->DrawText(font, "CEMETERY", mobRelativePos - 75 + mapHoverPos[i].x, position + 39 + mapHoverPos[i].y, 30, 2, { 255,255,255,255 });
					break;
				default:
					break;
				}
			}
			/*else
				render->DrawCircle(mobRelativePos + 30 + mapHoverPos[i].x, position + 39 + mapHoverPos[i].y, 40, { 0, 255, 0, 255 });*/
		}
	}
	
	return true;
}

bool ScreenPause::Unload(Textures* tex, AudioManager* audio, GuiManager* guiManager)
{
	guiManager->DestroyGuiControl(iconReturn);
	guiManager->DestroyGuiControl(iconSettings);
	guiManager->DestroyGuiControl(iconExit);

	guiManager->DestroyGuiControl(iconQuest);
	guiManager->DestroyGuiControl(iconTeam);
	guiManager->DestroyGuiControl(iconMap);

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
		easing->CreateSpline(&iconReturn->bounds.y, 580, 1500, SplineType::QUINT);

		easing->CreateSpline(&iconQuest->bounds.y, 435, 1500, SplineType::QUINT);
		easing->CreateSpline(&iconTeam->bounds.y, 435, 1500, SplineType::QUINT);
		easing->CreateSpline(&iconMap->bounds.y, 435, 1500, SplineType::QUINT);
	}
	else
	{
		position = 50;

		iconSettings->bounds.y = 108;
		iconExit->bounds.y = 108;
		iconReturn->bounds.y = 580;
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
	iconReturn->bounds.y = position + 580;

	iconQuest->bounds.y = position + 435;
	iconTeam->bounds.y = position + 435;
	iconMap->bounds.y = position + 435;

	Screen::Disable();
}

void ScreenPause::SetQuestManager(QuestManager* questManager)
{
	this->questManager = questManager;
}

void ScreenPause::SetMap(MapType map)
{
	currentMap = map;
}

bool ScreenPause::PointCircleCollCheck(float px, float py, float cx, float cy, float r) {

	// get distance between the point and circle's center
	// using the Pythagorean Theorem
	float distX = px - cx;
	float distY = py - cy;
	float distance = sqrt((distX * distX) + (distY * distY));

	// if the distance is less than the circle's
	// radius the point is inside!
	if (distance <= r) 
	{
		return true;
	}

	return false;
}