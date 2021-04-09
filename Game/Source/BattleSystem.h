#pragma once
#ifndef __BATTLESYSTEM_H__
#define __BATTLESYSTEM_H__

#include "Scene.h"
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

enum class EnemyState
{
	ATTACK,
	DEFEND,
	NONE
};

class BattleSystem
{
public:

	BattleSystem();
	~BattleSystem();

	// To get the player and the enemy information
	void SetupBattle(List<Player*>* players, Enemy* enemy);

	// Functions for the different battle states
	bool Update(Input* input, float dt);
	bool ResetBattle();

	// Functions for the Battle state
	void PlayerTurn();
	void EnemyTurn();
	void Won();
	void Lost();

	Player* GetPlayer();
	List<Player*>* BattleSystem::GetPlayersList();
	Enemy* GetEnemy();

	// Battle state (Current attacker and defender)
	BattleState battleState;
	// Menu state when the player attacks
	BattleGUIState battleGUIState;
	// Enemy state (to keep track of the action)
	EnemyState enemyState;

private:

	// Total numbers of party members implemented
	uint numPlayers;
	// List of players
	List<Player*>* players;
	// Current player
	Player* currentPlayer;
	// Enemy against whom the player is fighting
	Enemy* enemy;
	// Time counter for the enemy turn
	uint enemyTurnCounter;
};

#endif // __BATTLESYSTEM_H__