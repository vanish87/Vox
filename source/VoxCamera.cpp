#include "VoxGame.h"

// Camera controls
void VoxGame::UpdateCamera(float dt)
{
	if (m_cameraMode == CameraMode_MouseRotate)
	{
		UpdateCameraAutoCamera(dt, false);
	}
	else if (m_cameraMode == CameraMode_AutoCamera)
	{
		UpdateCameraAutoCamera(dt, true);
	}
	else if (m_cameraMode == CameraMode_FirstPerson)
	{
		UpdateCameraFirstPerson(dt);
	}

	if (m_gameMode == GameMode_Game && m_cameraMode != CameraMode_Debug)
	{
		UpdateCameraModeSwitching();
	}
}

void VoxGame::UpdateCameraModeSwitching()
{
	if (m_cameraDistance < 1.5f)
	{
		if (m_cameraMode != CameraMode_FirstPerson)
		{
			m_previousCameraMode = m_cameraMode;
			SetCameraMode(CameraMode_FirstPerson);
			m_pFirstPersonCameraOptionBox->SetToggled(true);
			m_pGameCamera->SetZoomAmount(1.5f);
		}
	}
	else
	{
		if (m_cameraMode == CameraMode_FirstPerson)
		{
			if (m_previousCameraMode == CameraMode_MouseRotate)
			{
				m_pMouseRotateCameraOptionBox->SetToggled(true);
			}
			else if (m_previousCameraMode == CameraMode_AutoCamera)
			{
				m_pAutoCameraOptionBox->SetToggled(true);
			}

			m_cameraDistance = 1.5f;
			m_maxCameraDistance = 1.5f;

			SetCameraMode(m_previousCameraMode);
			InitializeCameraRotation();
		}
	}
}

void VoxGame::InitializeCameraRotation()
{
	m_currentX = m_pVoxWindow->GetCursorX();
	m_currentY = m_pVoxWindow->GetCursorY();

	vec3 ratios = normalize(vec3(2.5f, 1.0f, 0.0f));

	m_targetCameraBehindPlayerPosition = m_pPlayer->GetCenter() + Player::PLAYER_CENTER_OFFSET;
	m_targetCameraBehindPlayerPosition += m_pPlayer->GetForwardVector() * -(m_cameraDistance*ratios.x);
	m_targetCameraBehindPlayerPosition += m_pPlayer->GetUpVector() * (m_cameraDistance*ratios.y);

	// Only set the position, since the player will be controlling the rotation of the camera
	m_pGameCamera->SetPosition(m_targetCameraBehindPlayerPosition);
}

void VoxGame::UpdateCameraAutoCamera(float dt, bool updateCameraPosition)
{
	if (updateCameraPosition)
	{
		float movementModifierNormal = 10.0f;
		float movementModifierMoving = 0.125f;
		float movementModifierChangeAmount = 2.0f;
		if (m_keyboardMovement || m_gamepadMovement)
		{
			m_autoCameraMovingModifier = movementModifierMoving;
		}
		else
		{
			if (m_autoCameraMovingModifier < movementModifierNormal)
			{
				m_autoCameraMovingModifier += movementModifierChangeAmount * dt;
			}
			else
			{
				m_autoCameraMovingModifier = movementModifierNormal;
			}
		}

		vec3 ratios = normalize(vec3(2.5f, 1.0f, 0.0f));
		float catchupSpeed = 5.0f * (1.0f - (m_cameraDistance / 20.0f)) * m_autoCameraMovingModifier;
		vec3 cameraFacing = m_pGameCamera->GetFacing();
		cameraFacing.y = 0.0f;

		m_targetCameraBehindPlayerPosition = m_pPlayer->GetCenter() + Player::PLAYER_CENTER_OFFSET;
		m_targetCameraBehindPlayerPosition += m_pPlayer->GetForwardVector() * -(m_cameraDistance*ratios.x);
		m_targetCameraBehindPlayerPosition += m_pPlayer->GetUpVector() * (m_cameraDistance*ratios.y);

		m_targetCameraPosition_AutoModeCached = m_targetCameraBehindPlayerPosition;

		// Update the target vectors based on the cached and calculated values
		{
			vec3 newPos = m_targetCameraPosition_AutoModeCached;
			vec3 toPos = newPos - m_targetCameraPosition_AutoMode;
			m_targetCameraPosition_AutoMode += toPos * (catchupSpeed * 2.0f) * dt;
		}

		// Position
		vec3 posDiff = m_targetCameraPosition_AutoMode - m_pGameCamera->GetPosition();
		vec3 newPos = vec3(m_pGameCamera->GetPosition().x, m_pGameCamera->GetPosition().y + ((posDiff.y * catchupSpeed) * dt), m_pGameCamera->GetPosition().z);
		m_pGameCamera->SetPosition(newPos);

		vec3 cameraLookAt = m_pPlayer->GetCenter() + Player::PLAYER_CENTER_OFFSET;
		vec3 targetFacing = normalize(cameraLookAt - m_targetCameraPosition_AutoMode);
		targetFacing.y = 0.0f;
		targetFacing = normalize(targetFacing);
		vec3 camFacing = m_pGameCamera->GetFacing();
		camFacing.y = 0.0f;
		camFacing = normalize(camFacing);
		vec3 crossResult = cross(targetFacing, camFacing);
		float dotResult = dot(targetFacing, camFacing);
		float rotationDegrees = RadToDeg(acos(dotResult));
		if(rotationDegrees > 1.0f)
		{
			if (crossResult.y > 0.0f)
			{
				rotationDegrees = -rotationDegrees;
			}
			float changeAmount = rotationDegrees * 1.0f * m_autoCameraMovingModifier;
			m_pGameCamera->RotateAroundPointY(changeAmount * dt);
		}
	}

	// Forward
	vec3 cameraLookAt = m_pPlayer->GetCenter() + Player::PLAYER_CENTER_OFFSET;
	vec3 cameraForward = normalize(cameraLookAt - m_pGameCamera->GetPosition());
	m_pGameCamera->SetFacing(cameraForward);
	
	// Right
	vec3 cameraRight = normalize(cross(cameraForward, m_pPlayer->GetUpVector()));
	m_pGameCamera->SetRight(cameraRight);

	// Up
	vec3 cameraUp = normalize(cross(cameraRight, cameraForward));
	m_pGameCamera->SetUp(cameraUp);
}

void VoxGame::UpdateCameraFirstPerson(float dt)
{
	m_pGameCamera->SetPosition(m_pPlayer->GetCenter() + Player::PLAYER_CENTER_OFFSET);
	m_pPlayer->SetForwardVector(m_pGameCamera->GetFacing());
}

void VoxGame::UpdateCameraClipping(float dt)
{
}

void VoxGame::UpdateCameraZoom(float dt)
{
	// Make sure we gradually move inwards/outwards
	if (m_cameraMode != CameraMode_FirstPerson)
	{
		float camDiff = fabs(m_cameraDistance - m_maxCameraDistance);
		float changeAmount = 0.0f;
		if (m_cameraDistance < m_maxCameraDistance)
		{
			changeAmount = camDiff * dt;
		}
		else if (m_cameraDistance >= m_maxCameraDistance)
		{
			changeAmount = -camDiff * dt;
		}

		m_cameraDistance += changeAmount;
		m_pGameCamera->Zoom(changeAmount);
	}
}