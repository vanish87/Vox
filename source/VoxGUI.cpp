#include "VoxGame.h"

void VoxGame::CreateGUI()
{
	m_pMainWindow = new GUIWindow(m_pRenderer, m_defaultFont, "Main");
	m_pMainWindow->AllowMoving(true);
	m_pMainWindow->AllowClosing(false);
	m_pMainWindow->AllowMinimizing(true);
	m_pMainWindow->AllowScrolling(true);
	m_pMainWindow->SetRenderTitleBar(true);
	m_pMainWindow->SetRenderWindowBackground(true);
	m_pMainWindow->SetOutlineRender(true);
	m_pMainWindow->SetDimensions(30, 50, 250, 140);
	m_pMainWindow->SetApplicationDimensions(m_windowWidth, m_windowHeight);

	m_pWireframeCheckBox = new CheckBox(m_pRenderer, m_defaultFont, "Wireframe");
	m_pWireframeCheckBox->SetDimensions(10, 10, 14, 14);
	m_pShadowsCheckBox = new CheckBox(m_pRenderer, m_defaultFont, "Shadows");
	m_pShadowsCheckBox->SetDimensions(10, 28, 14, 14);
	m_pShadowsCheckBox->SetToggled(true);
	m_pMSAACheckBox = new CheckBox(m_pRenderer, m_defaultFont, "MSAA");
	m_pMSAACheckBox->SetDimensions(10, 46, 14, 14);
	m_pMSAACheckBox->SetToggled(true);
	m_pDynamicLightingCheckBox = new CheckBox(m_pRenderer, m_defaultFont, "Lighting");
	m_pDynamicLightingCheckBox->SetDimensions(10, 64, 14, 14);
	m_pDynamicLightingCheckBox->SetToggled(true);
	m_pSSAOCheckBox = new CheckBox(m_pRenderer, m_defaultFont, "SSAO");
	m_pSSAOCheckBox->SetDimensions(10, 82, 14, 14);
	m_pSSAOCheckBox->SetToggled(true);
	m_pDeferredCheckBox = new CheckBox(m_pRenderer, m_defaultFont, "Deferred");
	m_pDeferredCheckBox->SetDimensions(10, 100, 14, 14);
	m_pDeferredCheckBox->SetToggled(true);
	m_pUpdateCheckBox = new CheckBox(m_pRenderer, m_defaultFont, "Update");
	m_pUpdateCheckBox->SetDimensions(10, 120, 14, 14);
	m_pUpdateCheckBox->SetToggled(true);

	m_pFullscreenButton = new Button(m_pRenderer, m_defaultFont, "FullScreen");
	m_pFullscreenButton->SetDimensions(150, 110, 80, 20);
	m_pFullscreenButton->SetCallBackFunction(_ToggleFullScreenPressed);
	m_pFullscreenButton->SetCallBackData(this);

	m_pMainWindow->AddComponent(m_pShadowsCheckBox);
	m_pMainWindow->AddComponent(m_pSSAOCheckBox);
	m_pMainWindow->AddComponent(m_pDynamicLightingCheckBox);
	m_pMainWindow->AddComponent(m_pWireframeCheckBox);
	m_pMainWindow->AddComponent(m_pMSAACheckBox);
	m_pMainWindow->AddComponent(m_pDeferredCheckBox);
	m_pMainWindow->AddComponent(m_pUpdateCheckBox);
	m_pMainWindow->AddComponent(m_pFullscreenButton);

	m_pGUI->AddWindow(m_pMainWindow);
}

void VoxGame::DestroyGUI()
{
	delete m_pMainWindow;
	delete m_pShadowsCheckBox;
	delete m_pSSAOCheckBox;
	delete m_pDynamicLightingCheckBox;
	delete m_pWireframeCheckBox;
	delete m_pMSAACheckBox;
	delete m_pDeferredCheckBox;
	delete m_pUpdateCheckBox;
	delete m_pFullscreenButton;
}

void VoxGame::UpdateGUI(float dt)
{
	m_shadows = m_pShadowsCheckBox->GetToggled();
	m_ssao = m_pSSAOCheckBox->GetToggled();
	m_dynamicLighting = m_pDynamicLightingCheckBox->GetToggled();
	m_modelWireframe = m_pWireframeCheckBox->GetToggled();
	m_multiSampling = m_pMSAACheckBox->GetToggled();
	m_deferredRendering = m_pDeferredCheckBox->GetToggled();
	m_animationUpdate = m_pUpdateCheckBox->GetToggled();

	if (m_deferredRendering)
	{
		m_pSSAOCheckBox->SetDisabled(false);
		m_pDynamicLightingCheckBox->SetDisabled(false);
		m_pMSAACheckBox->SetDisabled(true);
	}
	else
	{
		m_pSSAOCheckBox->SetDisabled(true);
		m_pDynamicLightingCheckBox->SetDisabled(true);
		m_pMSAACheckBox->SetDisabled(false);
	}

	m_pPlayer->SetWireFrameRender(m_modelWireframe);
	m_pBlockParticleManager->SetWireFrameRender(m_modelWireframe);
}

// GUI callbacks
void VoxGame::_ToggleFullScreenPressed(void *apData)
{
	VoxGame* lpVoxGame = (VoxGame*)apData;
	lpVoxGame->ToggleFullScreenPressed();
}

void VoxGame::ToggleFullScreenPressed()
{
	m_fullscreen = !m_fullscreen;

	m_pVoxWindow->ToggleFullScreen(m_fullscreen);
	m_pBlockParticleManager->SetupGLBuffers();
}