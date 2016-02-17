// ******************************************************************************
// Filename:    FrontendPage.cpp
// Project:     Game
// Author:      Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 12/10/14
//
// Copyright (c) 2005-2011, Steven Ball
// ******************************************************************************

#include "FrontendPage.h"
#include "FrontendManager.h"
#include "../Renderer/Renderer.h"
#include "../gui/openglgui.h"


FrontEndPage::FrontEndPage(Renderer* pRenderer, OpenGLGUI* pGUI, FrontendManager* pFrontendManager, eFrontendScreen pageType, int windowWidth, int windowHeight)
{
	m_pRenderer = pRenderer;
	m_pGUI = pGUI;
	m_pFrontendManager = pFrontendManager;

	m_pageType = pageType;

	char titleText[] = "[TITLE]";
	m_pPageTitle = new Label(m_pRenderer, m_pFrontendManager->GetFrontendFont_40(), titleText, Colour(1.0f, 1.0f, 1.0f, 1.0f));
	m_pPageTitle->SetOutline(true);
	m_pPageTitle->SetOutlineColour(Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pPageTitle->SetOutlineFont(m_pFrontendManager->GetFrontendFont_40_Outline());
	m_pPageTitle->SetDepth(4.0f);

	char subtitleText[] = "[Subtitle]";
	m_pPageSubtitle = new Label(m_pRenderer, m_pFrontendManager->GetFrontendFont_25(), subtitleText, Colour(1.0f, 1.0f, 1.0f, 1.0f));
	m_pPageSubtitle->SetOutline(true);
	m_pPageSubtitle->SetOutlineColour(Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pPageSubtitle->SetOutlineFont(m_pFrontendManager->GetFrontendFont_25_Outline());
	m_pPageSubtitle->SetDepth(4.0f);

	m_addedTitles = false;
	m_updateTitleTextCenterLocation = true;

	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
}

FrontEndPage::~FrontEndPage()
{
	delete m_pPageTitle;
	delete m_pPageSubtitle;
}

void FrontEndPage::SetWindowDimensions(int width, int height)
{
	m_windowWidth = width;
	m_windowHeight = height;

	m_updateTitleTextCenterLocation = true;
}

eFrontendScreen FrontEndPage::GetPageType()
{
	return m_pageType;
}

void FrontEndPage::SetTitleAndSubtitle(string title, string subtitle)
{
	m_pPageTitle->SetText(title);
	m_pPageSubtitle->SetText(subtitle);

	m_updateTitleTextCenterLocation = true;

	if(m_addedTitles == false)
	{
		m_pGUI->AddComponent(m_pPageTitle);
		m_pGUI->AddComponent(m_pPageSubtitle);

		m_addedTitles = true;
	}
}

void FrontEndPage::RemoveTitlesAndSubTitle()
{
	if(m_addedTitles == true)
	{
		m_pGUI->RemoveComponent(m_pPageTitle);
		m_pGUI->RemoveComponent(m_pPageSubtitle);

		m_addedTitles = false;
	}
}

vec3 FrontEndPage::GetCameraPosition()
{
	return m_cameraPosition;
}

vec3 FrontEndPage::GetCameraView()
{
	return m_cameraView;
}

void FrontEndPage::Update(float dt)
{
	/* Do nothing */
}

void FrontEndPage::Render()
{
	if(m_updateTitleTextCenterLocation)
	{
		int textWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_40(), "%s", m_pPageTitle->GetText().c_str());
		m_pPageTitle->SetLocation((int)(m_windowWidth*0.5f-textWidth*0.5f), m_windowHeight-50);

		textWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_25(), "%s", m_pPageSubtitle->GetText().c_str());
		m_pPageSubtitle->SetLocation((int)(m_windowWidth*0.5f-textWidth*0.5f), m_windowHeight-75);

		m_updateTitleTextCenterLocation = false;
	}
}

void FrontEndPage::Render2D()
{
	/* Do nothing */
}

void FrontEndPage::RenderDebug()
{
	/* Do nothing */
}
