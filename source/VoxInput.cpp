#include <stdio.h>
#include <stdlib.h>

#include "VoxGame.h"
#include "VoxInput.h"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

}