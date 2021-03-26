#include "BattleSystem.h"

BattleSystem::BattleSystem()
{
	battleState = BattleState::START;
}

BattleSystem::~BattleSystem()
{

}

bool BattleSystem::Update()
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
	this->players = players;
	this->enemy = enemy;
}

void BattleSystem::Start()
{
	// Play animations of the fighters (not necessary for the vertical slice)
	// Display introductory text
	// Change state to the fighter with most speed
	for (int i = 0; i < 4; i++)
	{
		if (players.At(i)->data != nullptr) players.At(i)->data.
	}
}

void BattleSystem::PlayerTurn()
{
}

void BattleSystem::EnemyTurn()
{
}

void BattleSystem::Won()
{
}

void BattleSystem::Lost()
{
}
