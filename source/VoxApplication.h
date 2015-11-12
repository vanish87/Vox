// ******************************************************************************
// Filename:	VoxApplication.h
// Project:		Vox
// Author:		Steven Ball
//
// Purpose:
//   Vox application class.
// 
// Revision History:
//   Initial Revision - 27/10/15
//
// Copyright (c) 2005-2015, Steven Ball
// ******************************************************************************

#pragma once

class VoxGame;


class VoxApplication
{
public:
	/* Public methods */
	VoxApplication(VoxGame* pVoxGame);
	~VoxApplication();

	void Create();
	void Destroy();
	void Update(float dt);

	int ShouldCloseApplication();

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
	VoxGame* m_pVoxGame;
};