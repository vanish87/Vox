// ******************************************************************************
// Filename:    ModsManager.cpp
// Project:     Vox
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 12/03/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "ModsManager.h"


ModsManager::ModsManager()
{
}

ModsManager::~ModsManager()
{
	ClearMods();
}

// Clearing
void ModsManager::ClearMods()
{
	for (unsigned int i = 0; i < m_vpMods.size(); i++)
	{
		delete m_vpMods[i];
		m_vpMods[i] = 0;
	}
	m_vpMods.clear();
}

// Loading
void ModsManager::LoadMods()
{
	// Load the HUD textures mod
	Mod* pNewMod = new Mod();
	pNewMod->m_modName = "Stonewash";
	pNewMod->m_gameplayMod = false;
	pNewMod->m_graphicsMod = false;
	pNewMod->m_soundMod = false;
	pNewMod->m_HUDMod = true;
	pNewMod->m_miscMod = false;
	m_vpMods.push_back(pNewMod);
}

// Accessors
int ModsManager::GetNumMods()
{
	return (int)m_vpMods.size();
}

Mod* ModsManager::GetMod(int index)
{
	return m_vpMods[index];
}
