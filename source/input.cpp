#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>

#include "models/VoxelCharacter.h"
#include "input.h"


extern bool modelWireframe;
extern bool modelTalking;
extern int modelAnimationIndex;
extern VoxelCharacter* pVoxelCharacter;
extern bool multiSampling;
extern int weaponIndex;
extern string weaponString;

void KeyPressed(GLFWwindow* window, int key, int scancode, int mods);
void KeyReleased(GLFWwindow* window, int key, int scancode, int mods);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action)
	{
        case GLFW_PRESS:
		{
			KeyPressed(window, key, scancode, mods);
			break;
		}
		case GLFW_RELEASE:
		{
			KeyReleased(window, key, scancode, mods);
			break;
		}
		case GLFW_REPEAT:
		{
			break;
		}
	}
}

void KeyPressed(GLFWwindow* window, int key, int scancode, int mods)
{
	switch(key)
	{
		case GLFW_KEY_W:
		{
			modelWireframe = !modelWireframe;
			pVoxelCharacter->SetWireFrameRender(modelWireframe);
			break;
		}
		case GLFW_KEY_E:
		{
			modelTalking = !modelTalking;
			pVoxelCharacter->SetTalkingAnimationEnabled(modelTalking);
			break;
		}
		case GLFW_KEY_Q:
		{
			modelAnimationIndex++;
			if (modelAnimationIndex >= pVoxelCharacter->GetNumAnimations())
			{
				modelAnimationIndex = 0;
			}

			pVoxelCharacter->PlayAnimation(AnimationSections_FullBody, false, AnimationSections_FullBody, pVoxelCharacter->GetAnimationName(modelAnimationIndex));
			break;
		}
		case GLFW_KEY_A:
		{
			switch(weaponIndex)
			{
				case 0:
				{
					weaponString = "Sword";
					pVoxelCharacter->LoadRightWeapon("media/gamedata/weapons/Sword/Sword.weapon");
					break;
				}
				case 1:
				{
					weaponString = "Sword & Shield";
					pVoxelCharacter->LoadLeftWeapon("media/gamedata/weapons/Shield/Shield.weapon");
					break;
				}
				case 2:
				{
					weaponString = "Staff";
					pVoxelCharacter->UnloadLeftWeapon();
					pVoxelCharacter->LoadRightWeapon("media/gamedata/weapons/Staff/Staff.weapon");
					break;
				}
				case 3:
				{
					weaponString = "Bow";
					pVoxelCharacter->UnloadRightWeapon();
					pVoxelCharacter->LoadLeftWeapon("media/gamedata/weapons/Bow/Bow.weapon");
					break;
				}
				case 4:
				{
					weaponString = "2HandedSword";
					pVoxelCharacter->UnloadLeftWeapon();
					pVoxelCharacter->LoadRightWeapon("media/gamedata/weapons/2HandedSword/2HandedSword.weapon");
					break;
				}
				case 5:
				{
					weaponString = "NONE";
					pVoxelCharacter->UnloadLeftWeapon();
					pVoxelCharacter->UnloadRightWeapon();
					break;
				}
			}

			weaponIndex++;
			if (weaponIndex == 6)
				weaponIndex = 0;

			break;
		}
		case GLFW_KEY_R:
		{
			multiSampling = !multiSampling;
			break;
		}
		
	}
}

void KeyReleased(GLFWwindow* window, int key, int scancode, int mods)
{
	switch(key)
	{
		case GLFW_KEY_W:
		{
			break;
		}
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	switch (action)
	{
		case GLFW_PRESS:
		{
			break;
		}
		case GLFW_RELEASE:
		{
			break;
		}
		case GLFW_REPEAT:
		{
			break;
		}
	}

	switch (button)
	{
		case GLFW_MOUSE_BUTTON_LEFT:
		{
			break;
		}
		case GLFW_MOUSE_BUTTON_RIGHT:
		{
			break;
		}
		case GLFW_MOUSE_BUTTON_MIDDLE:
		{
			break;
		}
	}
}