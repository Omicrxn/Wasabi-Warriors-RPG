#include "DialogSystem.h"
#include "DialogNode.h"

#include "Input.h"
#include "Render.h"
#include "Fonts.h"

#include <utility>

#include "ToUpperCase.h"
#include "Callbacks.h"

#include "Log.h"

DialogSystem::DialogSystem(Input* input, Render* render, Fonts* fonts)
{
	name.Create("dialog");
	this->input = input;
	this->render = render;
	this->fonts = fonts;
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
	LoadDialog("Assets/Dialog/1.xml");
	LoadDialog("Assets/Dialog/2.xml");
	LoadDialog("Assets/Dialog/3.xml");
	LoadDialog("Assets/Dialog/4.xml");
	LoadDialog("Assets/Dialog/5.xml");

	LoadDialog("Assets/Dialog/7.xml");

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
		if (input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN || input->GetKey(SDL_SCANCODE_F) == KeyState::KEY_DOWN /*|| input->GetControllerButton(CONTROLLER_BUTTON_A) == KeyState::KEY_DOWN*/)
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
		uint randNum = rand() % 5 + 1; // randNum in the range 1 to 5
		char HP[8] = { 0 };
		sprintf_s(HP, 8, "%03i", randNum);

		switch (randNum)
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
		case 5:
			StartDialog("5");
			break;
		default:
			break;
		}
	}
	else if (newDialog == true)
	{
		newDialog = false;
		dialogFinished = false;

		switch (dialogIndex)
		{
		case 7:
			StartDialog("7");
			break;
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

	return true;
}

void DialogSystem::DrawDialog()
{
	// Draw the background rectangle.
	render->DrawRectangle(SDL_Rect({ 0, (render->camera.h / 3) * 2, render->camera.w, render->camera.h / 3 }), {255, 255, 255, 255}, true, false);

	// Set the text to uppercase, since our font only supports uppercase.
	std::string text = ToUpperCase(currentDialog->attributes->at("value"));

	// Write the dialog line.
	fonts->BlitText(10, (render->camera.h / 3) * 2 + 10, 0, text.c_str());

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
			fonts->BlitText(30, (render->camera.h / 3) * 2 + 30 + (18 * y), 0, text.c_str());
			y++;
		}
		// Draw a small black rectangle next to the selected option.
		SDL_Rect selectedRectangle = SDL_Rect({ 20, (render->camera.h / 3) * 2 + 30 + (18 * selectedOption), 6, 6 });
		render->DrawRectangle(selectedRectangle, {0, 0, 0, 255}, true, false);
	}
}

bool DialogSystem::CleanUp()
{
	nodeRoutes.clear();
	callbacks.clear();
	dialogues.clear();

	return true;
}

bool DialogSystem::LoadDialog(const char* filename)
{
	bool ret = true;

	// Get the file path.
	std::string path = folder + filename;

	// Load the file.
	pugi::xml_parse_result result = dialogFile.load_file(path.c_str());

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