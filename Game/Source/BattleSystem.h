#pragma once
#ifndef __BATTLESYSTEM_H__
#define __BATTLESYSTEM_H__

#include "Player.h"
#include "Enemy.h"

#include "List.h"

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

	bool Update();

	// To get the player and the enemy information
	void SetupBattle(List<Player*> players, Enemy* enemy);

	// Functions for the different battle states
	void Start();
	void PlayerTurn();
	void EnemyTurn();
	void Won();
	void Lost();

	BattleState battleState = BattleState::NONE;

private:

	List<Player*> players;
	Enemy* enemy;
};

#endif // __BATTLESYSTEM_H__