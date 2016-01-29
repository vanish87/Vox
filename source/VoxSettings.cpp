// ******************************************************************************
// Filename:	VoxSettings.cpp
// Project:		Vox
// Author:		Steven Ball
//
// Revision History:
//   Initial Revision - 15/11/15
//
// Copyright (c) 2005-2015, Steven Ball
// ******************************************************************************

#include "VoxSettings.h"

#include <iostream>
#include "ini/INIReader.h"

#include <fstream>
#include <ostream>
#include <iostream>
#include <string>
using namespace std;


VoxSettings::VoxSettings()
{
}

VoxSettings::~VoxSettings()
{
}

// Load settings
void VoxSettings::LoadSettings()
{
	string settingsIniFile = "media/config/settings.ini";
	INIReader reader(settingsIniFile);

	if (reader.ParseError() < 0)
	{
		cout << "Can't load '" << settingsIniFile << "'\n";
		return;
	}

	// Graphics
	m_windowWidth = reader.GetInteger("Graphics", "WindowWidth", 800);
	m_windowHeight = reader.GetInteger("Graphics", "WindowHeight", 800);
	m_vsync = reader.GetBoolean("Graphics", "VSync", false);
	m_fullscreen = reader.GetBoolean("Graphics", "FullScreen", false);
	m_deferredRendering = reader.GetBoolean("Graphics", "DeferredRendering", false);
	m_shadows = reader.GetBoolean("Graphics", "Shadows", false);
	m_blur = reader.GetBoolean("Graphics", "Blur", false);
	m_ssao = reader.GetBoolean("Graphics", "SSAO", false);
	m_dynamicLighting = reader.GetBoolean("Graphics", "DynamicLighting", false);
	m_msaa = reader.GetBoolean("Graphics", "MSAA", false);
	m_instancedParticles = reader.GetBoolean("Graphics", "InstancedParticles", false);
	m_faceMerging = reader.GetBoolean("Graphics", "FaceMerging", false);

	// Landscape generation
	m_landscapeNoiseOctaves = (float)reader.GetReal("Landscape", "LandscapeNoiseOctaves", 4.0f);
	m_landscapeNoisePersistence = (float)reader.GetReal("Landscape", "LandscapeNoisePersistance", 0.3f);
	m_landscapeNoiseScale = (float)reader.GetReal("Landscape", "LandscapeNoiseScale", 0.01f);
	m_mountainNoiseOctaves = (float)reader.GetReal("Landscape", "MountainNoiseOctaves", 4.0f);
	m_mountainNoisePersistence = (float)reader.GetReal("Landscape", "MountainNoisePersistance", 0.3f);
	m_mountainNoiseScale = (float)reader.GetReal("Landscape", "MountainNoiseScale", 0.0075f);
	m_mountainMultiplier = (float)reader.GetReal("Landscape", "MountainNoiseMultiplier", 3.0f);

	// Debug
	m_debugRendering = reader.GetBoolean("Debug", "DebugRendering", false);
	m_stepUpdating = reader.GetBoolean("Debug", "StepUpdatng", false);
	m_wireframeRendering = reader.GetBoolean("Debug", "WireframeRendering", false);
}

// Save settings
void VoxSettings::SaveSettings()
{
}