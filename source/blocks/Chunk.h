// ******************************************************************************
// Filename:	Chunk.h
// Project:		Game
// Author:		Steven Ball
//
// Purpose:
//   A chunk is a collection of voxel blocks that are arranged together for
//   easier manipulation and management, when a single voxel in a chunk is
//   modified the whole chunk is refreshed. Chunks are rendered together
//   as a single vertex buffer and thus each chunk can be considered a single
//   draw call to render many voxels.
//
// Revision History:
//   Initial Revision - 01/11/15
//
// Copyright (c) 2005-2015, Steven Ball
// ******************************************************************************

#pragma once


class Chunk
{
public:
	/* Public methods */
	Chunk();
	~Chunk();

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
};
