#pragma once
#ifndef __BATTLESYSTEM_H__
#define __BATTLESYSTEM_H__

#include "Scene.h"

#include "List.h"

class Player;
class Enemy;
class Item;

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
	void SetupBattle(List<Player*>* players, Enemy* enemy, List<Item*>* items);

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

	bool IsTurnChanging();
	uint GetCounter();

	// Function to know if we are still in the battle
	bool OnBattle();

	// Functions to keep track of the inventory state in battle
	bool HasOpenedInventory();
	void SetInventoryOpening(bool InventoryOpeningState);
	bool HasClosedInventory();
	void SetInventoryClosure(bool InventoryClosureState);

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
	// List of items
	List<Item*>* items;
	// Time counter for the enemy turn
	uint turnCounter;
	// Bool to know if turn has been changed
	bool turnChanged;
	// Bool to keep track if inventory has been opened
	bool battleInventory = false;
	// Bool to know if we are done with the items
	bool hasFinishedItems = false;
};

#endif // __BATTLESYSTEM_H__