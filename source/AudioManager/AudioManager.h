// ******************************************************************************
// Filename:    AudioManager.h
// Project:     Vox
// Author:      Steven Ball
//
// Purpose:
//	 A singleton instance used as an entry point to play audio.
//   (Music and sound effects)
//
// Revision History:
//   Initial Revision - 04/05/12
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#pragma once

#include "../Maths/3dmaths.h"

#include <stdio.h>

class AudioManager
{
public:
	/* Public methods */
	static AudioManager* GetInstance();
	void Destroy();

    void Setup();
    void Shutdown();

	void Update(float dt);

    void SetEnableAudio(bool enable);

	void StopAllSounds();

protected:
	/* Protected methods */
	AudioManager();
	AudioManager(const AudioManager&);
	AudioManager &operator=(const AudioManager&);

private:
	/* Private methods */

public:
	/* Public members */

protected:
	/* Protected members */

private:
	/* Private members */

    bool m_audioEnabled;

	// Singleton instance
	static AudioManager *c_instance;
};
