#include "BattleSystem.h"

#include "Textures.h"
#include "Render.h"
#include "Font.h"
#include "Window.h"

#include "Player.h"
#include "Enemy.h"
#include "Item.h"

#include "GuiManager.h"
#include "GuiButton.h"

#include "Log.h"

#define TITLE_FADE_SPEED 0.05f

BattleSystem::BattleSystem()
{
	battleState = BattleState::PLAYER_TURN;
	playerState = PlayerState::NONE;
	enemyState = EnemyState::NONE;

	// Time for enemy to manage their actions
	turnCounter = 0;

	currentMusic = BattleMusic::NONE;

	specialAttack = SpecialAttack::NONE;
	specialDefense = SpecialDefense::NONE;
}

BattleSystem::~BattleSystem()
{

}

bool BattleSystem::Update(Input* input, float dt)
{
	switch (battleState)
	{
	case BattleState::PLAYER_TURN:
		PlayerTurn();
		break;
	case BattleState::ENEMY_TURN:
		EnemyTurn();
		break;
	case BattleState::WON:
		WinAndLose();
		break;
	case BattleState::LOST:
		WinAndLose();
		break;
	default:
		break;
	}

	return true;
}

void BattleSystem::SetupBattle(List<Player*>* players, Enemy* enemy, List<Item*>* items)
{
	// Register fighters in the battle system
	this->players = players;
	currentPlayer = players->At(0)->data;
	this->enemy = enemy;
	this->items = items;

	// Play animations of the fighters (not necessary for the vertical slice)
	// Display introductory text
	// Change state to the fighter with most speed
	/*for (int i = 0; i < numPlayers; i++)
	{
		if (this->players.At(i) != nullptr)
		{
			if (this->players.At(i)->data->stats.attackSpeed > currentPlayer->stats.attackSpeed)
				currentPlayer = this->players.At(i)->data;
		}
	}*/

	/*if (currentPlayer->stats.attackSpeed >= enemy->stats.attackSpeed)
		battleState = BattleState::PLAYER_TURN;
	else
		battleState = BattleState::ENEMY_TURN;*/
}

bool BattleSystem::ResetBattle()
{
	// Battle state (Current attacker and defender)
	battleState = BattleState::PLAYER_TURN;
	// Menu state when the player attacks
	playerState = PlayerState::NONE;
	// Enemy state (to keep track of the action)
	enemyState = EnemyState::NONE;

	// List of players
	/*List<Player*> players;*/
	// Current player
	currentPlayer = nullptr;
	// Enemy against whom the player is fighting
	enemy = nullptr;
	// Time counter for each turn
	turnCounter = 0;

	SetInventoryOpening(false);
	SetInventoryClosure(false);

	specialAttack = SpecialAttack::NONE;
	specialDefense = SpecialDefense::NONE;

	for (int i = 0; i < 3; i++)
	{
		specialAttacks[i] = false;
		specialDefenses[i] = false;
	}

	return true;
}

bool BattleSystem::WinAndLose()
{
	if (turnChanged) turnChanged = false;

	turnCounter++;
	if (turnCounter > 200)
	{
		battleState = BattleState::EXIT;
	}

	return false;
}

void BattleSystem::PlayerTurn()
{
	if (turnChanged)
	{
		turnChanged = false;
		UpdateAttacksAndDefenses();
	}

	if (turnCounter == 0)
	{
		// Manage player input for the battle system interface
		switch (playerState)
		{
		case PlayerState::ATTACK:
			switch (specialAttack)
			{
			case SpecialAttack::ATTACK_1:
				// Substract life points to the enemy
				enemy->stats.currentHP -= currentPlayer->stats.damage * 3;
				// Check if the enemy life has gone under 0 (death)
				if (enemy->stats.currentHP < 0) enemy->stats.currentHP = 0;
				break;
			case SpecialAttack::ATTACK_2:
				break;
			case SpecialAttack::ATTACK_3:
				break;
			case SpecialAttack::DEFAULT:
				if (specialAttacks[2])
				{
					// Substract life points to the enemy
					enemy->stats.currentHP -= currentPlayer->stats.damage + (currentPlayer->stats.damage * 0.3);
				}
				else
				{
					// Substract life points to the enemy
					enemy->stats.currentHP -= currentPlayer->stats.damage;
				}
				// Check if the enemy life has gone under 0 (death)
				if (enemy->stats.currentHP < 0) enemy->stats.currentHP = 0;
				break;
			case SpecialAttack::NONE:
				break;
			}
			break;
		case PlayerState::DEFENSE:
			switch (specialDefense)
			{
			case SpecialDefense::DEFENSE_1:
				// Add life points to the player
				currentPlayer->stats.currentHP += currentPlayer->stats.defense * 5;
				// Check if the player life has gone over their maximum HP
				if (currentPlayer->stats.currentHP > currentPlayer->stats.maxHP)
					currentPlayer->stats.currentHP = currentPlayer->stats.maxHP;
				break;
			case SpecialDefense::DEFENSE_2:
				break;
			case SpecialDefense::DEFENSE_3:
				break;
			case SpecialDefense::DEFAULT:
				if (specialDefenses[2])
				{
					// Add life points to the player
					currentPlayer->stats.currentHP += currentPlayer->stats.defense + (currentPlayer->stats.defense * 0.3);
				}
				else
				{
					// Add life points to the player
					currentPlayer->stats.currentHP += currentPlayer->stats.defense;
				}
				// Check if the player life has gone over their maximum HP
				if (currentPlayer->stats.currentHP > currentPlayer->stats.maxHP)
					currentPlayer->stats.currentHP = currentPlayer->stats.maxHP;
				break;
			case SpecialDefense::NONE:
				break;
			}
			break;
		case PlayerState::ITEM:
			// Get into the items menu
			// Items logic
			//currentPlayer->stats = items->At(0)->data->Interact(currentPlayer->stats);
			break;
		case PlayerState::RUN:
			// Return to the gameplay screen
			break;
		case PlayerState::NONE:
			// ...
			break;
		default:
			break;
		}
	}

	if (playerState != PlayerState::NONE)
	{
		if (playerState == PlayerState::ITEM)
		{
			if (hasFinishedItems) turnCounter++;
		}
		else if (playerState == PlayerState::ATTACK)
		{
			if (specialAttack != SpecialAttack::NONE && specialAttack != SpecialAttack::LOCKED) turnCounter++;
		}
		else if (playerState == PlayerState::DEFENSE)
		{
			if (specialDefense != SpecialDefense::NONE && specialDefense != SpecialDefense::LOCKED) turnCounter++;
		}
		else turnCounter++;
	}

	if (turnCounter > 200 && playerState != PlayerState::NONE)
	{
		switch (playerState)
		{
		case PlayerState::ATTACK:
			switch (specialAttack)
			{
			case SpecialAttack::ATTACK_1:
				specialAttacks[0] = true;
				break;
			case SpecialAttack::ATTACK_2:
				specialAttacks[1] = true;
				break;
			case SpecialAttack::ATTACK_3:
				specialAttacks[2] = true;
				break;
			default:
				break;
			}
			break;
		case PlayerState::DEFENSE:
			switch (specialDefense)
			{
			case SpecialDefense::DEFENSE_1:
				specialDefenses[0] = true;
				break;
			case SpecialDefense::DEFENSE_2:
				specialDefenses[1] = true;
				break;
			case SpecialDefense::DEFENSE_3:
				specialDefenses[2] = true;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		// Reset variables for the next time
		turnCounter = 0;
		playerState = PlayerState::NONE;

		// Check if the enemy has died
		if (enemy->stats.currentHP <= 0)
		{
			battleState = BattleState::WON;
			currentMusic = BattleMusic::WON;
			turnChanged = true;
		}
		else
		{
			battleState = BattleState::ENEMY_TURN;
			turnChanged = true;
		}
	}
}

void BattleSystem::EnemyTurn()
{
	if (turnChanged) turnChanged = false;

	// Define randomly an action for the enemy (only the first time)
	if (turnCounter == 0)
	{
		int num = rand() % 100;
		if (num <= 33)
		{
			// Player attack logic
			enemyState = EnemyState::ATTACK;

			// Substract life points to the player
			if (turnCounter == 0 && currentPlayer->isGod == false) // Player won't lose health if godmode is true
			{
				currentPlayer->stats.currentHP -= enemy->stats.damage;
				if (currentPlayer->stats.currentHP < 0) currentPlayer->stats.currentHP = 0;
			}
		}
		else if (num > 33 && num <= 66)
		{
			// Player defense logic
			enemyState = EnemyState::DEFEND;

			// Add life points to the enemy (only the first time)
			if (turnCounter == 0)
			{
				enemy->stats.currentHP += enemy->stats.defense;

				// Check if the enemy life has gone over their maximum
				if (enemy->stats.currentHP > enemy->stats.maxHP)
					enemy->stats.currentHP = enemy->stats.maxHP;
			}
		}
		else if (num > 66)
		{
			// Player item logic
			enemyState = EnemyState::ITEM;

			if (turnCounter == 0)
			{
				ApplyEnemyItem();
			}
		}
	}

	turnCounter++;

	if (turnCounter > 200)
	{
		// Reset variables for the next time
		turnCounter = 0;
		enemyState = EnemyState::NONE;

		// Check if the player has died
		if (currentPlayer->stats.currentHP <= 0)
		{
			battleState = BattleState::LOST;
			currentMusic = BattleMusic::LOST;
			turnChanged = true;
		}
		else
		{
			battleState = BattleState::PLAYER_TURN;
			turnChanged = true;
		}

		// Set as the current player the next party member (if it's available)
		if (players->Count() > 1)
		{
			for (int i = 0; i < players->Count(); i++)
			{
				if (players->At(i)->data != nullptr)
				{
					if (players->At(i)->data->name == currentPlayer->name)
					{
						if (i == players->Count() - 1)
							currentPlayer = players->At(0)->data;
						else if (players->At(i)->next->data != nullptr)
							currentPlayer = players->At(i)->next->data;

						break;
					}
				}
			}
		}
	}
}

Player* BattleSystem::GetPlayer()
{
	return currentPlayer;
}

List<Player*>* BattleSystem::GetPlayersList()
{
	return players;
}

Enemy* BattleSystem::GetEnemy()
{
	return enemy;
}

bool BattleSystem::IsTurnChanging()
{
	return turnChanged;
}

uint BattleSystem::GetCounter()
{
	return turnCounter;
}

bool BattleSystem::OnBattle()
{
	return currentPlayer != nullptr && enemy != nullptr;
}

bool BattleSystem::HasOpenedInventory()
{
	return battleInventory;
}

void BattleSystem::SetInventoryOpening(bool InventoryOpeningState)
{
	battleInventory = InventoryOpeningState;
}

bool BattleSystem::HasClosedInventory()
{
	return hasFinishedItems;
}

void BattleSystem::SetInventoryClosure(bool InventoryClosureState)
{
	hasFinishedItems = InventoryClosureState;
}

void BattleSystem::SetItem(SString itemName)
{
	this->itemName = itemName;
}

SString BattleSystem::GetItem()
{
	return itemName;
}

void BattleSystem::SetHasClickedConsume(bool hasClickedConsume)
{
	this->hasClickedConsume = hasClickedConsume;
}

bool BattleSystem::GetHasClickedConsume()
{
	return hasClickedConsume;
}

void BattleSystem::ApplyEnemyItem()
{
	pugi::xml_document docData;
	pugi::xml_node enemyItemsNode;

	pugi::xml_parse_result result = docData.load_file("entity_info.xml");
	enemyItemsNode = docData.child("entity").child("enemy_items");
	int itemCount = enemyItemsNode.attribute("item_count").as_int();

	pugi::xml_node itemListNode;
	itemListNode = enemyItemsNode.first_child();

	// Random number for a random item
	int num = rand() % itemCount;
	for (int i = 0; i < num; i++)
	{
		itemListNode = itemListNode.next_sibling();
	}

	// Creating a temporal item
	Item* item = new Item();
	// Loading item properties
	LOG("Loading enemy item entity info");
	item->name = itemListNode.attribute("name").as_string();
	item->SetDescription(itemListNode.attribute("description").as_string());
	item->stats.level = itemListNode.attribute("level").as_int();
	item->stats.damage = itemListNode.attribute("damage").as_int();
	item->stats.maxHP = itemListNode.attribute("max_hp").as_int();
	item->stats.currentHP = itemListNode.attribute("current_hp").as_int();
	item->stats.strength = itemListNode.attribute("strength").as_int();
	item->stats.defense = itemListNode.attribute("defense").as_int();
	item->stats.attackSpeed = itemListNode.attribute("attack_speed").as_int();
	item->stats.criticalRate = itemListNode.attribute("critical_rate").as_int();
	LOG("Enemy item entity info loaded");

	ConsumeItem(item);
}

void BattleSystem::ConsumeItem(Item* item)
{
	enemy->stats = item->Interact(enemy->stats);
	itemName = item->name;
}

void BattleSystem::UpdateAttacksAndDefenses()
{
	if (specialAttacks[2])
	{
		// Substract life points to the enemy
		enemy->stats.currentHP -= currentPlayer->stats.damage / 5;
		// Check if the enemy life has gone under 0 (death)
		if (enemy->stats.currentHP < 0) enemy->stats.currentHP = 0;
	}

	if (specialDefenses[2])
	{
		// Add life points to the player
		currentPlayer->stats.currentHP += currentPlayer->stats.defense / 5;
		// Check if the player life has gone over their maximum HP
		if (currentPlayer->stats.currentHP > currentPlayer->stats.maxHP)
			currentPlayer->stats.currentHP = currentPlayer->stats.maxHP;
	}
}

bool BattleSystem::GetSpecialAttack(int index)
{
	return specialAttacks[index];
}

bool BattleSystem::GetSpecialDefense(int index)
{
	return specialDefenses[index];
}
