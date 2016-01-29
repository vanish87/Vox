// ******************************************************************************
// Filename:	ChunkManager.cpp
// Project:	Game
// Author:	Steven Ball
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
	m_pPlayer = NULL;

	// Chunk material
	m_chunkMaterialID = -1;
	m_pRenderer->CreateMaterial(Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f), 64, &m_chunkMaterialID);

	// Loader radius
	m_loaderRadius = 128.0f;

	// Update lock
	m_stepLockEnabled = false;
	m_updateStepLock = true;

	// Rendering modes
	m_wireframeRender = false;
	m_faceMerging = true;

	// Create initial chunk
	CreateNewChunk(0, 0, 0);

	// Threading
	m_updateThreadActive = true;
	m_pUpdatingChunksThread = new thread(_UpdatingChunksThread, this);
}

ChunkManager::~ChunkManager()
{
	m_updateThreadActive = false;
	Sleep(200);
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

// Loader radius
void ChunkManager::SetLoaderRadius(float radius)
{
	m_loaderRadius = radius;
}

float ChunkManager::GetLoaderRadius()
{
	return m_loaderRadius;
}

// Step update
void ChunkManager::SetStepLockEnabled(bool enabled)
{
	m_stepLockEnabled = enabled;
}

void ChunkManager::StepNextUpdate()
{
	m_updateStepLock = false;
}

// Chunk Creation
void ChunkManager::CreateNewChunk(int x, int y, int z)
{
	ChunkCoordKeys coordKeys;
	coordKeys.x = x;
	coordKeys.y = y;
	coordKeys.z = z;

	// Create a new chunk at this grid position
	Chunk* pNewChunk = new Chunk(m_pRenderer, this);

	float xPos = x * (Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f);
	float yPos = y * (Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f);
	float zPos = z * (Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f);

	pNewChunk->SetPosition(vec3(xPos, yPos, zPos));
	pNewChunk->SetGrid(coordKeys.x, coordKeys.y, coordKeys.z);

	pNewChunk->Setup();
	pNewChunk->SetNeedsRebuild(false, true);
	pNewChunk->RebuildMesh();
	pNewChunk->CompleteMesh();

	UpdateChunkNeighbours(pNewChunk, x, y, z);

	m_ChunkMapMutexLock.lock();
	m_chunksMap[coordKeys] = pNewChunk;
	m_ChunkMapMutexLock.unlock();
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
	m_ChunkMapMutexLock.lock();
	map<ChunkCoordKeys, Chunk*>::iterator it = m_chunksMap.find(coordKeys);
	if (it != m_chunksMap.end())
	{
		m_chunksMap.erase(coordKeys);
	}
	m_ChunkMapMutexLock.unlock();

	// Clear chunk linkage
	if (m_pPlayer != NULL)
	{
		m_pPlayer->ClearChunkCacheForChunk(pChunk);
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

	m_ChunkMapMutexLock.lock();
	map<ChunkCoordKeys, Chunk*>::iterator it = m_chunksMap.find(chunkKey);
	if (it != m_chunksMap.end())
	{
		Chunk* lpReturn = m_chunksMap[chunkKey];
		m_ChunkMapMutexLock.unlock();
		return lpReturn;
	}
	m_ChunkMapMutexLock.unlock();

	return NULL;
}

// Getting the active block state given a position and chunk information
bool ChunkManager::GetBlockActiveFrom3DPosition(float x, float y, float z, vec3 *blockPos, int* blockX, int* blockY, int* blockZ, Chunk** pChunk)
{
	if (*pChunk == NULL)
	{
		*pChunk = GetChunkFromPosition(x, y, z);

		if (*pChunk == NULL)
		{
			return false;
		}
	}

	(*blockX) = (int)((abs(x) + Chunk::BLOCK_RENDER_SIZE) / (Chunk::BLOCK_RENDER_SIZE*2.0f));
	(*blockY) = (int)((abs(y) + Chunk::BLOCK_RENDER_SIZE) / (Chunk::BLOCK_RENDER_SIZE*2.0f));
	(*blockZ) = (int)((abs(z) + Chunk::BLOCK_RENDER_SIZE) / (Chunk::BLOCK_RENDER_SIZE*2.0f));

	(*blockX) = (*blockX) % Chunk::CHUNK_SIZE;
	(*blockY) = (*blockY) % Chunk::CHUNK_SIZE;
	(*blockZ) = (*blockZ) % Chunk::CHUNK_SIZE;

	(*blockPos).x = (*pChunk)->GetPosition().x + (*blockX) * (Chunk::BLOCK_RENDER_SIZE*2.0f);
	(*blockPos).y = (*pChunk)->GetPosition().y + (*blockY) * (Chunk::BLOCK_RENDER_SIZE*2.0f);
	(*blockPos).z = (*pChunk)->GetPosition().z + (*blockZ) * (Chunk::BLOCK_RENDER_SIZE*2.0f);

	if (x < 0.0f)
	{
		if ((*blockX) == 0)
		{
			(*blockPos).x = (*pChunk)->GetPosition().x;
		}
		else
		{
			(*blockPos).x = (*pChunk)->GetPosition().x - ((*blockX) * (Chunk::BLOCK_RENDER_SIZE*2.0f)) + (Chunk::CHUNK_SIZE * (Chunk::BLOCK_RENDER_SIZE*2.0f));

			(*blockX) = (Chunk::CHUNK_SIZE) - (*blockX);
		}
	}
	if (y < 0.0f)
	{
		if ((*blockY) == 0)
		{
			(*blockPos).y = (*pChunk)->GetPosition().y;
		}
		else
		{
			(*blockPos).y = (*pChunk)->GetPosition().y - ((*blockY) * (Chunk::BLOCK_RENDER_SIZE*2.0f)) + (Chunk::CHUNK_SIZE * (Chunk::BLOCK_RENDER_SIZE*2.0f));

			(*blockY) = (Chunk::CHUNK_SIZE) - (*blockY);
		}
	}
	if (z < 0.0f)
	{
		if ((*blockZ) == 0)
		{
			(*blockPos).z = (*pChunk)->GetPosition().z;
		}
		else
		{
			(*blockPos).z = (*pChunk)->GetPosition().z - ((*blockZ) * (Chunk::BLOCK_RENDER_SIZE*2.0f)) + (Chunk::CHUNK_SIZE * (Chunk::BLOCK_RENDER_SIZE*2.0f));

			(*blockZ) = (Chunk::CHUNK_SIZE) - (*blockZ);
		}
	}

	return (*pChunk)->GetActive((*blockX), (*blockY), (*blockZ));
}

// Rendering modes
void ChunkManager::SetWireframeRender(bool wireframe)
{
	m_wireframeRender = wireframe;
}

void ChunkManager::SetFaceMerging(bool faceMerge)
{
	m_faceMerging = faceMerge;
}

bool ChunkManager::GetFaceMerging()
{
	return m_faceMerging;
}

// Updating
void ChunkManager::Update(float dt)
{
}

void ChunkManager::_UpdatingChunksThread(void* pData)
{
	ChunkManager* lpChunkManager = (ChunkManager*)pData;
	lpChunkManager->UpdatingChunksThread();
}

void ChunkManager::UpdatingChunksThread()
{
	while (m_updateThreadActive)
	{
		while (m_pPlayer == NULL)
		{
			Sleep(100);
		}

		while (m_stepLockEnabled == true && m_updateStepLock == true)
		{
			Sleep(100);
		}

		ChunkCoordKeysList addChunkList;
		ChunkList rebuildChunkList;
		ChunkList unloadChunkList;

		m_ChunkMapMutexLock.lock();
		typedef map<ChunkCoordKeys, Chunk*>::iterator it_type;
		int numAddedChunks = 0;
		int MAX_NUM_CHUNKS_ADD = 10;
		for (it_type iterator = m_chunksMap.begin(); iterator != m_chunksMap.end(); iterator++)
		{
			Chunk* pChunk = iterator->second;

			if (pChunk != NULL)
			{
				pChunk->Update(0.01f);

				int gridX = pChunk->GetGridX();
				int gridY = pChunk->GetGridY();
				int gridZ = pChunk->GetGridZ();

				float xPos = gridX * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;
				float yPos = gridY * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;
				float zPos = gridZ * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;

				vec3 chunkCenter = vec3(xPos, yPos, zPos) + vec3(Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE, Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE, Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE);
				vec3 distanceVec = chunkCenter - m_pPlayer->GetCenter();
				float lengthValue = length(distanceVec);

				if (lengthValue > m_loaderRadius)
				{
					unloadChunkList.push_back(pChunk);
				}
				else
				{
					if (numAddedChunks < MAX_NUM_CHUNKS_ADD)
					{
						// Check neighbours
						if (pChunk->GetNumNeighbours() < 6 && (pChunk->IsEmpty() == false) || (gridY == 0))
						{
							if (pChunk->GetxMinus() == NULL)
							{
								ChunkCoordKeys coordKey;
								coordKey.x = gridX - 1;
								coordKey.y = gridY;
								coordKey.z = gridZ;
								float xPos = coordKey.x * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;
								float yPos = coordKey.y * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;
								float zPos = coordKey.z * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;

								vec3 chunkCenter = vec3(xPos, yPos, zPos) + vec3(Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE, Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE, Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE);
								vec3 distanceVec = chunkCenter - m_pPlayer->GetCenter();
								float lengthValue = length(distanceVec);

								if (lengthValue <= m_loaderRadius)
								{
									addChunkList.push_back(coordKey);
									numAddedChunks++;
								}
							}
							if (pChunk->GetxPlus() == NULL)
							{
								ChunkCoordKeys coordKey;
								coordKey.x = gridX + 1;
								coordKey.y = gridY;
								coordKey.z = gridZ;
								float xPos = coordKey.x * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;
								float yPos = coordKey.y * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;
								float zPos = coordKey.z * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;

								vec3 chunkCenter = vec3(xPos, yPos, zPos) + vec3(Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE, Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE, Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE);
								vec3 distanceVec = chunkCenter - m_pPlayer->GetCenter();
								float lengthValue = length(distanceVec);

								if (lengthValue <= m_loaderRadius)
								{
									addChunkList.push_back(coordKey);
									numAddedChunks++;
								}
							}
							if (pChunk->GetyMinus() == NULL)
							{
								ChunkCoordKeys coordKey;
								coordKey.x = gridX;
								coordKey.y = gridY - 1;
								coordKey.z = gridZ;
								float xPos = coordKey.x * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;
								float yPos = coordKey.y * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;
								float zPos = coordKey.z * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;

								vec3 chunkCenter = vec3(xPos, yPos, zPos) + vec3(Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE, Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE, Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE);
								vec3 distanceVec = chunkCenter - m_pPlayer->GetCenter();
								float lengthValue = length(distanceVec);

								if (lengthValue <= m_loaderRadius)
								{
									addChunkList.push_back(coordKey);
									numAddedChunks++;
								}
							}
							if (pChunk->GetyPlus() == NULL)
							{
								ChunkCoordKeys coordKey;
								coordKey.x = gridX;
								coordKey.y = gridY + 1;
								coordKey.z = gridZ;
								float xPos = coordKey.x * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;
								float yPos = coordKey.y * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;
								float zPos = coordKey.z * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;

								vec3 chunkCenter = vec3(xPos, yPos, zPos) + vec3(Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE, Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE, Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE);
								vec3 distanceVec = chunkCenter - m_pPlayer->GetCenter();
								float lengthValue = length(distanceVec);

								if (lengthValue <= m_loaderRadius)
								{
									addChunkList.push_back(coordKey);
									numAddedChunks++;
								}
							}
							if (pChunk->GetzMinus() == NULL)
							{
								ChunkCoordKeys coordKey;
								coordKey.x = gridX;
								coordKey.y = gridY;
								coordKey.z = gridZ - 1;
								float xPos = coordKey.x * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;
								float yPos = coordKey.y * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;
								float zPos = coordKey.z * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;

								vec3 chunkCenter = vec3(xPos, yPos, zPos) + vec3(Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE, Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE, Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE);
								vec3 distanceVec = chunkCenter - m_pPlayer->GetCenter();
								float lengthValue = length(distanceVec);

								if (lengthValue <= m_loaderRadius)
								{
									addChunkList.push_back(coordKey);
									numAddedChunks++;
								}
							}
							if (pChunk->GetzPlus() == NULL)
							{
								ChunkCoordKeys coordKey;
								coordKey.x = gridX;
								coordKey.y = gridY;
								coordKey.z = gridZ + 1;
								float xPos = coordKey.x * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;
								float yPos = coordKey.y * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;
								float zPos = coordKey.z * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f;

								vec3 chunkCenter = vec3(xPos, yPos, zPos) + vec3(Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE, Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE, Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE);
								vec3 distanceVec = chunkCenter - m_pPlayer->GetCenter();
								float lengthValue = length(distanceVec);

								if (lengthValue <= m_loaderRadius)
								{
									addChunkList.push_back(coordKey);
									numAddedChunks++;
								}
							}
						}
					}
				}
			}
		}
		m_ChunkMapMutexLock.unlock();

		// Adding chunks
		for (unsigned int i = 0; i < (int)addChunkList.size(); i++)
		{
			ChunkCoordKeys coordKey = addChunkList[i];
			Chunk* pChunk = GetChunk(coordKey.x, coordKey.y, coordKey.z);

			if (pChunk == NULL)
			{
				CreateNewChunk(coordKey.x, coordKey.y, coordKey.z);
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

		// Check for rebuild chunks
		m_ChunkMapMutexLock.lock();
		for (it_type iterator = m_chunksMap.begin(); iterator != m_chunksMap.end(); iterator++)
		{
			Chunk* pChunk = iterator->second;

			if (pChunk != NULL)
			{
				if (pChunk->NeedsRebuild())
				{
					rebuildChunkList.push_back(pChunk);
				}
			}
		}
		m_ChunkMapMutexLock.unlock();

		// Rebuilding chunks
		int numRebuildChunks = 0;
		int MAX_NUM_CHUNKS_REBUILD = 30;
		for (unsigned int i = 0; i < (int)rebuildChunkList.size() && numRebuildChunks < MAX_NUM_CHUNKS_REBUILD; i++)
		{
			Chunk* pChunk = rebuildChunkList[i];

			pChunk->SwitchToCachedMesh();
			pChunk->RebuildMesh();
			pChunk->CompleteMesh();
			pChunk->UndoCachedMesh();

			numRebuildChunks++;
		}
		rebuildChunkList.clear();

		if (m_stepLockEnabled == true && m_updateStepLock == false)
		{
			m_updateStepLock = true;
		}

		Sleep(10);
	}
}

// Rendering
void ChunkManager::Render()
{
	m_pRenderer->StartMeshRender();

	// Store cull mode
	CullMode cullMode = m_pRenderer->GetCullMode();

	if (m_wireframeRender)
	{
		m_pRenderer->SetLineWidth(1.0f);
		m_pRenderer->SetRenderMode(RM_WIREFRAME);
	}
	else
	{
		m_pRenderer->SetRenderMode(RM_SOLID);
	}

	m_pRenderer->PushMatrix();
		m_ChunkMapMutexLock.lock();
		typedef map<ChunkCoordKeys, Chunk*>::iterator it_type;
		for (it_type iterator = m_chunksMap.begin(); iterator != m_chunksMap.end(); iterator++)
		{
			Chunk* pChunk = iterator->second;

			if (pChunk != NULL)
			{
				pChunk->Render();
			}
		}
		m_ChunkMapMutexLock.unlock();
	m_pRenderer->PopMatrix();

	// Restore cull mode
	m_pRenderer->SetCullMode(cullMode);

	m_pRenderer->EndMeshRender();
}

void ChunkManager::RenderDebug()
{
	m_ChunkMapMutexLock.lock();
	typedef map<ChunkCoordKeys, Chunk*>::iterator it_type;
	for (it_type iterator = m_chunksMap.begin(); iterator != m_chunksMap.end(); iterator++)
	{
		Chunk* pChunk = iterator->second;

		if (pChunk != NULL)
		{
			pChunk->RenderDebug();
		}
	}
	m_ChunkMapMutexLock.unlock();
}

void ChunkManager::Render2D(Camera* pCamera, unsigned int viewport, unsigned int font)
{
	m_ChunkMapMutexLock.lock();
	typedef map<ChunkCoordKeys, Chunk*>::iterator it_type;
	for (it_type iterator = m_chunksMap.begin(); iterator != m_chunksMap.end(); iterator++)
	{
		Chunk* pChunk = iterator->second;

		if (pChunk != NULL)
		{
			pChunk->Render2D(pCamera, viewport, font);
		}
	}
	m_ChunkMapMutexLock.unlock();
}
