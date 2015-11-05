#include <stdio.h>
#include <stdlib.h>

#include "VoxGame.h"


// Event callbacks
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action)
	{
		case GLFW_PRESS:
		{
			VoxGame::GetInstance()->KeyPressed(key, scancode, mods);
			break;
		}
		case GLFW_RELEASE:
		{
			VoxGame::GetInstance()->KeyReleased(key, scancode, mods);
			break;
		}
		case GLFW_REPEAT:
		{
			break;
		}
	}
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	switch (action)
	{
		case GLFW_PRESS:
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT)
				VoxGame::GetInstance()->MouseLeftPressed();
			if (button == GLFW_MOUSE_BUTTON_RIGHT)
				VoxGame::GetInstance()->MouseRightPressed();
			if (button == GLFW_MOUSE_BUTTON_MIDDLE)
				VoxGame::GetInstance()->MouseMiddlePressed();

			break;
		}
		case GLFW_RELEASE:
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT)
				VoxGame::GetInstance()->MouseLeftReleased();
			if (button == GLFW_MOUSE_BUTTON_RIGHT)
				VoxGame::GetInstance()->MouseRightReleased();
			if (button == GLFW_MOUSE_BUTTON_MIDDLE)
				VoxGame::GetInstance()->MouseMiddleReleased();

			break;
		}
	}
}

void MouseScrollCallback(GLFWwindow* window, double x, double y)
{
	VoxGame::GetInstance()->MouseScroll(x, y);
}


// Controls
void VoxGame::UpdateControls(float dt)
{
	UpdateKeyboardControls(dt);
	UpdateMouseControls(dt);
	UpdateGamePadControls(dt);
}

void VoxGame::UpdateKeyboardControls(float dt)
{
	GameMode gameMode = GetGameMode();

	if (gameMode == GameMode_Debug)
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
	}

	if (gameMode == GameMode_Game)
	{
		// Player movements
		if (m_bKeyboardSpace)
		{
			m_pPlayer->Jump();
		}


		bool resetMovementVector = false;
		bool l_moving = true;
		float speedDescreaseFactor = 1.0f;
		if (m_bKeyboardForward == false && m_bKeyboardBackward == false && m_bKeyboardStrafeLeft == false && m_bKeyboardStrafeRight == false)
		{
			l_moving = false;

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
			vec3 playerUp = m_pPlayer->GetUpVector();
			vec3 playerRight = normalize(cross(playerUp, cameraRight));
			vec3 moveDirection = normalize(cross(playerUp, playerRight));
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
			vec3 playerUp = m_pPlayer->GetUpVector();
			vec3 playerRight = normalize(cross(playerUp, cameraRight));
			vec3 moveDirection = normalize(cross(playerUp, playerRight));
			m_movementDirection -= moveDirection;
		}

		if (length(m_movementDirection) > 0.001f && m_movementSpeed > m_movementStopThreshold)
		{
			m_movementDirection = normalize(m_movementDirection);
			m_pPlayer->MoveAbsolute(m_movementDirection, m_movementSpeed*speedDescreaseFactor * dt, true);
		}
	}
}

void VoxGame::UpdateMouseControls(float dt)
{
	int x = m_pVoxWindow->GetCursorX();
	int y = m_pVoxWindow->GetCursorY();

	GameMode gameMode = GetGameMode();

	if (gameMode == GameMode_Debug)
	{
		// Mouse camera movements
		if (m_bCameraRotate)
		{
			MouseCameraRotate(x, y);
		}
		if (m_bCameraZoom)
		{
			MouseCameraZoom(x, y);
		}
	}
}

void VoxGame::UpdateGamePadControls(float dt)
{

}

void VoxGame::KeyPressed(int key, int scancode, int mods)
{
	switch (key)
	{
		// Player movement
		case GLFW_KEY_W:
		{
			m_bKeyboardForward = true;
			break;
		}
		case GLFW_KEY_S:
		{
			m_bKeyboardBackward = true;
			break;
		}
		case GLFW_KEY_A:
		{
			m_bKeyboardLeft = true;
			m_bKeyboardStrafeLeft = true;
			break;
		}
		case GLFW_KEY_D:
		{
			m_bKeyboardRight = true;
			m_bKeyboardStrafeRight = true;
			break;
		}
		case GLFW_KEY_SPACE:
		{
			m_bKeyboardSpace = true;
			break;
		}
	}
}

void VoxGame::KeyReleased(int key, int scancode, int mods)
{
	switch (key)
	{
		// Player movement
		case GLFW_KEY_W:
		{
			m_bKeyboardForward = false;
			break;
		}
		case GLFW_KEY_S:
		{
			m_bKeyboardBackward = false;
			break;
		}
		case GLFW_KEY_A:
		{
			m_bKeyboardLeft = false;
			m_bKeyboardStrafeLeft = false;
			break;
		}
		case GLFW_KEY_D:
		{
			m_bKeyboardRight = false;
			m_bKeyboardStrafeRight = false;
			break;
		}
		case GLFW_KEY_SPACE:
		{
			m_bKeyboardSpace = false;
			break;
		}
	}
}

void VoxGame::MouseLeftPressed()
{
	m_pGUI->MousePressed(MOUSE_BUTTON1);

	if (!m_pGUI->IsMouseInteractingWithGUIComponent(false))
	{
		m_currentX = m_pVoxWindow->GetCursorX();
		m_currentY = m_pVoxWindow->GetCursorY();
		m_pressedX = m_currentX;
		m_pressedY = m_currentY;

		m_bCameraRotate = true;
	}
}

void VoxGame::MouseLeftReleased()
{
	m_pGUI->MouseReleased(MOUSE_BUTTON1);

	m_bCameraRotate = false;
}

void VoxGame::MouseRightPressed()
{
	m_pGUI->MousePressed(MOUSE_BUTTON2);

	m_currentX = m_pVoxWindow->GetCursorX();
	m_currentY = m_pVoxWindow->GetCursorY();
	m_pressedX = m_currentX;
	m_pressedY = m_currentY;

	m_bCameraZoom = true;
}

void VoxGame::MouseRightReleased()
{
	m_pGUI->MouseReleased(MOUSE_BUTTON2);

	m_bCameraZoom = false;
}

void VoxGame::MouseMiddlePressed()
{
	m_pGUI->MousePressed(MOUSE_BUTTON3);
}

void VoxGame::MouseMiddleReleased()
{
	m_pGUI->MouseReleased(MOUSE_BUTTON3);
}

void VoxGame::MouseScroll(double x, double y)
{
}

// Mouse controls
void VoxGame::MouseCameraRotate(int x, int y)
{
	float changeX;
	float changeY;

	// The mouse hasn't moved so just return
	if ((m_currentX == x) && (m_currentY == y))
	{
		return;
	}

	// Calculate and scale down the change in position
	changeX = (x - m_currentX) / 5.0f;
	changeY = (y - m_currentY) / 5.0f;

	// Upside down
	if (m_pGameCamera->GetUp().y < 0.0f)
	{
		changeX = -changeX;
	}
	
	// Limit the rotation, so we can't go 'over' or 'under' the player with out rotations
	vec3 cameraFacing = m_pGameCamera->GetFacing();
	float dotResult = acos(dot(cameraFacing, vec3(0.0f, 1.0f, 0.0f)));
	float rotationDegrees = RadToDeg(dotResult) - 90.0f;
	float limitAngle = 75.0f;
	if ((rotationDegrees > limitAngle && changeY < 0.0f) || (rotationDegrees < -limitAngle && changeY > 0.0f))
	{
		changeY = 0.0f;
	}

	m_pGameCamera->RotateAroundPoint(changeY*0.75f, 0.0f, 0.0f);
	m_pGameCamera->RotateAroundPointY(-changeX*0.75f);

	m_currentX = x;
	m_currentY = y;
}

void VoxGame::MouseCameraZoom(int x, int y)
{
	float changeX;
	float changeY;

	// The mouse hasn't moved so just return
	if ((m_currentX == x) && (m_currentY == y))
	{
		return;
	}

	// Calculate and scale down the change in position
	changeX = (x - m_currentX) / 5.0f;
	changeY = (y - m_currentY) / 5.0f;

	m_pGameCamera->Zoom(changeY*0.05f);

	m_currentX = x;
	m_currentY = y;
}