// ******************************************************************************
// Filename:    ModMenu.h
// Project:     Vox
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
#include "../../gui/guiwindow.h"
#include "../../gui/optioncontroller.h"
#include "../../gui/scrollbar.h"


class ModMenu : public FrontendPage
{
public:
	/* Public methods */
	ModMenu(Renderer* pRenderer, OpenGLGUI* pGUI, FrontendManager* pFrontPageManager, int windowWidth, int windowHeight);
	~ModMenu();

	void Reset();

	void SetWindowDimensions(int windowWidth, int windowHeight);

	void SetReturnToMainMenu(bool mainMenu);

	void SkinGUI();
	void UnSkinGUI();

	void Load();
	void Unload();

	void CreateGameplayModButtons();
	void RemoveGameplayModButtons();

	void CreateGraphicsModButtons();
	void RemoveGraphicsModButtons();

	void CreateSoundModButtons();
	void RemoveSoundModButtons();

	void CreateHUDModButtons();
	void RemoveHUDModButtons();

	void CreateMiscModButtons();
	void RemoveMiscModButtons();

	void Update(float dt);

	void Render();
	void Render2D();

protected:
	/* Protected methods */
	static void _CloseExitPressed(void *apData);
	void CloseExitPressed();

	static void _GameplayTabPressed(void *pData);
	void GameplayTabPressed();

	static void _GraphicsTabPressed(void *pData);
	void GraphicsTabPressed();

	static void _SoundTabPressed(void *pData);
	void SoundTabPressed();

	static void _GUITabPressed(void *pData);
	void GUITabPressed();

	static void _MiscTabPressed(void *pData);
	void MiscTabPressed();

private:
	/* Private methods */

public:
	/* Public members */

protected:
	/* Protected members */

private:
	/* Private members */
	bool m_loaded;

	int m_modWindowWidth;
	int m_modWindowHeight;
	int m_titlebarWidth;
	int m_titlebarHeight;

	// Are we in the front-end or in the game
	bool m_returnToMainMenu;

	int m_modButtonWidth;
	int m_modButtonheight;
	int m_modButtonSpace;

	// Mod window
	GUIWindow* m_pModWindow;
	Icon* m_pTitleBarIcon;
	Icon* m_pTitleBarBackgroundIcon;
	Icon* m_pModWindowBackgroundIcon;
	Button* m_pCloseExitButton;

	// Tabs
	OptionController* m_pModsModeController;
	OptionBox* m_pGameplayMode;
	OptionBox* m_pGraphicsMode;
	OptionBox* m_pSoundMode;
	OptionBox* m_pHUDMode;
	OptionBox* m_pMiscMode;

	ScrollBar* m_pModsScrollbar;
	std::vector<Button*> m_vpGameplayModButtons;
	std::vector<Button*> m_vpGraphicsModButtons;
	std::vector<Button*> m_vpSoundModButtons;
	std::vector<Button*> m_vpHUDModButtons;
	std::vector<Button*> m_vpMiscModButtons;
};
