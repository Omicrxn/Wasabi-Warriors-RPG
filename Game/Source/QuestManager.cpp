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

QuestManager::QuestManager(Input* input, Render* render, Textures* tex, SceneGameplay* sceneGameplay, AssetsManager* assetsManager)
{
	// Needed modules
	this->input = input;
	this->render = render;
	this->assetsManager = assetsManager;
	this->tex = tex;
	this->sceneGameplay = sceneGameplay;
}

QuestManager::~QuestManager()
{
}

bool QuestManager::Start()
{
	font = new Font("Fonts/comic_serif.xml", tex, assetsManager);

	// ToDo 2: Load the xml file, parse the result, and make a node pointing to quests parent
	///////////////////////////////////////////////////////////////////////////
	pugi::xml_node questNode;
	pugi::xml_document questData;

	char* buffer = nullptr;
	size_t size = assetsManager->LoadXML("Quests/quests.xml", &buffer);
	pugi::xml_parse_result parseResult = questData.load_buffer(buffer, size);
	RELEASE_ARRAY(buffer);

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

	notifier = Notifier::GetInstance();
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

		if (L->data->id == 2)
		{
			render->DrawText(font, "QUEST #1", 485, 120, 50, 3, { 252, 185, 109 });
			render->DrawText(font, "Go to Erika's", 555, 193, 30, 3, { 0, 0, 0, 255 });
			render->DrawText(font, "restaurant in", 555, 223, 30, 3, { 0, 0, 0, 255 });
			render->DrawText(font, "Kanagawa", 565, 253, 30, 3, { 0, 0, 0, 255 });

			render->DrawText(font, "You just found out that", 485, 300, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "Erika is dead.  ", 485, 330, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "The restaurants need you to", 485, 360, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "learn her secret recipe to ", 485, 390, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "keep working.", 485, 420, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "Go to the restaurant to", 485, 450, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "check the vault", 485, 480, 23, 3, { 0,0,0,255 });
		}

		if (L->data->id == 3)
		{
			render->DrawText(font, "QUEST #2", 485, 120, 50, 3, { 252, 185, 109 });
			render->DrawText(font, "Visit Erika's", 555, 193, 30, 3, { 0, 0, 0, 255 });
			render->DrawText(font, "tombstone", 555, 223, 30, 3, { 0, 0, 0, 255 });

			render->DrawText(font, "You should go pay respects", 485, 273, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "to Erika.", 485, 303, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "Go to the cemetery.", 485, 333, 23, 3, { 0,0,0,255 });
		}

		if (L->data->id == 4)
		{
			render->DrawText(font, "QUEST #3", 485, 120, 50, 3, { 252, 185, 109 });
			render->DrawText(font, "Find Erika's", 560, 193, 30, 3, { 0, 0, 0, 255 });
			render->DrawText(font, "apprentice", 560, 223, 30, 3, { 0, 0, 0, 255 });
			render->DrawText(font, "in Osaka", 565, 253, 30, 3, { 0, 0, 0, 255 });

			render->DrawText(font, "You need the next key to", 485, 300, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "the vault.", 485, 330, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "Go to Osaka to get it and", 485, 360, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "prepare to fight whatever", 485, 390, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "hitmen Takada sent there", 485, 420, 23, 3, { 0,0,0,255 });

		}

		if (L->data->id == 5)
		{
			render->DrawText(font, "QUEST #4", 485, 120, 50, 3, { 252, 185, 109 });
			render->DrawText(font, "Find Erika's", 560, 193, 30, 3, { 0, 0, 0, 255 });
			render->DrawText(font, "apprentice", 560, 223, 30, 3, { 0, 0, 0, 255 });
			render->DrawText(font, "in Dotonbori", 565, 253, 30, 3, { 0, 0, 0, 255 });

			render->DrawText(font, "You need the next key to", 485, 300, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "the vault.", 485, 330, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "Go to Dotonbori to get it", 485, 360, 23, 3, { 0,0,0,255 });
		}

		if (L->data->id == 6)
		{
			render->DrawText(font, "QUEST #5", 485, 120, 50, 3, { 252, 185, 109 });
			render->DrawText(font, "Takada Offices", 560, 193, 30, 3, { 0, 0, 0, 255 });

			render->DrawText(font, "Get past the recepcion", 485, 240, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "in Takada's building.", 485, 270, 23, 3, { 0,0,0,255 });
		}

		if (L->data->id == 7)
		{
			render->DrawText(font, "QUEST #6", 485, 120, 50, 3, { 252, 185, 109 });
			render->DrawText(font, "Get the Key", 560, 193, 30, 3, { 0, 0, 0, 255 });

			render->DrawText(font, "Defeat the common workers.", 485, 240, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "One of them must have the", 485, 270, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "key to the higher floors", 485, 300, 23, 3, { 0,0,0,255 });
		}

		if (L->data->id == 8)
		{
			render->DrawText(font, "QUEST #7", 485, 120, 50, 3, { 252, 185, 109 });
			render->DrawText(font, "Turn on the", 555, 193, 30, 3, { 0, 0, 0, 255 });
			render->DrawText(font, "electricity", 555, 223, 30, 3, { 0, 0, 0, 255 });

			render->DrawText(font, "Defeat the workers and get", 485, 273, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "to the computers to ", 485, 303, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "reactivate the electricity.", 485, 333, 23, 3, { 0,0,0,255 });
		}

		if (L->data->id == 9)
		{
			render->DrawText(font, "QUEST #8", 485, 120, 50, 3, { 252, 185, 109 });
			render->DrawText(font, "Defeat Takada", 560, 193, 30, 3, { 0, 0, 0, 255 });

			render->DrawText(font, "Face Takada in a battle", 485, 240, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "and avenge Erika", 485, 270, 23, 3, { 0,0,0,255 });
		}

		else {
			// Title Drawing
			render->DrawText(font, L->data->title.GetString(), 480, 223, 50, 3, { 0,0,0,255 });
			render->DrawText(font, L->data->title.GetString(), 480, 220, 50, 3, { 255,255,255,255 });

			// Description Drawing if pressed L
			///////////////////////////////////////////////////////////////////////////
			render->DrawText(font, L->data->description.GetString(), 480, 300, 23, 3, { 0,0,0,255 });
			///////////////////////////////////////////////////////////////////////////
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

			questsFinished.Add(activeQuestsList->data);
			sceneGameplay->RewardXP(activeQuestsList->data->rewardXP);
			sceneGameplay->RewardGold(activeQuestsList->data->rewardGold);
			questsActive.Del(activeQuestsList);
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
					// Notify new quest
					notifier->SendMobileNotification("New quest added! Check it on your phone.");
					questsActive.Add(inactiveQuestsList->data);
					inactiveQuestsList->data->status = 1;
					questsInactive.Del(inactiveQuestsList);
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
	if (input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		ListItem<Entity*>* L;
		switch (debugId)
		{
		case 1: // Quest 1
			sceneGameplay->GetGameProgress()->hasCalledCook = true;
			++debugId;
			break;
		case 2: // Quest 2
			sceneGameplay->GetGameProgress()->hasVisitedRestaurantQuest = true;
			++debugId;
			break;
		case 3: // Quest 3
			sceneGameplay->GetGameProgress()->hasVisitedErikaTombstone = true;
			++debugId;
			break;
		case 4: // Quest 4
			sceneGameplay->GetGameProgress()->hasSavedFirstApprentice = true;
			++debugId;
			break;
		case 5: // Quest 5
			sceneGameplay->GetGameProgress()->hasSavedLastApprentice = true;
			++debugId;
			break;
		case 6: // Quest 6
			sceneGameplay->GetGameProgress()->hasSpoken = true;
			++debugId;
			break;
		case 7: // Quest 7
			sceneGameplay->GetGameProgress()->hasKilledOfficers = true;
			++debugId;
			break;
		case 8: // Quest 8
			sceneGameplay->GetGameProgress()->hasActivated = true;
			++debugId;
			break;
		case 9: // Quest 9
			sceneGameplay->GetGameProgress()->hasKilledBoss = true;
			++debugId;
			break;
		default:
			break;
		}
	}

	// ToDo 8: Go to the xml and read about what the conditions of each quests is
	// All variables are declared and managed by the ModulePlayer
	/////////////////////////////////////////////////////////////////////////////
	if (sceneGameplay->GetGameProgress()->hasCalledCook == true && questsList.At(0)->data->isCompleted == false)
		CompleteQuest(1);

	if (sceneGameplay->GetGameProgress()->hasVisitedRestaurantQuest == true && questsList.At(1)->data->isCompleted == false)
		CompleteQuest(2);

	if (sceneGameplay->GetGameProgress()->hasVisitedErikaTombstone == true && questsList.At(2)->data->isCompleted == false)
		CompleteQuest(3);

	if (sceneGameplay->GetGameProgress()->hasSavedFirstApprentice == true && questsList.At(3)->data->isCompleted == false)
		CompleteQuest(4);

	if (sceneGameplay->GetGameProgress()->hasSavedLastApprentice == true && questsList.At(4)->data->isCompleted == false)
		CompleteQuest(5);

	if (sceneGameplay->GetGameProgress()->hasSpoken == true && questsList.At(5)->data->isCompleted == false)
		CompleteQuest(6);

	if (sceneGameplay->GetGameProgress()->hasKilledOfficers == true && questsList.At(6)->data->isCompleted == false)
		CompleteQuest(7);

	if (sceneGameplay->GetGameProgress()->hasActivated == true && questsList.At(7)->data->isCompleted == false)
		CompleteQuest(8);

	if (sceneGameplay->GetGameProgress()->hasKilledBoss == true && questsList.At(8)->data->isCompleted == false)
		CompleteQuest(9);

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