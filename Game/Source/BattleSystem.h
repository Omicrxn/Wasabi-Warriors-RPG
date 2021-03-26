#pragma once
#ifndef __BATTLESYSTEM_H__
#define __BATTLESYSTEM_H__

#include "Player.h"
#include "Enemy.h"

enum class BattleState
{
	START,
	PLAYER_TURN,
	ENEMY_TURN,
	WON,
	LOST,
	NONE
};

class BattleSystem
{
public:

	BattleSystem();
	~BattleSystem();

	bool Start();
	bool Update();

	void SetupBattle(Player* player, Enemy* enemy);

	BattleState battleState = BattleState::NONE;

private:

	Player* player;
	Enemy* enemy;
};

#endif // __BATTLESYSTEM_H__