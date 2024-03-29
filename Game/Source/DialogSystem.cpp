#include "DialogSystem.h"
#include "DialogNode.h"

#include "Input.h"
#include "Render.h"
#include "AssetsManager.h"
#include "Fonts.h"
#include "Textures.h"

#include <utility>

#include "ToUpperCase.h"
#include "Callbacks.h"

#include "Log.h"

DialogSystem::DialogSystem(Input* input, Render* render, Textures* tex, Fonts* fonts, AssetsManager* assetsManager)
{
	name.Create("dialog");
	this->input = input;
	this->render = render;
	this->fonts = fonts;
	this->tex = tex;
	this->assetsManager = assetsManager;

	dialogBackground = nullptr;
	speakerTexture = nullptr;

	rightDialogAnim = nullptr;

	erikaDialogAnim.PushBack({ 30, 981, 162, 209 });
	erikaDialogAnim.PushBack({ 213, 981, 162, 209 });
	erikaDialogAnim.loop = true;
	erikaDialogAnim.speed = 0.01f;
	
	takadaDialogAnim.PushBack({ 417, 1008, 150, 183 });
	takadaDialogAnim.PushBack({ 594, 1008, 150, 183 });
	takadaDialogAnim.loop = true;
	takadaDialogAnim.speed = 0.01f;
	
	mobileDialogAnim.PushBack({ 20, 14, 52, 66 });
	mobileDialogAnim.loop = true;
	mobileDialogAnim.speed = 0.01f;

	kenzoDialogAnim.PushBack({ 795, 519, 162, 186 });
	kenzoDialogAnim.PushBack({ 987, 519, 162, 186 });
	kenzoDialogAnim.loop = true;
	kenzoDialogAnim.speed = 0.01f;

	reiDialogAnim.PushBack({ 795, 308, 162, 188 });
	reiDialogAnim.PushBack({ 987, 308, 162, 188 });
	reiDialogAnim.loop = true;
	reiDialogAnim.speed = 0.01f;

	eikenDialogAnim.PushBack({ 795, 732, 162, 192 });
	eikenDialogAnim.PushBack({ 987, 732, 162, 192 });
	eikenDialogAnim.loop = true;
	eikenDialogAnim.speed = 0.01f;

	makiDialogAnim.PushBack({ 1428, 1305, 138, 156 });
	makiDialogAnim.PushBack({ 1581, 1305, 138, 156 });
	makiDialogAnim.PushBack({ 1734, 1305, 138, 156 });
	makiDialogAnim.loop = true;
	makiDialogAnim.speed = 0.01f;

	oscarDialogAnim.PushBack({ 1389, 1700, 162, 189 });
	oscarDialogAnim.PushBack({ 1389, 1482, 162, 189 });
	oscarDialogAnim.loop = true;
	oscarDialogAnim.speed = 0.01f;
	
	shopKeeperDialogAnim.PushBack({ 1563, 1491, 168, 183 });
	shopKeeperDialogAnim.PushBack({ 1737, 1491, 168, 183 });
	shopKeeperDialogAnim.loop = true;
	shopKeeperDialogAnim.speed = 0.01f;
	
	pedestrianrDialogAnim.PushBack({ 1920, 1302, 150, 174 });
	pedestrianrDialogAnim.PushBack({ 1923, 1488, 150, 171 });
	pedestrianrDialogAnim.loop = true;
	pedestrianrDialogAnim.speed = 0.01f;
	
	receptionistDialogAnim.PushBack({ 1563, 1896, 162, 180 });
	receptionistDialogAnim.PushBack({ 1749, 1896, 162, 180 });
	receptionistDialogAnim.loop = true;
	receptionistDialogAnim.speed = 0.01f;
}

DialogSystem::~DialogSystem()
{
}

bool DialogSystem::Awake(pugi::xml_node& config)
{
	// Get dialog file directory
	folder = config.child("folder").child_value();

	return true;
}

bool DialogSystem::Start()
{
	// Random dialogs
	LoadDialog("Dialog/1.xml");
	LoadDialog("Dialog/2.xml");
	LoadDialog("Dialog/3.xml");
	LoadDialog("Dialog/4.xml");
	//LoadDialog("Dialog/5.xml");

	// Defined dialogs
	LoadDialog("Dialog/7.xml");
	LoadDialog("Dialog/8.xml");
	LoadDialog("Dialog/9.xml");
	LoadDialog("Dialog/10.xml");
	LoadDialog("Dialog/11.xml");
	LoadDialog("Dialog/12.xml");
	LoadDialog("Dialog/13.xml");
	LoadDialog("Dialog/14.xml");
	LoadDialog("Dialog/15.xml");
	LoadDialog("Dialog/16.xml");
	LoadDialog("Dialog/17.xml");
	LoadDialog("Dialog/18.xml");
	LoadDialog("Dialog/19.xml");
	LoadDialog("Dialog/20.xml");
	LoadDialog("Dialog/21.xml");
	LoadDialog("Dialog/22.xml");
	LoadDialog("Dialog/23.xml");
	LoadDialog("Dialog/24.xml");
	LoadDialog("Dialog/25.xml");
	LoadDialog("Dialog/26.xml");
	LoadDialog("Dialog/29.xml");

	// Needed textures
	dialogBackground = tex->Load("Textures/Dialog/dialog_background.png");
	speakerTexture = tex->Load("Textures/UI/guiTextureSpritesheet.png");
	speakerTexture2 = tex->Load("Textures/UI/outsideGUI.png");
	backgroundRect = { 0,0,1240,220 };

	// Register a callback function with the name say_hello. This is just an example.
	callbacks[std::string("say_hello")] = std::function<void()>(&SayHello);

	return true;
}

bool DialogSystem::PreUpdate()
{
	return true;
}

bool DialogSystem::Update(float dt)
{
	if (currentDialog != nullptr)
	{
		// The key to skip to the next dialog line.
		if (input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN || input->GetKey(SDL_SCANCODE_F) == KeyState::KEY_DOWN || input->GetControllerButton(CONTROLLER_BUTTON_A) == KeyState::KEY_DOWN)
		{
			NextDialog();
		}

		// Select the next option.
		if (input->GetKey(SDL_SCANCODE_DOWN) == KeyState::KEY_DOWN || input->GetControllerButton(CONTROLLER_BUTTON_DOWN) == KeyState::KEY_DOWN)
		{
			selectedOption += 1;
			if (selectedOption == currentDialog->children->size())
				selectedOption = currentDialog->children->size() - 1;
		}

		// Select the previous option.
		if (input->GetKey(SDL_SCANCODE_UP) == KeyState::KEY_DOWN || input->GetControllerButton(CONTROLLER_BUTTON_UP) == KeyState::KEY_DOWN)
		{
			selectedOption -= 1;
			if (selectedOption < 0) selectedOption = 0;
		}
	}

	/* ONLY FOR TESTING */

	if (newRandomDialog == true)
	{
		newRandomDialog = false;
		dialogFinished = false;

		// Generating a random number to pick a random dialog
		uint randNum = rand() % 4 + 1; // randNum in the range 1 to 4
		char HP[8] = { 0 };
		sprintf_s(HP, 8, "%03i", randNum);

		/*switch (randNum)
		{
		
		default:
			break;
		}*/
	}
	else if (newDialog == true)
	{
		newDialog = false;
		dialogFinished = false;

		switch (dialogIndex)
		{
		case 1:
			StartDialog("1");
			break;
		case 2:
			StartDialog("2");
			break;
		case 3:
			StartDialog("3");
			break;
		case 4:
			StartDialog("4");
			break;
		case 7:
			StartDialog("7");
			break;
		case 8:
			StartDialog("8");
			break;
		case 9:
			StartDialog("9");
			break;
		case 10:
			StartDialog("10");
			break;
		case 11:
			StartDialog("11");
			break;
		case 12:
			StartDialog("12");
			break;
		case 13:
			StartDialog("13");
			break;
		case 14:
			StartDialog("14");
			break;
		case 15:
			StartDialog("15");
			break;
		case 16:
			StartDialog("16");
			break;
		case 17:
			StartDialog("17");
			break;
		case 18:
			StartDialog("18");
			break;
		case 19:
			StartDialog("19");
			break;
		case 20:
			StartDialog("20");
			break;
		case 21:
			StartDialog("21");
			break;
		case 22:
			StartDialog("22");
			break;
		case 23:
			StartDialog("23");
			break;
		case 24:
			StartDialog("24");
			break;
		case 25:
			StartDialog("25");
			break;
		case 26:
			StartDialog("26");
			break;
		case 29:
			StartDialog("29");
		default:
			break;
		}
	}

	/* END ONLY FOR TESTING */

	return true;
}

bool DialogSystem::PostUpdate()
{
	if (currentDialog == nullptr) return true;

	DrawDialog();
	DrawDialogSpeaker();

	return true;
}

void DialogSystem::DrawDialog()
{
	// Draw the background rectangle.
	/*render->DrawRectangle(SDL_Rect({ 0, (render->camera.h / 3) * 2, render->camera.w, render->camera.h / 3 }), {255, 255, 255, 255}, true, false);*/
	render->DrawTexture(dialogBackground, 20, 480, &backgroundRect, 0);

	// Set the text to uppercase, since our font only supports uppercase.
	std::string text = ToUpperCase(currentDialog->attributes->at("value"));
	
	// Write the dialog line.
	/*fonts->BlitText(10 + 220, (render->camera.h / 3) * 2 + 10 + 50, 0, text.c_str());*/
	/*render->DrawText(font, text.c_str(), 10 + 220, (render->camera.h / 3) * 2 + 10 + 50, 25, 3, { 255,255,255,255 });*/
	short int lenght = text.length();
	LOG("SIZE IS %i", lenght);
	if (lenght >= 70)
	{
		std::string newT = text.substr(0, lenght/2);
		LOG("%s", newT);
		render->DrawText(font, newT.c_str(), 10 + 220, (render->camera.h / 3) * 2 + 10 + 50, 25, 3, { 255,255,255,255 });
		newT = text.substr(lenght / 2, lenght);
		LOG("%s", newT);
		render->DrawText(font, newT.c_str(), 10 + 220, (render->camera.h / 3) * 2 + 10 + 70, 25, 3, { 255,255,255,255 });
	}
	else {
		render->DrawText(font, text.c_str(), 10 + 220, (render->camera.h / 3) * 2 + 10 + 50, 25, 3, { 255,255,255,255 });
	}

	// If the current node is a question, we should also draw the possible answers
	if (currentDialog->type == DialogNode::NodeType::OPTIONS)
	{
		std::vector<DialogNode*>::iterator i = currentDialog->children->begin();
		int y = 0;
		// Iterate through the answers.
		for (i; i < currentDialog->children->end(); i++)
		{
			// Set them to uppercase.
			text = ToUpperCase((*i)->attributes->at("value"));
			// Draw them, increasing the y offset at every iteration.
			/*fonts->BlitText(30 + 220, (render->camera.h / 3) * 2 + 30 + (18 * y) + 50, 0, text.c_str());*/
			render->DrawText(font, text.c_str(), 30 + 220, (render->camera.h / 3) * 2 + 30 + (18 * y) + 50, 25, 3, { 255,255,255,255 });
			y++;
		}
		// Draw a small white rectangle next to the selected option.
		SDL_Rect selectedRectangle = SDL_Rect({ 20 + 220, (render->camera.h / 3) * 2 + 38 + (18 * selectedOption) + 50, 10, 10 });
		render->DrawRectangle(selectedRectangle, {255, 255, 255, 255}, true, false);
	}
}

bool DialogSystem::CleanUp()
{
	nodeRoutes.clear();
	callbacks.clear();
	dialogues.clear();

	// Unloading textures
	tex->UnLoad(dialogBackground);
	tex->UnLoad(speakerTexture);
	tex->UnLoad(speakerTexture2);

	return true;
}

bool DialogSystem::LoadDialog(const char* filename)
{
	bool ret = true;

	// Get the file path.
	std::string path = folder + filename;

	// Load the file.
	char* buffer = nullptr;
	size_t size = assetsManager->LoadXML(path.c_str(), &buffer);
	pugi::xml_parse_result result = dialogFile.load_buffer(buffer, size);
	RELEASE_ARRAY(buffer);

	if (result == NULL) {
		LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}

	if (ret == true) {

		/* Start TODO 1 */

		// Get the dialog root.
		pugi::xml_node dialogRoot = dialogFile.child("dialog");
		std::string id = dialogRoot.attribute("id").as_string();
		// Parse the dialog XML into the tree structure.
		DialogNode* dialog = ParseDialogXML(dialogRoot);
		// Insert the dialog into the dictionary.
		dialogues.insert(std::make_pair(id, dialog));

		/* End TODO 1 */
	}

	return ret;
}

void DialogSystem::StartDialog(const char* id)
{
	// If the dialog does not exist, return.
	if (dialogues.find(id) == dialogues.end()) return;
	// If it does exist, set it to currentDialog and run NextDialog() to start.
	currentDialog = dialogues.at(id);
	NextDialog();
}

bool DialogSystem::DialogHasFinished()
{
	return dialogFinished;
}

int DialogSystem::GetDialogIndex()
{
	return dialogIndex;
}
void DialogSystem::SetDialogFinished(bool dialogFinished)
{
	this->dialogFinished = dialogFinished;
}

void DialogSystem::NextDialog()
{
	/* Start TODO 4 */

	// If we have reached the end, currentDialog will be nullptr.
	if (currentDialog == nullptr)
	{
		dialogFinished = true;
		return;
	}
	else
	{
		dialogFinished = false;
	}

	// If the currentDialog is DIALOG, it means we are at the root of the tree. We can just skip to the first child.
	if (currentDialog->type == DialogNode::NodeType::DIALOG)
	{
		currentDialog = *currentDialog->children->begin();
	}
	// If currentDialog is OPTIONS, we should skip to the selected child.
	else if (currentDialog->type == DialogNode::NodeType::OPTIONS)
	{
		currentDialog = *currentDialog->children->at(selectedOption)->children->begin();
	}
	// Else, we just skip to the next line.
	else
	{
		currentDialog = currentDialog->Next();
	}

	// We reset the selectedOption to 0.
	selectedOption = 0;

	// Again, if we have reached the end of the dialog we return.
	if (currentDialog == nullptr)
	{
		dialogFinished = true;
		return;
	}
	else
	{
		dialogFinished = false;
	}

	// If the current line has a callback, we execute it.
	if (currentDialog->attributes->find("callback") != currentDialog->attributes->end())
	{
		callbacks.at(currentDialog->attributes->at("callback"))();
	}

	// If the current dialog is GOTO, we get the "route" and travel to the new line.
	if (currentDialog->type == DialogNode::NodeType::GOTO) {
		std::string route = currentDialog->attributes->at("route");
		currentDialog = nodeRoutes.at(route);
	}

	/* End TODO 4 */
}

void DialogSystem::NewDialog(int dialogIndex)
{
	if (dialogIndex == -1) this->newRandomDialog = true;
	else this->newDialog = true;

	this->dialogIndex = dialogIndex;
}

const char* DialogSystem::GetRightSpeaker()
{
	if (currentDialog != nullptr)
	{
		return currentDialog->attributes->at("right-speaker").c_str();
	}
	return nullptr;
}

const char* DialogSystem::GetLeftSpeaker()
{
	if (currentDialog != nullptr)
	{
		return currentDialog->attributes->at("left-speaker").c_str();
	}
	return nullptr;
}

DialogNode* DialogSystem::ParseDialogXML(pugi::xml_node currentNode)
{
	DialogNode* dialogNode = new DialogNode();

	/* Start TODO 2 */

	// We set the type variable acording to the XML node name. 
	std::string type = currentNode.name();
	if (type == "dialog") {}
	else if (type == "line")
	{
		dialogNode->type = DialogNode::NodeType::LINE;
	}
	else if (type == "options")
	{
		dialogNode->type = DialogNode::NodeType::OPTIONS;
	}
	else if (type == "option")
	{
		dialogNode->type = DialogNode::NodeType::OPTION;
	}
	else if (type == "goto")
	{
		dialogNode->type = DialogNode::NodeType::GOTO;
	}
	else
		return nullptr;


	Attributes* attributes = new Attributes();

	// We iterate over the attributes and store them in a dictionary.
	pugi::xml_attribute_iterator aIt = currentNode.attributes_begin();
	for (aIt; aIt != currentNode.attributes_end(); aIt++)
	{
		attributes->insert(std::make_pair(aIt->name(), aIt->value()));
	}

	dialogNode->SetAttributes(attributes);

	// If the node contains an "id", we store it in a dictionary that will be useful for GOTOs.
	if (dialogNode->attributes->find("id") != dialogNode->attributes->end())
	{
		nodeRoutes.insert(std::make_pair(dialogNode->attributes->at("id"), dialogNode));
	}

	// If the type is not LINE, we execute ParseDialogXML recursively over its children and store them in the "children" vector.
	if (dialogNode->type != DialogNode::NodeType::LINE)
	{
		pugi::xml_node_iterator cIt = currentNode.begin();
		for (cIt; cIt != currentNode.end(); cIt++)
		{
			DialogNode* child = ParseDialogXML(*cIt);
			child->parent = dialogNode;
			if (dialogNode->children->size() != 0)
			{
				child->prev = *(dialogNode->children->end() - 1);
				(*(dialogNode->children->end() - 1))->next = child;
			}
			dialogNode->AddChild(child);
		}
	}

	/* End TODO 2 */

	return dialogNode;
}

void DialogSystem::DrawDialogSpeaker()
{
	if (GetRightSpeaker() != nullptr)
	{
		// Get the speakers strings
		SString rightSpeaker = GetRightSpeaker();
		SString leftSpeaker = GetLeftSpeaker();

		rightDialogAnim = nullptr;
		leftDialogAnim = nullptr;

		// Compare and set the right speaker
		if (rightSpeaker == "Erika") 
		{
			rightDialogAnim = &erikaDialogAnim;
		}
		else if (rightSpeaker == "Takada")
		{
			rightDialogAnim = &takadaDialogAnim;
		}
		else if (rightSpeaker == "Oscar")
		{
			rightDialogAnim = &oscarDialogAnim;
		}
		else if (rightSpeaker == "Kenzo")
		{
			rightDialogAnim = &kenzoDialogAnim;
		}
		else if (rightSpeaker == "Eiken")
		{
			rightDialogAnim = &eikenDialogAnim;
		}
		else if (rightSpeaker == "Rei")
		{
			rightDialogAnim = &reiDialogAnim;
		}
		else if (rightSpeaker == "Maki")
		{
			rightDialogAnim = &makiDialogAnim;
		}
		else if (rightSpeaker == "Pedestrian")
		{
			rightDialogAnim = &pedestrianrDialogAnim;
		}
		else if (rightSpeaker == "Shopkeeper")
		{
			rightDialogAnim = &shopKeeperDialogAnim;
		}
		else if (rightSpeaker == "Receptionist")
		{
			rightDialogAnim = &receptionistDialogAnim;
		}
		else if (rightSpeaker == "None")
		{
			rightDialogAnim = nullptr;
		}
		else if (rightSpeaker == "Phone")
		{
			rightDialogAnim = &mobileDialogAnim;
		}

		// Compare and set the left speaker
		if (leftSpeaker == "Kenzo")
		{
			leftDialogAnim = &kenzoDialogAnim;
		}
		else if (leftSpeaker == "Eiken")
		{
			leftDialogAnim = &eikenDialogAnim;
		}
		else if (leftSpeaker == "Rei")
		{
			leftDialogAnim = &reiDialogAnim;
		}
		else if (leftSpeaker == "Erika")
		{
			leftDialogAnim = &erikaDialogAnim;
		}
		else if (leftSpeaker == "Takada")
		{
			leftDialogAnim = &takadaDialogAnim;
		}
		else if (leftSpeaker == "Oscar")
		{
			leftDialogAnim = &oscarDialogAnim;
		}
		else if (leftSpeaker == "Maki")
		{
			leftDialogAnim = &makiDialogAnim;
		}
		else if (leftSpeaker == "Pedestrian")
		{
			leftDialogAnim = &pedestrianrDialogAnim;
		}
		else if (leftSpeaker == "Shopkeeper")
		{
			leftDialogAnim = &shopKeeperDialogAnim;
		}
		else if (leftSpeaker == "Receptionist")
		{
			leftDialogAnim = &receptionistDialogAnim;
		}
		else if (leftSpeaker == "None")
		{
			leftDialogAnim = nullptr;
		}
		else if (leftSpeaker == "Phone")
		{
			leftDialogAnim = &mobileDialogAnim;
		}

		// Draw speakers
		if (leftDialogAnim != nullptr)
		{
			leftDialogAnim->Update();
			if (leftDialogAnim == &mobileDialogAnim)
			{
				render->scale = 2;
				render->DrawTexture(speakerTexture2, 95/ render->scale, 520 / render->scale, &leftDialogAnim->GetCurrentFrame(), 0.0f);
				render->scale = 1;
			}
			else
				render->DrawTexture(speakerTexture, 60, 485, &leftDialogAnim->GetCurrentFrame(), 0.0f);
		}
		//render->DrawTexture(speakerTexture, 60, 485, &leftDialogAnim->GetCurrentFrame(), 0.0f);
		if (rightDialogAnim != nullptr)
		{
			rightDialogAnim->Update();
			if (rightDialogAnim == &mobileDialogAnim)
			{
				render->scale = 2;
				render->DrawTexture(speakerTexture2, 1075 / render->scale, 520 / render->scale, &rightDialogAnim->GetCurrentFrame(), 0.0f);
				render->scale = 1;
			}
			else
				render->DrawTexture(speakerTexture, 1055, 485, &rightDialogAnim->GetCurrentFrame(), 0.0f);
		}
	}
}

void DialogSystem::SetFont(Font* font)
{
	this->font = font;
}
