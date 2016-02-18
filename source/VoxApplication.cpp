// ******************************************************************************
// Filename:	VoxApplication.cpp
// Project:	Vox
// Author:	Steven Ball
// 
// Revision History:
//   Initial Revision - 27/10/15
//
// Copyright (c) 2005-2015, Steven Ball
// ******************************************************************************

#include "VoxApplication.h"
#include "VoxGame.h"
#include "VoxSettings.h"


VoxApplication::VoxApplication(VoxGame* pVoxGame, VoxSettings* pVoxSettings)
{
	m_pVoxGame = pVoxGame;
	m_pVoxSettings = pVoxSettings;
}

VoxApplication::~VoxApplication()
{
}

// Creation
void VoxApplication::Create()
{
}

void VoxApplication::Destroy()
{
}

// Update
void VoxApplication::Update(float dt)
{
}
