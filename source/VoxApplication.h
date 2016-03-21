// ******************************************************************************
// Filename:	VoxApplication.h
// Project:	Vox
// Author:	Steven Ball
//
// Purpose:
//   The Vox application class is the base level class for an application,
//   contains an instance of the Vox window and also the Vox game.
// 
// Revision History:
//   Initial Revision - 27/10/15
//
// Copyright (c) 2005-2015, Steven Ball
// ******************************************************************************

#pragma once

class VoxGame;
class VoxSettings;


class VoxApplication
{
public:
	/* Public methods */
	VoxApplication(VoxGame* pVoxGame, VoxSettings* pVoxSettings);
	~VoxApplication();
	
	// Creation
	void Create();
	void Destroy();

	// Update
	void Update(float dt);

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
	VoxSettings* m_pVoxSettings;
};
