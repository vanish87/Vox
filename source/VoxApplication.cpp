// ******************************************************************************
// Filename:	VoxApplication.cpp
// Project:		Vox
// Author:		Steven Ball
// 
// Revision History:
//   Initial Revision - 27/10/15
//
// Copyright (c) 2005-2015, Steven Ball
// ******************************************************************************

#include "VoxApplication.h"
#include "VoxGame.h"


VoxApplication::VoxApplication(VoxGame* pVoxGame)
{
	m_pVoxGame = pVoxGame;
}

VoxApplication::~VoxApplication()
{
}

void VoxApplication::Create()
{
}

void VoxApplication::Destroy()
{
}

void VoxApplication::Update(float dt)
{
}

int VoxApplication::ShouldCloseApplication()
{
	return 0;
}