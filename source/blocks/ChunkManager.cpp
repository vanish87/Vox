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
#include "../Player/Player.h"


ChunkManager::ChunkManager(Renderer* pRenderer)
{
	m_pRenderer = pRenderer;

	// Chunk material
	m_chunkMaterialID = -1;
	m_pRenderer->CreateMaterial(Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f), 256, &m_chunkMaterialID);

	// Create initial chunk
	CreateNewChunk(0, 0, 0);
}

ChunkManager::~ChunkManager()
{
}

void ChunkManager::SetPlayer(Player* pPlayer)
{
	m_pPlayer = pPlayer;
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

	UpdateChunkNeighbours(pNewChunk, x, y, z);

	m_chunksMap[coordKeys] = pNewChunk;
}

void ChunkManager::UpdateChunkNeighbours(Chunk* pChunk, int x, int y, int z)
{
	pChunk->SetNumNeighbours(0);

	Chunk* pChunkXMinus = GetChunk(x - 1, y, z);
	Chunk* pChunkXPlus = GetChunk(x + 1, y, z);
	Chunk* pChunkYMinus = GetChunk(x, y - 1, z);
	Chunk* pChunkYPlus = GetChunk(x, y + 1, z);
	Chunk* pChunkZMinus = GetChunk(x, y, z - 1);
	Chunk* pChunkZPlus = GetChunk(x, y, z + 1);

	if (pChunkXMinus)
	{
		pChunk->SetNumNeighbours(pChunk->GetNumNeighbours() + 1);
		pChunk->SetxMinus(pChunkXMinus);
		if (pChunkXMinus->GetxPlus() == NULL)
		{
			pChunkXMinus->SetNumNeighbours(pChunkXMinus->GetNumNeighbours() + 1);
			pChunkXMinus->SetxPlus(pChunk);
		}
	}
	if (pChunkXPlus)
	{
		pChunk->SetNumNeighbours(pChunk->GetNumNeighbours() + 1);
		pChunk->SetxPlus(pChunkXPlus);
		if (pChunkXPlus->GetxMinus() == NULL)
		{
			pChunkXPlus->SetNumNeighbours(pChunkXPlus->GetNumNeighbours() + 1);
			pChunkXPlus->SetxMinus(pChunk);
		}
	}
	if (pChunkYMinus)
	{
		pChunk->SetNumNeighbours(pChunk->GetNumNeighbours() + 1);
		pChunk->SetyMinus(pChunkYMinus);
		if (pChunkYMinus->GetyPlus() == NULL)
		{
			pChunkYMinus->SetNumNeighbours(pChunkYMinus->GetNumNeighbours() + 1);
			pChunkYMinus->SetyPlus(pChunk);
		}
	}
	if (pChunkYPlus)
	{
		pChunk->SetNumNeighbours(pChunk->GetNumNeighbours() + 1);
		pChunk->SetyPlus(pChunkYPlus);
		if (pChunkYPlus->GetyMinus() == NULL)
		{
			pChunkYPlus->SetNumNeighbours(pChunkYPlus->GetNumNeighbours() + 1);
			pChunkYPlus->SetyMinus(pChunk);
		}
	}
	if (pChunkZMinus)
	{
		pChunk->SetNumNeighbours(pChunk->GetNumNeighbours() + 1);
		pChunk->SetzMinus(pChunkZMinus);
		if (pChunkZMinus->GetzPlus() == NULL)
		{
			pChunkZMinus->SetNumNeighbours(pChunkZMinus->GetNumNeighbours() + 1);
			pChunkZMinus->SetzPlus(pChunk);
		}
	}
	if (pChunkZPlus)
	{
		pChunk->SetNumNeighbours(pChunk->GetNumNeighbours() + 1);
		pChunk->SetzPlus(pChunkZPlus);
		if (pChunkZPlus->GetzMinus() == NULL)
		{
			pChunkZPlus->SetNumNeighbours(pChunkZPlus->GetNumNeighbours() + 1);
			pChunkZPlus->SetzMinus(pChunk);
		}
	}
}

void ChunkManager::UnloadChunk(Chunk* pChunk)
{
	ChunkCoordKeys coordKeys;
	coordKeys.x = pChunk->GetGridX();
	coordKeys.y = pChunk->GetGridY();
	coordKeys.z = pChunk->GetGridZ();

	Chunk* pChunkXMinus = GetChunk(coordKeys.x - 1, coordKeys.y, coordKeys.z);
	Chunk* pChunkXPlus = GetChunk(coordKeys.x + 1, coordKeys.y, coordKeys.z);
	Chunk* pChunkYMinus = GetChunk(coordKeys.x, coordKeys.y - 1, coordKeys.z);
	Chunk* pChunkYPlus = GetChunk(coordKeys.x, coordKeys.y + 1, coordKeys.z);
	Chunk* pChunkZMinus = GetChunk(coordKeys.x, coordKeys.y, coordKeys.z - 1);
	Chunk* pChunkZPlus = GetChunk(coordKeys.x, coordKeys.y, coordKeys.z + 1);

	if (pChunkXMinus)
	{
		if (pChunkXMinus->GetxPlus())
		{
			pChunkXMinus->SetNumNeighbours(pChunkXMinus->GetNumNeighbours() - 1);
			pChunkXMinus->SetxPlus(NULL);
		}
	}
	if (pChunkXPlus)
	{
		if (pChunkXPlus->GetxMinus())
		{
			pChunkXPlus->SetNumNeighbours(pChunkXPlus->GetNumNeighbours() - 1);
			pChunkXPlus->SetxMinus(NULL);
		}
	}
	if (pChunkYMinus)
	{
		if (pChunkYMinus->GetyPlus())
		{
			pChunkYMinus->SetNumNeighbours(pChunkYMinus->GetNumNeighbours() - 1);
			pChunkYMinus->SetyPlus(NULL);
		}
	}
	if (pChunkYPlus)
	{
		if (pChunkYPlus->GetyMinus())
		{
			pChunkYPlus->SetNumNeighbours(pChunkYPlus->GetNumNeighbours() - 1);
			pChunkYPlus->SetyMinus(NULL);
		}
	}
	if (pChunkZMinus)
	{
		if (pChunkZMinus->GetzPlus())
		{
			pChunkZMinus->SetNumNeighbours(pChunkZMinus->GetNumNeighbours() - 1);
			pChunkZMinus->SetzPlus(NULL);
		}
	}
	if (pChunkZPlus)
	{
		if (pChunkZPlus->GetzMinus())
		{
			pChunkZPlus->SetNumNeighbours(pChunkZPlus->GetNumNeighbours() - 1);
			pChunkZPlus->SetzMinus(NULL);
		}
	}

	// Remove from map
	map<ChunkCoordKeys, Chunk*>::iterator it = m_chunksMap.find(coordKeys);
	if (it != m_chunksMap.end())
	{
		m_chunksMap.erase(coordKeys);
	}

	// Unload and delete
	pChunk->Unload();
	delete pChunk;
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
	ChunkCoordKeysList addChunkList;
	ChunkList unloadChunkList;

	typedef map<ChunkCoordKeys, Chunk*>::iterator it_type;
	for (it_type iterator = m_chunksMap.begin(); iterator != m_chunksMap.end(); iterator++)
	{
		Chunk* pChunk = iterator->second;

		if (pChunk != NULL)
		{
			pChunk->Update(dt);

			int gridX = pChunk->GetGridX();
			int gridY = pChunk->GetGridY();
			int gridZ = pChunk->GetGridZ();

			float xPos = gridX * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;
			float yPos = gridY * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;
			float zPos = gridZ * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;

			vec3 distanceVec = vec3(xPos, yPos, zPos) - m_pPlayer->GetCenter();
			float lengthValue = length(distanceVec);

			if (lengthValue > 32.0f)
			{
				unloadChunkList.push_back(pChunk);
			}
			else
			{
				// Check neighbours
				if (pChunk->GetNumNeighbours() < 6)
				{
					if (pChunk->GetxMinus() == NULL)
					{
						ChunkCoordKeys coordKey;
						coordKey.x = gridX - 1;
						coordKey.y = gridY;
						coordKey.z = gridZ;
						addChunkList.push_back(coordKey);
					}
					if (pChunk->GetxPlus() == NULL)
					{
						ChunkCoordKeys coordKey;
						coordKey.x = gridX + 1;
						coordKey.y = gridY;
						coordKey.z = gridZ;
						addChunkList.push_back(coordKey);
					}
					if (pChunk->GetyMinus() == NULL)
					{
						ChunkCoordKeys coordKey;
						coordKey.x = gridX;
						coordKey.y = gridY - 1;
						coordKey.z = gridZ;
						addChunkList.push_back(coordKey);
					}
					if (pChunk->GetyPlus() == NULL)
					{
						ChunkCoordKeys coordKey;
						coordKey.x = gridX;
						coordKey.y = gridY + 1;
						coordKey.z = gridZ;
						addChunkList.push_back(coordKey);
					}
					if (pChunk->GetzMinus() == NULL)
					{
						ChunkCoordKeys coordKey;
						coordKey.x = gridX;
						coordKey.y = gridY;
						coordKey.z = gridZ - 1;
						addChunkList.push_back(coordKey);
					}
					if (pChunk->GetzPlus() == NULL)
					{
						ChunkCoordKeys coordKey;
						coordKey.x = gridX;
						coordKey.y = gridY;
						coordKey.z = gridZ + 1;
						addChunkList.push_back(coordKey);
					}
				}
			}
		}
	}

	// Adding chunks
	for (unsigned int i = 0; i < (int)addChunkList.size(); i++)
	{
		ChunkCoordKeys coordKey = addChunkList[i];
		Chunk* pChunk = GetChunk(coordKey.x, coordKey.y, coordKey.z);

		if (pChunk == NULL)
		{
			float xPos = coordKey.x * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;
			float yPos = coordKey.y * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;
			float zPos = coordKey.z * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;

			vec3 distanceVec = vec3(xPos, yPos, zPos) - m_pPlayer->GetCenter();
			float lengthValue = length(distanceVec);

			if (lengthValue <= 32.0f)
			{
				CreateNewChunk(coordKey.x, coordKey.y, coordKey.z);
			}
		}
		else
		{
			UpdateChunkNeighbours(pChunk, coordKey.x, coordKey.y, coordKey.z);
		}
	}
	addChunkList.clear();

	// Unloading chunks
	for (unsigned int i = 0; i < (int)unloadChunkList.size(); i++)
	{
		Chunk* pChunk = unloadChunkList[i];

		UnloadChunk(pChunk);
	}
	unloadChunkList.clear();
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

void ChunkManager::Render2D(Camera* pCamera, unsigned int viewport, unsigned int font)
{
	typedef map<ChunkCoordKeys, Chunk*>::iterator it_type;
	for (it_type iterator = m_chunksMap.begin(); iterator != m_chunksMap.end(); iterator++)
	{
		Chunk* pChunk = iterator->second;

		if (pChunk != NULL)
		{
			pChunk->Render2D(pCamera, viewport, font);
		}
	}
}