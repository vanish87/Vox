// ******************************************************************************
// Filename:	ChunkManager.h
// Project:		Game
// Author:		Steven Ball
//
// Purpose:
//   ChunkManager handles all the chunk operations and management of the
//   individual chunks, deciding which chunks to load, unload, create, update
//   and also the rendering of the chunks.
//
// Revision History:
//   Initial Revision - 01/11/15
//
// Copyright (c) 2005-2015, Steven Ball
// ******************************************************************************

#pragma once

#include "../Renderer/Renderer.h"

#include "Chunk.h"


class ChunkManager
{
public:
	/* Public methods */
	ChunkManager(Renderer* pRenderer);
	~ChunkManager();

	// Updating
	void Update(float dt);
	
	// Rendering
	void Render();

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
	Renderer* m_pRenderer;
};
