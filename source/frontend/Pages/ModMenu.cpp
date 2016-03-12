// ******************************************************************************
// Filename:    OptionsMenu.cpp
// Project:     Vox
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 12/10/14
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "ModMenu.h"
#include "../../VoxGame.h"
#include "../FrontendManager.h"
#include "../../gui/openglgui.h"


ModMenu::ModMenu(Renderer* pRenderer, OpenGLGUI* pGUI, FrontendManager* pFrontPageManager, int windowWidth, int windowHeight)
	: FrontendPage(pRenderer, pGUI, pFrontPageManager, FrontendScreen_ModMenu, windowWidth, windowHeight)
{
	// Mod Window
	m_pModWindow = new GUIWindow(m_pRenderer, m_pFrontendManager->GetFrontendFont_Medium(), "Mods");
	m_pModWindow->AllowMoving(true);
	m_pModWindow->AllowClosing(false);
	m_pModWindow->AllowMinimizing(false);
	m_pModWindow->AllowScrolling(false);
	m_pModWindow->SetRenderTitleBar(true);
	m_pModWindow->SetRenderWindowBackground(true);
	m_pModWindow->SetApplicationDimensions(m_windowWidth, m_windowHeight);
	m_pModWindow->Hide();

	m_pTitleBarIcon = new Icon(m_pRenderer, "media/textures/gui/Stonewash/ModMenu/mod_title_icon.tga", 44, 44);
	m_pTitleBarIcon->SetDepth(4.0f);

	m_pModWindowBackgroundIcon = new Icon(m_pRenderer, "media/textures/gui/Stonewash/ModMenu/mod_window_background.tga", 600, 475);
	m_pModWindowBackgroundIcon->SetDepth(1.0f);

	m_pTitleBarBackgroundIcon = new Icon(m_pRenderer, "media/textures/gui/Stonewash/ModMenu/titlebar_background.tga", 86, 35);
	m_pTitleBarBackgroundIcon->SetDepth(1.0f);

	m_pCloseExitButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_30(), m_pFrontendManager->GetFrontendFont_30_Outline(), "", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pCloseExitButton->SetDefaultIcon(m_pFrontendManager->GetCloseExitButtonIcon());
	m_pCloseExitButton->SetHoverIcon(m_pFrontendManager->GetCloseExitButtonIcon_Hover());
	m_pCloseExitButton->SetSelectedIcon(m_pFrontendManager->GetCloseExitButtonIcon_Pressed());
	m_pCloseExitButton->SetDisabledIcon(m_pFrontendManager->GetCloseExitButtonIcon());
	m_pCloseExitButton->SetLabelOffset(0, 5);
	m_pCloseExitButton->SetCallBackFunction(_CloseExitPressed);
	m_pCloseExitButton->SetCallBackData(this);

	m_pModWindow->SetBackgroundIcon(m_pModWindowBackgroundIcon);
	m_pModWindow->SetTitlebarBackgroundIcon(m_pTitleBarBackgroundIcon);

	m_pModWindow->AddComponent(m_pTitleBarIcon);
	m_pModWindow->AddComponent(m_pCloseExitButton);

	// Tabs
	m_pModsModeController = new OptionController(m_pRenderer, m_pFrontendManager->GetFrontendFont_25(), "Mode");
	m_pModsModeController->SetDisplayLabel(false);
	m_pModsModeController->SetDisplayBorder(false);

	m_pGameplayMode = new OptionBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_18(), m_pFrontendManager->GetFrontendFont_18_Outline(), "Gameplay", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pGameplayMode->SetCallBackFunction(_GameplayTabPressed);
	m_pGameplayMode->SetCallBackData(this);
	m_pGameplayMode->SetDepth(2.0f);
	m_pGameplayMode->SetPressedOffset(0, -2);
	//m_pGameplayMode->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pGameplayMode->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	m_pGraphicsMode = new OptionBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_18(), m_pFrontendManager->GetFrontendFont_18_Outline(), "Graphics", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pGraphicsMode->SetCallBackFunction(_GraphicsTabPressed);
	m_pGraphicsMode->SetCallBackData(this);
	m_pGraphicsMode->SetDepth(2.1f);
	m_pGraphicsMode->SetPressedOffset(0, -2);
	//m_pGraphicsMode->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pGraphicsMode->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	m_pSoundMode = new OptionBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_18(), m_pFrontendManager->GetFrontendFont_18_Outline(), "Sound", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pSoundMode->SetCallBackFunction(_SoundTabPressed);
	m_pSoundMode->SetCallBackData(this);
	m_pSoundMode->SetDepth(2.2f);
	m_pSoundMode->SetPressedOffset(0, -2);
	//m_pSoundMode->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pSoundMode->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	m_pGUIMode = new OptionBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_18(), m_pFrontendManager->GetFrontendFont_18_Outline(), "HUD", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pGUIMode->SetCallBackFunction(_GUITabPressed);
	m_pGUIMode->SetCallBackData(this);
	m_pGUIMode->SetDepth(2.3f);
	m_pGUIMode->SetPressedOffset(0, -2);
	//m_pGUIMode->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pGUIMode->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	m_pMiscMode = new OptionBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_18(), m_pFrontendManager->GetFrontendFont_18_Outline(), "Misc", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pMiscMode->SetCallBackFunction(_MiscTabPressed);
	m_pMiscMode->SetCallBackData(this);
	m_pMiscMode->SetDepth(2.4f);
	m_pMiscMode->SetPressedOffset(0, -2);
	//m_pMiscMode->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pMiscMode->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	// Scrollbar
	m_pModsScrollbar = new ScrollBar(m_pRenderer);
	m_pModsScrollbar->SetScrollDirection(EScrollBarDirection_Vertical);
	m_pModsScrollbar->SetScrollSize(0.25f);
	m_pModsScrollbar->SetScrollPosition(1.0f);
	m_pModsScrollbar->SetDepth(4.0f);
	m_pModsScrollbar->SetScissorEnabled(true);

	m_pModsModeController->Add(m_pGameplayMode);
	m_pModsModeController->Add(m_pGraphicsMode);
	m_pModsModeController->Add(m_pSoundMode);
	m_pModsModeController->Add(m_pGUIMode);
	m_pModsModeController->Add(m_pMiscMode);	
	m_pGameplayMode->SetToggled(true);

	m_pModWindow->AddComponent(m_pModsModeController);
	m_pModWindow->AddComponent(m_pModsScrollbar);

	SetWindowDimensions(m_windowWidth, m_windowHeight);

	m_returnToMainMenu = false;
}

ModMenu::~ModMenu()
{
	RemoveGameplayModButtons();
	RemoveGraphicsModButtons();
	RemoveSoundModButtons();
	RemoveHUDModButtons();
	RemoveMiscModButtons();

	delete m_pModWindow;
	delete m_pTitleBarIcon;
	delete m_pTitleBarBackgroundIcon;
	delete m_pModWindowBackgroundIcon;
	delete m_pCloseExitButton;

	// Tabs
	delete m_pModsModeController;
	delete m_pGameplayMode;
	delete m_pGraphicsMode;
	delete m_pSoundMode;
	delete m_pGUIMode;
	delete m_pMiscMode;

	// Scrollbar
	delete m_pModsScrollbar;
}

void ModMenu::Reset()
{
}

void ModMenu::SetWindowDimensions(int windowWidth, int windowHeight)
{
	FrontendPage::SetWindowDimensions(windowWidth, windowHeight);

	m_modWindowWidth = 600;
	m_modWindowHeight = 475;
	m_titlebarWidth = 118;
	m_titlebarHeight = 35;

	m_pModWindow->SetDimensions((int)((m_windowWidth*0.5f) - (m_modWindowWidth*0.5f)), (int)((m_windowHeight*0.5f) - (m_modWindowHeight*0.5f)), m_modWindowWidth, m_modWindowHeight);
	m_pModWindow->SetTitleBarDimensions(0, 0, m_titlebarWidth, m_titlebarHeight);
	m_pModWindow->SetTitleOffset(50, 5);
	m_pModWindow->SetApplicationDimensions(m_windowWidth, m_windowHeight);
	m_pModWindow->SetApplicationBorder(25, 15, 10, 10);

	m_pTitleBarIcon->SetDimensions(0, m_modWindowHeight, 44, 44);

	m_pCloseExitButton->SetDimensions(m_modWindowWidth-32, m_modWindowHeight, 32, 32);
	
	// Tabs
	m_pModsModeController->SetDimensions(0, m_modWindowHeight, m_modWindowWidth, 32);
	m_pGameplayMode->SetDisplayLabel(false);
	m_pGameplayMode->SetDimensions(m_modWindowWidth-32-375, 0, 75, 32);
	m_pGameplayMode->SetDisplayLabel(true);
	int textWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_18_Outline(), "%s", m_pGameplayMode->GetLabel().GetText().c_str());
	m_pGameplayMode->SetLabelPosition((int)(75*0.5f-textWidth*0.5f), 8);

	m_pGraphicsMode->SetDisplayLabel(false);
	m_pGraphicsMode->SetDimensions(m_modWindowWidth-32-300, 0, 75, 32);
	m_pGraphicsMode->SetDisplayLabel(true);
	textWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_18_Outline(), "%s", m_pGraphicsMode->GetLabel().GetText().c_str());
	m_pGraphicsMode->SetLabelPosition((int)(75*0.5f-textWidth*0.5f), 8);

	m_pSoundMode->SetDisplayLabel(false);
	m_pSoundMode->SetDimensions(m_modWindowWidth-32-225, 0, 75, 32);
	m_pSoundMode->SetDisplayLabel(true);
	textWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_18_Outline(), "%s", m_pSoundMode->GetLabel().GetText().c_str());
	m_pSoundMode->SetLabelPosition((int)(75*0.5f-textWidth*0.5f), 8);

	m_pGUIMode->SetDisplayLabel(false);
	m_pGUIMode->SetDimensions(m_modWindowWidth-32-150, 0, 75, 32);
	m_pGUIMode->SetDisplayLabel(true);
	textWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_18_Outline(), "%s", m_pGUIMode->GetLabel().GetText().c_str());
	m_pGUIMode->SetLabelPosition((int)(75*0.5f-textWidth*0.5f), 8);

	m_pMiscMode->SetDisplayLabel(false);
	m_pMiscMode->SetDimensions(m_modWindowWidth-32-75, 0, 75, 32);
	m_pMiscMode->SetDisplayLabel(true);
	textWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_18_Outline(), "%s", m_pMiscMode->GetLabel().GetText().c_str());
	m_pMiscMode->SetLabelPosition((int)(75*0.5f-textWidth*0.5f), 8);

	// Scrollbar
	int scrollbarHeight = m_modWindowHeight-6;
	m_pModsScrollbar->SetDimensions(m_modWindowWidth-28, 4, 24, scrollbarHeight);
	m_pModsScrollbar->SetScrollArea(-(m_modWindowWidth-40), 0, m_modWindowWidth-40, scrollbarHeight);
}

void ModMenu::SetReturnToMainMenu(bool mainMenu)
{
	m_returnToMainMenu = mainMenu;
}

void ModMenu::SkinGUI()
{

}

void ModMenu::UnSkinGUI()
{

}

void ModMenu::Load()
{
	m_pGameplayMode->SetToggled(true);
	GameplayTabPressed();

	m_loaded = true;
}

void ModMenu::Unload()
{
	RemoveGameplayModButtons();
	RemoveGraphicsModButtons();
	RemoveSoundModButtons();
	RemoveHUDModButtons();
	RemoveMiscModButtons();

	m_pGUI->RemoveWindow(m_pModWindow);

	m_loaded = false;
}

void ModMenu::CreateGameplayModButtons()
{

}

void ModMenu::RemoveGameplayModButtons()
{

}

void ModMenu::CreateGraphicsModButtons()
{

}

void ModMenu::RemoveGraphicsModButtons()
{

}

void ModMenu::CreateSoundModButtons()
{

}

void ModMenu::RemoveSoundModButtons()
{

}

void ModMenu::CreateHUDModButtons()
{
	int buttonWidth = 170;
	int buttonHeight = 150;
	int buttonSpacer = 15;
	int buttonAndSpacerWidth = buttonWidth + buttonSpacer;
	int buttonAndSpacerHeight = buttonHeight + buttonSpacer;

	int buttonX = -(m_modWindowWidth-42);
	int buttonY = m_modWindowHeight - buttonHeight - 17;
	for(int y = 0; y < 3; y++)
	{
		buttonX = -(m_modWindowWidth-42);

		for(int x = 0; x < 3; x++)
		{
			Button* m_pNewButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_35(), "");
			m_pNewButton->SetDimensions(buttonX, buttonY, buttonWidth, buttonHeight);
			m_pNewButton->SetPressedOffset(0, -4);

			m_pModsScrollbar->AddScrollAreaItem(m_pNewButton);

			m_vpHUDModButtons.push_back(m_pNewButton);

			buttonX += buttonAndSpacerWidth;
		}

		buttonY -= buttonAndSpacerHeight;
	}
}

void ModMenu::RemoveHUDModButtons()
{
	m_pModsScrollbar->ClearScrollAreaItems();
	for(unsigned int i = 0; i < m_vpHUDModButtons.size(); i++)
	{
		delete m_vpHUDModButtons[i];
		m_vpHUDModButtons[i] = 0;
	}
	m_vpHUDModButtons.clear();
}

void ModMenu::CreateMiscModButtons()
{

}

void ModMenu::RemoveMiscModButtons()
{

}

void ModMenu::Update(float dt)
{
	FrontendPage::Update(dt);

	VoxGame::GetInstance()->SetGlobalBlurAmount(0.00125f);

	// HUD
	int buttonWidth = 170;
	int buttonHeight = 150;
	int buttonSpacer = 15;
	int buttonAndSpacer = buttonHeight + buttonSpacer;

	int visibleSize = m_pModsScrollbar->GetScrollArea().m_height;
	int neededHeight = ((int)m_vpHUDModButtons.size() / 3) * buttonAndSpacer + 4;
	int heightDiff = neededHeight - visibleSize;

	if(heightDiff > 0)
	{
		m_pModsScrollbar->SetScrollSize((float)visibleSize / (float)neededHeight);
		m_pModsScrollbar->SetDisabled(false);

		float scrollRatio = m_pModsScrollbar->GetScrollRatio();
		int offsetButtonY = (int)(heightDiff * scrollRatio);

		int modButtonCounter = 0;
		int yCounter = 0;
		while(modButtonCounter < m_vpHUDModButtons.size())
		{
			for(int x = 0; x < 3 && modButtonCounter < m_vpHUDModButtons.size(); x++)
			{
				int yPos = m_modWindowHeight - buttonHeight - 17 - (buttonAndSpacer * yCounter);
				Point position = m_vpHUDModButtons[modButtonCounter]->GetLocation();
				m_vpHUDModButtons[modButtonCounter]->SetLocation(position.m_x, yPos + offsetButtonY);

				modButtonCounter++;
			}

			yCounter++;
		}
	}
	else
	{
		m_pModsScrollbar->SetScrollSize(0.0f);
		m_pModsScrollbar->SetDisabled(true);
	}
}

void ModMenu::Render()
{
	FrontendPage::Render();
}

void ModMenu::Render2D()
{
	FrontendPage::Render2D();
}

void ModMenu::_CloseExitPressed(void *pData)
{
	ModMenu* lpModMenu = (ModMenu*)pData;
	lpModMenu->CloseExitPressed();
}

void ModMenu::CloseExitPressed()
{
	if(m_returnToMainMenu)
	{
		m_pFrontendManager->SetFrontendScreen(FrontendScreen_MainMenu);
	}
	else
	{
		m_pFrontendManager->SetFrontendScreen(FrontendScreen_PauseMenu);
	}
}

void ModMenu::_GameplayTabPressed(void *pData)
{
	ModMenu* lpModMenu = (ModMenu*)pData;
	lpModMenu->GameplayTabPressed();
}

void ModMenu::GameplayTabPressed()
{
	m_pGUI->RemoveWindow(m_pModWindow);

	RemoveGraphicsModButtons();
	RemoveSoundModButtons();
	RemoveHUDModButtons();
	RemoveMiscModButtons();

	CreateGameplayModButtons();

	m_pGUI->AddWindow(m_pModWindow);
	m_pModWindow->Show();
}

void ModMenu::_GraphicsTabPressed(void *pData)
{
	ModMenu* lpModMenu = (ModMenu*)pData;
	lpModMenu->GraphicsTabPressed();
}

void ModMenu::GraphicsTabPressed()
{
	m_pGUI->RemoveWindow(m_pModWindow);

	RemoveGameplayModButtons();
	RemoveSoundModButtons();
	RemoveHUDModButtons();
	RemoveMiscModButtons();

	CreateGraphicsModButtons();

	m_pGUI->AddWindow(m_pModWindow);
	m_pModWindow->Show();
}

void ModMenu::_SoundTabPressed(void *pData)
{
	ModMenu* lpModMenu = (ModMenu*)pData;
	lpModMenu->SoundTabPressed();
}

void ModMenu::SoundTabPressed()
{
	m_pGUI->RemoveWindow(m_pModWindow);

	RemoveGameplayModButtons();
	RemoveGraphicsModButtons();
	RemoveHUDModButtons();
	RemoveMiscModButtons();

	CreateSoundModButtons();

	m_pGUI->AddWindow(m_pModWindow);
	m_pModWindow->Show();
}

void ModMenu::_GUITabPressed(void *pData)
{
	ModMenu* lpModMenu = (ModMenu*)pData;
	lpModMenu->GUITabPressed();
}

void ModMenu::GUITabPressed()
{
	m_pGUI->RemoveWindow(m_pModWindow);

	RemoveGameplayModButtons();
	RemoveGraphicsModButtons();
	RemoveSoundModButtons();
	RemoveMiscModButtons();

	CreateHUDModButtons();

	m_pGUI->AddWindow(m_pModWindow);
	m_pModWindow->Show();
}

void ModMenu::_MiscTabPressed(void *pData)
{
	ModMenu* lpModMenu = (ModMenu*)pData;
	lpModMenu->MiscTabPressed();
}

void ModMenu::MiscTabPressed()
{
	m_pGUI->RemoveWindow(m_pModWindow);

	RemoveGameplayModButtons();
	RemoveGraphicsModButtons();
	RemoveSoundModButtons();
	RemoveHUDModButtons();

	CreateMiscModButtons();

	m_pGUI->AddWindow(m_pModWindow);
	m_pModWindow->Show();
}