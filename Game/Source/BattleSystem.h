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
	EXIT,
	NONE
};

enum class PlayerState
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

enum class BattleMusic
{
	NONE,
	WON,
	LOST
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
	bool WinAndLose();

	// Functions for the Battle state
	void PlayerTurn();
	void EnemyTurn();

	Player* GetPlayer();
	List<Player*>* BattleSystem::GetPlayersList();
	Enemy* GetEnemy();

	// Battle state (Current attacker and defender)
	BattleState battleState;
	// Menu state when the player attacks
	PlayerState playerState;
	// Enemy state (to keep track of the action)
	EnemyState enemyState;

	BattleMusic currentMusic;

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
	uint turnCounter;
};

#endif // __BATTLESYSTEM_H__