// ******************************************************************************
// Filename:    VoxUpdate.cpp
// Project:     Vox
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 27/10/15
//
// Copyright (c) 2005-2015, Steven Ball
// ******************************************************************************

#include "VoxGame.h"

#include "utils/Interpolator.h"
#include "utils/TimeManager.h"

#ifdef __linux__
#include <sys/time.h>
#endif //__linux__


// Updating
void VoxGame::Update()
{
	// FPS
#ifdef _WIN32
	QueryPerformanceCounter(&m_fpsCurrentTicks);
	m_deltaTime = ((float)(m_fpsCurrentTicks.QuadPart - m_fpsPreviousTicks.QuadPart) / (float)m_fpsTicksPerSecond.QuadPart);
#else
	struct timeval tm;
	gettimeofday(&tm, NULL);
	m_fpsCurrentTicks = (double)tm.tv_sec + (double)tm.tv_usec / 1000000.0;
	m_deltaTime = (m_fpsCurrentTicks - m_fpsPreviousTicks);
#endif //_WIN32
	m_fps = 1.0f / m_deltaTime;
	m_fpsPreviousTicks = m_fpsCurrentTicks;

	// Update interpolator singleton
	Interpolator::GetInstance()->Update(m_deltaTime);

	// Pause the interpolator we are are paused.
	Interpolator::GetInstance()->SetPaused(m_bPaused);

	// Update the time manager (countdowntimers);
	TimeManager::GetInstance()->Update(m_deltaTime);

	// Update the initial wait timer and variables, so we dont do gameplay updates straight away
	if (m_initialStartWait == true)
	{
		if (m_initialWaitTimer > m_initialWaitTime)
		{
			m_initialStartWait = false;
		}
		else
		{
			m_initialWaitTimer += m_deltaTime;
			m_initialStartWait = true;
		}
	}

	// Animation update
	if (m_bPaused == false && m_initialStartWait == false)
	{
		// Update the lighting manager
		m_pLightingManager->Update(m_deltaTime);

		// Block particle manager
		m_pBlockParticleManager->Update(m_deltaTime);

		// Instance manager
		m_pInstanceManager->Update(m_deltaTime);

		// Scenery manager
		m_pSceneryManager->Update(m_deltaTime);

		// Inventory manager
		m_pInventoryManager->Update(m_deltaTime);

		// Item manager
		m_pItemManager->Update(m_deltaTime);
		m_pItemManager->UpdateItemLights(m_deltaTime);
		m_pItemManager->UpdateItemParticleEffects(m_deltaTime);
		m_interactItemMutex.lock();
		m_pInteractItem = m_pItemManager->CheckItemPlayerInteraction();
		m_interactItemMutex.unlock();

		// Projectile manager
		m_pProjectileManager->Update(m_deltaTime);
		m_pProjectileManager->UpdateProjectileLights(m_deltaTime);
		m_pProjectileManager->UpdateProjectileParticleEffects(m_deltaTime);

		// Player
		if (m_animationUpdate)
		{
			m_pPlayer->Update(m_deltaTime);
		}

		// Camera faked position
		if (m_cameraMode == CameraMode_MouseRotate || m_cameraMode == CameraMode_AutoCamera || m_cameraMode == CameraMode_NPCDialog)
		{
			vec3 playerMovementChanged = m_pPlayer->GetPositionMovementAmount();
			m_pGameCamera->SetFakePosition(m_pGameCamera->GetFakePosition() + playerMovementChanged);
		}
	}

	// Update the chunk manager
	m_pChunkManager->Update(m_deltaTime);

	// Update controls
	UpdateControls(m_deltaTime);

	// Update the camera based on movements
	if (m_gameMode == GameMode_Game)
	{
		UpdateCamera(m_deltaTime);
		m_pPlayer->SetCameraPosition(m_pGameCamera->GetPosition());
		m_pPlayer->SetCameraForward(normalize(m_pGameCamera->GetFacing()));
	}

	// Update the dynamic camera zoom
	UpdateCameraZoom(m_deltaTime);

	// Update the camera clipping
	m_targetCameraPositionBeforeClipping = m_pGameCamera->GetFakePosition();
	UpdateCameraClipping(m_deltaTime);

	// Update the player's alpha and transparency based on camera distance to player
	if (m_gameMode == GameMode_Game && m_cameraMode != CameraMode_Debug)
	{
		UpdatePlayerAlpha(m_deltaTime);
	}

	// Update the frontend
	if (m_gameMode == GameMode_FrontEnd)
	{
		m_pFrontendManager->Update(m_deltaTime);
	}

	// Update the GUI
	int x = m_pVoxWindow->GetCursorX();
	int y = m_pVoxWindow->GetCursorY();
	m_pGUI->Update(m_deltaTime);
	if (m_pVoxWindow->IsCursorOn())
	{
		m_pGUI->ImportMouseMotion(x, m_windowHeight - y);
	}
	UpdateGUI(m_deltaTime);

	// Update game GUI
	UpdateGameGUI(m_deltaTime);

	// Update lights
	UpdateLights(m_deltaTime);

	// Update the application and window
	m_pVoxApplication->Update(m_deltaTime);
	m_pVoxWindow->Update(m_deltaTime);
}

void VoxGame::UpdatePlayerAlpha(float dt)
{
	vec3 toPlayer = ((m_pPlayer->GetCenter() + Player::PLAYER_CENTER_OFFSET) - m_pGameCamera->GetPosition());
	float distance = length(toPlayer);
	float alpha = (distance) / 7.5f;

	if (m_cameraMode == CameraMode_NPCDialog)
	{
		alpha = 1.0f;
	}

	m_pPlayer->SetPlayerAlpha(alpha);
}

void VoxGame::UpdateLights(float dt)
{
	m_pRenderer->EditLightPosition(m_defaultLight, m_defaultLightPosition);
}

void VoxGame::UpdateGameGUI(float dt)
{
	if (m_pInventoryGUI->IsLoaded())
	{
		m_pInventoryGUI->Update(dt);
	}
	if (m_pCharacterGUI->IsLoaded())
	{
		m_pCharacterGUI->Update(dt);
	}
	if (m_pLootGUI->IsLoaded())
	{
		m_pLootGUI->Update(dt);
	}
	if (m_pCraftingGUI->IsLoaded())
	{
		m_pCraftingGUI->Update(dt);
	}
}