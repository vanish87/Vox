#pragma once

#include <windows.h>

#include "VoxApplication.h"
#include "VoxWindow.h"

#include "Renderer/Renderer.h"
#include "Renderer/camera.h"
#include "models/VoxelCharacter.h"


class VoxGame
{
public:
	/* Public methods */
	static VoxGame* GetInstance();

	void Create();
	void Destroy();
	void Update();
	void Render();

	void KeyPressed(int key, int scancode, int mods);
	void KeyReleased(int key, int scancode, int mods);
	void MouseLeftPressed();
	void MouseLeftReleased();
	void MouseRightPressed();
	void MouseRightReleased();
	void MouseMiddlePressed();
	void MouseMiddleReleased();
	void MouseScroll(double x, double y);

	void PollEvents();
	bool ShouldClose();

protected:
	/* Protected methods */
	VoxGame() {};
	VoxGame(const VoxGame&) {};
	VoxGame &operator=(const VoxGame&) {};

private:
	/* Private methods */

public:
	/* Public members */

protected:
	/* Protected members */

private:
	/* Private members */
	VoxApplication* m_pVoxApplication;
	VoxWindow* m_pVoxWindow;

	Renderer* m_pRenderer;
	Camera* m_pGameCamera;
	QubicleBinaryManager* m_pQubicleBinaryManager;
	VoxelCharacter* m_pVoxelCharacter;

	// View ports
	unsigned int m_defaultViewport;

	// Fonts
	unsigned int m_defaultFont;

	// FPS and deltatime
	LARGE_INTEGER m_fpsPreviousTicks;
	LARGE_INTEGER m_fpsTicksPerSecond;
	LARGE_INTEGER m_fpsCurrentTicks;
	float m_deltaTime;
	float m_fps;

	bool modelWireframe;
	bool modelTalking;
	int modelAnimationIndex;
	bool multiSampling;
	int weaponIndex;
	string weaponString;

	// Singleton instance
	static VoxGame *c_instance;
};