#include "BattleSystem.h"

BattleSystem::BattleSystem()
{

}

BattleSystem::~BattleSystem()
{

}

bool BattleSystem::Start()
{
	battleState = BattleState::START;

	return true;
}

bool BattleSystem::Update()
{
	return true;
}

void BattleSystem::SetupBattle(Player* player, Enemy* enemy)
{
	this->player = player;
	this->enemy = enemy;
}