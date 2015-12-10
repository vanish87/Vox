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

#include "../Renderer/Renderer.h"
#include "../Renderer/Camera.h"


class Chunk
{
public:
	/* Public methods */
	Chunk(Renderer* pRenderer);
	~Chunk();

	// Initialize
	void Initialize();

	// Creation and destruction
	void Unload();
	void Setup();

	// Saving and loading
	void SaveChunk();
	void LoadChunk();

	// Position
	void SetPosition(vec3 pos);
	vec3 GetPosition();

	// Neighbours
	int GetNumNeighbours();
	void SetNumNeighbours(int neighbours);
	Chunk* GetxMinus();
	Chunk* GetxPlus();
	Chunk* GetyMinus();
	Chunk* GetyPlus();
	Chunk* GetzMinus();
	Chunk* GetzPlus();
	void SetxMinus(Chunk* pChunk);
	void SetxPlus(Chunk* pChunk);
	void SetyMinus(Chunk* pChunk);
	void SetyPlus(Chunk* pChunk);
	void SetzMinus(Chunk* pChunk);
	void SetzPlus(Chunk* pChunk);

	// Grid
	void SetGrid(int x, int y, int z);
	int GetGridX();
	int GetGridY();
	int GetGridZ();

	// Active
	bool GetActive(int x, int y, int z);

	// Block colour
	void SetColour(int x, int y, int z, float r, float g, float b, float a);
	void GetColour(int x, int y, int z, float* r, float* g, float* b, float* a);
	void SetColour(int x, int y, int z, unsigned int colour);
	unsigned int GetColour(int x, int y, int z);

	// Flags
	bool IsEmpty();
	bool IsSurrounded();

	// Create mesh
	void CreateMesh();

	// Rebuild
	void RebuildMesh();

	// Updating
	void Update(float dt);
	
	// Rendering
	void Render();
	void RenderDebug();
	void Render2D(Camera* pCamera, unsigned int viewport, unsigned int font);

protected:
	/* Protected methods */

private:
	/* Private methods */

public:
	/* Public members */
	static const int CHUNK_SIZE = 16;
	static const int CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
	static const int CHUNK_SIZE_CUBED = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
	static const float BLOCK_RENDER_SIZE;
	static const float CHUNK_RADIUS;

protected:
	/* Protected members */

private:
	/* Private members */
	Renderer* m_pRenderer;

	// Chunk neighbours
	int m_numNeighbours;
	Chunk* m_pxMinus;
	Chunk* m_pxPlus;
	Chunk* m_pyMinus;
	Chunk* m_pyPlus;
	Chunk* m_pzMinus;
	Chunk* m_pzPlus;

	// Grid co-ordinates
	int m_gridX;
	int m_gridY;
	int m_gridZ;

	// Chunk position
	vec3 m_position;

	// Flags for empty chunk and completely surrounded
	bool m_emptyChunk;
	bool m_surroundedChunk;

	// The blocks data
	unsigned int *m_colour;

	// Render mesh
	OpenGLTriangleMesh* m_pMesh;
};
