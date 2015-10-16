#include "../glew/include/GL/glew.h"

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#pragma comment (lib, "opengl32")
#pragma comment (lib, "glu32")

#include <stdio.h>
#include <stdlib.h>

#include "VoxGame.h"

#include "utils/Interpolator.h"

// Initialize the singleton instance
VoxGame *VoxGame::c_instance = 0;

VoxGame* VoxGame::GetInstance()
{
	if (c_instance == 0)
		c_instance = new VoxGame;

	return c_instance;
}

void VoxGame::Create()
{
	m_pVoxApplication = new VoxApplication();
	m_pVoxWindow = new VoxWindow();

	// Create application and window
	m_pVoxApplication->Create();
	m_pVoxWindow->Create();

	/* Setup the FPS and deltatime counters */
	QueryPerformanceCounter(&m_fpsPreviousTicks);
	QueryPerformanceCounter(&m_fpsCurrentTicks);
	QueryPerformanceFrequency(&m_fpsTicksPerSecond);
	m_deltaTime = 0.0f;
	m_fps = 0.0f;

	/* Create the renderer */
	int windowWidth = 800;
	int windowHeight = 800;
	m_pRenderer = new Renderer(windowWidth, windowHeight, 32, 8);

	/* Create cameras */
	m_pGameCamera = new Camera(m_pRenderer);
	m_pGameCamera->SetPosition(Vector3d(0.0f, 1.25f, 3.0f));
	m_pGameCamera->SetFacing(Vector3d(0.0f, 0.0f, -1.0f));
	m_pGameCamera->SetUp(Vector3d(0.0f, 1.0f, 0.0f));
	m_pGameCamera->SetRight(Vector3d(1.0f, 0.0f, 0.0f));

	/* Create viewports */
	m_pRenderer->CreateViewport(0, 0, windowWidth, windowHeight, 60.0f, &m_defaultViewport);

	/* Create fonts */
	m_pRenderer->CreateFreeTypeFont("media/fonts/arial.ttf", 12, &m_defaultFont);

	/* Create the qubicle binary file manager */
	m_pQubicleBinaryManager = new QubicleBinaryManager(m_pRenderer);

	/* Create test voxel character */
	m_pVoxelCharacter = new VoxelCharacter(m_pRenderer, m_pQubicleBinaryManager);
	char characterBaseFolder[128];
	char qbFilename[128];
	char ms3dFilename[128];
	char animListFilename[128];
	char facesFilename[128];
	char characterFilename[128];
	string modelName = "Steve";
	string typeName = "Human";
	sprintf_s(characterBaseFolder, 128, "media/gamedata/models");
	sprintf_s(qbFilename, 128, "media/gamedata/models/%s/%s.qb", typeName.c_str(), modelName.c_str());
	sprintf_s(ms3dFilename, 128, "media/gamedata/models/%s/%s.ms3d", typeName.c_str(), typeName.c_str());
	sprintf_s(animListFilename, 128, "media/gamedata/models/%s/%s.animlist", typeName.c_str(), typeName.c_str());
	sprintf_s(facesFilename, 128, "media/gamedata/models/%s/%s.faces", typeName.c_str(), modelName.c_str());
	sprintf_s(characterFilename, 128, "media/gamedata/models/%s/%s.character", typeName.c_str(), modelName.c_str());
	m_pVoxelCharacter->LoadVoxelCharacter(typeName.c_str(), qbFilename, ms3dFilename, animListFilename, facesFilename, characterFilename, characterBaseFolder);
	m_pVoxelCharacter->SetBreathingAnimationEnabled(true);
	m_pVoxelCharacter->SetWinkAnimationEnabled(true);
	m_pVoxelCharacter->SetTalkingAnimationEnabled(false);
	m_pVoxelCharacter->SetRandomMouthSelection(true);
	m_pVoxelCharacter->SetRandomLookDirection(true);
	m_pVoxelCharacter->SetWireFrameRender(false);
	m_pVoxelCharacter->SetCharacterScale(0.08f);

	modelWireframe = false;
	modelTalking = false;
	modelAnimationIndex = 0;
	multiSampling = true;
	weaponIndex = 0;
	weaponString = "NONE";
}

void VoxGame::Destroy()
{
	if (c_instance)
	{
		m_pVoxWindow->Destroy();
		m_pVoxApplication->Destroy();

		delete m_pVoxWindow;
		delete m_pVoxApplication;

		delete c_instance;
	}
}

void VoxGame::Update()
{
	// Update interpolator singleton
	Interpolator::GetInstance()->Update();

	// Delta time
	double timeNow = (double)timeGetTime() / 1000.0;
	static double timeOld = timeNow - (1.0 / 50.0);
	m_deltaTime = (float)timeNow - (float)timeOld;
	timeOld = timeNow;

	// FPS
	QueryPerformanceCounter(&m_fpsCurrentTicks);
	m_fps = 1.0f / ((float)(m_fpsCurrentTicks.QuadPart - m_fpsPreviousTicks.QuadPart) / (float)m_fpsTicksPerSecond.QuadPart);
	m_fpsPreviousTicks = m_fpsCurrentTicks;

	// Update the voxel model
	float animationSpeeds[AnimationSections_NUMSECTIONS] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	Matrix4x4 worldMatrix;
	m_pVoxelCharacter->Update(m_deltaTime, animationSpeeds);
	m_pVoxelCharacter->UpdateWeaponTrails(m_deltaTime, worldMatrix);

	// Update the application and window
	m_pVoxApplication->Update(m_deltaTime);
	m_pVoxWindow->Update(m_deltaTime);
}

void VoxGame::Render()
{
	// Begin rendering
	m_pRenderer->BeginScene(true, true, true);

		// ---------------------------------------
		// Render 3d
		// ---------------------------------------
		m_pRenderer->PushMatrix();
			// Set the default projection mode
			m_pRenderer->SetProjectionMode(PM_PERSPECTIVE, m_defaultViewport);

			// Set the lookat camera
			m_pGameCamera->Look();

			if(multiSampling)
			{
				m_pRenderer->EnableMultiSampling();
			}
			else
			{
				m_pRenderer->DisableMultiSampling();
			}

			Matrix4x4 worldMatrix;

			// Render the voxel character
			Colour OulineColour(1.0f, 1.0f, 0.0f, 1.0f);
			m_pRenderer->PushMatrix();
				m_pRenderer->MultiplyWorldMatrix(worldMatrix);

				m_pVoxelCharacter->RenderWeapons(false, false, false, OulineColour);
				m_pVoxelCharacter->Render(false, false, false, OulineColour, false);
			m_pRenderer->PopMatrix();

			// Render the voxel character Face
			m_pRenderer->PushMatrix();
				m_pRenderer->MultiplyWorldMatrix(worldMatrix);
				glActiveTextureARB(GL_TEXTURE0_ARB);
				glDisable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);

				m_pVoxelCharacter->RenderFace();
			m_pRenderer->PopMatrix();

		m_pRenderer->PopMatrix();

		// ---------------------------------------
		// Render 2d
		// ---------------------------------------
		char lFPSBuff[128];
		sprintf_s(lFPSBuff, "FPS: %.0f  Delta: %.4f", m_fps, m_deltaTime);
		char lAnimationBuff[128];
		sprintf_s(lAnimationBuff, "Animation [%i/%i]: %s", modelAnimationIndex, m_pVoxelCharacter->GetNumAnimations() - 1, m_pVoxelCharacter->GetAnimationName(modelAnimationIndex));
		char lWeaponBuff[128];
		sprintf_s(lWeaponBuff, "Weapon: %s", weaponString.c_str());

		m_pRenderer->PushMatrix();
			glActiveTextureARB(GL_TEXTURE0_ARB);
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);

			m_pRenderer->SetRenderMode(RM_SOLID);
			m_pRenderer->SetProjectionMode(PM_2D, m_defaultViewport);
			m_pRenderer->SetLookAtCamera(Vector3d(0.0f, 0.0f, 50.0f), Vector3d(0.0f, 0.0f, 0.0f), Vector3d(0.0f, 1.0f, 0.0f));

			m_pRenderer->RenderFreeTypeText(m_defaultFont, 15.0f, 15.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, lFPSBuff);

			m_pRenderer->RenderFreeTypeText(m_defaultFont, 335.0f, 35.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, lAnimationBuff);
			m_pRenderer->RenderFreeTypeText(m_defaultFont, 335.0f, 15.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, lWeaponBuff);

			m_pRenderer->RenderFreeTypeText(m_defaultFont, 635.0f, 95.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, "R - Toggle MSAA");
			m_pRenderer->RenderFreeTypeText(m_defaultFont, 635.0f, 75.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, "E - Toggle Talking");
			m_pRenderer->RenderFreeTypeText(m_defaultFont, 635.0f, 55.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, "W - Toggle Wireframe");
			m_pRenderer->RenderFreeTypeText(m_defaultFont, 635.0f, 35.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, "Q - Cycle Animations");
			m_pRenderer->RenderFreeTypeText(m_defaultFont, 635.0f, 15.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, "A - Cycle Weapons");
		m_pRenderer->PopMatrix();

	// End rendering
	m_pRenderer->EndScene();

	// Render the window
	m_pVoxWindow->Render();
}

void VoxGame::KeyPressed(int key, int scancode, int mods)
{
	switch(key)
	{
		case GLFW_KEY_W:
		{
			modelWireframe = !modelWireframe;
			m_pVoxelCharacter->SetWireFrameRender(modelWireframe);
			break;
		}
		case GLFW_KEY_E:
		{
			modelTalking = !modelTalking;
			m_pVoxelCharacter->SetTalkingAnimationEnabled(modelTalking);
			break;
		}
		case GLFW_KEY_Q:
		{
			modelAnimationIndex++;
			if (modelAnimationIndex >= m_pVoxelCharacter->GetNumAnimations())
			{
				modelAnimationIndex = 0;
			}

			m_pVoxelCharacter->PlayAnimation(AnimationSections_FullBody, false, AnimationSections_FullBody, m_pVoxelCharacter->GetAnimationName(modelAnimationIndex));
			break;
		}
		case GLFW_KEY_A:
		{
			switch (weaponIndex)
			{
			case 0:
			{
				weaponString = "Sword";
				m_pVoxelCharacter->LoadRightWeapon("media/gamedata/weapons/Sword/Sword.weapon");
				break;
			}
			case 1:
			{
				weaponString = "Sword & Shield";
				m_pVoxelCharacter->LoadLeftWeapon("media/gamedata/weapons/Shield/Shield.weapon");
				break;
			}
			case 2:
			{
				weaponString = "Staff";
				m_pVoxelCharacter->UnloadLeftWeapon();
				m_pVoxelCharacter->LoadRightWeapon("media/gamedata/weapons/Staff/Staff.weapon");
				break;
			}
			case 3:
			{
				weaponString = "Bow";
				m_pVoxelCharacter->UnloadRightWeapon();
				m_pVoxelCharacter->LoadLeftWeapon("media/gamedata/weapons/Bow/Bow.weapon");
				break;
			}
			case 4:
			{
				weaponString = "2HandedSword";
				m_pVoxelCharacter->UnloadLeftWeapon();
				m_pVoxelCharacter->LoadRightWeapon("media/gamedata/weapons/2HandedSword/2HandedSword.weapon");
				break;
			}
			case 5:
			{
				weaponString = "NONE";
				m_pVoxelCharacter->UnloadLeftWeapon();
				m_pVoxelCharacter->UnloadRightWeapon();
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

void VoxGame::KeyReleased(int key, int scancode, int mods)
{
	switch(key)
	{
	}
}

void VoxGame::MouseLeftPressed()
{
}

void VoxGame::MouseLeftReleased()
{
}

void VoxGame::MouseRightPressed()
{
}

void VoxGame::MouseRightReleased()
{
}

void VoxGame::MouseMiddlePressed()
{
}

void VoxGame::MouseMiddleReleased()
{
}

void VoxGame::MouseScroll(double x, double y)
{
}

void VoxGame::PollEvents()
{
	m_pVoxWindow->PollEvents();
}

bool VoxGame::ShouldClose()
{
	return (m_pVoxWindow->ShouldCloseWindow() == 1) || (m_pVoxApplication->ShouldCloseApplication() == 1);
}