// ******************************************************************************
// Filename:    ModsManager.h
// Project:     Vox
// Author:      Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 12/03/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#pragma once

#include <vector>
#include <string>
using namespace std;


class Mod
{
public:
	string m_modName;
	string m_modDescription;

};

class ModsManager
{
public:
	/* Public methods */
	ModsManager();
	~ModsManager();

	// Clearing
	void ClearMods();

	// Loading
	void LoadMods();

	// Accessors
	int GetNumMods();
	Mod* GetMod(int index);

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
	vector<Mod*> m_vpMods;
};
