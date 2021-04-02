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

class BattleSystem
{
public:

	static BattleSystem* GetInstance();

	~BattleSystem();

	// To get the player and the enemy information
	void SetupBattle(List<Player*> players, Enemy* enemy);

	// Functions for the different battle states
	bool Start();
	bool Update(Input* input);
	void PlayerTurn();
	void EnemyTurn();
	void Won();
	void Lost();

	Player* GetPlayer();
	Enemy* GetEnemy();

	BattleState battleState;
	BattleGUIState battleGUIState;

private:

	static BattleSystem* instance;

	BattleSystem();

	// Needed modules
	EntityManager* entityManager;

	// Total numbers of party members implemented
	uint numPlayers;
	List<Player*> players;
	Player* currentPlayer;
	Enemy* enemy;
};

#endif // __BATTLESYSTEM_H__