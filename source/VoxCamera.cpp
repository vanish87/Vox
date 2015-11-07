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
	if (m_keyboardMovement)
	{
		if (m_bKeyboardForward)
		{
			vec3 behindPlayer = m_pPlayer->GetCenter();
			behindPlayer += m_pPlayer->GetRightVector() * 0.0f;
			behindPlayer += m_pPlayer->GetForwardVector() * -12.5f;
			behindPlayer += m_pPlayer->GetUpVector() * 12.5f;

			m_targetCameraPosition_AutoModeCached = behindPlayer;

			m_targetCameraFacing_AutoModeCached = m_pPlayer->GetCenter() - m_targetCameraPosition_AutoModeCached;
			m_targetCameraFacing_AutoModeCached = normalize(m_targetCameraFacing_AutoModeCached);

			m_targetCameraUp_AutoModeCached = normalize(cross(m_targetCameraFacing_AutoModeCached, m_pPlayer->GetRightVector()));
		}
		else if (m_bKeyboardStrafeRight)
		{
			vec3 behindPlayer = m_pPlayer->GetCenter();
			behindPlayer += m_pPlayer->GetRightVector() * -12.5f;
			behindPlayer += m_pPlayer->GetForwardVector() * 0.0f;
			behindPlayer += m_pPlayer->GetUpVector() * 12.5f;

			m_targetCameraPosition_AutoModeCached = behindPlayer;

			m_targetCameraFacing_AutoModeCached = m_pPlayer->GetCenter() - m_targetCameraPosition_AutoModeCached;
			m_targetCameraFacing_AutoModeCached = normalize(m_targetCameraFacing_AutoModeCached);

			m_targetCameraUp_AutoModeCached = normalize(m_pPlayer->GetUpVector());;
		}
		else if (m_bKeyboardStrafeLeft)
		{
			vec3 behindPlayer = m_pPlayer->GetCenter();
			behindPlayer += m_pPlayer->GetRightVector() * 12.5f;
			behindPlayer += m_pPlayer->GetForwardVector() * 0.0f;
			behindPlayer += m_pPlayer->GetUpVector() * 12.5f;

			m_targetCameraPosition_AutoModeCached = behindPlayer;

			m_targetCameraFacing_AutoModeCached = m_pPlayer->GetCenter() - m_targetCameraPosition_AutoModeCached;
			m_targetCameraFacing_AutoModeCached = normalize(m_targetCameraFacing_AutoModeCached);

			m_targetCameraUp_AutoModeCached = normalize(m_pPlayer->GetUpVector());;
		}
		else if (m_bKeyboardBackward)
		{
			vec3 behindPlayer = m_pPlayer->GetCenter();
			behindPlayer += m_pPlayer->GetRightVector() * 0.0f;
			behindPlayer += m_pPlayer->GetForwardVector() * 12.5f;
			behindPlayer += m_pPlayer->GetUpVector() * 12.5f;

			m_targetCameraPosition_AutoModeCached = behindPlayer;

			m_targetCameraFacing_AutoModeCached = m_pPlayer->GetCenter() - m_targetCameraPosition_AutoModeCached;
			m_targetCameraFacing_AutoModeCached = normalize(m_targetCameraFacing_AutoModeCached);

			m_targetCameraUp_AutoModeCached = normalize(cross(m_targetCameraFacing_AutoModeCached, -m_pPlayer->GetRightVector()));
		}
	}
	else
	{
		vec3 behindPlayer = m_pPlayer->GetCenter();
		behindPlayer += m_pPlayer->GetRightVector() * 0.0f;
		behindPlayer += m_pPlayer->GetForwardVector() * -12.5f;
		behindPlayer += m_pPlayer->GetUpVector() * 12.5f;

		m_targetCameraPosition_AutoModeCached = behindPlayer;

		m_targetCameraFacing_AutoModeCached = m_pPlayer->GetCenter() - m_targetCameraPosition_AutoModeCached;
		m_targetCameraFacing_AutoModeCached = normalize(m_targetCameraFacing_AutoModeCached);

		m_targetCameraUp_AutoModeCached = normalize(cross(m_targetCameraFacing_AutoModeCached, m_pPlayer->GetRightVector()));
	}

	// Update the target vectors based on the cached and calculated values
	{
		vec3 newPos = m_targetCameraPosition_AutoModeCached;
		vec3 newFacing = m_targetCameraFacing_AutoModeCached;
		vec3 newUp = m_targetCameraUp_AutoModeCached;

		vec3 toPos = newPos - m_targetCameraPosition_AutoMode;
		vec3 toUp = newUp - m_targetCameraUp_AutoMode;
		vec3 toFacing = newFacing - m_targetCameraFacing_AutoMode;

		m_targetCameraPosition_AutoMode += toPos * 13.0f * dt;
		m_targetCameraUp_AutoMode += toUp * 13.0f * dt;
		m_targetCameraFacing_AutoMode += toFacing * 13.0f * dt;
	}

	vec3 posDiff = m_targetCameraPosition_AutoMode - m_pGameCamera->GetPosition();
	vec3 upDiff = m_targetCameraUp_AutoMode - m_pGameCamera->GetUp();
	vec3 facingDiff = m_targetCameraFacing_AutoMode - m_pGameCamera->GetFacing();

	m_pGameCamera->SetPosition(m_pGameCamera->GetPosition() + ((posDiff *3.0f) * dt));
	m_pGameCamera->SetUp(normalize(m_pGameCamera->GetUp() + ((upDiff * 3.0f) * dt)));
	m_pGameCamera->SetFacing(normalize(m_pGameCamera->GetFacing() + ((facingDiff * 3.0f) * dt)));
	m_pGameCamera->SetRight(cross(m_pGameCamera->GetFacing(), m_pGameCamera->GetUp()));
}

void VoxGame::UpdateCameraFirstPerson(float dt)
{

}

void VoxGame::UpdateCameraClipping(float dt)
{

}