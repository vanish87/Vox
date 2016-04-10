// ******************************************************************************
// Filename:    RandomLootManager.cpp
// Project:     Vox
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 01/04/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "RandomLootManager.h"
#include "../utils/Random.h"

#include <fstream>
#include <ostream>
#include <iostream>
#include <string>
using namespace std;


RandomLootManager::RandomLootManager()
{
	// Create the enemy ingredient spawn data lists
	//AddEnemyIngredientSpawnData(eEnemyType_GreenSlime, eItem_SlimeJelly, 1, 1);
	//AddEnemyIngredientSpawnData(eEnemyType_RedSlime, eItem_SlimeJelly, 1, 1);
	//AddEnemyIngredientSpawnData(eEnemyType_BlueSlime, eItem_SlimeJelly, 1, 1);
	//AddEnemyIngredientSpawnData(eEnemyType_YellowSlime, eItem_SlimeJelly, 1, 1);
}

RandomLootManager::~RandomLootManager()
{
	ClearEnemyIngredientsSpawnData();
}

// Deletion
void RandomLootManager::ClearEnemyIngredientsSpawnData()
{
	for (unsigned int i = 0; i < m_vpEnemyIngredientsSpawnDataList.size(); i++)
	{
		delete m_vpEnemyIngredientsSpawnDataList[i];
		m_vpEnemyIngredientsSpawnDataList[i] = 0;
	}
	m_vpEnemyIngredientsSpawnDataList.clear();
}

// Enemy dropping ingredients
void RandomLootManager::GetSpawnedIngredientItemForEnemy(eEnemyType sourceEnemy, eItem *item, int *quantity)
{
	for (unsigned int i = 0; i < m_vpEnemyIngredientsSpawnDataList.size(); i++)
	{
		if (m_vpEnemyIngredientsSpawnDataList[i]->m_sourceEnemy == sourceEnemy)
		{
			*item = m_vpEnemyIngredientsSpawnDataList[i]->m_spawnedItem;

			if (m_vpEnemyIngredientsSpawnDataList[i]->m_minSpawn == m_vpEnemyIngredientsSpawnDataList[i]->m_maxSpawn)
			{
				*quantity = m_vpEnemyIngredientsSpawnDataList[i]->m_minSpawn;
			}
			else
			{
				*quantity = GetRandomNumber(m_vpEnemyIngredientsSpawnDataList[i]->m_minSpawn, m_vpEnemyIngredientsSpawnDataList[i]->m_maxSpawn);
			}

			return;
		}
	}

	*item = eItem_None;
	*quantity = 0;
}

// Random loot
InventoryItem* RandomLootManager::GetRandomLootItem()
{
	return NULL;
}

void RandomLootManager::AddEnemyIngredientSpawnData(eEnemyType sourceEnemy, eItem spawnedItem, int minSpawn, int maxSpawn)
{
	EnemyIngredientsSpawnData* pEnemyIngredientsSpawnData = new EnemyIngredientsSpawnData();
	
	pEnemyIngredientsSpawnData->m_sourceEnemy = sourceEnemy;
	pEnemyIngredientsSpawnData->m_spawnedItem = spawnedItem;
	pEnemyIngredientsSpawnData->m_minSpawn = minSpawn;
	pEnemyIngredientsSpawnData->m_maxSpawn = maxSpawn;

	m_vpEnemyIngredientsSpawnDataList.push_back(pEnemyIngredientsSpawnData);
}