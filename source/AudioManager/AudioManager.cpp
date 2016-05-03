// ******************************************************************************
// Filename:    AudioManager.cpp
// Project:     Vox
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 04/05/12
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "AudioManager.h"

// Initialize the singleton instance
AudioManager *AudioManager::c_instance = 0;

AudioManager* AudioManager::GetInstance()
{
	if(c_instance == 0)
	{
		c_instance = new AudioManager;

		c_instance->Setup();
	}

	return c_instance;
}

void AudioManager::Destroy()
{
	if(c_instance)
	{
        Shutdown();

		delete c_instance;
	}
}

void AudioManager::Setup()
{
}

void AudioManager::Shutdown()
{
}

AudioManager::AudioManager()
{
    SetEnableAudio(true);
}

void AudioManager::Update(float dt)
{
}
	
void AudioManager::SetEnableAudio(bool enable)
{
    m_audioEnabled = enable;
}

void AudioManager::StopAllSounds()
{
	if(m_audioEnabled)
	{
	}
}