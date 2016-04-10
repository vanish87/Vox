// ******************************************************************************
// Filename:    RandomLootManager.h
// Project:     Vox
// Author:      Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 01/04/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#pragma once

#include "Item.h"
#include "ItemsEnum.h"
#include "../Enemy/Enemy.h"
#include "../Inventory/InventoryManager.h"

#include <vector>
#include <string>
using namespace std;


class EnemyIngredientsSpawnData
{
public:
	eEnemyType m_sourceEnemy;
	eItem m_spawnedItem;
	int m_minSpawn;
	int m_maxSpawn;
};

typedef vector<EnemyIngredientsSpawnData*> EnemyIngredientsSpawnDataList;

class RandomLootManager
{
public:
	/* Public methods */
	RandomLootManager();
	~RandomLootManager();

	// Deletion
	void ClearEnemyIngredientsSpawnData();

	// Enemy dropping ingredients
	void GetSpawnedIngredientItemForEnemy(eEnemyType sourceEnemy, eItem *item, int *quantity);

	// Random loot
	InventoryItem* GetRandomLootItem();

protected:
	/* Protected methods */

private:
	/* Private methods */
	void AddEnemyIngredientSpawnData(eEnemyType sourceEnemy, eItem spawnedItem, int minSpawn, int maxSpawn);

public:
	/* Public members */

protected:
	/* Protected members */

private:
	/* Private members */
	EnemyIngredientsSpawnDataList m_vpEnemyIngredientsSpawnDataList;
};
