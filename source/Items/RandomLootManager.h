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
#include "..\Inventory\InventoryManager.h"

#include <vector>
#include <string>
using namespace std;


class RandomLootManager
{
public:
	/* Public methods */
	RandomLootManager();
	~RandomLootManager();

	InventoryItem* GetRandomLootItem();

protected:
	/* Protected methods */

private:
	/* Private methods */

public:
	/* Public members */

protected:
	/* Protected members */

private:
	/* Private members */
};
