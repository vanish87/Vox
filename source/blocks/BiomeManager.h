// ******************************************************************************
// Filename:    BiomeManager.h
// Project:     Vox
// Author:      Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 06/04/15
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#pragma once

#include "../Renderer/Renderer.h"
#include "BlocksEnum.h"


enum Biome
{
	Biome_None = 0,

	Biome_GrassLand,
	Biome_Desert,
	Biome_Jungle,
	Biome_Tundra,
	Biome_Swamp,
	Biome_AshLand,
	Biome_Nightmare,

	BiomeType_NumBiomes,
};

class BiomeHeightBoundary
{
public:
	float m_heightUpperBoundary;
	float m_red1;
	float m_green1;
	float m_blue1;
	float m_red2;
	float m_green2;
	float m_blue2;
	BlockType m_blockType;
};

typedef std::vector<BiomeHeightBoundary*> BiomeHeightBoundaryList;

class BiomeManager
{
public:
	/* Public methods */
	BiomeManager(Renderer* pRenderer);
	~BiomeManager();

	void ClearBoundaryData();

	void AddBiomeBoundary(Biome biome, float heightUpperBoundary, float red1, float green1, float blue1, float red2, float green2, float blue2, BlockType blockType);

	Biome GetBiome(vec3 position);

	bool IsInTown(vec3 position);

	void GetChunkColourAndBlockType(float xPos, float yPos, float zPos, float noiseValue, float landscapeGradient, float *r, float *g, float *b, BlockType *blockType);

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

	BiomeHeightBoundaryList m_vpBiomeHeightBoundaryList[BiomeType_NumBiomes];
};
