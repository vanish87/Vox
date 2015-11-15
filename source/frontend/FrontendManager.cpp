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

	// Scrollbar
	m_pScrollbarArrowDownIcon = new Icon(m_pRenderer, "", 18, 18);
	m_pScrollbarArrowDownIcon->SetDepth(2.0f);
	m_pScrollbarArrowDownIconHover = new Icon(m_pRenderer, "", 18, 18);
	m_pScrollbarArrowDownIconHover->SetDepth(2.0f);
	m_pScrollbarArrowDownIconPressed = new Icon(m_pRenderer, "", 18, 18);
	m_pScrollbarArrowDownIconPressed->SetDepth(2.0f);
	m_pScrollbarArrowDownIconDisabled = new Icon(m_pRenderer, "", 18, 18);
	m_pScrollbarArrowDownIconDisabled->SetDepth(2.0f);
	m_pScrollbarArrowUpIcon = new Icon(m_pRenderer, "", 18, 18);
	m_pScrollbarArrowUpIcon->SetDepth(2.0f);
	m_pScrollbarArrowUpIconHover = new Icon(m_pRenderer, "", 18, 18);
	m_pScrollbarArrowUpIconHover->SetDepth(2.0f);
	m_pScrollbarArrowUpIconPressed = new Icon(m_pRenderer, "", 18, 18);
	m_pScrollbarArrowUpIconPressed->SetDepth(2.0f);
	m_pScrollbarArrowUpIconDisabled = new Icon(m_pRenderer, "", 18, 18);
	m_pScrollbarArrowUpIconDisabled->SetDepth(2.0f);
	m_pScrollbarBackgroundIcon = new Icon(m_pRenderer, "", 18, 18);
	m_pScrollbarBackgroundIcon->SetDepth(2.0f);
	m_pScrollbarBackgroundIconDisabled = new Icon(m_pRenderer, "", 18, 18);
	m_pScrollbarBackgroundIconDisabled->SetDepth(2.0f);
	m_pScrollbarIcon = new Icon(m_pRenderer, "", 18, 18);
	m_pScrollbarIcon->SetDepth(4.0f);
	m_pScrollbarIconHover = new Icon(m_pRenderer, "", 18, 18);
	m_pScrollbarIconHover->SetDepth(4.0f);
	m_pScrollbarIconPressed = new Icon(m_pRenderer, "", 18, 18);
	m_pScrollbarIconPressed->SetDepth(4.0f);
	m_pScrollbarIconDisabled = new Icon(m_pRenderer, "", 18, 18);
	m_pScrollbarIconDisabled->SetDepth(4.0f);

	// Buttons
	m_pButtonIcon = new Icon*[ButtonSize_NUM];
	m_pButtonIconHover = new Icon*[ButtonSize_NUM];
	m_pButtonIconPressed = new Icon*[ButtonSize_NUM];
	m_pButtonIconDisabled = new Icon*[ButtonSize_NUM];
	for (int i = 0; i < ButtonSize_NUM; i++)
	{
		int width = 0;
		int height = 0;
		if (ButtonSize(i) == ButtonSize_85x25) { width = 85; height = 25; }

		m_pButtonIcon[ButtonSize(i)] = new Icon(m_pRenderer, "", width, height);
		m_pButtonIcon[ButtonSize(i)]->SetDepth(2.0f);
		m_pButtonIconHover[ButtonSize(i)] = new Icon(m_pRenderer, "", width, height);
		m_pButtonIconHover[ButtonSize(i)]->SetDepth(2.0f);
		m_pButtonIconPressed[ButtonSize(i)] = new Icon(m_pRenderer, "", width, height);
		m_pButtonIconPressed[ButtonSize(i)]->SetDepth(2.0f);
		m_pButtonIconDisabled[ButtonSize(i)] = new Icon(m_pRenderer, "", width, height);
		m_pButtonIconDisabled[ButtonSize(i)]->SetDepth(2.0f);
	}

	LoadCommonGraphics("Default");
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

	// Scrollbar
	delete m_pScrollbarArrowDownIcon;
	delete m_pScrollbarArrowDownIconHover;
	delete m_pScrollbarArrowDownIconPressed;
	delete m_pScrollbarArrowDownIconDisabled;
	delete m_pScrollbarArrowUpIcon;
	delete m_pScrollbarArrowUpIconHover;
	delete m_pScrollbarArrowUpIconPressed;
	delete m_pScrollbarArrowUpIconDisabled;
	delete m_pScrollbarBackgroundIcon;
	delete m_pScrollbarBackgroundIconDisabled;
	delete m_pScrollbarIcon;
	delete m_pScrollbarIconHover;
	delete m_pScrollbarIconPressed;
	delete m_pScrollbarIconDisabled;

	// Buttons
	for (int i = 0; i < ButtonSize_NUM; i++)
	{
		delete m_pButtonIcon;
		delete m_pButtonIconHover;
		delete m_pButtonIconPressed;
		delete m_pButtonIconDisabled;
	}
}

// Load the icon graphics based on a theme
void FrontendManager::LoadCommonGraphics(string themeName)
{
	string iconName;

	// Checkbox
	iconName = "media/textures/gui/" + themeName + "/common/checkbox/checkDefault.tga";
	m_pCheckboxIcon->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/checkbox/checkHover.tga";
	m_pCheckboxIconHover->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/checkbox/checkPressed.tga";
	m_pCheckboxIconPressed->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/checkbox/checkDisabled.tga";
	m_pCheckboxIconDisabled->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/checkbox/checkToggled.tga";
	m_pCheckboxIconToggled->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/checkbox/checkToggledHover.tga";
	m_pCheckboxIconToggledHover->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/checkbox/checkToggledPressed.tga";
	m_pCheckboxIconToggledPressed->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/checkbox/checkToggledDisabled.tga";
	m_pCheckboxIconToggledDisabled->SetIcon(iconName);

	// Optionbox
	iconName = "media/textures/gui/" + themeName + "/common/optionbox/optionDefault.tga";
	m_pOptionboxIcon->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/optionbox/optionHover.tga";
	m_pOptionboxIconHover->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/optionbox/optionPressed.tga";
	m_pOptionboxIconPressed->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/optionbox/optionDisabled.tga";
	m_pOptionboxIconDisabled->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/optionbox/optionToggled.tga";
	m_pOptionboxIconToggled->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/optionbox/optionToggledHover.tga";
	m_pOptionboxIconToggledHover->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/optionbox/optionToggledPressed.tga";
	m_pOptionboxIconToggledPressed->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/optionbox/optionToggledDisabled.tga";
	m_pOptionboxIconToggledDisabled->SetIcon(iconName);

	// Scrollbar
	iconName = "media/textures/gui/" + themeName + "/common/scrollbar/arrowDownDefault.tga";
	m_pScrollbarArrowDownIcon->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/scrollbar/arrowDownHover.tga";
	m_pScrollbarArrowDownIconHover->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/scrollbar/arrowDownPressed.tga";
	m_pScrollbarArrowDownIconPressed->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/scrollbar/arrowDownDisabled.tga";
	m_pScrollbarArrowDownIconDisabled->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/scrollbar/arrowUpDefault.tga";
	m_pScrollbarArrowUpIcon->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/scrollbar/arrowUpHover.tga";
	m_pScrollbarArrowUpIconHover->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/scrollbar/arrowUpPressed.tga";
	m_pScrollbarArrowUpIconPressed->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/scrollbar/arrowUpDisabled.tga";
	m_pScrollbarArrowUpIconDisabled->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/scrollbar/backgroundDefault.tga";
	m_pScrollbarBackgroundIcon->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/scrollbar/backgroundDisabled.tga";
	m_pScrollbarBackgroundIconDisabled->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/scrollbar/scrollbarDefault.tga";
	m_pScrollbarIcon->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/scrollbar/scrollbarHover.tga";
	m_pScrollbarIconHover->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/scrollbar/scrollbarPressed.tga";
	m_pScrollbarIconPressed->SetIcon(iconName);
	iconName = "media/textures/gui/" + themeName + "/common/scrollbar/scrollbarDisabled.tga";
	m_pScrollbarIconDisabled->SetIcon(iconName);

	// Buttons
	for (int i = 0; i < ButtonSize_NUM; i++)
	{
		string sizeFolder = "";
		if (ButtonSize(i) == ButtonSize_85x25) { sizeFolder = "85x25";  }

		iconName = "media/textures/gui/" + themeName + "/common/buttons/" + sizeFolder + "/buttonDefault.tga";
		m_pButtonIcon[ButtonSize(i)]->SetIcon(iconName);
		iconName = "media/textures/gui/" + themeName + "/common/buttons/" + sizeFolder + "/buttonHover.tga";
		m_pButtonIconHover[ButtonSize(i)]->SetIcon(iconName);
		iconName = "media/textures/gui/" + themeName + "/common/buttons/" + sizeFolder + "/buttonPressed.tga";
		m_pButtonIconPressed[ButtonSize(i)]->SetIcon(iconName);
		iconName = "media/textures/gui/" + themeName + "/common/buttons/" + sizeFolder + "/buttonDisabled.tga";
		m_pButtonIconDisabled[ButtonSize(i)]->SetIcon(iconName);
	}
}

// Setup icons for components
void FrontendManager::SetCheckboxIcons(CheckBox* pCheckBox)
{
	pCheckBox->SetDefaultIcon(GetCheckboxIcon());
	pCheckBox->SetHoverIcon(GetCheckboxIconHover());
	pCheckBox->SetSelectedIcon(GetCheckboxIconPressed());
	pCheckBox->SetDisabledIcon(GetCheckboxIconDisabled());
	pCheckBox->SetToggledIcon(GetCheckboxIconToggled());
	pCheckBox->SetToggledHoverIcon(GetCheckboxIconToggledHover());
	pCheckBox->SetToggledSelectedIcon(GetCheckboxIconToggledPressed());
	pCheckBox->SetToggledDisabledIcon(GetCheckboxIconToggledDisabled());
}

void FrontendManager::SetOptionboxIcons(OptionBox* pOptionBox)
{
	pOptionBox->SetDefaultIcon(GetOptionboxIcon());
	pOptionBox->SetHoverIcon(GetOptionboxIconHover());
	pOptionBox->SetSelectedIcon(GetOptionboxIconPressed());
	pOptionBox->SetDisabledIcon(GetOptionboxIconDisabled());
	pOptionBox->SetToggledIcon(GetOptionboxIconToggled());
	pOptionBox->SetToggledHoverIcon(GetOptionboxIconToggledHover());
	pOptionBox->SetToggledSelectedIcon(GetOptionboxIconToggledPressed());
	pOptionBox->SetToggledDisabledIcon(GetOptionboxIconToggledDisabled());
}

void FrontendManager::SetScrollbarIcons(ScrollBar* pScrollbar)
{
	pScrollbar->SetRightArrowDefaultIcon(GetScrollbarArrowDownIcon());
	pScrollbar->SetRightArrowHoverIcon(GetScrollbarArrowDownHoverIcon());
	pScrollbar->SetRightArrowSelectedIcon(GetScrollbarArrowDownPressedIcon());
	pScrollbar->SetRightArrowDisabledIcon(GetScrollbarArrowDownDisabledIcon());
	pScrollbar->SetLeftArrowDefaultIcon(GetScrollbarArrowUpIcon());
	pScrollbar->SetLeftArrowHoverIcon(GetScrollbarArrowUpHoverIcon());
	pScrollbar->SetLeftArrowSelectedIcon(GetScrollbarArrowUpPressedIcon());
	pScrollbar->SetLeftArrowDisabledIcon(GetScrollbarArrowUpDisabledIcon());
	pScrollbar->SetBackbarIcon(GetScrollbarBackgroundIcon());
	pScrollbar->SetScrollbarDefaultIcon(GetScrollbarIcon());
	pScrollbar->SetScrollbarHoverIcon(GetScrollbarHoverIcon());
	pScrollbar->SetScrollbarSelectedIcon(GetScrollbarPressedIcon());
	pScrollbar->SetScrollbarDisabledIcon(GetScrollbarDisabledIcon());
}

void FrontendManager::SetPulldownMenuIcons(PulldownMenu* pPulldownMenu)
{
	SetScrollbarIcons(pPulldownMenu->GetPulldownScrollBar());
	pPulldownMenu->SetPulldownIconDefault(GetScrollbarArrowDownIcon());
	pPulldownMenu->SetPulldownIconHover(GetScrollbarArrowDownHoverIcon());
	pPulldownMenu->SetPulldownIconPressed(GetScrollbarArrowDownPressedIcon());
	pPulldownMenu->SetPulldownIconDisabled(GetScrollbarArrowDownDisabledIcon());
}

void FrontendManager::SetButtonIcons(Button* pButton, ButtonSize size)
{
	pButton->SetDefaultIcon(GetButtonIcon(ButtonSize_85x25));
	pButton->SetHoverIcon(GetButtonIconHover(ButtonSize_85x25));
	pButton->SetSelectedIcon(GetButtonIconPressed(ButtonSize_85x25));
	pButton->SetDisabledIcon(GetButtonIconDisabled(ButtonSize_85x25));
}

// Updating
void FrontendManager::Update(float dt)
{

}

// Rendering
void FrontendManager::Render()
{

}
