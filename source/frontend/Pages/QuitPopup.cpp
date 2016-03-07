// ******************************************************************************
//
// Filename:	QuitPopup.cpp
// Project:		Game
// Author:		Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 12/10/14
//
// Copyright (c) 2005-2011, Steven Ball
//
// ******************************************************************************

#include "QuitPopup.h"
#include "../FrontendManager.h"
#include "../../gui/openglgui.h"
#include "../../VoxGame.h"


QuitPopup::QuitPopup(Renderer* pRenderer, OpenGLGUI* pGUI, FrontendManager* pFrontendManager, int windowWidth, int windowHeight)
	: FrontendPage(pRenderer, pGUI, pFrontendManager, FrontendScreen_QuitPopup, windowWidth, windowHeight)
{
	char quitText[] = "Are you sure you want to quit?";
	m_pQuitText = new Label(m_pRenderer, pFrontendManager->GetFrontendFont_40(), quitText, Colour(1.0f, 1.0f, 1.0f, 1.0f));
	m_pQuitText->SetOutline(true);
	m_pQuitText->SetOutlineColour(Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pQuitText->SetOutlineFont(pFrontendManager->GetFrontendFont_40_Outline());
	m_pQuitText->SetDepth(3.0f);

	//m_pBackgroundIcon = new Icon(m_pRenderer, "media/textures/gui/StoneWash/QuitPopup/background.tga", 400, 200);
	//m_pBackgroundIcon->SetDepth(2.0f);

	m_pConfirmButton = new Button(m_pRenderer, pFrontendManager->GetFrontendFont_30(), pFrontendManager->GetFrontendFont_30_Outline(), "Yes", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pConfirmButton->SetLabelOffset(0, 3);
	m_pConfirmButton->SetPressedOffset(0, 3);
	m_pConfirmButton->SetDepth(3.0f);
	m_pConfirmButton->SetCallBackFunction(_ConfirmPressed);
	m_pConfirmButton->SetCallBackData(this);	

	m_pCancelButton = new Button(m_pRenderer, pFrontendManager->GetFrontendFont_30(), pFrontendManager->GetFrontendFont_30_Outline(), "No", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pCancelButton->SetLabelOffset(0, 3);
	m_pCancelButton->SetDepth(3.0f);
	m_pCancelButton->SetPressedOffset(0, -3);
	m_pCancelButton->SetCallBackFunction(_CancelPressed);
	m_pCancelButton->SetCallBackData(this);	

	SetWindowDimensions(m_windowWidth, m_windowHeight);
}

QuitPopup::~QuitPopup()
{
	delete m_pQuitText;
	//delete m_pBackgroundIcon;
	delete m_pCancelButton;
	delete m_pConfirmButton;
}

void QuitPopup::Reset()
{
}

void QuitPopup::SetWindowDimensions(int windowWidth, int windowHeight)
{
	FrontendPage::SetWindowDimensions(windowWidth, windowHeight);

	m_backgroundWidth = 400;
	m_backgroundHeight = 175;

	int textWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_40(), "%s", m_pQuitText->GetText().c_str());
	m_pQuitText->SetLocation((int)((m_windowWidth*0.5f)-(textWidth*0.5f)), (int)((m_windowHeight*0.5f) + 25));

	//m_pBackgroundIcon->SetDimensions((int)((m_windowWidth*0.5f) - (m_backgroundWidth*0.5f)), (int)((m_windowHeight*0.5f) - (m_backgroundHeight*0.5f)+5), m_backgroundWidth, m_backgroundHeight);

	m_pCancelButton->SetDimensions((int)((m_windowWidth*0.5f) - 65 - (110*0.5f)), (int)((m_windowHeight*0.5f)-50), 110, 47);
	m_pConfirmButton->SetDimensions((int)((m_windowWidth*0.5f) + 65 - (110*0.5f)), (int)((m_windowHeight*0.5f)-50), 110, 47);
}

void QuitPopup::Load()
{
	m_pGUI->AddComponent(m_pQuitText);
	//m_pGUI->AddComponent(m_pBackgroundIcon);
	m_pGUI->AddComponent(m_pCancelButton);
	m_pGUI->AddComponent(m_pConfirmButton);
}

void QuitPopup::Unload()
{
	m_pGUI->RemoveComponent(m_pQuitText);
	//->RemoveComponent(m_pBackgroundIcon);
	m_pGUI->RemoveComponent(m_pCancelButton);
	m_pGUI->RemoveComponent(m_pConfirmButton);
}

void QuitPopup::Update(float dt)
{
	FrontendPage::Update(dt);

	m_cameraPosition = vec3(0.0f, 9.0f, 7.0f);
	m_cameraView = vec3(0.0f, 9.0f, 0.0f);
}

void QuitPopup::Render()
{
	FrontendPage::Render();
}

void QuitPopup::Render2D()
{
	FrontendPage::Render2D();
}

void QuitPopup::_ConfirmPressed(void *apData)
{
	QuitPopup* lpQuitPopup = (QuitPopup*)apData;
	lpQuitPopup->ConfirmPressed();
}

void QuitPopup::ConfirmPressed()
{
	VoxGame::GetInstance()->SetGameQuit(true);
}

void QuitPopup::_CancelPressed(void *apData)
{
	QuitPopup* lpQuitPopup = (QuitPopup*)apData;
	lpQuitPopup->CancelPressed();
}

void QuitPopup::CancelPressed()
{
	VoxGame::GetInstance()->CancelQuitPopup();

	Unload();
}
