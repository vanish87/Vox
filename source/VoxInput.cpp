// ******************************************************************************
// Filename:    VoxInput.cpp
// Project:     Vox
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 27/10/15
//
// Copyright (c) 2005-2015, Steven Ball
// ******************************************************************************

#include <stdio.h>
#include <stdlib.h>

#include "VoxGame.h"


// Input callbacks
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

void CharacterCallback(GLFWwindow* window, unsigned int keyCode)
{
	VoxGame::GetInstance()->CharacterEntered(keyCode);
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

// Input
void VoxGame::KeyPressed(int key, int scancode, int mods)
{
	m_pGUI->KeyPressed(key, mods);

	if (m_pGUI->IsKeyboardInteractingWithGUIComponent())
	{
		return;  // For textbox entry
	}

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
		case GLFW_KEY_F:
		{
			m_bKeyboardUp = true;
			break;
		}
		case GLFW_KEY_V:
		{
			m_bKeyboardDown = true;
			break;
		}
		case GLFW_KEY_SPACE:
		{
			m_bKeyboardSpace = true;
			break;
		}
		case GLFW_KEY_ESCAPE:
		{
			m_bKeyboardMenu = true;
			break;
		}		
	}
}

void VoxGame::KeyReleased(int key, int scancode, int mods)
{
	m_pGUI->KeyReleased(key, mods);

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
		case GLFW_KEY_F:
		{
			m_bKeyboardUp = false;
			break;
		}
		case GLFW_KEY_V:
		{
			m_bKeyboardDown = false;
			break;
		}
		case GLFW_KEY_SPACE:
		{
			m_bKeyboardSpace = false;
			break;
		}
		case GLFW_KEY_ESCAPE:
		{
			m_bKeyboardMenu = false;
			break;
		}
		case GLFW_KEY_O:
		{
			m_pDebugCameraOptionBox->SetToggled(true);
			CameraModeChanged();
			break;
		}
		case GLFW_KEY_L:
		{
			SetPaused(!IsPaused());
			break;
		}
		case GLFW_KEY_P:
		{
			if (m_pGUI->IsKeyboardInteractingWithGUIComponent() == false)
			{
				if (m_pMainWindow->IsVisible() == false)
				{
					ShowGUI();
				}
				else
				{
					HideGUI();
				}

				break;
			}
		}		
	}
}

void VoxGame::CharacterEntered(int keyCode)
{
	m_pGUI->CharacterEntered(keyCode);
}

void VoxGame::MouseLeftPressed()
{
	if (m_pVoxWindow->IsCursorOn())
	{
		m_pGUI->MousePressed(MOUSE_BUTTON1);
	}

	if (m_pVoxWindow->IsCursorOn() == false || !m_pGUI->IsMouseInteractingWithGUIComponent(false))
	{
		m_currentX = m_pVoxWindow->GetCursorX();
		m_currentY = m_pVoxWindow->GetCursorY();
		m_pressedX = m_currentX;
		m_pressedY = m_currentY;

		if (m_gameMode == GameMode_Debug || m_cameraMode == CameraMode_Debug)
		{
			// Turn cursor off
			if (m_pVoxWindow->IsCursorOn() == true)
			{
				m_pVoxWindow->TurnCursorOff();
			}

			m_bCameraRotate = true;
		}
		else if (m_gameMode == GameMode_Game)
		{
			m_bAttackPressed_Mouse = true;
		}
	}
}

void VoxGame::MouseLeftReleased()
{
	if (m_pVoxWindow->IsCursorOn())
	{
		m_pGUI->MouseReleased(MOUSE_BUTTON1);
	}

	if (m_gameMode == GameMode_Debug || m_cameraMode == CameraMode_Debug)
	{
		if (!m_pGUI->IsMouseInteractingWithGUIComponent(false))
		{
			// Turn cursor on
			if (m_pVoxWindow->IsCursorOn() == false)
			{
				m_pVoxWindow->TurnCursorOn(true);
			}
		}

		m_bCameraRotate = false;
	}
	else if (m_gameMode == GameMode_Game)
	{
		m_bAttackReleased_Mouse = true;
		m_bAttackPressed_Mouse = false;
		m_bCanDoAttack_Mouse = true;
	}
}

void VoxGame::MouseRightPressed()
{
	if (m_pVoxWindow->IsCursorOn())
	{
		m_pGUI->MousePressed(MOUSE_BUTTON2);
	}

	if (m_pVoxWindow->IsCursorOn() == false || !m_pGUI->IsMouseInteractingWithGUIComponent(false))
	{
		m_currentX = m_pVoxWindow->GetCursorX();
		m_currentY = m_pVoxWindow->GetCursorY();
		m_pressedX = m_currentX;
		m_pressedY = m_currentY;
	}
}

void VoxGame::MouseRightReleased()
{
	if (m_pVoxWindow->IsCursorOn())
	{
		m_pGUI->MouseReleased(MOUSE_BUTTON2);
	}
}

void VoxGame::MouseMiddlePressed()
{
	if (m_pVoxWindow->IsCursorOn())
	{
		m_pGUI->MousePressed(MOUSE_BUTTON3);
	}
}

void VoxGame::MouseMiddleReleased()
{
	if (m_pVoxWindow->IsCursorOn())
	{
		m_pGUI->MouseReleased(MOUSE_BUTTON3);
	}
}

void VoxGame::MouseScroll(double x, double y)
{
	if (m_pVoxWindow->IsCursorOn() == false || !m_pGUI->IsMouseInteractingWithGUIComponent(false))
	{
		if (m_cameraMode != CameraMode_FirstPerson)
		{
			m_maxCameraDistance += (float)(-y*0.5f);

			WrapCameraZoomValue();
		}
		else
		{
			if (y < 0.0f)
			{
				m_cameraDistance = 2.0f;
				m_maxCameraDistance = 2.0f;
			}
		}
	}
}

void VoxGame::WrapCameraZoomValue()
{
	float minAmount = 0.5f;
	float maxAmount = 15.0f;

	// Camera rotation modes
	if (m_gameMode == GameMode_Game && (m_cameraMode == CameraMode_AutoCamera || m_cameraMode == CameraMode_MouseRotate))
	{
		minAmount = 1.0f;
		maxAmount = 15.0f;
	}

	// First person mode
	if (m_gameMode == GameMode_Game && m_cameraMode == CameraMode_FirstPerson)
	{
		minAmount = 1.25f;
		maxAmount = 1.75f;
	}

	if (m_maxCameraDistance <= minAmount)
	{
		m_maxCameraDistance = minAmount;
	}

	if (m_maxCameraDistance >= maxAmount)
	{
		m_maxCameraDistance = maxAmount;
	}
}

// Mouse controls
void VoxGame::MouseCameraRotate()
{
	int x = m_pVoxWindow->GetCursorX();
	int y = m_pVoxWindow->GetCursorY();

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
	
	// First person mode
	if (m_cameraMode == CameraMode_FirstPerson)
	{
		changeY = -changeY;
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

	if (m_cameraMode == CameraMode_FirstPerson)
	{
		m_pGameCamera->Rotate(changeY*0.75f, 0.0f, 0.0f);
		m_pGameCamera->RotateY(-changeX*0.75f);
	}
	else
	{
		m_pGameCamera->RotateAroundPoint(changeY*0.75f, 0.0f, 0.0f, true);
		m_pGameCamera->RotateAroundPointY(-changeX*0.75f, true);
	}

	m_currentX = x;
	m_currentY = y;
}

// Joystick controls
void VoxGame::JoystickCameraMove(float dt)
{
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

	float changeX = axisX * 10.0f * dt;
	float changeY = axisY * 10.0f * dt;

	m_pGameCamera->Fly(-changeY);
	m_pGameCamera->Strafe(changeX);
}

void VoxGame::JoystickCameraRotate(float dt)
{
	float axisX = m_pVoxWindow->GetJoystickAxisValue(0, 4);
	float axisY = m_pVoxWindow->GetJoystickAxisValue(0, 3);

	// Dead zones
	if (fabs(axisX) < m_pVoxWindow->GetJoystickAnalogDeadZone())
	{
		axisX = 0.0f;
	}
	if (fabs(axisY) < m_pVoxWindow->GetJoystickAnalogDeadZone())
	{
		axisY = 0.0f;
	}

	float changeX = axisX * 150.0f * dt;
	float changeY = axisY * 150.0f * dt;

	// Upside down
	if (m_pGameCamera->GetUp().y < 0.0f)
	{
		changeX = -changeX;
	}

	// First person mode
	if (m_cameraMode == CameraMode_FirstPerson)
	{
		changeY = -changeY;
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

	if (m_cameraMode == CameraMode_FirstPerson)
	{
		m_pGameCamera->Rotate(changeY, 0.0f, 0.0f);
		m_pGameCamera->RotateY(-changeX);
	}
	else
	{
		m_pGameCamera->RotateAroundPoint(changeY, 0.0f, 0.0f, true);
		m_pGameCamera->RotateAroundPointY(-changeX, true);
	}
}

void VoxGame::JoystickCameraZoom(float dt)
{
	bool zoomOut = m_pVoxWindow->GetJoystickButton(0, 4);
	bool zoomIn = m_pVoxWindow->GetJoystickButton(0, 5);

	float zoomAmount = 0.0f;
	if (zoomIn)
	{
		zoomAmount = 10.0f;
	}
	if (zoomOut)
	{
		zoomAmount = -10.0f;
	}

	float changeY = zoomAmount * dt;

	if (m_cameraMode != CameraMode_FirstPerson)
	{
		m_maxCameraDistance += (float)(-changeY);

		WrapCameraZoomValue();
	}
	else
	{
		if (changeY < 0.0f)
		{
			m_cameraDistance = 2.0f;
			m_maxCameraDistance = 2.0f;
		}
	}

	WrapCameraZoomValue();
}
