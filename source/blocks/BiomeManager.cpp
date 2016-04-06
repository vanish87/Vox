// ******************************************************************************
// Filename:    BiomeManager.cpp
// Project:     Vox
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 06/04/15
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "BiomeManager.h"
#include "ChunkManager.h"


BiomeManager::BiomeManager(Renderer* pRenderer)
{
	m_pRenderer = pRenderer;

	//                    Biome        Boundary       r1     g1     b1     r2     g2     b2        BlockType
	AddBiomeBoundary(Biome_GrassLand,  -0.50f,		0.50f, 0.50f, 0.50f, 0.35f, 0.40f, 0.37f,	BlockType_Stone);
	AddBiomeBoundary(Biome_GrassLand,	0.00f,		0.94f, 0.74f, 0.34f, 0.50f, 0.29f, 0.20f,	BlockType_Dirt); // Shore
	AddBiomeBoundary(Biome_GrassLand,	0.50f,		0.00f, 0.56f, 0.26f, 0.75f, 0.90f, 0.00f,	BlockType_Grass);
	AddBiomeBoundary(Biome_GrassLand,	1.00f,		0.85f, 0.85f, 0.85f, 0.77f, 0.65f, 0.80f,	BlockType_Snow);

	AddBiomeBoundary(Biome_Desert,	    1.00f,		0.94f, 0.90f, 0.12f, 0.98f, 1.0f, 0.60f,	BlockType_Sand);

	AddBiomeBoundary(Biome_Jungle,	    1.00f,		0.14f, 0.35f, 0.12f, 0.34f, 0.65f, 0.42f,	BlockType_Leaf);

	AddBiomeBoundary(Biome_Tundra,		-0.25f,		0.05f, 0.05f, 0.78f, 0.05f, 0.05f, 0.78f,	BlockType_Stone);
	AddBiomeBoundary(Biome_Tundra,		-0.00f,		0.37f, 0.61f, 0.85f, 0.30f, 0.56f, 0.95f,	BlockType_Stone);
	AddBiomeBoundary(Biome_Tundra,		0.0625f,	0.75f, 0.83f, 0.60f, 0.78f, 0.88f, 0.65f,	BlockType_Grass); // Shore
	AddBiomeBoundary(Biome_Tundra,	    0.40f,		1.00f, 1.00f, 1.00f, 1.00f, 1.00f, 1.00f,	BlockType_Snow);
	AddBiomeBoundary(Biome_Tundra,	    0.65f,		0.76f, 0.87f, 1.00f, 1.00f, 1.00f, 1.00f,	BlockType_Snow);
	AddBiomeBoundary(Biome_Tundra,	    0.80f,		0.68f, 0.80f, 1.00f, 0.76f, 0.87f, 1.00f,	BlockType_Snow);
	AddBiomeBoundary(Biome_Tundra,	    0.90f,		0.60f, 0.73f, 1.00f, 0.68f, 0.80f, 1.00f,	BlockType_Snow);
	AddBiomeBoundary(Biome_Tundra,	    1.00f,		0.51f, 0.63f, 1.00f, 0.60f, 0.73f, 1.00f,	BlockType_Snow);

	AddBiomeBoundary(Biome_Swamp,	    1.00f,		0.00f, 0.75f, 1.00f, 0.00f, 1.00f, 0.75f,	BlockType_Default);

	AddBiomeBoundary(Biome_AshLand,		-0.35f,		0.18f, 0.12f, 0.12f, 0.23f, 0.17f, 0.18f,	BlockType_Stone);
	AddBiomeBoundary(Biome_AshLand,	    0.40f,		0.24f, 0.25f, 0.26f, 0.24f, 0.25f, 0.26f,	BlockType_Stone);
	AddBiomeBoundary(Biome_AshLand,		1.00f,		0.25f, 0.25f, 0.25f, 0.17f, 0.15f, 0.10f,	BlockType_Stone);

	AddBiomeBoundary(Biome_Nightmare,	0.00f,		0.38f, 0.36f, 0.39f, 0.33f, 0.33f, 0.5f,	BlockType_Dirt);
	AddBiomeBoundary(Biome_Nightmare,	1.00f,		0.50f, 0.47f, 0.62f, 0.38f, 0.32f, 0.62f,	BlockType_Stone);
}

BiomeManager::~BiomeManager()
{
	ClearBoundaryData();
}

void BiomeManager::ClearBoundaryData()
{
	for(int i = 0; i < BiomeType_NumBiomes; i++)
	{
		for(unsigned int j = 0; j < m_vpBiomeHeightBoundaryList[i].size(); j++)
		{
			delete m_vpBiomeHeightBoundaryList[i][j];
			m_vpBiomeHeightBoundaryList[i][j] = 0;
		}
		m_vpBiomeHeightBoundaryList[i].clear();
	}
}

void BiomeManager::AddBiomeBoundary(Biome biome, float heightUpperBoundary, float red1, float green1, float blue1, float red2, float green2, float blue2, BlockType blockType)
{
	BiomeHeightBoundary* pNewBiomeHeightBoundary = new BiomeHeightBoundary();

	pNewBiomeHeightBoundary->m_heightUpperBoundary = heightUpperBoundary;
	pNewBiomeHeightBoundary->m_red1 = red1;
	pNewBiomeHeightBoundary->m_green1 = green1;
	pNewBiomeHeightBoundary->m_blue1 = blue1;
	pNewBiomeHeightBoundary->m_red2 = red2;
	pNewBiomeHeightBoundary->m_green2 = green2;
	pNewBiomeHeightBoundary->m_blue2 = blue2;
	pNewBiomeHeightBoundary->m_blockType = blockType;

	m_vpBiomeHeightBoundaryList[biome].push_back(pNewBiomeHeightBoundary);
}

Biome BiomeManager::GetBiome(vec3 position)
{
	// TODO : Better biome generation
	return Biome_GrassLand;
}

// Town
bool BiomeManager::IsInTown(vec3 position)
{
	return false;
}

// Safe zone
bool BiomeManager::IsInSafeZone(vec3 position)
{
	return false;
}

void BiomeManager::GetChunkColourAndBlockType(float xPos, float yPos, float zPos, float noiseValue, float landscapeGradient, float *r, float *g, float *b, BlockType *blockType)
{
	Biome biome = GetBiome(vec3(xPos, yPos, zPos));

	float red1 = 0.0f;
	float green1 = 0.0f;
	float blue1 = 0.0f;
	float red2 = 1.0f;
	float green2 = 1.0f;
	float blue2 = 1.0f;

	bool foundBoundary = false;
	int boundaryCount = (int)m_vpBiomeHeightBoundaryList[biome].size();
	for(int i = 0; i < boundaryCount && foundBoundary == false; i++)
	{
		if (i == 3)
		{
			int hello = 1;
		}
		if(noiseValue <= m_vpBiomeHeightBoundaryList[biome][i]->m_heightUpperBoundary)
		{
			red1 = m_vpBiomeHeightBoundaryList[biome][i]->m_red1;
			green1 = m_vpBiomeHeightBoundaryList[biome][i]->m_green1;
			blue1 = m_vpBiomeHeightBoundaryList[biome][i]->m_blue1;

			red2 = m_vpBiomeHeightBoundaryList[biome][i]->m_red2;
			green2 = m_vpBiomeHeightBoundaryList[biome][i]->m_green2;
			blue2 = m_vpBiomeHeightBoundaryList[biome][i]->m_blue2;

			*blockType = m_vpBiomeHeightBoundaryList[biome][i]->m_blockType;

			foundBoundary = true;
		}
	}

	if(foundBoundary == false)
	{
		red1 = m_vpBiomeHeightBoundaryList[biome][boundaryCount-1]->m_red1;
		green1 = m_vpBiomeHeightBoundaryList[biome][boundaryCount-1]->m_green1;
		blue1 = m_vpBiomeHeightBoundaryList[biome][boundaryCount-1]->m_blue1;

		red2 = m_vpBiomeHeightBoundaryList[biome][boundaryCount-1]->m_red2;
		green2 = m_vpBiomeHeightBoundaryList[biome][boundaryCount-1]->m_green2;
		blue2 = m_vpBiomeHeightBoundaryList[biome][boundaryCount-1]->m_blue2;

		*blockType = m_vpBiomeHeightBoundaryList[biome][boundaryCount-1]->m_blockType;
	}

	*r = red1 + ((red2 - red1) * landscapeGradient);
	*g = green1 + ((green2 - green1) * landscapeGradient);
	*b = blue1 + ((blue2 - blue1) * landscapeGradient);
}