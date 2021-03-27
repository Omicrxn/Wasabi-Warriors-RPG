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

enum class BattleGUIState
{
	ATTACK,
	DEFEND,
	ITEM,
	RUN,
	NONE
};

class BattleSystem
{
public:

	BattleSystem();
	~BattleSystem();

	bool Update(Input* input);

	// To get the player and the enemy information
	void SetupBattle(List<Player*> players, Enemy* enemy);

	// Functions for the different battle states
	void Start();
	void PlayerTurn();
	void EnemyTurn();
	void Won();
	void Lost();

	BattleState battleState;
	BattleGUIState battleGUIState;
	BattleGUIState battleGUIStateRegister[4];
	int battleGUIPosition;

private:

	// Total numbers of party members implemented
	uint numPlayers;
	List<Player*> players;
	Player* currentPlayer;
	Enemy* enemy;
};

#endif // __BATTLESYSTEM_H__