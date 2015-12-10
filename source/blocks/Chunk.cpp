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

#include "Chunk.h"

const float Chunk::BLOCK_RENDER_SIZE = 0.5f;
const float Chunk::CHUNK_RADIUS = sqrt(((CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f)*(CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE*2.0f))*2.0f) / 2.0f + ((Chunk::BLOCK_RENDER_SIZE*2.0f)*2.0f);


Chunk::Chunk(Renderer* pRenderer)
{
	m_pRenderer = pRenderer;

	Initialize();
}

Chunk::~Chunk()
{
	if (m_pMesh != NULL)
	{
		m_pRenderer->ClearMesh(m_pMesh);
		m_pMesh = NULL;
	}

	delete m_colour;
}

// Initialize
void Chunk::Initialize()
{
	// Neighbours
	m_numNeighbours = 0;
	m_pxMinus = NULL;
	m_pxPlus = NULL;
	m_pyMinus = NULL;
	m_pyPlus = NULL;
	m_pzMinus = NULL;
	m_pzPlus = NULL;

	// Grid
	m_gridX = 0;
	m_gridY = 0;
	m_gridZ = 0;

	// Flags
	m_emptyChunk = false;
	m_surroundedChunk = false;

	// Mesh
	m_pMesh = NULL;

	// Blocks data
	m_colour = new unsigned int[CHUNK_SIZE_CUBED];
	for (int i = 0; i < CHUNK_SIZE_CUBED; i++)
	{
		m_colour[i] = 0;
	}
}

// Creation and destruction
void Chunk::Unload()
{

}

void Chunk::Setup()
{

}

// Saving and loading
void Chunk::SaveChunk()
{

}

void Chunk::LoadChunk()
{

}

// Position
void Chunk::SetPosition(vec3 pos)
{
	m_position = pos;
}

vec3 Chunk::GetPosition()
{
	return m_position;
}

// Neighbours
int Chunk::GetNumNeighbours()
{
	return m_numNeighbours;
}

void Chunk::SetNumNeighbours(int neighbours)
{
	m_numNeighbours = neighbours;
}

Chunk* Chunk::GetxMinus()
{
	return m_pxMinus;
}

Chunk* Chunk::GetxPlus()
{
	return m_pxPlus;
}

Chunk* Chunk::GetyMinus()
{
	return m_pyMinus;
}

Chunk* Chunk::GetyPlus()
{
	return m_pyPlus;
}

Chunk* Chunk::GetzMinus()
{
	return m_pzMinus;
}

Chunk* Chunk::GetzPlus()
{
	return m_pzPlus;
}

void Chunk::SetxMinus(Chunk* pChunk)
{
	m_pxMinus = pChunk;
}

void Chunk::SetxPlus(Chunk* pChunk)
{
	m_pxPlus = pChunk;
}

void Chunk::SetyMinus(Chunk* pChunk)
{
	m_pyMinus = pChunk;
}

void Chunk::SetyPlus(Chunk* pChunk)
{
	m_pyPlus = pChunk;
}

void Chunk::SetzMinus(Chunk* pChunk)
{
	m_pzMinus = pChunk;
}

void Chunk::SetzPlus(Chunk* pChunk)
{
	m_pzPlus = pChunk;
}

// Grid
void Chunk::SetGrid(int x, int y, int z)
{
	m_gridX = x;
	m_gridY = y;
	m_gridZ = z;
}

int Chunk::GetGridX()
{
	return m_gridX;
}

int Chunk::GetGridY()
{
	return m_gridY;
}

int Chunk::GetGridZ()
{
	return m_gridZ;
}

// Active
bool Chunk::GetActive(int x, int y, int z)
{
	unsigned colour = m_colour[x + y * CHUNK_SIZE + z * CHUNK_SIZE_SQUARED];
	unsigned int alpha = (colour & 0xFF000000) >> 24;
	unsigned int blue = (colour & 0x00FF0000) >> 16;
	unsigned int green = (colour & 0x0000FF00) >> 8;
	unsigned int red = (colour & 0x000000FF);

	if (alpha == 0)
	{
		return false;
	}

	return true;
}

// Block colour
void Chunk::SetColour(int x, int y, int z, float r, float g, float b, float a)
{
	if (r > 1.0f) r = 1.0f;
	if (g > 1.0f) g = 1.0f;
	if (b > 1.0f) b = 1.0f;
	if (r < 0.0f) r = 0.0f;
	if (g < 0.0f) g = 0.0f;
	if (b < 0.0f) b = 0.0f;

	unsigned int alpha = (int)(a * 255) << 24;
	unsigned int blue = (int)(b * 255) << 16;
	unsigned int green = (int)(g * 255) << 8;
	unsigned int red = (int)(r * 255);

	unsigned int colour = red + green + blue + alpha;
	SetColour(x, y, z, colour);
}

void Chunk::GetColour(int x, int y, int z, float* r, float* g, float* b, float* a)
{
	if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE)
		return;

	unsigned int colour = m_colour[x + y * CHUNK_SIZE + z * CHUNK_SIZE_SQUARED];
	unsigned int alpha = (colour & 0xFF000000) >> 24;
	unsigned int blue = (colour & 0x00FF0000) >> 16;
	unsigned int green = (colour & 0x0000FF00) >> 8;
	unsigned int red = (colour & 0x000000FF);

	*r = (float)(red / 255.0f);
	*g = (float)(green / 255.0f);
	*b = (float)(blue / 255.0f);
	*a = 1.0f;
}

void Chunk::SetColour(int x, int y, int z, unsigned int colour)
{
	if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE)
		return;

	bool changed = ((m_colour[x + y * CHUNK_SIZE + z * CHUNK_SIZE_SQUARED] == colour) == false);

	m_colour[x + y * CHUNK_SIZE + z * CHUNK_SIZE_SQUARED] = colour;
}

unsigned int Chunk::GetColour(int x, int y, int z)
{
	return m_colour[x + y * CHUNK_SIZE + z * CHUNK_SIZE_SQUARED];
}

// Flags
bool Chunk::IsEmpty()
{
	return m_emptyChunk;
}

bool Chunk::IsSurrounded()
{
	return m_surroundedChunk;
}

// Create mesh
void Chunk::CreateMesh()
{

}

// Rebuild
void Chunk::RebuildMesh()
{

}

// Updating
void Chunk::Update(float dt)
{

}

// Rendering
void Chunk::Render()
{

}

void Chunk::RenderDebug()
{
	float l_length = (Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE) - 0.05f;
	float l_height = (Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE) - 0.05f;
	float l_width = (Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE) - 0.05f;

	m_pRenderer->SetRenderMode(RM_WIREFRAME);
	m_pRenderer->SetCullMode(CM_NOCULL);
	m_pRenderer->SetLineWidth(1.0f);

	m_pRenderer->PushMatrix();
		m_pRenderer->TranslateWorldMatrix(m_position.x, m_position.y, m_position.z);

		m_pRenderer->ImmediateColourAlpha(1.0f, 1.0f, 0.0f, 1.0f);
		if (IsEmpty())
		{
			m_pRenderer->ImmediateColourAlpha(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else if (IsSurrounded())
		{
			m_pRenderer->ImmediateColourAlpha(0.0f, 1.0f, 1.0f, 1.0f);
		}

		m_pRenderer->EnableImmediateMode(IM_QUADS);
			m_pRenderer->ImmediateNormal(0.0f, 0.0f, -1.0f);
			m_pRenderer->ImmediateVertex(l_length, -l_height, -l_width);
			m_pRenderer->ImmediateVertex(-l_length, -l_height, -l_width);
			m_pRenderer->ImmediateVertex(-l_length, l_height, -l_width);
			m_pRenderer->ImmediateVertex(l_length, l_height, -l_width);

			m_pRenderer->ImmediateNormal(0.0f, 0.0f, 1.0f);
			m_pRenderer->ImmediateVertex(-l_length, -l_height, l_width);
			m_pRenderer->ImmediateVertex(l_length, -l_height, l_width);
			m_pRenderer->ImmediateVertex(l_length, l_height, l_width);
			m_pRenderer->ImmediateVertex(-l_length, l_height, l_width);

			m_pRenderer->ImmediateNormal(1.0f, 0.0f, 0.0f);
			m_pRenderer->ImmediateVertex(l_length, -l_height, l_width);
			m_pRenderer->ImmediateVertex(l_length, -l_height, -l_width);
			m_pRenderer->ImmediateVertex(l_length, l_height, -l_width);
			m_pRenderer->ImmediateVertex(l_length, l_height, l_width);

			m_pRenderer->ImmediateNormal(-1.0f, 0.0f, 0.0f);
			m_pRenderer->ImmediateVertex(-l_length, -l_height, -l_width);
			m_pRenderer->ImmediateVertex(-l_length, -l_height, l_width);
			m_pRenderer->ImmediateVertex(-l_length, l_height, l_width);
			m_pRenderer->ImmediateVertex(-l_length, l_height, -l_width);

			m_pRenderer->ImmediateNormal(0.0f, -1.0f, 0.0f);
			m_pRenderer->ImmediateVertex(-l_length, -l_height, -l_width);
			m_pRenderer->ImmediateVertex(l_length, -l_height, -l_width);
			m_pRenderer->ImmediateVertex(l_length, -l_height, l_width);
			m_pRenderer->ImmediateVertex(-l_length, -l_height, l_width);

			m_pRenderer->ImmediateNormal(0.0f, 1.0f, 0.0f);
			m_pRenderer->ImmediateVertex(l_length, l_height, -l_width);
			m_pRenderer->ImmediateVertex(-l_length, l_height, -l_width);
			m_pRenderer->ImmediateVertex(-l_length, l_height, l_width);
			m_pRenderer->ImmediateVertex(l_length, l_height, l_width);
		m_pRenderer->DisableImmediateMode();
	m_pRenderer->PopMatrix();

	m_pRenderer->SetCullMode(CM_BACK);
}