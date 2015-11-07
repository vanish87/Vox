#include "VoxGame.h"

// Camera controls
void VoxGame::UpdateCamera(float dt)
{
	if (m_cameraMode == CameraMode_MouseRotate)
	{
		UpdateCameraMouseRotate(dt);
	}
	else if (m_cameraMode == CameraMode_AutoCamera)
	{
		UpdateCameraAutoCamera(dt);
	}
	else if (m_cameraMode == CameraMode_FirstPerson)
	{
		UpdateCameraFirstPerson(dt);
	}
}

void VoxGame::UpdateCameraMouseRotate(float dt)
{
}

void VoxGame::UpdateCameraAutoCamera(float dt)
{
	vec3 ratios = normalize(vec3(2.5f, 1.0f, 0.0f));
	float catchupSpeed = 5.0f;

	if (m_keyboardMovement)
	{
		if (m_bKeyboardForward && m_bKeyboardBackward == false)
		{
			vec3 behindPlayer = m_pPlayer->GetCenter() + Player::PLAYER_CENTER_OFFSET;
			behindPlayer += m_pPlayer->GetRightVector() * 0.0f;
			behindPlayer += m_pPlayer->GetForwardVector() * -m_cameraDistance*ratios.x;
			behindPlayer += m_pPlayer->GetUpVector() * m_cameraDistance*ratios.y;

			m_targetCameraPosition_AutoModeCached = behindPlayer;

			m_targetCameraFacing_AutoModeCached = m_pPlayer->GetCenter() - m_targetCameraPosition_AutoModeCached;
			m_targetCameraFacing_AutoModeCached = normalize(m_targetCameraFacing_AutoModeCached);

			m_targetCameraUp_AutoModeCached = normalize(cross(m_targetCameraFacing_AutoModeCached, m_pPlayer->GetRightVector()));
		}
		else if (m_bKeyboardRight && m_bKeyboardLeft == false)
		{
			vec3 behindPlayer = m_pPlayer->GetCenter() + Player::PLAYER_CENTER_OFFSET;
			behindPlayer += m_pPlayer->GetRightVector() * -m_cameraDistance*ratios.x;
			behindPlayer += m_pPlayer->GetForwardVector() * 0.0f;
			behindPlayer += m_pPlayer->GetUpVector() * m_cameraDistance*ratios.y;

			m_targetCameraPosition_AutoModeCached = behindPlayer;

			m_targetCameraFacing_AutoModeCached = m_pPlayer->GetCenter() - m_targetCameraPosition_AutoModeCached;
			m_targetCameraFacing_AutoModeCached = normalize(m_targetCameraFacing_AutoModeCached);

			m_targetCameraUp_AutoModeCached = normalize(m_pPlayer->GetUpVector());;
		}
		else if (m_bKeyboardLeft && m_bKeyboardRight == false)
		{
			vec3 behindPlayer = m_pPlayer->GetCenter() + Player::PLAYER_CENTER_OFFSET;
			behindPlayer += m_pPlayer->GetRightVector() * m_cameraDistance*ratios.x;
			behindPlayer += m_pPlayer->GetForwardVector() * 0.0f;
			behindPlayer += m_pPlayer->GetUpVector() * m_cameraDistance*ratios.y;

			m_targetCameraPosition_AutoModeCached = behindPlayer;

			m_targetCameraFacing_AutoModeCached = m_pPlayer->GetCenter() - m_targetCameraPosition_AutoModeCached;
			m_targetCameraFacing_AutoModeCached = normalize(m_targetCameraFacing_AutoModeCached);

			m_targetCameraUp_AutoModeCached = normalize(m_pPlayer->GetUpVector());;
		}
		else if (m_bKeyboardBackward && m_bKeyboardForward == false)
		{
			vec3 behindPlayer = m_pPlayer->GetCenter() + Player::PLAYER_CENTER_OFFSET;
			behindPlayer += m_pPlayer->GetRightVector() * 0.0f;
			behindPlayer += m_pPlayer->GetForwardVector() * m_cameraDistance*ratios.x;
			behindPlayer += m_pPlayer->GetUpVector() * m_cameraDistance*ratios.y;

			m_targetCameraPosition_AutoModeCached = behindPlayer;

			m_targetCameraFacing_AutoModeCached = m_pPlayer->GetCenter() - m_targetCameraPosition_AutoModeCached;
			m_targetCameraFacing_AutoModeCached = normalize(m_targetCameraFacing_AutoModeCached);

			m_targetCameraUp_AutoModeCached = normalize(cross(m_targetCameraFacing_AutoModeCached, -m_pPlayer->GetRightVector()));
		}
	}
	else
	{
		vec3 behindPlayer = m_pPlayer->GetCenter() + Player::PLAYER_CENTER_OFFSET;
		behindPlayer += m_pPlayer->GetRightVector() * 0.0f;
		behindPlayer += m_pPlayer->GetForwardVector() * -m_cameraDistance*ratios.x;
		behindPlayer += m_pPlayer->GetUpVector() * m_cameraDistance*ratios.y;

		m_targetCameraPosition_AutoModeCached = behindPlayer;

		m_targetCameraFacing_AutoModeCached = m_pPlayer->GetCenter() - m_targetCameraPosition_AutoModeCached;
		m_targetCameraFacing_AutoModeCached = normalize(m_targetCameraFacing_AutoModeCached);

		m_targetCameraUp_AutoModeCached = normalize(cross(m_targetCameraFacing_AutoModeCached, m_pPlayer->GetRightVector()));

		catchupSpeed = 2.0f;
	}

	// Update the target vectors based on the cached and calculated values
	{
		vec3 newPos = m_targetCameraPosition_AutoModeCached;
		vec3 newFacing = m_targetCameraFacing_AutoModeCached;
		vec3 newUp = m_targetCameraUp_AutoModeCached;

		vec3 toPos = newPos - m_targetCameraPosition_AutoMode;
		vec3 toUp = newUp - m_targetCameraUp_AutoMode;
		vec3 toFacing = newFacing - m_targetCameraFacing_AutoMode;

		m_targetCameraPosition_AutoMode += toPos * 10.0f * dt;
		m_targetCameraUp_AutoMode += toUp * 10.0f * dt;
		m_targetCameraFacing_AutoMode += toFacing * 10.0f * dt;
	}

	vec3 posDiff = m_targetCameraPosition_AutoMode - m_pGameCamera->GetPosition();
	vec3 upDiff = m_targetCameraUp_AutoMode - m_pGameCamera->GetUp();
	vec3 facingDiff = m_targetCameraFacing_AutoMode - m_pGameCamera->GetFacing();

	m_pGameCamera->SetPosition(m_pGameCamera->GetPosition() + ((posDiff * catchupSpeed) * dt));
	m_pGameCamera->SetUp(normalize(m_pGameCamera->GetUp() + ((upDiff * catchupSpeed) * dt)));
	m_pGameCamera->SetFacing(normalize(m_pGameCamera->GetFacing() + ((facingDiff * catchupSpeed) * dt)));
	m_pGameCamera->SetRight(cross(m_pGameCamera->GetFacing(), m_pGameCamera->GetUp()));
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