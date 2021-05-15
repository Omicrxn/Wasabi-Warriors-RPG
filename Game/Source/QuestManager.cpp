#include "QuestManager.h"
#include "Log.h"
#include "App.h"
#include "Module.h"
#include "External/PugiXml/src/pugixml.hpp"
#include "Player.h"
#include "List.h"
#include "Input.h"
#include "Render.h"
#include "Font.h"
#include "Textures.h"
#include "SDL/include/SDL_scancode.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Collisions.h"
#include "SceneGameplay.h"

#include <string>
#include <iostream>
using namespace std;

QuestManager::QuestManager(Input* input, Render* render, Textures* tex, SceneGameplay* sceneGameplay)
{
	// Needed modules
	this->input = input;
	this->render = render;
	this->tex = tex;
	this->sceneGameplay = sceneGameplay;
}

QuestManager::~QuestManager()
{
}

bool QuestManager::Start()
{
	font = new Font("Assets/Fonts/poppins.xml", tex);

	// ToDo 2: Load the xml file, parse the result, and make a node pointing to quests parent
	///////////////////////////////////////////////////////////////////////////
	pugi::xml_node questNode;
	pugi::xml_document questData;
	pugi::xml_parse_result parseResult = questData.load_file("quests.xml");
	if (parseResult == NULL)
		LOG("Could not load xml file <quests.xml> pugi error: %s", parseResult.description());
	else
		questNode = questData.child("quests");
	if (questNode == NULL)
		LOG("Could not load <questsData> xml_document");
	///////////////////////////////////////////////////////////////////////////

	questNode = questNode.child("quest");
	while (questNode != NULL)
	{
		Quest* quest = new Quest();

		quest->id = questNode.attribute("id").as_int();
		quest->type = questNode.attribute("type").as_int();
		quest->title = questNode.attribute("title").as_string();
		quest->description = questNode.attribute("description").as_string();
		quest->objective = questNode.attribute("objective").as_string();
		quest->quantity = questNode.attribute("quantity").as_int();
		quest->demandingNPC = questNode.attribute("demandingNPC").as_string();
		quest->rewardingNPC = questNode.attribute("rewardingNPC").as_string();
		quest->rewardXP = questNode.attribute("rewardXP").as_int();
		quest->rewardGold = questNode.attribute("rewardGold").as_int();
		quest->requiredId = questNode.attribute("requiredId").as_int();
		quest->isCompleted = questNode.attribute("isCompleted").as_bool();
		quest->status = questNode.attribute("status").as_int();

		// ToDo 3: Order the different lists (questsActive, questsInactive, questsFinished & questsList) with its respective quests (watch status)
		///////////////////////////////////////////////////////////////////////////
		if (quest->status == 0) // 0 inactive but loaded
		{
			questsInactive.Add(quest);
		}
		else if (quest->status == 1) // 1 active and loaded
		{
			questsActive.Add(quest);
		}
		else if (quest->status == 2) // 2 completed
		{
			questsFinished.Add(quest);
		}

		questsList.Add(quest);
		///////////////////////////////////////////////////////////////////////////

		questNode = questNode.next_sibling("quest");
	}
	return true;
}

bool QuestManager::Update(float dt)
{
	CheckQuestsLogic();
	CheckObjectivesCompletion();

	return true;
}

bool QuestManager::PostUpdate()
{
	DrawActiveQuests();
	return true;
}

bool QuestManager::CleanUp()
{
	questsList.Clear();
	questsInactive.Clear();
	questsActive.Clear();
	questsFinished.Clear();

	RELEASE(font);
	font = nullptr;

	return true;
}

// ToDo 3: Take a look at how quests are drawn into screen. Make the description of each quest be shown if user 
// presses L button (I am giving you each x, y, size, spacing and tint in order to prevent you to waste time)
// case 1: 300, 70, 45, 0, { 200,200,200,155 }	case 2: 200, 70, 45, 0, { 200,200,200,155 }
// case 3: 245, 110, 45, 0, { 200,200,200,155 } case 4: 0, 140, 45, 0, { 200,200,200,155 }
// case 5: 0, 100, 45, 0, { 200,200,200,155 }	case 6: 0, 100, 45, 0, { 200,200,200,155 }
///////////////////////////////////////////////////////////////////////////
bool QuestManager::DrawActiveQuests()
{
	string numToStr;
	const char* numToStr2;
	ListItem<Quest*>* L = questsActive.start;
	while (L != NULL)
	{
		switch (L->data->id)
		{
		case 1: // new quest chain 1
			// Title Drawing
			render->DrawText(font, L->data->title.GetString(), 53, 223, 50, 3, { 0,0,0,255 });
			render->DrawText(font, L->data->title.GetString(), 50, 220, 50, 3, { 255,255,255,255 });

			// Description Drawing if pressed L
			///////////////////////////////////////////////////////////////////////////
			if (input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
				render->DrawText(font, L->data->description.GetString(), 350, 230, 35, 3, { 200,200,200,255 });
			///////////////////////////////////////////////////////////////////////////

			break;
		case 2: // quest chain 1 (quest 2)
			// Title Drawing
			render->DrawText(font, L->data->title.GetString(), 53, 223, 50, 3, { 0,0,0,255 });
			render->DrawText(font, L->data->title.GetString(), 50, 220, 50, 3, { 255,255,255,255 });

			// Description Drawing if pressed L
			///////////////////////////////////////////////////////////////////////////
			if (input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
				render->DrawText(font, L->data->description.GetString(), 350, 230, 35, 3, { 200,200,200,255 });
			///////////////////////////////////////////////////////////////////////////
			break;

		case 3: // new quest chain 1
			// Title Drawing
			render->DrawText(font, L->data->title.GetString(), 53, 223, 50, 3, { 0,0,0,255 });
			render->DrawText(font, L->data->title.GetString(), 50, 220, 50, 3, { 255,255,255,255 });

			// Description Drawing if pressed L
			///////////////////////////////////////////////////////////////////////////
			if (input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
				render->DrawText(font, L->data->description.GetString(), 350, 230, 35, 3, { 200,200,200,255 });
			///////////////////////////////////////////////////////////////////////////
			break;
		case 4: // new quest chain 1
			// Title Drawing
			render->DrawText(font, L->data->title.GetString(), 53, 223, 50, 3, { 0,0,0,255 });
			render->DrawText(font, L->data->title.GetString(), 50, 220, 50, 3, { 255,255,255,255 });

			// Description Drawing if pressed L
			///////////////////////////////////////////////////////////////////////////
			if (input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
				render->DrawText(font, L->data->description.GetString(), 350, 230, 35, 3, { 200,200,200,255 });
			///////////////////////////////////////////////////////////////////////////
			break;
		default:
			break;
		}

		L = L->next;
	}
	return true;
}
///////////////////////////////////////////////////////////////////////////

bool QuestManager::CheckQuestsLogic()
{
	// For changing from one list to another (has finished quest)
	// ToDo 5: Implement code that passes a completed quest from the active list to the finished one
	// (remember to give rewards --> look at 2 functions of ModulePlayer)
	/////////////////////////////////////////////////////////////////////////////
	ListItem<Quest*>* activeQuestsList = questsActive.start;
	while (activeQuestsList != nullptr)
	{
		if (activeQuestsList->data->isCompleted == true)
		{
			activeQuestsList->data->status = 2;
			questsActive.Del(activeQuestsList);
			questsFinished.Add(activeQuestsList->data);
			sceneGameplay->RewardXP(activeQuestsList->data->rewardXP);
			sceneGameplay->RewardGold(activeQuestsList->data->rewardGold);
		}

		activeQuestsList = activeQuestsList->next;
	}
	/////////////////////////////////////////////////////////////////////////////

	// Quest chain logic (if required quest is completed)
	// ToDo 6: Implement the code that gives a basic chainquest logic. If an id in finished list meets the
	// requiredId from the inactive list, do the corresponding changes 
	/////////////////////////////////////////////////////////////////////////////
	ListItem<Quest*>* inactiveQuestsList = questsInactive.start;
	while (inactiveQuestsList != NULL)
	{
		if (inactiveQuestsList->data->requiredId != 0)
		{
			ListItem<Quest*>* L2 = questsFinished.start;
			while (L2 != NULL)
			{
				if (inactiveQuestsList->data->requiredId == L2->data->id)
				{
					questsActive.Add(inactiveQuestsList->data);
					questsInactive.Del(inactiveQuestsList);
					inactiveQuestsList->data->status = 1;
				}
				L2 = L2->next;
			}
		}

		inactiveQuestsList = inactiveQuestsList->next;
	}
	/////////////////////////////////////////////////////////////////////////////

	// Complex ChainQuests Hardcoded to put it at active list
	/*if (sceneGameplay->player->chopTreeCount == 10 && app->player->turtleKilled)
	{
		ListItem<Quest*>* chainQuestLookingList = questsInactive.start;
		while (chainQuestLookingList != NULL)
		{
			if (chainQuestLookingList->data->id == 5)
			{
				questsActive.Add(chainQuestLookingList->data);
				questsInactive.Del(chainQuestLookingList);
				chainQuestLookingList->data->status = 1;
			}
			chainQuestLookingList = chainQuestLookingList->next;
		}
	}*/

	return true;
}

bool QuestManager::CheckObjectivesCompletion()
{
	// Debug: Complete quest with id selected
	if (input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		ListItem<Entity*>* L;
		switch (debugId)
		{
		case 1: // Quest 1
			sceneGameplay->GetGameProgress()->hasSpoken = true;
			++debugId;
			break;
		case 2: // Quest 2
			sceneGameplay->GetGameProgress()->hasKilledOfficers = true;
			++debugId;
			break;
		case 3: // Quest 3
			sceneGameplay->GetGameProgress()->hasActivated = true;
			++debugId;
			break;
		case 4: // Quest 4
			sceneGameplay->GetGameProgress()->hasKilledBoss = true;
			++debugId;
			break;
		case 5:
			/*app->player->snailDelivered = true;
			L = app->entities->entities.start;
			while (L != NULL)
			{
				if (L->data->entityType == EntityType::ITEM_SNAIL)
					app->entities->entities.Del(L);
				L = L->next;
			}
			++debugId;*/
			break;
		case 6:
			/*app->player->monsterKilled = true;
			L = app->entities->entities.start;
			while (L != NULL)
			{
				if (L->data->entityType == EntityType::MONSTER)
					app->entities->entities.Del(L);
				L = L->next;
			}
			++debugId;*/
		default:
			break;
		}
	}

	// ToDo 8: Go to the xml and read about what the conditions of each quests is
	// All variables are declared and managed by the ModulePlayer
	/////////////////////////////////////////////////////////////////////////////
	if (sceneGameplay->GetGameProgress()->hasSpoken == true)
		CompleteQuest(1);

	if (sceneGameplay->GetGameProgress()->hasKilledOfficers == true)
		CompleteQuest(2);

	if (sceneGameplay->GetGameProgress()->hasActivated == true)
		CompleteQuest(3);

	if (sceneGameplay->GetGameProgress()->hasKilledBoss == true)
		CompleteQuest(4);

	/*if (app->player->snailDelivered == true)
		CompleteQuest(5);

	if (app->player->monsterKilled == true)
		CompleteQuest(6);*/
	/////////////////////////////////////////////////////////////////////////////

	return true;
}

// ToDo 7: Fulfill the CompleteQuest function that given a quest id parameter, changes the variable
// isCompleted to true if that id meets another same id within the whole list of quests
/////////////////////////////////////////////////////////////////////////////
bool QuestManager::CompleteQuest(int id)
{
	ListItem<Quest*>* L = questsList.start;
	while (L != nullptr)
	{
		if (id == L->data->id)
		{
			L->data->isCompleted = true;
		}
		L = L->next;
	}
	return true;
}
/////////////////////////////////////////////////////////////////////////////