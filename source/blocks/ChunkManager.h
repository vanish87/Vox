// ******************************************************************************
// Filename:	ChunkManager.h
// Project:	Game
// Author:	Steven Ball
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

#include <map>
using namespace std;

#include "../tinythread/tinythread.h"
using namespace tthread;

class Player;
class VoxSettings;

struct ChunkCoordKeys {
	int x;
	int y;
	int z;
};

inline bool const operator==(const ChunkCoordKeys& l, const ChunkCoordKeys& r) {
	return l.x == r.x && l.y == r.y && l.z == r.z;
};

inline bool const operator<(const ChunkCoordKeys& l, const ChunkCoordKeys& r) {
	if (l.x < r.x)  return true;
	if (l.x > r.x)  return false;

	if (l.y < r.y)  return true;
	if (l.y > r.y)  return false;

	if (l.z < r.z)  return true;
	if (l.z > r.z)  return false;

	return false;
};

typedef std::vector<Chunk*> ChunkList;
typedef std::vector<ChunkCoordKeys> ChunkCoordKeysList;


class ChunkManager
{
public:
	/* Public methods */
	ChunkManager(Renderer* pRenderer, VoxSettings* pVoxSettings);
	~ChunkManager();

	// Player pointer
	void SetPlayer(Player* pPlayer);

	// Initial chunk creation
	void InitializeChunkCreation();

	// Chunk rendering material
	unsigned int GetChunkMaterialID();

	// Loader radius
	void SetLoaderRadius(float radius);
	float GetLoaderRadius();

	// Step update
	void SetStepLockEnabled(bool enabled);
	void StepNextUpdate();

	// Chunk Creation
	void CreateNewChunk(int x, int y, int z);
	void UnloadChunk(Chunk* pChunk);
	void UpdateChunkNeighbours(Chunk* pChunk, int x, int y, int z);

	// Getting chunk and positional information
	void GetGridFromPosition(vec3 position, int* gridX, int* gridY, int* gridZ);
	Chunk* GetChunkFromPosition(float posX, float posY, float posZ);
	Chunk* GetChunk(int aX, int aY, int aZ);

	// Getting the active block state given a position and chunk information
	bool GetBlockActiveFrom3DPosition(float x, float y, float z, vec3 *blockPos, int* blockX, int* blockY, int* blockZ, Chunk** pChunk);

	// Rendering modes
	void SetWireframeRender(bool wireframe);
	void SetFaceMerging(bool faceMerge);
	bool GetFaceMerging();

	// Updating
	void Update(float dt);
	static void _UpdatingChunksThread(void* pData);
	void UpdatingChunksThread();

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

protected:
	/* Protected members */

private:
	/* Private members */
	Renderer* m_pRenderer;
	Player* m_pPlayer;
	VoxSettings* m_pVoxSettings;

	// Chunk Material
	unsigned int m_chunkMaterialID;

	// Loader radius
	float m_loaderRadius;

	// Update step lock
	bool m_stepLockEnabled;
	bool m_updateStepLock;

	// Render modes
	bool m_wireframeRender;
	bool m_faceMerging;

	// Chunks storage
	map<ChunkCoordKeys, Chunk*> m_chunksMap;

	// Threading
	thread* m_pUpdatingChunksThread;
	mutex m_ChunkMapMutexLock;
	bool m_updateThreadActive;
	bool m_updateThreadFinished;
};
