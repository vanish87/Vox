// ******************************************************************************
// Filename:	ChunkManager.cpp
// Project:		Game
// Author:		Steven Ball
//
// Revision History:
//   Initial Revision - 01/11/15
//
// Copyright (c) 2005-2015, Steven Ball
// ******************************************************************************

#include "ChunkManager.h"


ChunkManager::ChunkManager(Renderer* pRenderer)
{
	m_pRenderer = pRenderer;

	// Chunk material
	m_chunkMaterialID = -1;
	m_pRenderer->CreateMaterial(Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f), 256, &m_chunkMaterialID);

	// Create initial chunk
	CreateNewChunk(0, 0, 0);
	CreateNewChunk(0, 0, 1);
}

ChunkManager::~ChunkManager()
{
}

// Chunk rendering material
unsigned int ChunkManager::GetChunkMaterialID()
{
	return m_chunkMaterialID;
}

// Chunk Creation
void ChunkManager::CreateNewChunk(int x, int y, int z)
{
	ChunkCoordKeys coordKeys;
	coordKeys.x = x;
	coordKeys.y = y;
	coordKeys.z = z;

	// Create a new chunk at this grid position
	Chunk* pNewChunk = new Chunk(m_pRenderer);

	float xPos = x * (Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f);
	float yPos = y * (Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f);
	float zPos = z * (Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f);

	pNewChunk->SetPosition(vec3(xPos, yPos, zPos));
	pNewChunk->SetGrid(coordKeys.x, coordKeys.y, coordKeys.z);

	pNewChunk->RebuildMesh();

	m_chunksMap[coordKeys] = pNewChunk;
}

void ChunkManager::UnloadChunk(Chunk* pChunk)
{
	ChunkCoordKeys coordKeys;
	coordKeys.x = pChunk->GetGridX();
	coordKeys.y = pChunk->GetGridY();
	coordKeys.z = pChunk->GetGridZ();
}

// Getting chunk and positional information
void ChunkManager::GetGridFromPosition(vec3 position, int* gridX, int* gridY, int* gridZ)
{
	*gridX = (int)((position.x + Chunk::BLOCK_RENDER_SIZE) / Chunk::CHUNK_SIZE);
	*gridY = (int)((position.y + Chunk::BLOCK_RENDER_SIZE) / Chunk::CHUNK_SIZE);
	*gridZ = (int)((position.z + Chunk::BLOCK_RENDER_SIZE) / Chunk::CHUNK_SIZE);

	if (position.x <= -0.5f)
		*gridX -= 1;
	if (position.y <= -0.5f)
		*gridY -= 1;
	if (position.z <= -0.5f)
		*gridZ -= 1;
}

Chunk* ChunkManager::GetChunkFromPosition(float posX, float posY, float posZ)
{
	int gridX = (int)((posX + Chunk::BLOCK_RENDER_SIZE) / Chunk::CHUNK_SIZE);
	int gridY = (int)((posY + Chunk::BLOCK_RENDER_SIZE) / Chunk::CHUNK_SIZE);
	int gridZ = (int)((posZ + Chunk::BLOCK_RENDER_SIZE) / Chunk::CHUNK_SIZE);

	if (posX <= -0.5f)
		gridX -= 1;
	if (posY <= -0.5f)
		gridY -= 1;
	if (posZ <= -0.5f)
		gridZ -= 1;

	return GetChunk(gridX, gridY, gridZ);
}

Chunk* ChunkManager::GetChunk(int aX, int aY, int aZ)
{
	ChunkCoordKeys chunkKey;
	chunkKey.x = aX;
	chunkKey.y = aY;
	chunkKey.z = aZ;

	map<ChunkCoordKeys, Chunk*>::iterator it = m_chunksMap.find(chunkKey);
	if (it != m_chunksMap.end())
	{
		Chunk* lpReturn = m_chunksMap[chunkKey];
		return lpReturn;
	}

	return NULL;
}

// Updating
void ChunkManager::Update(float dt)
{

}

// Rendering
void ChunkManager::Render()
{
	typedef map<ChunkCoordKeys, Chunk*>::iterator it_type;
	for (it_type iterator = m_chunksMap.begin(); iterator != m_chunksMap.end(); iterator++)
	{
		Chunk* pChunk = iterator->second;

		if (pChunk != NULL)
		{
			pChunk->Render();
		}
	}
}

void ChunkManager::RenderDebug()
{
	typedef map<ChunkCoordKeys, Chunk*>::iterator it_type;
	for (it_type iterator = m_chunksMap.begin(); iterator != m_chunksMap.end(); iterator++)
	{
		Chunk* pChunk = iterator->second;

		if (pChunk != NULL)
		{
			pChunk->RenderDebug();
		}
	}
}