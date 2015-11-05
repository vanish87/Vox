// ******************************************************************************
// Filename:	FrontendManager.cpp
// Project:		Game
// Author:		Steven Ball
//
// Revision History:
//   Initial Revision - 01/11/15
//
// Copyright (c) 2005-2015, Steven Ball
// ******************************************************************************

#include "FrontendManager.h"


FrontendManager::FrontendManager(Renderer* pRenderer)
{
	m_pRenderer = pRenderer;

	// Checkbox
	m_pCheckboxIcon = new Icon(m_pRenderer, "", 16, 16);
	m_pCheckboxIcon->SetDepth(2.0f);
	m_pCheckboxIconHover = new Icon(m_pRenderer, "", 16, 16);
	m_pCheckboxIconHover->SetDepth(2.0f);
	m_pCheckboxIconPressed = new Icon(m_pRenderer, "", 16, 16);
	m_pCheckboxIconPressed->SetDepth(2.0f);
	m_pCheckboxIconDisabled = new Icon(m_pRenderer, "", 16, 16);
	m_pCheckboxIconDisabled->SetDepth(2.0f);
	m_pCheckboxIconToggled = new Icon(m_pRenderer, "", 16, 16);
	m_pCheckboxIconToggled->SetDepth(2.0f);
	m_pCheckboxIconToggledHover = new Icon(m_pRenderer, "", 16, 16);
	m_pCheckboxIconToggledHover->SetDepth(2.0f);
	m_pCheckboxIconToggledPressed = new Icon(m_pRenderer, "", 16, 16);
	m_pCheckboxIconToggledPressed->SetDepth(2.0f);
	m_pCheckboxIconToggledDisabled = new Icon(m_pRenderer, "", 16, 16);
	m_pCheckboxIconToggledDisabled->SetDepth(2.0f);

	// Optionbox
	m_pOptionboxIcon = new Icon(m_pRenderer, "", 16, 16);
	m_pOptionboxIcon->SetDepth(2.0f);
	m_pOptionboxIconHover = new Icon(m_pRenderer, "", 16, 16);
	m_pOptionboxIconHover->SetDepth(2.0f);
	m_pOptionboxIconPressed = new Icon(m_pRenderer, "", 16, 16);
	m_pOptionboxIconPressed->SetDepth(2.0f);
	m_pOptionboxIconDisabled = new Icon(m_pRenderer, "", 16, 16);
	m_pOptionboxIconDisabled->SetDepth(2.0f);
	m_pOptionboxIconToggled = new Icon(m_pRenderer, "", 16, 16);
	m_pOptionboxIconToggled->SetDepth(2.0f);
	m_pOptionboxIconToggledHover = new Icon(m_pRenderer, "", 16, 16);
	m_pOptionboxIconToggledHover->SetDepth(2.0f);
	m_pOptionboxIconToggledPressed = new Icon(m_pRenderer, "", 16, 16);
	m_pOptionboxIconToggledPressed->SetDepth(2.0f);
	m_pOptionboxIconToggledDisabled = new Icon(m_pRenderer, "", 16, 16);
	m_pOptionboxIconToggledDisabled->SetDepth(2.0f);

	LoadCommonGraphics("default");
}

void FrontendManager::LoadCommonGraphics(string themeName)
{
	string iconName;

	// Checkbox
	iconName = "media/textures/gui/" + themeName + "/Common/checkbox/checkDefault.tga";
	m_pCheckboxIcon->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/Common/checkbox/checkHover.tga";
	m_pCheckboxIconHover->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/Common/checkbox/checkPressed.tga";
	m_pCheckboxIconPressed->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/Common/checkbox/checkDisabled.tga";
	m_pCheckboxIconDisabled->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/Common/checkbox/checkToggled.tga";
	m_pCheckboxIconToggled->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/Common/checkbox/checkToggledHover.tga";
	m_pCheckboxIconToggledHover->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/Common/checkbox/checkToggledPressed.tga";
	m_pCheckboxIconToggledPressed->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/Common/checkbox/checkToggledDisabled.tga";
	m_pCheckboxIconToggledDisabled->SetIcon(iconName);

	// Optionbox
	iconName = "media/textures/gui/" + themeName + "/Common/optionbox/optionDefault.tga";
	m_pOptionboxIcon->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/Common/optionbox/optionHover.tga";
	m_pOptionboxIconHover->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/Common/optionbox/optionPressed.tga";
	m_pOptionboxIconPressed->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/Common/optionbox/optionDisabled.tga";
	m_pOptionboxIconDisabled->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/Common/optionbox/optionToggled.tga";
	m_pOptionboxIconToggled->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/Common/optionbox/optionToggledHover.tga";
	m_pOptionboxIconToggledHover->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/Common/optionbox/optionToggledPressed.tga";
	m_pOptionboxIconToggledPressed->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/Common/optionbox/optionToggledDisabled.tga";
	m_pOptionboxIconToggledDisabled->SetIcon(iconName);
}

FrontendManager::~FrontendManager()
{
	// Checkbox
	delete m_pCheckboxIcon;
	delete m_pCheckboxIconHover;
	delete m_pCheckboxIconPressed;
	delete m_pCheckboxIconDisabled;
	delete m_pCheckboxIconToggled;
	delete m_pCheckboxIconToggledHover;
	delete m_pCheckboxIconToggledPressed;
	delete m_pCheckboxIconToggledDisabled;

	// Optionbox
	delete m_pOptionboxIcon;
	delete m_pOptionboxIconHover;
	delete m_pOptionboxIconPressed;
	delete m_pOptionboxIconDisabled;
	delete m_pOptionboxIconToggled;
	delete m_pOptionboxIconToggledHover;
	delete m_pOptionboxIconToggledPressed;
	delete m_pOptionboxIconToggledDisabled;
}

// Updating
void FrontendManager::Update(float dt)
{

}

// Rendering
void FrontendManager::Render()
{

}