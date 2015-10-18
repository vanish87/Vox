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
	m_pRenderer = NULL;
	m_pGameCamera = NULL;
	m_pQubicleBinaryManager = NULL;
	m_pVoxelCharacter = NULL;

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
	m_windowWidth = 800;
	m_windowHeight = 800;
	m_pRenderer = new Renderer(m_windowWidth, m_windowHeight, 32, 8);

	/* Create cameras */
	m_pGameCamera = new Camera(m_pRenderer);
	m_pGameCamera->SetPosition(Vector3d(0.0f, 1.25f, 3.0f));
	m_pGameCamera->SetFacing(Vector3d(0.0f, 0.0f, -1.0f));
	m_pGameCamera->SetUp(Vector3d(0.0f, 1.0f, 0.0f));
	m_pGameCamera->SetRight(Vector3d(1.0f, 0.0f, 0.0f));

	/* Create viewports */
	m_pRenderer->CreateViewport(0, 0, m_windowWidth, m_windowHeight, 60.0f, &m_defaultViewport);

	/* Create fonts */
	m_pRenderer->CreateFreeTypeFont("media/fonts/arial.ttf", 12, &m_defaultFont);

	/* Create lights */

	/* Create materials */

	/* Create the frame buffers */
	bool frameBufferCreated = false;
	frameBufferCreated = m_pRenderer->CreateFrameBuffer(-1, true, true, true, true, m_windowWidth, m_windowHeight, 1.0f, "SSAO", &m_SSAOFrameBuffer);
	frameBufferCreated = m_pRenderer->CreateFrameBuffer(-1, true, true, true, true, m_windowWidth, m_windowHeight, 5.0f, "Shadow", &m_shadowFrameBuffer);

	/* Create the shaders */
	m_defaultShader = -1;
	m_SSAOShader = -1;
	m_shadowShader = -1;
	m_pRenderer->LoadGLSLShader("media/shaders/default.vertex", "media/shaders/default.pixel", &m_defaultShader);
	m_pRenderer->LoadGLSLShader("media/shaders/fullscreen/SSAO.vertex", "media/shaders/fullscreen/SSAO.pixel", &m_SSAOShader);
	m_pRenderer->LoadGLSLShader("media/shaders/shadow.vertex", "media/shaders/shadow.pixel", &m_shadowShader);

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
	m_pVoxelCharacter->GetRightWeapon()->StartWeaponTrails();
	m_pVoxelCharacter->GetLeftWeapon()->StartWeaponTrails();

	// Keyboard movement
	m_bKeyboardForward = false;
	m_bKeyboardBackward = false;
	m_bKeyboardStrafeLeft = false;
	m_bKeyboardStrafeRight = false;
	m_bKeyboardLeft = false;
	m_bKeyboardRight = false;
	m_bKeyboardUp = false;
	m_bKeyboardDown = false;
	m_bKeyboardSpace = false;

	// Camera movement
	m_bCameraRotate = false;

	m_pressedX = 0;
	m_pressedY = 0;	
	m_currentX = 0;
	m_currentY = 0;

	// Toggle flags
	m_renderModeIndex = 0;
	m_renderModeString = "SSAO";
	m_displayHelpText = true;
	m_modelWireframe = false;
	m_modelTalking = false;
	m_modelAnimationIndex = 0;
	m_multiSampling = true;
	m_weaponIndex = 0;
	m_weaponString = "NONE";
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

	// Update controls
	UpdateControls(m_deltaTime);

	// Update the application and window
	m_pVoxApplication->Update(m_deltaTime);
	m_pVoxWindow->Update(m_deltaTime);
}

void VoxGame::Render()
{
	// Begin rendering
	m_pRenderer->BeginScene(true, true, true);

		// SSAO frame buffer rendering start
		if (m_renderModeIndex == 0)
		{
			m_pRenderer->StartRenderingToFrameBuffer(m_SSAOFrameBuffer);
		}

		// ---------------------------------------
		// Render 3d
		// ---------------------------------------
		m_pRenderer->PushMatrix();
			// Set the default projection mode
			m_pRenderer->SetProjectionMode(PM_PERSPECTIVE, m_defaultViewport);
			
			// Set back culling as default
			m_pRenderer->SetCullMode(CM_BACK);

			// Set the lookat camera
			m_pGameCamera->Look();

			// Multisampling MSAA
			if(m_multiSampling)
			{
				m_pRenderer->EnableMultiSampling();
			}
			else
			{
				m_pRenderer->DisableMultiSampling();
			}

			m_pRenderer->BeginGLSLShader(m_defaultShader);

			Matrix4x4 worldMatrix;

			// Render the voxel character
			Colour OulineColour(1.0f, 1.0f, 0.0f, 1.0f);
			m_pRenderer->PushMatrix();
				m_pRenderer->MultiplyWorldMatrix(worldMatrix);

				m_pVoxelCharacter->RenderWeapons(false, false, false, OulineColour);
				m_pVoxelCharacter->Render(false, false, false, OulineColour, false);
			m_pRenderer->PopMatrix();

			m_pRenderer->EndGLSLShader(m_defaultShader);

			// Render the voxel character face
			m_pRenderer->PushMatrix();
				m_pRenderer->MultiplyWorldMatrix(worldMatrix);
				glActiveTextureARB(GL_TEXTURE0_ARB);
				glDisable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);

				m_pVoxelCharacter->RenderFace();
			m_pRenderer->PopMatrix();

			// Render the voxel character weapon trails
			m_pRenderer->PushMatrix();
				m_pVoxelCharacter->RenderWeaponTrails();
			m_pRenderer->PopMatrix();

		m_pRenderer->PopMatrix();

		m_pRenderer->PushMatrix();
			// ---------------------------------------
			// Render 2d
			// ---------------------------------------
		m_pRenderer->PopMatrix();

		// SSAO frame buffer rendering stop
		if (m_renderModeIndex == 0)
		{
			m_pRenderer->StopRenderingToFrameBuffer(m_SSAOFrameBuffer);
		}

		// Render the SSAO texture
		if(m_renderModeIndex == 0)
		{
			RenderSSAOTexture();
		}

		// Render debug information and text
		RenderDebugInformation();

	// End rendering
	m_pRenderer->EndScene();

	// Render the window
	m_pVoxWindow->Render();
}

void VoxGame::PollEvents()
{
	m_pVoxWindow->PollEvents();
}

bool VoxGame::ShouldClose()
{
	return (m_pVoxWindow->ShouldCloseWindow() == 1) || (m_pVoxApplication->ShouldCloseApplication() == 1);
}

// Window functionality
void VoxGame::ResizeWindow(int width, int height)
{
	m_windowWidth = width;
	m_windowHeight = height;

	m_pVoxWindow->ResizeWindow(m_windowWidth, m_windowHeight);

	if(m_pRenderer)
	{
		// Let the renderer know we have resized the window
		m_pRenderer->ResizeWindow(m_windowWidth, m_windowHeight);

		// Resize the main viewport
		m_pRenderer->ResizeViewport(m_defaultViewport, 0, 0, m_windowWidth, m_windowHeight, 60.0f);

		// Resize the frame buffers
		bool frameBufferResize = false;
		frameBufferResize = m_pRenderer->CreateFrameBuffer(m_SSAOFrameBuffer, true, true, true, true, m_windowWidth, m_windowHeight, 1.0f, "SSAO", &m_SSAOFrameBuffer);
		frameBufferResize = m_pRenderer->CreateFrameBuffer(m_shadowFrameBuffer, true, true, true, true, m_windowWidth, m_windowHeight, 5.0f, "Shadow", &m_shadowFrameBuffer);
	}
}

// Controls
void VoxGame::UpdateControls(float dt)
{
	int x = m_pVoxWindow->GetCursorX();
	int y = m_pVoxWindow->GetCursorY();

	// Keyboard movements
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

	// Camera movements
	if (m_bCameraRotate)
	{
		MouseCameraRotate(x, y);
	}
}

void VoxGame::KeyPressed(int key, int scancode, int mods)
{
	switch(key)
	{
		case GLFW_KEY_UP:
		{
			m_bKeyboardForward = true;
			break;
		}
		case GLFW_KEY_DOWN:
		{
			m_bKeyboardBackward = true;
			break;
		}
		case GLFW_KEY_LEFT:
		{
			m_bKeyboardStrafeLeft = true;
			break;
		}
		case GLFW_KEY_RIGHT:
		{
			m_bKeyboardStrafeRight = true;
			break;
		}
	}
}

void VoxGame::KeyReleased(int key, int scancode, int mods)
{
	switch(key)
	{
		case GLFW_KEY_UP:
		{
			m_bKeyboardForward = false;
			break;
		}
		case GLFW_KEY_DOWN:
		{
			m_bKeyboardBackward = false;
			break;
		}
		case GLFW_KEY_LEFT:
		{
			m_bKeyboardStrafeLeft = false;
			break;
		}
		case GLFW_KEY_RIGHT:
		{
			m_bKeyboardStrafeRight = false;
			break;
		}

		case GLFW_KEY_H:
		{
			m_displayHelpText = !m_displayHelpText;
			break;
		}
		case GLFW_KEY_W:
		{
			m_modelWireframe = !m_modelWireframe;
			m_pVoxelCharacter->SetWireFrameRender(m_modelWireframe);
			break;
		}
		case GLFW_KEY_E:
		{
			m_modelTalking = !m_modelTalking;
			m_pVoxelCharacter->SetTalkingAnimationEnabled(m_modelTalking);
			break;
		}
		case GLFW_KEY_Q:
		{
			m_modelAnimationIndex++;
			if(m_modelAnimationIndex >= m_pVoxelCharacter->GetNumAnimations())
			{
				m_modelAnimationIndex = 0;
			}

			m_pVoxelCharacter->PlayAnimation(AnimationSections_FullBody, false, AnimationSections_FullBody, m_pVoxelCharacter->GetAnimationName(m_modelAnimationIndex));
			break;
		}
		case GLFW_KEY_Z:
		{
			m_pVoxelCharacter->PlayAnimation(AnimationSections_FullBody, false, AnimationSections_FullBody, m_pVoxelCharacter->GetAnimationName(m_modelAnimationIndex));
			break;
		}
		case GLFW_KEY_R:
		{
			m_multiSampling = !m_multiSampling;
			break;
		}
		case GLFW_KEY_T:
		{
			m_renderModeIndex++;
			if(m_renderModeIndex >= 2)
			{
				m_renderModeIndex = 0;
			}

			if (m_renderModeIndex == 0)
				m_renderModeString = "SSAO";
			if (m_renderModeIndex == 1)
				m_renderModeString = "Normal";

			break;
		}
		case GLFW_KEY_A:
		{
			switch(m_weaponIndex)
			{
				case 0:
				{
					m_weaponString = "Sword";
					m_pVoxelCharacter->LoadRightWeapon("media/gamedata/weapons/Sword/Sword.weapon");
					break;
				}
				case 1:
				{
					m_weaponString = "Sword & Shield";
					m_pVoxelCharacter->LoadLeftWeapon("media/gamedata/weapons/Shield/Shield.weapon");
					break;
				}
				case 2:
				{
					m_weaponString = "Staff";
					m_pVoxelCharacter->UnloadLeftWeapon();
					m_pVoxelCharacter->LoadRightWeapon("media/gamedata/weapons/Staff/Staff.weapon");
					break;
				}
				case 3:
				{
					m_weaponString = "Bow";
					m_pVoxelCharacter->UnloadRightWeapon();
					m_pVoxelCharacter->LoadLeftWeapon("media/gamedata/weapons/Bow/Bow.weapon");
					break;
				}
				case 4:
				{
					m_weaponString = "2HandedSword";
					m_pVoxelCharacter->UnloadLeftWeapon();
					m_pVoxelCharacter->LoadRightWeapon("media/gamedata/weapons/2HandedSword/2HandedSword.weapon");
					break;
				}
				case 5:
				{
					m_weaponString = "NONE";
					m_pVoxelCharacter->UnloadLeftWeapon();
					m_pVoxelCharacter->UnloadRightWeapon();
					break;
				}
			}

			m_weaponIndex++;
			if(m_weaponIndex == 6)
				m_weaponIndex = 0;

			break;
		}
	}
}

void VoxGame::MouseLeftPressed()
{
	m_currentX = m_pVoxWindow->GetCursorX();
	m_currentY = m_pVoxWindow->GetCursorY();
	m_pressedX = m_currentX;
	m_pressedY = m_currentY;

	m_bCameraRotate = true;
}

void VoxGame::MouseLeftReleased()
{
	m_bCameraRotate = false;
}

void VoxGame::MouseRightPressed()
{
	m_currentX = m_pVoxWindow->GetCursorX();
	m_currentY = m_pVoxWindow->GetCursorY();
	m_pressedX = m_currentX;
	m_pressedY = m_currentY;
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

	m_pGameCamera->RotateAroundPoint(changeY*0.5f, -changeX*0.5f, 3.0f);

	m_currentX = x;
	m_currentY = y;
}


// Rendering
void VoxGame::RenderSSAOTexture()
{
	m_pRenderer->PushMatrix();
		m_pRenderer->SetProjectionMode(PM_2D, m_defaultViewport);

		m_pRenderer->SetLookAtCamera(Vector3d(0.0f, 0.0f, 250.0f), Vector3d(0.0f, 0.0f, 0.0f), Vector3d(0.0f, 1.0f, 0.0f));

		// SSAO
		m_pRenderer->BeginGLSLShader(m_SSAOShader);
		glShader* pShader = m_pRenderer->GetShader(m_SSAOShader);

		unsigned int textureId = glGetUniformLocationARB(pShader->GetProgramObject(), "bgl_DepthTexture");
		glActiveTextureARB(GL_TEXTURE0_ARB);
		m_pRenderer->BindRawTextureId(m_pRenderer->GetDepthTextureFromFrameBuffer(m_SSAOFrameBuffer));
		glUniform1iARB(textureId, 0);

		unsigned int textureId2 = glGetUniformLocationARB(pShader->GetProgramObject(), "bgl_RenderedTexture");
		glActiveTextureARB(GL_TEXTURE1_ARB);
		m_pRenderer->BindRawTextureId(m_pRenderer->GetDiffuseTextureFromFrameBuffer(m_SSAOFrameBuffer));
		glUniform1iARB(textureId2, 1);

		unsigned int textureId3 = glGetUniformLocationARB(pShader->GetProgramObject(), "light");
		glActiveTextureARB(GL_TEXTURE2_ARB);
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		pShader->setUniform1i("screenWidth", (int)(m_windowWidth*5.0f));
		pShader->setUniform1i("screenHeight", (int)(m_windowHeight*5.0f));
		pShader->setUniform1f("nearZ", 0.01f);
		pShader->setUniform1f("farZ", 1000.0f);

		pShader->setUniform1f("samplingMultiplier", 1.5f);

		m_pRenderer->SetRenderMode(RM_TEXTURED);
			m_pRenderer->EnableImmediateMode(IM_QUADS);
			m_pRenderer->ImmediateTextureCoordinate(0.0f, 0.0f);
			m_pRenderer->ImmediateVertex(0.0f, 0.0f, 1.0f);
			m_pRenderer->ImmediateTextureCoordinate(1.0f, 0.0f);
			m_pRenderer->ImmediateVertex((float)m_windowWidth, 0.0f, 1.0f);
			m_pRenderer->ImmediateTextureCoordinate(1.0f, 1.0f);
			m_pRenderer->ImmediateVertex((float)m_windowWidth, (float)m_windowHeight, 1.0f);
			m_pRenderer->ImmediateTextureCoordinate(0.0f, 1.0f);
			m_pRenderer->ImmediateVertex(0.0f, (float)m_windowHeight, 1.0f);
		m_pRenderer->DisableImmediateMode();

		glActiveTextureARB(GL_TEXTURE2_ARB);
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTextureARB(GL_TEXTURE1_ARB);
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTextureARB(GL_TEXTURE0_ARB);
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		m_pRenderer->EndGLSLShader(m_SSAOShader);
	m_pRenderer->PopMatrix();
}

void VoxGame::RenderDebugInformation()
{
	char lCameraBuff[256];
	sprintf_s(lCameraBuff, 256, "Pos(%.2f, %.2f, %.2f), Facing(%.2f, %.2f, %.2f) = %.2f, Up(%.2f, %.2f, %.2f) = %.2f, Right(%.2f, %.2f, %.2f) = %.2f",
		m_pGameCamera->GetPosition().x, m_pGameCamera->GetPosition().y, m_pGameCamera->GetPosition().z,
		m_pGameCamera->GetFacing().x, m_pGameCamera->GetFacing().y, m_pGameCamera->GetFacing().z, m_pGameCamera->GetFacing().GetLength(),
		m_pGameCamera->GetUp().x, m_pGameCamera->GetUp().y, m_pGameCamera->GetUp().z, m_pGameCamera->GetUp().GetLength(),
		m_pGameCamera->GetRight().x, m_pGameCamera->GetRight().y, m_pGameCamera->GetRight().z, m_pGameCamera->GetRight().GetLength());
	char lFPSBuff[128];
	sprintf_s(lFPSBuff, "FPS: %.0f  Delta: %.4f", m_fps, m_deltaTime);
	char lAnimationBuff[128];
	sprintf_s(lAnimationBuff, "Animation [%i/%i]: %s", m_modelAnimationIndex, m_pVoxelCharacter->GetNumAnimations() - 1, m_pVoxelCharacter->GetAnimationName(m_modelAnimationIndex));
	char lWeaponBuff[128];
	sprintf_s(lWeaponBuff, "Weapon: %s", m_weaponString.c_str());

	int l_nTextHeight = m_pRenderer->GetFreeTypeTextHeight(m_defaultFont, "a");

	m_pRenderer->PushMatrix();
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		m_pRenderer->SetRenderMode(RM_SOLID);
		m_pRenderer->SetProjectionMode(PM_2D, m_defaultViewport);
		m_pRenderer->SetLookAtCamera(Vector3d(0.0f, 0.0f, 250.0f), Vector3d(0.0f, 0.0f, 0.0f), Vector3d(0.0f, 1.0f, 0.0f));

		m_pRenderer->RenderFreeTypeText(m_defaultFont, 15.0f, 15.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, lFPSBuff);

		if (m_displayHelpText)
		{
			m_pRenderer->RenderFreeTypeText(m_defaultFont, 15.0f, m_windowHeight - l_nTextHeight - 10.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, lCameraBuff);

			m_pRenderer->RenderFreeTypeText(m_defaultFont, (int)(m_windowWidth * 0.5f) - 75.0f, 35.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, lAnimationBuff);
			m_pRenderer->RenderFreeTypeText(m_defaultFont, (int)(m_windowWidth * 0.5f) - 75.0f, 15.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, lWeaponBuff);

			m_pRenderer->RenderFreeTypeText(m_defaultFont, m_windowWidth - 150.0f, 155.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, "T - Render Mode [%s]", m_renderModeString.c_str());
			m_pRenderer->RenderFreeTypeText(m_defaultFont, m_windowWidth - 150.0f, 135.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, "R - Toggle MSAA [%s]", m_multiSampling ? "On" : "Off");
			m_pRenderer->RenderFreeTypeText(m_defaultFont, m_windowWidth - 150.0f, 115.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, "E - Toggle Talking [%s]", m_modelTalking ? "On" : "Off");
			m_pRenderer->RenderFreeTypeText(m_defaultFont, m_windowWidth - 150.0f, 95.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, "H - Toggle HelpText");
			m_pRenderer->RenderFreeTypeText(m_defaultFont, m_windowWidth - 150.0f, 75.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, "W - Toggle Wireframe");
			m_pRenderer->RenderFreeTypeText(m_defaultFont, m_windowWidth - 150.0f, 55.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, "Q - Cycle Animations");
			m_pRenderer->RenderFreeTypeText(m_defaultFont, m_windowWidth - 150.0f, 35.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, "A - Cycle Weapons");
			m_pRenderer->RenderFreeTypeText(m_defaultFont, m_windowWidth - 150.0f, 15.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, "Z - Play Animation");
		}
	m_pRenderer->PopMatrix();
}