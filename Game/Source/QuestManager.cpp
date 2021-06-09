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
			render->DrawText(font, "Go to the", 480, 193, 50, 3, { 0, 0, 0, 255 });
			render->DrawText(font, "restaurant of", 480, 223, 50, 3, { 0, 0, 0, 255 });
			render->DrawText(font, "Erika", 480, 253, 50, 3, { 0, 0, 0, 255 });
			render->DrawText(font, "Go to the", 480, 190, 50, 3, { 255, 255, 255, 255 });
			render->DrawText(font, "restaurant of", 480, 220, 50, 3, { 255, 255, 255, 255 });
			render->DrawText(font, "Erika", 480, 250, 50, 3, { 255, 255, 255, 255 });

			render->DrawText(font, "You just found out that", 480, 300, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "Erika and you need to check", 480, 330, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "the restaurant.", 480, 360, 23, 3, { 0,0,0,255 });
		}

		if (L->data->id == 3)
		{
			// Title Drawing
			render->DrawText(font, "Visit the", 480, 193, 50, 3, { 0,0,0,255 });
			render->DrawText(font, "Visit the", 480, 190, 50, 3, { 255,255,255,255 });
			render->DrawText(font, "tombstone", 480, 223, 50, 3, { 0,0,0,255 });
			render->DrawText(font, "tombstone", 480, 220, 50, 3, { 255,255,255,255 });

			// Description Drawing if pressed L
			///////////////////////////////////////////////////////////////////////////
			//render->DrawText(font, L->data->description.GetString(), 480, 300, 35, 3, { 200,200,200,255 });
			render->DrawText(font, "After checking out her", 480, 270, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "restaurant, you should go", 480, 300, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "and pay respects to Erika on", 480, 330, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "the Cemetery.", 480, 360, 23, 3, { 0,0,0,255 });
			///////////////////////////////////////////////////////////////////////////
		}

		if (L->data->id == 4)
		{
			render->DrawText(font, "Find the first", 480, 193, 50, 3, { 0,0,0,255 });
			render->DrawText(font, "Find the first", 480, 190, 50, 3, { 255,255,255,255 });
			render->DrawText(font, "Apprentice", 480, 223, 50, 3, { 0,0,0,255 });
			render->DrawText(font, "Apprentice", 480, 220, 50, 3, { 255,255,255,255 });

			render->DrawText(font, "You should rescue the first", 480, 270, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "Apprentice on the", 480, 300, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "restaurant in Osaka.", 480, 330, 23, 3, { 0,0,0,255 });
		}

		if (L->data->id == 5)
		{
			render->DrawText(font, "Find the last", 480, 193, 50, 3, { 0,0,0,255 });
			render->DrawText(font, "Find the last", 480, 190, 50, 3, { 255,255,255,255 });
			render->DrawText(font, "Apprentice", 480, 223, 50, 3, { 0,0,0,255 });
			render->DrawText(font, "Apprentice", 480, 220, 50, 3, { 255,255,255,255 });

			render->DrawText(font, "You should rescue the other", 480, 270, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "Apprentice on the", 480, 300, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "restaurant in Dotonbori.", 480, 330, 23, 3, { 0,0,0,255 });
		}

		if (L->data->id == 6)
		{
			// Title Drawing
			render->DrawText(font, "Takada offices", 480, 223, 50, 3, { 0,0,0,255 });
			render->DrawText(font, "Takada offices", 480, 220, 50, 3, { 255,255,255,255 });

			// Description Drawing if pressed L
			///////////////////////////////////////////////////////////////////////////
			//render->DrawText(font, L->data->description.GetString(), 480, 300, 35, 3, { 200,200,200,255 });
			render->DrawText(font, "Go to the Takada offices and", 480, 270, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "talk with the receptionist.", 480, 300, 23, 3, { 0,0,0,255 });
			///////////////////////////////////////////////////////////////////////////
		}

		if (L->data->id == 7)
		{
			render->DrawText(font, "Get the code", 480, 223, 50, 3, { 0,0,0,255 });
			render->DrawText(font, "Get the code", 480, 220, 50, 3, { 255,255,255,255 });

			render->DrawText(font, "Defeat the clerks in order", 480, 270, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "to get the code boss door", 480, 300, 23, 3, { 0,0,0,255 });
		}

		if (L->data->id == 8)
		{
			render->DrawText(font, "Central signal", 480, 223, 50, 3, { 0,0,0,255 });
			render->DrawText(font, "Central signal", 480, 220, 50, 3, { 255,255,255,255 });

			render->DrawText(font, "Reactivat the central signal", 480, 270, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "from the president's", 480, 300, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "computer.", 480, 330, 23, 3, { 0,0,0,255 });
		}

		if (L->data->id == 9)
		{
			render->DrawText(font, "Defeat Takuda", 480, 223, 50, 3, { 0,0,0,255 });
			render->DrawText(font, "Defeat Takuda", 480, 220, 50, 3, { 255,255,255,255 });

			render->DrawText(font, "Face Takuda in a battle", 480, 270, 23, 3, { 0,0,0,255 });
			render->DrawText(font, "and may the best win...", 480, 300, 23, 3, { 0,0,0,255 });
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
		//switch (L->data->id)
		//{
		//case 7: // new quest chain 1
		//	// Title Drawing
		//	render->DrawText(font, L->data->title.GetString(), 480, 223, 50, 3, { 0,0,0,255 });
		//	render->DrawText(font, L->data->title.GetString(), 480, 220, 50, 3, { 255,255,255,255 });

		//	// Description Drawing if pressed L
		//	///////////////////////////////////////////////////////////////////////////
		//	//render->DrawText(font, L->data->description.GetString(), 480, 300, 35, 3, { 200,200,200,255 });
		//	render->DrawText(font, "Go to the Takada offices and", 480, 300, 23, 3, { 0,0,0,255 });
		//	render->DrawText(font, "talk with the receptionist.", 480, 350, 23, 3, { 0,0,0,255 });
		//	///////////////////////////////////////////////////////////////////////////

		//	break;
		//case 2: // quest chain 1 (quest 2)
		//	// Title Drawing
		//	render->DrawText(font, L->data->title.GetString(), 480, 223, 50, 3, { 0,0,0,255 });
		//	render->DrawText(font, L->data->title.GetString(), 480, 220, 50, 3, { 255,255,255,255 });

		//	// Description Drawing if pressed L
		//	///////////////////////////////////////////////////////////////////////////
		//	render->DrawText(font, L->data->description.GetString(), 480, 300, 23, 3, { 0,0,0,255 });
		//	///////////////////////////////////////////////////////////////////////////
		//	break;

		//case 3: // new quest chain 1
		//	// Title Drawing
		//	render->DrawText(font, L->data->title.GetString(), 480, 223, 50, 3, { 0,0,0,255 });
		//	render->DrawText(font, L->data->title.GetString(), 480, 220, 50, 3, { 255,255,255,255 });

		//	// Description Drawing if pressed L
		//	///////////////////////////////////////////////////////////////////////////
		//	render->DrawText(font, L->data->description.GetString(), 480, 300, 23, 3, { 0,0,0,255 });
		//	///////////////////////////////////////////////////////////////////////////
		//	break;
		//case 4: // new quest chain 1
		//	// Title Drawing
		//	render->DrawText(font, L->data->title.GetString(), 480, 223, 50, 3, { 0,0,0,255 });
		//	render->DrawText(font, L->data->title.GetString(), 480, 220, 50, 3, { 255,255,255,255 });

		//	// Description Drawing if pressed L
		//	///////////////////////////////////////////////////////////////////////////
		//	render->DrawText(font, L->data->description.GetString(), 480, 300, 23, 3, { 0,0,0,255 });
		//	///////////////////////////////////////////////////////////////////////////
		//	break;
		//default:
		//	break;
		//}

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
					notifier->SendMobileNotification("New quest added!");
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