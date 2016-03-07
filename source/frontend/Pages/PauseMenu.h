// ******************************************************************************
// Filename:    PauseMenu.h
// Project:     Game
// Author:      Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 12/10/14
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#pragma once

#include "../FrontendPage.h"
#include "../../gui/button.h"


class PauseMenu : public FrontendPage
{
public:
	/* Public methods */
	PauseMenu(Renderer* pRenderer, OpenGLGUI* pGUI, FrontendManager* pFrontendManager, int windowWidth, int windowHeight);
	~PauseMenu();

	void Reset();

	void SetWindowDimensions(int width, int height);

	void Load();
	void Unload();

	void Update(float dt);

	void Render();
	void Render2D();

protected:
	/* Protected methods */
	static void _ResumePressed(void *apData);
	void ResumePressed();

	static void _OptionsPressed(void *apData);
	void OptionsPressed();

	static void _ModsPressed(void *apData);
	void ModsPressed();

	static void _QuitPressed(void *apData);
	void QuiPressed();

private:
	/* Private methods */

public:
	/* Public members */

protected:
	/* Protected members */

private:
	/* Private members */
	Button* m_pResumeButton;
	Button* m_pOptionsButton;
	Button* m_pModsButton;
	Button* m_pQuitButton;

	Icon* m_pBackgroundIcon;
};
