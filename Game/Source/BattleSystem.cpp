#include "BattleSystem.h"

BattleSystem::BattleSystem()
{
	battleState = BattleState::START;
	battleGUIState = BattleGUIState::NONE;
	battleGUIStateRegister[0] = BattleGUIState::ATTACK;
	battleGUIStateRegister[1] = BattleGUIState::DEFEND;
	battleGUIStateRegister[2] = BattleGUIState::ITEM;
	battleGUIStateRegister[3] = BattleGUIState::RUN;
	battleGUIPosition = 1;

	// Right now we only have one party member implemented
	numPlayers = 1;
}

BattleSystem::~BattleSystem()
{

}

bool BattleSystem::Update(Input* input)
{
	switch (battleState)
	{
	case BattleState::START:
		Start();
		break;
	case BattleState::PLAYER_TURN:
		if (input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
			battleGUIPosition -= 1;
		if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
			battleGUIPosition += 1;

		if (battleGUIPosition > 3)
			battleGUIPosition = 0;
		else if (battleGUIPosition < 0)
			battleGUIPosition = 3;

		battleGUIState = battleGUIStateRegister[battleGUIPosition];

		if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
			PlayerTurn();
		break;
	case BattleState::ENEMY_TURN:
		EnemyTurn();
		break;
	case BattleState::WON:
		Won();
		break;
	case BattleState::LOST:
		Lost();
		break;
	case BattleState::NONE:
		// Warn the battle screen that the combat has finished in order to get out of it and return to the gameplay screen.
		break;
	default:
		break;
	}

	return true;
}

void BattleSystem::SetupBattle(List<Player*> players, Enemy* enemy)
{
	this->players = players;
	currentPlayer = players.At(0)->data;
	this->enemy = enemy;
}

void BattleSystem::Start()
{
	// Play animations of the fighters (not necessary for the vertical slice)
	// Display introductory text
	// Change state to the fighter with most speed
	for (int i = 0; i < numPlayers; i++)
	{
		if (players.At(i)->data != nullptr)
		{
			if (players.At(i)->data->stats.attackSpeed > currentPlayer->stats.attackSpeed)
				currentPlayer = players.At(i)->data;
		}
	}

	if (currentPlayer->stats.attackSpeed > enemy->stats.attackSpeed)
		battleState = BattleState::PLAYER_TURN;
	else
		battleState = BattleState::ENEMY_TURN;
}

void BattleSystem::PlayerTurn()
{
	// Manage player input for the battle system interface
	switch (battleGUIState)
	{
	case BattleGUIState::ATTACK:
		// Enter the attack menu when we have it implemented
		// At the moment:
		// Display the player attack text
		// Play animation
		// Substract life points to the enemy
		enemy->stats.currentHP -= currentPlayer->stats.damage;
		// Check if the enemy has died
		if (enemy->stats.currentHP <= 0)
			battleState = BattleState::WON;
		break;
	case BattleGUIState::DEFEND:
		// Add life points to the player
		currentPlayer->stats.currentHP += currentPlayer->stats.defense;
		// Check if the player life has gone over their maximum HP
		if (currentPlayer->stats.currentHP > currentPlayer->stats.maxHP)
			currentPlayer->stats.currentHP = currentPlayer->stats.maxHP;
		break;
	case BattleGUIState::ITEM:
		// Get into the items menu
		// Items logic
		break;
	case BattleGUIState::RUN:
		// Return to the gameplay screen
		battleState = BattleState::NONE;
		break;
	case BattleGUIState::NONE:
		// ...
		break;
	default:
		break;
	}

	// Set as the current player the next party member (if it's available)
	for (int i = 0; i < 4; i++)
	{
		if (players.At(i)->data->stats.name == currentPlayer->stats.name)
			currentPlayer = players.At(i)->next->data;
	}
}

void BattleSystem::EnemyTurn()
{
	// Define randomly an action for the enemy
	int num = rand() % 100;
	if (num < 50)
	{
		// Player attack logic
		// Display enemy attack text
		// Play animation
		// Substract life points to the player
		currentPlayer->stats.currentHP -= enemy->stats.damage;
		// Check if the player has died
		if (currentPlayer->stats.currentHP <= 0)
			battleState = BattleState::LOST;
		else
			battleState = BattleState::PLAYER_TURN;
	}
	else
	{
		// Player defense logic
		// Display enemy defense text
		// Play animation
		// Add life points to the enemy
		enemy->stats.currentHP += enemy->stats.defense;
		// Check if the enemy life has gone over their maximum
		if (enemy->stats.currentHP > enemy->stats.maxHP)
			enemy->stats.currentHP = enemy->stats.maxHP;

		BattleState::PLAYER_TURN;
	}
}

void BattleSystem::Won()
{
	// Display winner text
	// Get out of the battle screen and return to the gameplay screen
}

void BattleSystem::Lost()
{
	// Display loser text
	// Get out of the battle screen and return to the gameplay screen
}
