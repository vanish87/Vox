#include <stdio.h>
#include <stdlib.h>

#include "VoxGame.h"


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