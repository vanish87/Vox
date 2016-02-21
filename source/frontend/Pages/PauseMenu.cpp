// ******************************************************************************
// Filename:    PauseMenu.cpp
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

#include "PauseMenu.h"
#include "..\FrontEndManager.h"
#include "..\gui\openglgui.h"
#include "..\VoxGame.h"


PauseMenu::PauseMenu(Renderer* pRenderer, OpenGLGUI* pGUI, FrontendManager* pFrontendManager, int windowWidth, int windowHeight)
	: FrontendPage(pRenderer, pGUI, pFrontendManager, FrontendScreen_PauseMenu, windowWidth, windowHeight)
{
	m_pBackgroundIcon = new Icon(m_pRenderer, "media/textures/gui/StoneWash/PauseMenu/pause_background.tga", 230, 265);
	m_pBackgroundIcon->SetDepth(1.5f);

	m_pResumeButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_30(), m_pFrontendManager->GetFrontendFont_30_Outline(), "Resume", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pResumeButton->SetCallBackFunction(_ResumePressed);
	m_pResumeButton->SetCallBackData(this);
	m_pResumeButton->SetDepth(2.0f);
	m_pResumeButton->SetLabelOffset(0, 3);
	m_pResumeButton->SetPressedOffset(0, -3);

	m_pOptionsButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_30(), m_pFrontendManager->GetFrontendFont_30_Outline(), "Options", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pOptionsButton->SetCallBackFunction(_OptionsPressed);
	m_pOptionsButton->SetCallBackData(this);
	m_pOptionsButton->SetDepth(2.0f);
	m_pOptionsButton->SetLabelOffset(0, 3);
	m_pOptionsButton->SetPressedOffset(0, -3);

	m_pModsButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_30(), m_pFrontendManager->GetFrontendFont_30_Outline(), "Mods", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pModsButton->SetCallBackFunction(_ModsPressed);
	m_pModsButton->SetCallBackData(this);
	m_pModsButton->SetDepth(2.0f);
	m_pModsButton->SetLabelOffset(0, 3);
	m_pModsButton->SetPressedOffset(0, -3);

	m_pQuitButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_30(), m_pFrontendManager->GetFrontendFont_30_Outline(), "Quit", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pQuitButton->SetCallBackFunction(_QuitPressed);
	m_pQuitButton->SetCallBackData(this);
	m_pQuitButton->SetDepth(2.0f);
	m_pQuitButton->SetLabelOffset(0, 3);
	m_pQuitButton->SetPressedOffset(0, -3);

	SetWindowDimensions(m_windowWidth, m_windowHeight);
}

PauseMenu::~PauseMenu()
{
	delete m_pBackgroundIcon;

	delete m_pResumeButton;
	delete m_pOptionsButton;
	delete m_pModsButton;
	delete m_pQuitButton;
}

void PauseMenu::Reset()
{
}

void PauseMenu::SetWindowDimensions(int windowWidth, int windowHeight)
{
	FrontendPage::SetWindowDimensions(windowWidth, windowHeight);

	m_pBackgroundIcon->SetDimensions((int)((m_windowWidth*0.5f)-(230*0.5f)), (int)((m_windowHeight*0.5f)-(335*0.5f)), 230, 305);

	m_pResumeButton->SetDimensions((int)((m_windowWidth*0.5f)-(110*0.5f)), (int)((m_windowHeight*0.5f)+(265*0.5f)-65), 110, 47);
	m_pOptionsButton->SetDimensions((int)((m_windowWidth*0.5f)-(110*0.5f)), (int)((m_windowHeight*0.5f)+(265*0.5f)-125), 110, 47);
	m_pModsButton->SetDimensions((int)((m_windowWidth*0.5f)-(110*0.5f)), (int)((m_windowHeight*0.5f)+(265*0.5f)-185), 110, 47);
	m_pQuitButton->SetDimensions((int)((m_windowWidth*0.5f)-(110*0.5f)), (int)((m_windowHeight*0.5f)+(265*0.5f)-275), 110, 47);
}

void PauseMenu::Load()
{
	m_pGUI->AddComponent(m_pResumeButton);
	m_pGUI->AddComponent(m_pOptionsButton);
	m_pGUI->AddComponent(m_pModsButton);
	m_pGUI->AddComponent(m_pQuitButton);

	m_pGUI->AddComponent(m_pBackgroundIcon);
}

void PauseMenu::Unload()
{
	m_pGUI->RemoveComponent(m_pBackgroundIcon);

	m_pGUI->RemoveComponent(m_pResumeButton);
	m_pGUI->RemoveComponent(m_pOptionsButton);
	m_pGUI->RemoveComponent(m_pModsButton);
	m_pGUI->RemoveComponent(m_pQuitButton);
}

void PauseMenu::Update(float dt)
{
	FrontendPage::Update(dt);
}

void PauseMenu::Render()
{
	FrontendPage::Render();
}

void PauseMenu::Render2D()
{
	FrontendPage::Render2D();
}

void PauseMenu::_ResumePressed(void *apData)
{
	PauseMenu* lpMainMenu = (PauseMenu*)apData;
	lpMainMenu->ResumePressed();
}

void PauseMenu::ResumePressed()
{
	Unload();

	VoxGame::GetInstance()->UnsetPauseMenu();
}

void PauseMenu::_OptionsPressed(void *apData)
{
	PauseMenu* lpMainMenu = (PauseMenu*)apData;
	lpMainMenu->OptionsPressed();
}

void PauseMenu::OptionsPressed()
{
}

void PauseMenu::_ModsPressed(void *apData)
{
	PauseMenu* lpMainMenu = (PauseMenu*)apData;
	lpMainMenu->ModsPressed();
}

void PauseMenu::ModsPressed()
{
}

void PauseMenu::_QuitPressed(void *apData)
{
	PauseMenu* lpMainMenu = (PauseMenu*)apData;
	lpMainMenu->QuiPressed();
}

void PauseMenu::QuiPressed()
{
	VoxGame::GetInstance()->UnsetPauseMenu();
	VoxGame::GetInstance()->QuitToFrontEnd();
}