#include "BattleSystem.h"

BattleSystem* BattleSystem::instance = nullptr;

BattleSystem* BattleSystem::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new BattleSystem();
	}

	return instance;
}

BattleSystem::BattleSystem()
{
	battleState = BattleState::PLAYER_TURN;
	battleGUIState = BattleGUIState::NONE;

	// Right now we only have one party member implemented
	numPlayers = 1;

	enemyTurnCounter = 0;
	enemyAttack = false;
	enemyDefend = false;
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
	// Register fighters in the battle system
	this->players = players;
	currentPlayer = players.At(0)->data;
	this->enemy = enemy;

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

	/*if (currentPlayer->stats.attackSpeed >= enemy->stats.attackSpeed)
		battleState = BattleState::PLAYER_TURN;
	else
		battleState = BattleState::ENEMY_TURN;*/
}

bool BattleSystem::Start()
{
	return true;
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

		battleState = BattleState::ENEMY_TURN;

		// Set as the current player the next party member (if it's available)
		for (int i = 0; i < 4; i++)
		{
			if (players.At(i)->data->stats.name == currentPlayer->stats.name)
			{
				if (players.At(i)->next->data != nullptr)
					currentPlayer = players.At(i)->next->data;
			}
		}
		break;
	case BattleGUIState::DEFEND:
		// Add life points to the player
		currentPlayer->stats.currentHP += currentPlayer->stats.defense;
		// Check if the player life has gone over their maximum HP
		if (currentPlayer->stats.currentHP > currentPlayer->stats.maxHP)
			currentPlayer->stats.currentHP = currentPlayer->stats.maxHP;

		battleState = BattleState::ENEMY_TURN;
		break;
	case BattleGUIState::ITEM:
		// Get into the items menu
		// Items logic

		battleState = BattleState::ENEMY_TURN;
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
}

void BattleSystem::EnemyTurn()
{
	// Define randomly an action for the enemy (only the first time)
	if (enemyTurnCounter == 0)
	{
		int num = rand() % 100;
		if (num < 50)
		{
			// Player attack logic
			enemyAttack = true;

			// Substract life points to the player
			if (enemyTurnCounter == 0)
				currentPlayer->stats.currentHP -= enemy->stats.damage;
		}
		else
		{
			// Player defense logic
			enemyDefend = true;

			// Add life points to the enemy (only the first time)
			if (enemyTurnCounter == 0)
			{
				enemy->stats.currentHP += enemy->stats.defense;

				// Check if the enemy life has gone over their maximum
				if (enemy->stats.currentHP > enemy->stats.maxHP)
					enemy->stats.currentHP = enemy->stats.maxHP;
			}
		}
	}

	enemyTurnCounter++;

	if (enemyTurnCounter > 300)
	{
		// Reset variables for the next time
		enemyTurnCounter = 0;
		enemyAttack = false;
		enemyDefend = false;

		// Check if the player has died
		if (currentPlayer->stats.currentHP <= 0)
			battleState = BattleState::LOST;
		else
			battleState = BattleState::PLAYER_TURN;
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

Player* BattleSystem::GetPlayer()
{
	return currentPlayer;
}

List<Player*> BattleSystem::GetPlayersList()
{
	return players;
}

Enemy* BattleSystem::GetEnemy()
{
	return enemy;
}

bool BattleSystem::IsEnemyAttacking()
{
	return enemyAttack;
}

bool BattleSystem::IsEnemyDefending()
{
	return enemyDefend;
}