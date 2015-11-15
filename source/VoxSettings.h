// ******************************************************************************
// Filename:	VoxApplication.h
// Project:		Vox
// Author:		Steven Ball
//
// Purpose:
//   Vox application class.
// 
// Revision History:
//   Initial Revision - 27/10/15
//
// Copyright (c) 2005-2015, Steven Ball
// ******************************************************************************

#pragma once

class VoxGame;


class VoxSettings
{
public:
	/* Public methods */
	VoxSettings();
	~VoxSettings();

	// Load settings
	void LoadSettings();

protected:
	/* Protected methods */

private:
	/* Private methods */

public:
	/* Public members */

	// Graphics
	int m_windowWidth;
	int m_windowHeight;
	bool m_deferredRendering;
	bool m_shadows;
	bool m_blur;
	bool m_ssao;
	bool m_dynamicLighting;
	bool m_msaa;
	bool m_instancedParticles;
	bool m_wireframeRendering;
	bool m_debugRendering;

protected:
	/* Protected members */

private:
	/* Private members */
};