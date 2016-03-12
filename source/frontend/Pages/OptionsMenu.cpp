// ******************************************************************************
//
// Filename:	OptionsMenu.cpp
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

#include "OptionsMenu.h"
#include "../../VoxGame.h"
#include "../FrontendManager.h"
#include "../../gui/openglgui.h"


OptionsMenu::OptionsMenu(Renderer* pRenderer, OpenGLGUI* pGUI, FrontendManager* pFrontPageManager, int windowWidth, int windowHeight)
	: FrontendPage(pRenderer, pGUI, pFrontPageManager, FrontendScreen_OptionsMenu, windowWidth, windowHeight)
{
	// Options Window
	m_pOptionsWindow = new GUIWindow(m_pRenderer, m_pFrontendManager->GetFrontendFont_Medium(), "Options");
	m_pOptionsWindow->AllowMoving(true);
	m_pOptionsWindow->AllowClosing(false);
	m_pOptionsWindow->AllowMinimizing(false);
	m_pOptionsWindow->AllowScrolling(false);
	m_pOptionsWindow->SetRenderTitleBar(true);
	m_pOptionsWindow->SetRenderWindowBackground(true);
	m_pOptionsWindow->SetApplicationDimensions(m_windowWidth, m_windowHeight);
	m_pOptionsWindow->Hide();

	m_pTitleBarIcon = new Icon(m_pRenderer, "media/textures/gui/Stonewash/OptionsMenu/options_title_icon.tga", 44, 44);
	m_pTitleBarIcon->SetDepth(4.0f);

	m_pOptionsWindowBackgroundIcon = new Icon(m_pRenderer, "media/textures/gui/Stonewash/OptionsMenu/options_window_background.tga", 600, 475);
	m_pOptionsWindowBackgroundIcon->SetDepth(1.0f);

	m_pTitleBarBackgroundIcon = new Icon(m_pRenderer, "media/textures/gui/Stonewash/OptionsMenu/titlebar_background.tga", 86, 35);
	m_pTitleBarBackgroundIcon->SetDepth(1.0f);

	m_pCloseExitButton = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_30(), m_pFrontendManager->GetFrontendFont_30_Outline(), "", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pCloseExitButton->SetDefaultIcon(m_pFrontendManager->GetCloseExitButtonIcon());
	m_pCloseExitButton->SetHoverIcon(m_pFrontendManager->GetCloseExitButtonIcon_Hover());
	m_pCloseExitButton->SetSelectedIcon(m_pFrontendManager->GetCloseExitButtonIcon_Pressed());
	m_pCloseExitButton->SetDisabledIcon(m_pFrontendManager->GetCloseExitButtonIcon());
	m_pCloseExitButton->SetLabelOffset(0, 5);
	m_pCloseExitButton->SetCallBackFunction(_CloseExitPressed);
	m_pCloseExitButton->SetCallBackData(this);

	m_pOptionsWindow->SetBackgroundIcon(m_pOptionsWindowBackgroundIcon);
	m_pOptionsWindow->SetTitlebarBackgroundIcon(m_pTitleBarBackgroundIcon);

	m_pOptionsWindow->AddComponent(m_pTitleBarIcon);
	m_pOptionsWindow->AddComponent(m_pCloseExitButton);

	// Tabs
	m_pOptionsModeController = new OptionController(m_pRenderer, m_pFrontendManager->GetFrontendFont_25(), "Mode");
	m_pOptionsModeController->SetDisplayLabel(false);
	m_pOptionsModeController->SetDisplayBorder(false);

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

	m_pControlsMode = new OptionBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_18(), m_pFrontendManager->GetFrontendFont_18_Outline(), "Controls", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pControlsMode->SetCallBackFunction(_ControlsTabPressed);
	m_pControlsMode->SetCallBackData(this);
	m_pControlsMode->SetDepth(2.3f);
	m_pControlsMode->SetPressedOffset(0, -2);
	//m_pControlsMode->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pControlsMode->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	m_pInterfaceMode = new OptionBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_18(), m_pFrontendManager->GetFrontendFont_18_Outline(), "Interface", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pInterfaceMode->SetCallBackFunction(_InterfaceTabPressed);
	m_pInterfaceMode->SetCallBackData(this);
	m_pInterfaceMode->SetDepth(2.4f);
	m_pInterfaceMode->SetPressedOffset(0, -2);
	//m_pInterfaceMode->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pInterfaceMode->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	m_pOptionsModeController->Add(m_pGameplayMode);
	m_pOptionsModeController->Add(m_pGraphicsMode);
	m_pOptionsModeController->Add(m_pSoundMode);
	m_pOptionsModeController->Add(m_pControlsMode);
	m_pOptionsModeController->Add(m_pInterfaceMode);	
	m_pGameplayMode->SetToggled(true);

	m_pOptionsWindow->AddComponent(m_pOptionsModeController);

	// Gameplay
	m_pInvertedMouseMode = new CheckBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_20(), m_pFrontendManager->GetFrontendFont_20_Outline(), "Inverted Mouse", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	//m_pInvertedMouseMode->SetCallBackFunction(_InvertedMousePressed);
	//m_pInvertedMouseMode->SetCallBackData(this);
	m_pInvertedMouseMode->SetDisplayLabel(true);
	m_pInvertedMouseMode->SetDepth(2.0f);
	m_pInvertedMouseMode->SetPressedOffset(0, -2);
	//m_pInvertedMouseMode->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pInvertedMouseMode->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	m_pNormalCamOption = new OptionBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_20(), m_pFrontendManager->GetFrontendFont_20_Outline(), "Normal Cam", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	//m_pNormalCamOption->SetDefaultIcon(m_pFrontendManager->GetRadioOptionIcon());
	//m_pNormalCamOption->SetHoverIcon(m_pFrontendManager->GetRadioOptionIcon_Hover());
	//m_pNormalCamOption->SetSelectedIcon(m_pFrontendManager->GetRadioOptionIcon_Pressed());
	//m_pNormalCamOption->SetDisabledIcon(m_pFrontendManager->GetRadioOptionIcon());
	//m_pNormalCamOption->SetToggledIcon(m_pFrontendManager->GetRadioOptionIcon_Toggled());
	//m_pNormalCamOption->SetToggledHoverIcon(m_pFrontendManager->GetRadioOptionIcon_Toggled_Hover());
	//m_pNormalCamOption->SetToggledSelectedIcon(m_pFrontendManager->GetRadioOptionIcon_Toggled_Pressed());
	//m_pNormalCamOption->SetToggledDisabledIcon(m_pFrontendManager->GetRadioOptionIcon());
	//m_pNormalCamOption->SetCallBackFunction(_SwitchToNormalCam);
	//m_pNormalCamOption->SetCallBackData(this);
	m_pNormalCamOption->SetDisplayLabel(true);
	m_pNormalCamOption->SetPressedOffset(0, -2);
	//m_pNormalCamOption->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pNormalCamOption->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());
	m_pFollowCamOption = new OptionBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_20(), m_pFrontendManager->GetFrontendFont_20_Outline(), "Follow Cam", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	//m_pFollowCamOption->SetDefaultIcon(m_pFrontendManager->GetRadioOptionIcon());
	//m_pFollowCamOption->SetHoverIcon(m_pFrontendManager->GetRadioOptionIcon_Hover());
	//m_pFollowCamOption->SetSelectedIcon(m_pFrontendManager->GetRadioOptionIcon_Pressed());
	//m_pFollowCamOption->SetDisabledIcon(m_pFrontendManager->GetRadioOptionIcon());
	//m_pFollowCamOption->SetToggledIcon(m_pFrontendManager->GetRadioOptionIcon_Toggled());
	//m_pFollowCamOption->SetToggledHoverIcon(m_pFrontendManager->GetRadioOptionIcon_Toggled_Hover());
	//m_pFollowCamOption->SetToggledSelectedIcon(m_pFrontendManager->GetRadioOptionIcon_Toggled_Pressed());
	//m_pFollowCamOption->SetToggledDisabledIcon(m_pFrontendManager->GetRadioOptionIcon());
	//m_pFollowCamOption->SetCallBackFunction(_SwitchToFollowCam);
	//m_pFollowCamOption->SetCallBackData(this);
	m_pFollowCamOption->SetDisplayLabel(true);
	m_pFollowCamOption->SetPressedOffset(0, -2);
	//m_pFollowCamOption->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pFollowCamOption->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());
	m_pCameraSelector = new OptionController(m_pRenderer, m_pFrontendManager->GetFrontendFont_25(), m_pFrontendManager->GetFrontendFont_25_Outline(), "Camera Mode", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pCameraSelector->SetDisplayLabel(true);
	m_pCameraSelector->SetDisplayBorder(true);
	m_pCameraSelector->SetDepth(2.0f);
	m_pCameraSelector->Add(m_pNormalCamOption);
	m_pCameraSelector->Add(m_pFollowCamOption);
	m_pNormalCamOption->SetToggled(true);

	m_pMouseSensativitySlider = new Slider(m_pRenderer, m_pFrontendManager->GetFrontendFont_20(), m_pFrontendManager->GetFrontendFont_20_Outline(), "Mouse Sensitivity", 5.0f, 100.0f, 45.0f, Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pMouseSensativitySlider->SetSliderParams(5, 10, ESliderDirection_Horizontal);
	//m_pMouseSensativitySlider->SetVaueChangedCallBackFunction(_MouseSensativityChanged);
	//m_pMouseSensativitySlider->SetVaueChangedCallBackData(this);
	//m_pMouseSensativitySlider->SetScrollerDefaultIcon(m_pFrontendManager->GetSliderIconDefault());
	//m_pMouseSensativitySlider->SetScrollerHoverIcon(m_pFrontendManager->GetSliderIconHover());
	//m_pMouseSensativitySlider->SetScrollerDraggingIcon(m_pFrontendManager->GetSliderIconDrag());
	m_pMouseSensativitySlider->SetLabelPositionOffset(0, 14);
	m_pMouseSensativitySlider->SetValuePrecision(0);
	m_pMouseSensativitySlider->SetRenderBackgroundBar(false);
	m_pMouseSensativitySlider->SetDepth(2.0f);

	m_pGamepadSensativitySlider = new Slider(m_pRenderer, m_pFrontendManager->GetFrontendFont_20(), m_pFrontendManager->GetFrontendFont_20_Outline(), "Gamepad Sensitivity", 5.0f, 100.0f, 20.0f, Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pGamepadSensativitySlider->SetSliderParams(5, 10, ESliderDirection_Horizontal);
	//m_pGamepadSensativitySlider->SetVaueChangedCallBackFunction(_GamepadSensativityChanged);
	//m_pGamepadSensativitySlider->SetVaueChangedCallBackData(this);
	//m_pGamepadSensativitySlider->SetScrollerDefaultIcon(m_pFrontendManager->GetSliderIconDefault());
	//m_pGamepadSensativitySlider->SetScrollerHoverIcon(m_pFrontendManager->GetSliderIconHover());
	//m_pGamepadSensativitySlider->SetScrollerDraggingIcon(m_pFrontendManager->GetSliderIconDrag());
	m_pGamepadSensativitySlider->SetLabelPositionOffset(0, 14);
	m_pGamepadSensativitySlider->SetValuePrecision(0);
	m_pGamepadSensativitySlider->SetRenderBackgroundBar(false);
	m_pGamepadSensativitySlider->SetDepth(2.0f);

	m_pSliderBackgroundIcon_MouseSensativity = new Icon(m_pRenderer, "media/textures/gui/Stonewash/OptionsMenu/slider_background.tga", 207, 31);
	m_pSliderBackgroundIcon_MouseSensativity->SetDepth(1.5f);
	m_pSliderBackgroundIcon_GamepadSensativity = new Icon(m_pRenderer, "media/textures/gui/Stonewash/OptionsMenu/slider_background.tga", 207, 31);
	m_pSliderBackgroundIcon_GamepadSensativity->SetDepth(1.5f);

	m_vpGameplayComponents.push_back(m_pInvertedMouseMode);
	m_vpGameplayComponents.push_back(m_pCameraSelector);
	m_vpGameplayComponents.push_back(m_pMouseSensativitySlider);
	m_vpGameplayComponents.push_back(m_pGamepadSensativitySlider);
	m_vpGameplayComponents.push_back(m_pSliderBackgroundIcon_MouseSensativity);
	m_vpGameplayComponents.push_back(m_pSliderBackgroundIcon_GamepadSensativity);

	// Graphics

	// Sound
	m_pSoundEffects = new CheckBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_20(), m_pFrontendManager->GetFrontendFont_20_Outline(), "Sound", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	//m_pSoundEffects->SetDefaultIcon(m_pFrontendManager->GetCheckboxIcon());
	//m_pSoundEffects->SetHoverIcon(m_pFrontendManager->GetCheckboxIcon_Hover());
	//m_pSoundEffects->SetSelectedIcon(m_pFrontendManager->GetCheckboxIcon_Pressed());
	//m_pSoundEffects->SetToggledIcon(m_pFrontendManager->GetCheckboxIcon_Toggled());
	//m_pSoundEffects->SetToggledHoverIcon(m_pFrontendManager->GetCheckboxIcon_Toggled_Hover());
	//m_pSoundEffects->SetToggledSelectedIcon(m_pFrontendManager->GetCheckboxIcon_Toggled_Pressed());
	//m_pSoundEffects->SetCallBackFunction(_SoundEffectsPressed);
	//m_pSoundEffects->SetCallBackData(this);
	m_pSoundEffects->SetDisplayLabel(true);
	m_pSoundEffects->SetDepth(2.0f);
	m_pSoundEffects->SetPressedOffset(0, -2);
	//m_pSoundEffects->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pSoundEffects->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	m_pSoundEffectsVolume = new Slider(m_pRenderer, m_pFrontendManager->GetFrontendFont_20(), m_pFrontendManager->GetFrontendFont_20_Outline(), "", 0.0f, 1.0f, 0.5f, Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pSoundEffectsVolume->SetSliderParams(5, 10, ESliderDirection_Horizontal);
	//m_pSoundEffectsVolume->SetVaueChangedCallBackFunction(_SoundEffectsChanged);
	//m_pSoundEffectsVolume->SetVaueChangedCallBackData(this);
	//m_pSoundEffectsVolume->SetScrollerDefaultIcon(m_pFrontendManager->GetSliderIconDefault());
	//m_pSoundEffectsVolume->SetScrollerHoverIcon(m_pFrontendManager->GetSliderIconHover());
	//m_pSoundEffectsVolume->SetScrollerDraggingIcon(m_pFrontendManager->GetSliderIconDrag());
	m_pSoundEffectsVolume->SetRenderBackgroundBar(false);
	m_pSoundEffectsVolume->SetDepth(2.0f);

	m_pMusic = new CheckBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_20(), m_pFrontendManager->GetFrontendFont_20_Outline(), "Music", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	//m_pMusic->SetDefaultIcon(m_pFrontendManager->GetCheckboxIcon());
	//m_pMusic->SetHoverIcon(m_pFrontendManager->GetCheckboxIcon_Hover());
	//m_pMusic->SetSelectedIcon(m_pFrontendManager->GetCheckboxIcon_Pressed());
	//m_pMusic->SetToggledIcon(m_pFrontendManager->GetCheckboxIcon_Toggled());
	//m_pMusic->SetToggledHoverIcon(m_pFrontendManager->GetCheckboxIcon_Toggled_Hover());
	//m_pMusic->SetToggledSelectedIcon(m_pFrontendManager->GetCheckboxIcon_Toggled_Pressed());
	//m_pMusic->SetCallBackFunction(_MusicPressed);
	//m_pMusic->SetCallBackData(this);
	m_pMusic->SetDisplayLabel(true);
	m_pMusic->SetDepth(2.0f);
	m_pMusic->SetPressedOffset(0, -2);
	//m_pMusic->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pMusic->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	m_pMusicVolume = new Slider(m_pRenderer, m_pFrontendManager->GetFrontendFont_20(), m_pFrontendManager->GetFrontendFont_20_Outline(), "", 0.0f, 1.0f, 0.5f, Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	m_pMusicVolume->SetSliderParams(5, 10, ESliderDirection_Horizontal);
	//m_pMusicVolume->SetVaueChangedCallBackFunction(_MusicVolumeChanged);
	//m_pMusicVolume->SetVaueChangedCallBackData(this);
	//m_pMusicVolume->SetScrollerDefaultIcon(m_pFrontendManager->GetSliderIconDefault());
	//m_pMusicVolume->SetScrollerHoverIcon(m_pFrontendManager->GetSliderIconHover());
	//m_pMusicVolume->SetScrollerDraggingIcon(m_pFrontendManager->GetSliderIconDrag());
	m_pMusicVolume->SetRenderBackgroundBar(false);
	m_pMusicVolume->SetDepth(2.0f);

	m_pSliderBackgroundIcon_Sound = new Icon(m_pRenderer, "media/textures/gui/Stonewash/OptionsMenu/slider_background.tga", 207, 31);
	m_pSliderBackgroundIcon_Sound->SetDepth(1.5f);
	m_pSliderBackgroundIcon_Music = new Icon(m_pRenderer, "media/textures/gui/Stonewash/OptionsMenu/slider_background.tga", 207, 31);
	m_pSliderBackgroundIcon_Music->SetDepth(1.5f);

	m_vpSoundComponents.push_back(m_pSoundEffects);
	m_vpSoundComponents.push_back(m_pSoundEffectsVolume);
	m_vpSoundComponents.push_back(m_pMusic);
	m_vpSoundComponents.push_back(m_pMusicVolume);
	m_vpSoundComponents.push_back(m_pSliderBackgroundIcon_Sound);
	m_vpSoundComponents.push_back(m_pSliderBackgroundIcon_Music);

	// Interface
	m_pRenderGUI = new CheckBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_20(), m_pFrontendManager->GetFrontendFont_20_Outline(), "Render GUI", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	//m_pRenderGUI->SetDefaultIcon(m_pFrontendManager->GetCheckboxIcon());
	//m_pRenderGUI->SetHoverIcon(m_pFrontendManager->GetCheckboxIcon_Hover());
	//m_pRenderGUI->SetSelectedIcon(m_pFrontendManager->GetCheckboxIcon_Pressed());
	//m_pRenderGUI->SetToggledIcon(m_pFrontendManager->GetCheckboxIcon_Toggled());
	//m_pRenderGUI->SetToggledHoverIcon(m_pFrontendManager->GetCheckboxIcon_Toggled_Hover());
	//m_pRenderGUI->SetToggledSelectedIcon(m_pFrontendManager->GetCheckboxIcon_Toggled_Pressed());
	//m_pRenderGUI->SetCallBackFunction(_RenderGUIPressed);
	//m_pRenderGUI->SetCallBackData(this);
	m_pRenderGUI->SetDisplayLabel(true);
	m_pRenderGUI->SetDepth(2.0f);
	m_pRenderGUI->SetPressedOffset(0, -2);
	//m_pRenderGUI->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pRenderGUI->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	m_pRenderCrosshair = new CheckBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_20(), m_pFrontendManager->GetFrontendFont_20_Outline(), "Render Crosshair", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	//m_pRenderCrosshair->SetDefaultIcon(m_pFrontendManager->GetCheckboxIcon());
	//m_pRenderCrosshair->SetHoverIcon(m_pFrontendManager->GetCheckboxIcon_Hover());
	//m_pRenderCrosshair->SetSelectedIcon(m_pFrontendManager->GetCheckboxIcon_Pressed());
	//m_pRenderCrosshair->SetToggledIcon(m_pFrontendManager->GetCheckboxIcon_Toggled());
	//m_pRenderCrosshair->SetToggledHoverIcon(m_pFrontendManager->GetCheckboxIcon_Toggled_Hover());
	//m_pRenderCrosshair->SetToggledSelectedIcon(m_pFrontendManager->GetCheckboxIcon_Toggled_Pressed());
	//m_pRenderCrosshair->SetCallBackFunction(_RenderCrosshairPressed);
	//m_pRenderCrosshair->SetCallBackData(this);
	m_pRenderCrosshair->SetDisplayLabel(true);
	m_pRenderCrosshair->SetDepth(2.0f);
	m_pRenderCrosshair->SetPressedOffset(0, -2);
	//m_pRenderCrosshair->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pRenderCrosshair->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	m_pHUDAnimations = new CheckBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_20(), m_pFrontendManager->GetFrontendFont_20_Outline(), "HUD Animations", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	//m_pHUDAnimations->SetDefaultIcon(m_pFrontendManager->GetCheckboxIcon());
	//m_pHUDAnimations->SetHoverIcon(m_pFrontendManager->GetCheckboxIcon_Hover());
	//m_pHUDAnimations->SetSelectedIcon(m_pFrontendManager->GetCheckboxIcon_Pressed());
	//m_pHUDAnimations->SetToggledIcon(m_pFrontendManager->GetCheckboxIcon_Toggled());
	//m_pHUDAnimations->SetToggledHoverIcon(m_pFrontendManager->GetCheckboxIcon_Toggled_Hover());
	//m_pHUDAnimations->SetToggledSelectedIcon(m_pFrontendManager->GetCheckboxIcon_Toggled_Pressed());
	//m_pHUDAnimations->SetCallBackFunction(_HUDAnimationsPressed);
	//m_pHUDAnimations->SetCallBackData(this);
	m_pHUDAnimations->SetDisplayLabel(true);
	m_pHUDAnimations->SetDepth(2.0f);
	m_pHUDAnimations->SetPressedOffset(0, -2);
	//m_pHUDAnimations->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pHUDAnimations->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	m_pCustomCursors = new CheckBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_20(), m_pFrontendManager->GetFrontendFont_20_Outline(), "Custom Cursors", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	//m_pCustomCursors->SetDefaultIcon(m_pFrontendManager->GetCheckboxIcon());
	//m_pCustomCursors->SetHoverIcon(m_pFrontendManager->GetCheckboxIcon_Hover());
	//m_pCustomCursors->SetSelectedIcon(m_pFrontendManager->GetCheckboxIcon_Pressed());
	//m_pCustomCursors->SetToggledIcon(m_pFrontendManager->GetCheckboxIcon_Toggled());
	//m_pCustomCursors->SetToggledHoverIcon(m_pFrontendManager->GetCheckboxIcon_Toggled_Hover());
	//m_pCustomCursors->SetToggledSelectedIcon(m_pFrontendManager->GetCheckboxIcon_Toggled_Pressed());
	//m_pCustomCursors->SetCallBackFunction(_HUDAnimationsPressed);
	//m_pCustomCursors->SetCallBackData(this);
	m_pCustomCursors->SetDisplayLabel(true);
	m_pCustomCursors->SetDepth(2.0f);
	m_pCustomCursors->SetPressedOffset(0, -2);
	//m_pCustomCursors->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pCustomCursors->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	m_pEquipHoverIcon = new CheckBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_20(), m_pFrontendManager->GetFrontendFont_20_Outline(), "Equip Helper Cursor", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	//m_pEquipHoverIcon->SetDefaultIcon(m_pFrontendManager->GetCheckboxIcon());
	//m_pEquipHoverIcon->SetHoverIcon(m_pFrontendManager->GetCheckboxIcon_Hover());
	//m_pEquipHoverIcon->SetSelectedIcon(m_pFrontendManager->GetCheckboxIcon_Pressed());
	//m_pEquipHoverIcon->SetToggledIcon(m_pFrontendManager->GetCheckboxIcon_Toggled());
	//m_pEquipHoverIcon->SetToggledHoverIcon(m_pFrontendManager->GetCheckboxIcon_Toggled_Hover());
	//m_pEquipHoverIcon->SetToggledSelectedIcon(m_pFrontendManager->GetCheckboxIcon_Toggled_Pressed());
	//m_pEquipHoverIcon->SetCallBackFunction(_HUDAnimationsPressed);
	//m_pEquipHoverIcon->SetCallBackData(this);
	m_pEquipHoverIcon->SetDisplayLabel(true);
	m_pEquipHoverIcon->SetDepth(2.0f);
	m_pEquipHoverIcon->SetPressedOffset(0, -2);
	//m_pEquipHoverIcon->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pEquipHoverIcon->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	m_pToolTipAppearDelay = new CheckBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_20(), m_pFrontendManager->GetFrontendFont_20_Outline(), "Tooltip Delay", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	//m_pToolTipAppearDelay->SetDefaultIcon(m_pFrontendManager->GetCheckboxIcon());
	//m_pToolTipAppearDelay->SetHoverIcon(m_pFrontendManager->GetCheckboxIcon_Hover());
	//m_pToolTipAppearDelay->SetSelectedIcon(m_pFrontendManager->GetCheckboxIcon_Pressed());
	//m_pToolTipAppearDelay->SetToggledIcon(m_pFrontendManager->GetCheckboxIcon_Toggled());
	//m_pToolTipAppearDelay->SetToggledHoverIcon(m_pFrontendManager->GetCheckboxIcon_Toggled_Hover());
	//m_pToolTipAppearDelay->SetToggledSelectedIcon(m_pFrontendManager->GetCheckboxIcon_Toggled_Pressed());
	//m_pToolTipAppearDelay->SetCallBackFunction(_HUDAnimationsPressed);
	//m_pToolTipAppearDelay->SetCallBackData(this);
	m_pToolTipAppearDelay->SetDisplayLabel(true);
	m_pToolTipAppearDelay->SetDepth(2.0f);
	m_pToolTipAppearDelay->SetPressedOffset(0, -2);
	//m_pToolTipAppearDelay->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pToolTipAppearDelay->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	m_pConfirmDeleteItem = new CheckBox(m_pRenderer, m_pFrontendManager->GetFrontendFont_20(), m_pFrontendManager->GetFrontendFont_20_Outline(), "Confirm Item Deletion", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	//m_pConfirmDeleteItem->SetDefaultIcon(m_pFrontendManager->GetCheckboxIcon());
	//m_pConfirmDeleteItem->SetHoverIcon(m_pFrontendManager->GetCheckboxIcon_Hover());
	//m_pConfirmDeleteItem->SetSelectedIcon(m_pFrontendManager->GetCheckboxIcon_Pressed());
	//m_pConfirmDeleteItem->SetToggledIcon(m_pFrontendManager->GetCheckboxIcon_Toggled());
	//m_pConfirmDeleteItem->SetToggledHoverIcon(m_pFrontendManager->GetCheckboxIcon_Toggled_Hover());
	//m_pConfirmDeleteItem->SetToggledSelectedIcon(m_pFrontendManager->GetCheckboxIcon_Toggled_Pressed());
	//m_pConfirmDeleteItem->SetCallBackFunction(_HUDAnimationsPressed);
	//m_pConfirmDeleteItem->SetCallBackData(this);
	m_pConfirmDeleteItem->SetDisplayLabel(true);
	m_pConfirmDeleteItem->SetDepth(2.0f);
	m_pConfirmDeleteItem->SetPressedOffset(0, -2);
	//m_pConfirmDeleteItem->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pConfirmDeleteItem->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	m_pResetGUIToDefault = new Button(m_pRenderer, m_pFrontendManager->GetFrontendFont_25(), m_pFrontendManager->GetFrontendFont_25_Outline(), "Reset GUI", Colour(1.0f, 1.0f, 1.0f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f));
	//m_pResetGUIToDefault->SetDefaultIcon(m_pFrontendManager->GetButtonIcon_Default_115());
	//m_pResetGUIToDefault->SetHoverIcon(m_pFrontendManager->GetButtonIcon_Hover_115());
	//m_pResetGUIToDefault->SetSelectedIcon(m_pFrontendManager->GetButtonIcon_Pressed_115());
	//m_pResetGUIToDefault->SetDisabledIcon(m_pFrontendManager->GetButtonIcon_Disabled_115());
	m_pResetGUIToDefault->SetLabelOffset(0, 3);
	//m_pResetGUIToDefault->SetCallBackFunction(_ResetGUIPressed);
	//m_pResetGUIToDefault->SetCallBackData(this);;
	m_pResetGUIToDefault->SetPressedOffset(0, -2);
	//m_pResetGUIToDefault->SetHoverLabelColour(m_pFrontendManager->GetHoverFontColour());
	//m_pResetGUIToDefault->SetPressedLabelColour(m_pFrontendManager->GetPressedFontColour());

	m_vpInterfaceComponents.push_back(m_pRenderGUI);
	m_vpInterfaceComponents.push_back(m_pRenderCrosshair);
	m_vpInterfaceComponents.push_back(m_pHUDAnimations);
	m_vpInterfaceComponents.push_back(m_pCustomCursors);
	m_vpInterfaceComponents.push_back(m_pEquipHoverIcon);
	m_vpInterfaceComponents.push_back(m_pToolTipAppearDelay);
	m_vpInterfaceComponents.push_back(m_pConfirmDeleteItem);
	m_vpInterfaceComponents.push_back(m_pResetGUIToDefault);

	// Controls

	SetWindowDimensions(m_windowWidth, m_windowHeight);

	m_returnToMainMenu = false;
}

OptionsMenu::~OptionsMenu()
{
	// Window
	delete m_pOptionsWindow;
	delete m_pTitleBarIcon;
	delete m_pTitleBarBackgroundIcon;
	delete m_pOptionsWindowBackgroundIcon;
	delete m_pCloseExitButton;

	// Tabs
	delete m_pOptionsModeController;
	delete m_pGameplayMode;
	delete m_pGraphicsMode;
	delete m_pSoundMode;
	delete m_pInterfaceMode;
	delete m_pControlsMode;

	// Gameplay
	delete m_pInvertedMouseMode;
	delete m_pNormalCamOption;
	delete m_pFollowCamOption;
	delete m_pCameraSelector;
	delete m_pMouseSensativitySlider;
	delete m_pGamepadSensativitySlider;
	delete m_pSliderBackgroundIcon_MouseSensativity;
	delete m_pSliderBackgroundIcon_GamepadSensativity;

	// Graphics

	// Sound
	delete m_pSoundEffects;
	delete m_pSoundEffectsVolume;
	delete m_pMusic;
	delete m_pMusicVolume;
	delete m_pSliderBackgroundIcon_Sound;
	delete m_pSliderBackgroundIcon_Music;

	// Interface
	delete m_pRenderGUI;
	delete m_pRenderCrosshair;
	delete m_pHUDAnimations;
	delete m_pCustomCursors;
	delete m_pEquipHoverIcon;
	delete m_pToolTipAppearDelay;
	delete m_pConfirmDeleteItem;
	delete m_pResetGUIToDefault;

	// Controls
}

void OptionsMenu::Reset()
{
}

void OptionsMenu::SetWindowDimensions(int windowWidth, int windowHeight)
{
	FrontendPage::SetWindowDimensions(windowWidth, windowHeight);

	m_optionsWindowWidth = 600;
	m_optionsWindowHeight = 475;
	m_titlebarWidth = 135;
	m_titlebarHeight = 35;

	// Window
	m_pOptionsWindow->SetDimensions((int)((m_windowWidth*0.5f) - (m_optionsWindowWidth*0.5f)), (int)((m_windowHeight*0.5f) - (m_optionsWindowHeight*0.5f)), m_optionsWindowWidth, m_optionsWindowHeight);
	m_pOptionsWindow->SetTitleBarDimensions(0, 0, m_titlebarWidth, m_titlebarHeight);
	m_pOptionsWindow->SetTitleOffset(50, 5);
	m_pOptionsWindow->SetApplicationDimensions(m_windowWidth, m_windowHeight);
	m_pOptionsWindow->SetApplicationBorder(25, 15, 10, 10);

	m_pTitleBarIcon->SetDimensions(0, m_optionsWindowHeight, 44, 44);
	m_pCloseExitButton->SetDimensions(m_optionsWindowWidth-32, m_optionsWindowHeight, 32, 32);

	// Tabs
	m_pOptionsModeController->SetDimensions(0, m_optionsWindowHeight, m_optionsWindowWidth, 32);
	m_pGameplayMode->SetDisplayLabel(false);
	m_pGameplayMode->SetDimensions(m_optionsWindowWidth-32-375, 0, 75, 32);
	m_pGameplayMode->SetDisplayLabel(true);
	int textWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_18_Outline(), "%s", m_pGameplayMode->GetLabel().GetText().c_str());
	m_pGameplayMode->SetLabelPosition((int)(75*0.5f-textWidth*0.5f), 8);

	m_pGraphicsMode->SetDisplayLabel(false);
	m_pGraphicsMode->SetDimensions(m_optionsWindowWidth-32-300, 0, 75, 32);
	m_pGraphicsMode->SetDisplayLabel(true);
	textWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_18_Outline(), "%s", m_pGraphicsMode->GetLabel().GetText().c_str());
	m_pGraphicsMode->SetLabelPosition((int)(75*0.5f-textWidth*0.5f), 8);

	m_pSoundMode->SetDisplayLabel(false);
	m_pSoundMode->SetDimensions(m_optionsWindowWidth-32-225, 0, 75, 32);
	m_pSoundMode->SetDisplayLabel(true);
	textWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_18_Outline(), "%s", m_pSoundMode->GetLabel().GetText().c_str());
	m_pSoundMode->SetLabelPosition((int)(75*0.5f-textWidth*0.5f), 8);

	m_pInterfaceMode->SetDisplayLabel(false);
	m_pInterfaceMode->SetDimensions(m_optionsWindowWidth-32-150, 0, 75, 32);
	m_pInterfaceMode->SetDisplayLabel(true);
	textWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_18_Outline(), "%s", m_pInterfaceMode->GetLabel().GetText().c_str());
	m_pInterfaceMode->SetLabelPosition((int)(75*0.5f-textWidth*0.5f), 8);

	m_pControlsMode->SetDisplayLabel(false);
	m_pControlsMode->SetDimensions(m_optionsWindowWidth-32-75, 0, 75, 32);
	m_pControlsMode->SetDisplayLabel(true);
	textWidth = m_pRenderer->GetFreeTypeTextWidth(m_pFrontendManager->GetFrontendFont_18_Outline(), "%s", m_pControlsMode->GetLabel().GetText().c_str());
	m_pControlsMode->SetLabelPosition((int)(75*0.5f-textWidth*0.5f), 8);

	// Gameplay
	m_pInvertedMouseMode->SetDimensions(25, m_optionsWindowHeight-65, 32, 32);

	m_pNormalCamOption->SetDimensions(5, 43, 32, 32);
	m_pFollowCamOption->SetDimensions(5, 7, 32, 32);
	m_pCameraSelector->SetDimensions(25, m_optionsWindowHeight-190, 140, 80);

	m_pMouseSensativitySlider->SetDimensions(135, m_optionsWindowHeight-265, 171, 24);
	m_pMouseSensativitySlider->SetCurrentValue(m_pMouseSensativitySlider->GetCurrentValue(), true);
	m_pMouseSensativitySlider->SetLabelPositionOffset(0, 30);

	m_pGamepadSensativitySlider->SetDimensions(135, m_optionsWindowHeight-300, 171, 24);
	m_pGamepadSensativitySlider->SetCurrentValue(m_pGamepadSensativitySlider->GetCurrentValue(), true);
	m_pGamepadSensativitySlider->SetLabelPositionOffset(0, 30);

	m_pSliderBackgroundIcon_MouseSensativity->SetDimensions(131, m_optionsWindowHeight-268, 207, 31);
	m_pSliderBackgroundIcon_GamepadSensativity->SetDimensions(131, m_optionsWindowHeight-303, 207, 31);

	// Graphics

	// Sound
	m_pSoundEffects->SetDimensions(25, m_optionsWindowHeight-65, 32, 32);
	m_pSoundEffectsVolume->SetDimensions(115, m_optionsWindowHeight-65, 171, 24);
	m_pSoundEffectsVolume->SetCurrentValue(m_pSoundEffectsVolume->GetCurrentValue(), true);

	m_pMusic->SetDimensions(25, m_optionsWindowHeight-105, 32, 32);
	m_pMusicVolume->SetDimensions(115, m_optionsWindowHeight-105, 171, 24);
	m_pMusicVolume->SetCurrentValue(m_pMusicVolume->GetCurrentValue(), true);

	m_pSliderBackgroundIcon_Sound->SetDimensions(111, m_optionsWindowHeight-68, 207, 31);
	m_pSliderBackgroundIcon_Music->SetDimensions(111, m_optionsWindowHeight-108, 207, 31);

	// Interface
	m_pRenderGUI->SetDimensions(25, m_optionsWindowHeight-65, 32, 32);
	m_pRenderCrosshair->SetDimensions(25, m_optionsWindowHeight-105, 32, 32);
	m_pHUDAnimations->SetDimensions(25, m_optionsWindowHeight-145, 32, 32);
	m_pCustomCursors->SetDimensions(25, m_optionsWindowHeight-185, 32, 32);
	m_pEquipHoverIcon->SetDimensions(25, m_optionsWindowHeight-225, 32, 32);
	m_pToolTipAppearDelay->SetDimensions(25, m_optionsWindowHeight-265, 32, 32);
	m_pConfirmDeleteItem->SetDimensions(25, m_optionsWindowHeight-305, 32, 32);
	m_pResetGUIToDefault->SetDimensions(25, m_optionsWindowHeight-375, 120, 45);

	// Controls
}

void OptionsMenu::SetReturnToMainMenu(bool mainMenu)
{
	m_returnToMainMenu = mainMenu;
}

void OptionsMenu::SkinGUI()
{

}

void OptionsMenu::UnSkinGUI()
{

}

void OptionsMenu::Load()
{
	m_pGameplayMode->SetToggled(true);
	GameplayTabPressed();

	m_loaded = true;
}

void OptionsMenu::Unload()
{
	// Remove ALL tab sections
	for(int i = 0; i < m_vpGameplayComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpGameplayComponents[i]);
	}
	for(int i = 0; i < m_vpGraphicsComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpGraphicsComponents[i]);
	}
	for(int i = 0; i < m_vpSoundComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpSoundComponents[i]);
	}
	for(int i = 0; i < m_vpControlsComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpControlsComponents[i]);
	}
	for(int i = 0; i < m_vpInterfaceComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpInterfaceComponents[i]);
	}

	m_pGUI->RemoveWindow(m_pOptionsWindow);

	m_loaded = false;
}

void OptionsMenu::Update(float dt)
{
	FrontendPage::Update(dt);

	VoxGame::GetInstance()->SetGlobalBlurAmount(0.00125f);
}

void OptionsMenu::Render()
{
	FrontendPage::Render();
}

void OptionsMenu::Render2D()
{
	FrontendPage::Render2D();
}

void OptionsMenu::_CloseExitPressed(void *pData)
{
	OptionsMenu* lpOptionsMenu = (OptionsMenu*)pData;
	lpOptionsMenu->CloseExitPressed();
}

void OptionsMenu::CloseExitPressed()
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

void OptionsMenu::_GameplayTabPressed(void *pData)
{
	OptionsMenu* lpOptionsMenu = (OptionsMenu*)pData;
	lpOptionsMenu->GameplayTabPressed();
}

void OptionsMenu::GameplayTabPressed()
{
	if(m_loaded)
	{
		m_pGUI->RemoveWindow(m_pOptionsWindow);
	}

	// Remove
	for(int i = 0; i < m_vpGameplayComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpGameplayComponents[i]);
	}
	for(int i = 0; i < m_vpGraphicsComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpGraphicsComponents[i]);
	}
	for(int i = 0; i < m_vpSoundComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpSoundComponents[i]);
	}
	for(int i = 0; i < m_vpControlsComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpControlsComponents[i]);
	}
	for(int i = 0; i < m_vpInterfaceComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpInterfaceComponents[i]);
	}

	// Add
	for(int i = 0; i < m_vpGameplayComponents.size(); i++)
	{
		m_pOptionsWindow->AddComponent(m_vpGameplayComponents[i]);
	}

	m_pGUI->AddWindow(m_pOptionsWindow);
	m_pOptionsWindow->Show();
}

void OptionsMenu::_GraphicsTabPressed(void *pData)
{
	OptionsMenu* lpOptionsMenu = (OptionsMenu*)pData;
	lpOptionsMenu->GraphicsTabPressed();
}

void OptionsMenu::GraphicsTabPressed()
{
	if(m_loaded)
	{
		m_pGUI->RemoveWindow(m_pOptionsWindow);
	}

	// Remove
	for(int i = 0; i < m_vpGameplayComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpGameplayComponents[i]);
	}
	for(int i = 0; i < m_vpGraphicsComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpGraphicsComponents[i]);
	}
	for(int i = 0; i < m_vpSoundComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpSoundComponents[i]);
	}
	for(int i = 0; i < m_vpControlsComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpControlsComponents[i]);
	}
	for(int i = 0; i < m_vpInterfaceComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpInterfaceComponents[i]);
	}

	// Add
	for(int i = 0; i < m_vpGraphicsComponents.size(); i++)
	{
		m_pOptionsWindow->AddComponent(m_vpGraphicsComponents[i]);
	}

	m_pGUI->AddWindow(m_pOptionsWindow);
	m_pOptionsWindow->Show();
}

void OptionsMenu::_SoundTabPressed(void *pData)
{
	OptionsMenu* lpOptionsMenu = (OptionsMenu*)pData;
	lpOptionsMenu->SoundTabPressed();
}

void OptionsMenu::SoundTabPressed()
{
	if(m_loaded)
	{
		m_pGUI->RemoveWindow(m_pOptionsWindow);
	}

	// Remove
	for(int i = 0; i < m_vpGameplayComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpGameplayComponents[i]);
	}
	for(int i = 0; i < m_vpGraphicsComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpGraphicsComponents[i]);
	}
	for(int i = 0; i < m_vpSoundComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpSoundComponents[i]);
	}
	for(int i = 0; i < m_vpControlsComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpControlsComponents[i]);
	}
	for(int i = 0; i < m_vpInterfaceComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpInterfaceComponents[i]);
	}

	// Add
	for(int i = 0; i < m_vpSoundComponents.size(); i++)
	{
		m_pOptionsWindow->AddComponent(m_vpSoundComponents[i]);
	}

	m_pGUI->AddWindow(m_pOptionsWindow);
	m_pOptionsWindow->Show();
}

void OptionsMenu::_InterfaceTabPressed(void *pData)
{
	OptionsMenu* lpOptionsMenu = (OptionsMenu*)pData;
	lpOptionsMenu->InterfaceTabPressed();
}

void OptionsMenu::InterfaceTabPressed()
{
	if(m_loaded)
	{
		m_pGUI->RemoveWindow(m_pOptionsWindow);
	}

	// Remove
	for(int i = 0; i < m_vpGameplayComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpGameplayComponents[i]);
	}
	for(int i = 0; i < m_vpGraphicsComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpGraphicsComponents[i]);
	}
	for(int i = 0; i < m_vpSoundComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpSoundComponents[i]);
	}
	for(int i = 0; i < m_vpControlsComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpControlsComponents[i]);
	}
	for(int i = 0; i < m_vpInterfaceComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpInterfaceComponents[i]);
	}

	// Add
	for(int i = 0; i < m_vpInterfaceComponents.size(); i++)
	{
		m_pOptionsWindow->AddComponent(m_vpInterfaceComponents[i]);
	}

	m_pGUI->AddWindow(m_pOptionsWindow);
	m_pOptionsWindow->Show();
}

void OptionsMenu::_ControlsTabPressed(void *pData)
{
	OptionsMenu* lpOptionsMenu = (OptionsMenu*)pData;
	lpOptionsMenu->ControlsTabPressed();
}

void OptionsMenu::ControlsTabPressed()
{
	if(m_loaded)
	{
		m_pGUI->RemoveWindow(m_pOptionsWindow);
	}

	// Remove
	for(int i = 0; i < m_vpGameplayComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpGameplayComponents[i]);
	}
	for(int i = 0; i < m_vpGraphicsComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpGraphicsComponents[i]);
	}
	for(int i = 0; i < m_vpSoundComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpSoundComponents[i]);
	}
	for(int i = 0; i < m_vpControlsComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpControlsComponents[i]);
	}
	for(int i = 0; i < m_vpInterfaceComponents.size(); i++)
	{
		m_pOptionsWindow->RemoveComponent(m_vpInterfaceComponents[i]);
	}

	// Add
	for(int i = 0; i < m_vpControlsComponents.size(); i++)
	{
		m_pOptionsWindow->AddComponent(m_vpControlsComponents[i]);
	}

	m_pGUI->AddWindow(m_pOptionsWindow);
	m_pOptionsWindow->Show();
}