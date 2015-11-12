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
}

void VoxGame::UpdateCameraAutoCamera(float dt, bool updateCameraPosition)
{
	if (updateCameraPosition)
	{
		vec3 ratios = normalize(vec3(2.5f, 1.0f, 0.0f));
		float catchupSpeed = 50.0f * (1.0f - (m_cameraDistance / 20.0f));

		vec3 cameraFacing = m_pGameCamera->GetFacing();
		cameraFacing.y = 0.0f;
		m_targetCameraBehindPlayerPosition = m_pPlayer->GetCenter() + Player::PLAYER_CENTER_OFFSET;
		m_targetCameraBehindPlayerPosition += cameraFacing * -(m_cameraDistance*ratios.x);
		m_targetCameraBehindPlayerPosition += m_pPlayer->GetUpVector() * (m_cameraDistance*ratios.y);

		m_targetCameraPosition_AutoModeCached = m_targetCameraBehindPlayerPosition;

		// Update the target vectors based on the cached and calculated values
		{
			vec3 newPos = m_targetCameraPosition_AutoModeCached;
			vec3 toPos = newPos - m_targetCameraPosition_AutoMode;
			m_targetCameraPosition_AutoMode += toPos * (catchupSpeed*2.0f) * dt;
		}

		// Position
		vec3 posDiff = m_targetCameraPosition_AutoMode - m_pGameCamera->GetPosition();
		m_pGameCamera->SetPosition(m_pGameCamera->GetPosition() + ((posDiff * catchupSpeed) * dt));
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
}

void VoxGame::UpdateCameraClipping(float dt)
{
}

void VoxGame::UpdateCameraZoom(float dt)
{
	// Make sure we gradually move inwards/outwards
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