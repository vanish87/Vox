#include "VoxGame.h"

// Controls
void VoxGame::UpdateControls(float dt)
{
	if (m_gamepadMovement == false)
	{
		UpdateKeyboardControls(dt);
		UpdateMouseControls(dt);
	}

	if (m_keyboardMovement == false)
	{
		if (m_pVoxWindow->IsJoyStickConnected(0))
		{
			UpdateGamePadControls(dt);
		}
	}
}

void VoxGame::UpdateKeyboardControls(float dt)
{
	GameMode gameMode = GetGameMode();

	if (gameMode == GameMode_Debug || m_cameraMode == CameraMode_Debug)
	{
		// Keyboard camera movements
		if (m_bKeyboardForward)
		{
			m_pGameCamera->Fly(20.0f * dt);
		}
		if (m_bKeyboardBackward)
		{
			m_pGameCamera->Fly(-20.0f * dt);
		}
		if (m_bKeyboardStrafeLeft)
		{
			m_pGameCamera->Strafe(-20.0f * dt);
		}
		if (m_bKeyboardStrafeRight)
		{
			m_pGameCamera->Strafe(20.0f * dt);
		}
		if (m_bKeyboardUp)
		{
			m_pGameCamera->Levitate(20.0f * dt);
		}
		if (m_bKeyboardDown)
		{
			m_pGameCamera->Levitate(-20.0f * dt);
		}
	}
	else if (gameMode == GameMode_Game)
	{
		// Jumping
		if (m_bKeyboardSpace)
		{
			m_pPlayer->Jump();
		}

		// Attacking
		if (m_bAttackPressed && m_bCanDoAttack)
		{
			m_pPlayer->Attack();
			m_bCanDoAttack = false;
		}

		// Player movements
		bool resetMovementVector = false;
		if (m_bKeyboardForward == false && m_bKeyboardBackward == false && m_bKeyboardStrafeLeft == false && m_bKeyboardStrafeRight == false)
		{
			// Reduce the movement speed (drag)
			m_movementSpeed -= m_maxMovementSpeed / (m_movementDragTime / dt);

			if (m_movementSpeed <= 0.0f)
			{
				m_movementSpeed = 0.0f;
				m_keyboardMovement = false;
				m_pPlayer->StopMoving();
			}
		}
		else
		{
			m_keyboardMovement = true;

			// Increase the movement speed since we are pressing a movement key
			m_movementSpeed += m_maxMovementSpeed / (m_movementIncreaseTime / dt);

			// Don't allow faster than max movement
			if (m_movementSpeed > m_maxMovementSpeed)
			{
				m_movementSpeed = m_maxMovementSpeed;
			}
		}

		if (m_bKeyboardForward)
		{
			if (resetMovementVector == false)
			{
				m_movementDirection = vec3(0.0f, 0.0f, 0.0f);
				resetMovementVector = true;
			}

			vec3 cameraRight = m_pGameCamera->GetRight();
			vec3 playerUp = m_pPlayer->GetUpVector();
			vec3 moveDirection = normalize(cross(cameraRight, playerUp));
			m_movementDirection -= moveDirection;
		}

		if (m_bKeyboardBackward)
		{
			if (resetMovementVector == false)
			{
				m_movementDirection = vec3(0.0f, 0.0f, 0.0f);
				resetMovementVector = true;
			}

			vec3 cameraRight = m_pGameCamera->GetRight();
			vec3 playerUp = m_pPlayer->GetUpVector();
			vec3 moveDirection = normalize(cross(cameraRight, playerUp));
			m_movementDirection += moveDirection;
		}

		if (m_bKeyboardStrafeLeft)
		{
			if (resetMovementVector == false)
			{
				m_movementDirection = vec3(0.0f, 0.0f, 0.0f);
				resetMovementVector = true;
			}

			vec3 cameraRight = m_pGameCamera->GetRight();
			vec3 moveDirection = -cameraRight;
			m_movementDirection += moveDirection;
		}

		if (m_bKeyboardStrafeRight)
		{
			if (resetMovementVector == false)
			{
				m_movementDirection = vec3(0.0f, 0.0f, 0.0f);
				resetMovementVector = true;
			}

			vec3 cameraRight = m_pGameCamera->GetRight();
			vec3 moveDirection = -cameraRight;
			m_movementDirection -= moveDirection;
		}

		if (length(m_movementDirection) > 0.001f && m_movementSpeed > m_movementStopThreshold)
		{
			bool shouldChangePlayerFacing = (m_cameraMode != CameraMode_FirstPerson);

			m_movementDirection = normalize(m_movementDirection);
			m_pGameCamera->SetPosition(m_pGameCamera->GetPosition() + m_movementDirection * m_movementSpeed * dt);
			m_pPlayer->MoveAbsolute(m_movementDirection, m_movementSpeed * dt, shouldChangePlayerFacing);
		}
	}
}

void VoxGame::UpdateMouseControls(float dt)
{
	GameMode gameMode = GetGameMode();
	if (gameMode == GameMode_Debug || m_cameraMode == CameraMode_Debug)
	{
		if (m_bCameraRotate)
		{
			MouseCameraRotate();
		}
	}
	else if (gameMode == GameMode_Game)
	{
		if (m_cameraMode == CameraMode_MouseRotate)
		{
			MouseCameraRotate();
		}
		if (m_cameraMode == CameraMode_AutoCamera)
		{
			/* Do nothing, camera rotation is not controlled by player. */
		}
		else if (m_cameraMode == CameraMode_FirstPerson)
		{
			MouseCameraRotate();
		}
	}
}

void VoxGame::UpdateGamePadControls(float dt)
{
	JoystickCameraZoom(dt);

	m_bJoystickJump = m_pVoxWindow->GetJoystickButton(0, 0);
	if (m_bAttackPressed)
	{
		m_bCanDoJoystickAttack = m_pVoxWindow->GetJoystickAxisValue(0, 2) >= -0.75f;
	}
	m_bAttackPressed = m_pVoxWindow->GetJoystickAxisValue(0, 2) <= -0.75f;

	GameMode gameMode = GetGameMode();
	if (gameMode == GameMode_Debug || m_cameraMode == CameraMode_Debug)
	{
		JoystickCameraMove(dt);
		JoystickCameraRotate(dt);
	}
	else if (gameMode == GameMode_Game)
	{
		if (m_cameraMode == CameraMode_MouseRotate)
		{
			JoystickCameraRotate(dt);
		}
		if (m_cameraMode == CameraMode_AutoCamera)
		{
			/* Do nothing, camera rotation is not controlled by player. */
		}
		else if (m_cameraMode == CameraMode_FirstPerson)
		{
			JoystickCameraRotate(dt);
		}

		// Jumping
		if (m_bJoystickJump)
		{
			m_pPlayer->Jump();
		}

		// Attacking
		if (m_bAttackPressed && m_bCanDoJoystickAttack)
		{
			m_pPlayer->Attack();
			m_bCanDoJoystickAttack = false;
		}

		// Player movements
		float axisX = m_pVoxWindow->GetJoystickAxisValue(0, 0);
		float axisY = m_pVoxWindow->GetJoystickAxisValue(0, 1);

		// Dead zones
		if (fabs(axisX) < m_pVoxWindow->GetJoystickAnalogDeadZone())
		{
			axisX = 0.0f;
		}
		if (fabs(axisY) < m_pVoxWindow->GetJoystickAnalogDeadZone())
		{
			axisY = 0.0f;
		}

		if (fabs(axisX) <= 0.0f && fabs(axisY) <= 0.0f)
		{
			// Reduce the movement speed (drag)
			m_movementSpeed -= m_maxMovementSpeed / (m_movementDragTime / dt);

			if (m_movementSpeed <= 0.0f)
			{
				m_movementSpeed = 0.0f;
				m_gamepadMovement = false;
				m_pPlayer->StopMoving();
			}
		}
		else
		{
			m_gamepadMovement = true;

			// Increase the movement speed since we are pressing a movement key
			m_movementSpeed += m_maxMovementSpeed / (m_movementIncreaseTime / dt);

			// Don't allow faster than max movement
			if (m_movementSpeed > m_maxMovementSpeed)
			{
				m_movementSpeed = m_maxMovementSpeed;
			}
		}

		vec3 cameraRight = m_pGameCamera->GetRight();
		vec3 playerUp = m_pPlayer->GetUpVector();
		vec3 moveDirection = normalize(cross(cameraRight, playerUp));
		m_movementDirection += moveDirection * axisY;
		m_movementDirection += cameraRight * axisX;

		if (length(m_movementDirection) > 0.001f && m_movementSpeed > m_movementStopThreshold)
		{
			bool shouldChangePlayerFacing = (m_cameraMode != CameraMode_FirstPerson);

			m_movementDirection = normalize(m_movementDirection);
			m_pGameCamera->SetPosition(m_pGameCamera->GetPosition() + m_movementDirection * m_movementSpeed * dt);
 			m_pPlayer->MoveAbsolute(m_movementDirection, m_movementSpeed * dt, shouldChangePlayerFacing);
		}
	}
}