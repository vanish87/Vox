// ******************************************************************************
// Filename:    Skybox.h
// Project:     Game
// Author:      Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 22/09/14
//
// Copyright (c) 2005-2011, Steven Ball
// ******************************************************************************

#pragma once

#include "../Renderer/Renderer.h"


class Skybox
{
public:
	/* Public methods */
	Skybox(Renderer* pRenderer);
	~Skybox();

	void SetSkybox1(string name);
	void SetSkybox2(string name);

	unsigned int GetCubeMapTexture1();
	unsigned int GetCubeMapTexture2();

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

	unsigned int m_cubeTextureId1;
	unsigned int m_cubeTextureId2;

	string m_skyBox1Name;
	string m_skyBox2Name;
};
