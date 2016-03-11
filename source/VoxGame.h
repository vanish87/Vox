// ******************************************************************************
// Filename:    VoxGame.h
// Project:     Vox
// Author:      Steven Ball
//
// Purpose:
//   Vox game class.
//
// Revision History:
//   Initial Revision - 27/10/15
//
// Copyright (c) 2005-2015, Steven Ball
// ******************************************************************************

#pragma once

#include "Renderer/Renderer.h"
#include "gui/openglgui.h"
#include "Renderer/camera.h"
#include "Lighting/LightingManager.h"
#include "Particles/BlockParticleManager.h"
#include "Player/Player.h"
#include "Inventory/InventoryManager.h"
#include "Items/ItemManager.h"
#include "blocks/ChunkManager.h"
#include "frontend/FrontendManager.h"
#include "Skybox/Skybox.h"
#include "scenery/SceneryManager.h"
#include "Instance/InstanceManager.h"
#include "Projectile/ProjectileManager.h"
#include "TextEffects/TextEffectsManager.h"
#include "VoxApplication.h"
#include "VoxWindow.h"
#include "VoxSettings.h"
#include "GameGUI/ActionBar.h"
#include "GameGUI/CharacterGUI.h"
#include "GameGUI/CraftingGUI.h"
#include "GameGUI/InventoryGUI.h"
#include "GameGUI/LootGUI.h"

#ifdef __linux__
typedef struct POINT {
  float x;
  float y;
} POINT;
#endif //__linux__

// Game modes
enum GameMode
{
	GameMode_Debug = 0,
	GameMode_Loading,
	GameMode_FrontEnd,
	GameMode_Game,
};

// Camera modes
enum CameraMode
{
	CameraMode_Debug = 0,
	CameraMode_MouseRotate,
	CameraMode_AutoCamera,
	CameraMode_FirstPerson,
	CameraMode_NPCDialog,
};

class VoxGame
{
public:
	/* Public methods */
	static VoxGame* GetInstance();

	// Creation
	void Create(VoxSettings* pVoxSettings);

	// Destruction
	void Destroy();
	void DestroyGUI();

	// Quitting
	void CancelQuitPopup();
	void ShowQuitPopup();
	void SetGameQuit(bool quit);

	// Pause
	bool IsPaused();
	void SetPaused(bool pause);
	void SetPauseMenu();
	void UnsetPauseMenu();

	// Blur
	void SetGlobalBlurAmount(float blurAmount);

	// Cinematic letterbox
	void OpenLetterBox();
	void CloseLetterBox();

	// Paperdoll rendering
	void SetPaperdollRotation(float rotation);
	void RotatePaperdollModel(float rot);
	unsigned int GetDynamicPaperdollTexture();

	// Events
	void PollEvents();
	bool ShouldClose();

	// Window functionality
	int GetWindowCursorX();
	int GetWindowCursorY();
	void ResizeWindow(int width, int height);
	void CloseWindow();
	void UpdateJoySticks();

	// Controls
	void UpdateControls(float dt);
	void UpdateKeyboardControls(float dt);
	void UpdateMouseControls(float dt);
	void UpdateGamePadControls(float dt);

	// Camera controls
	void UpdateCamera(float dt);
	void UpdateCameraModeSwitching();
	void InitializeCameraRotation();
	void UpdateCameraAutoCamera(float dt, bool updateCameraPosition);
	void UpdateCameraFirstPerson(float dt);
	void UpdateCameraNPCDialog(float dt);
	void UpdateCameraClipping(float dt);
	void UpdateCameraZoom(float dt);
	bool ShouldRestorePreviousCameraMode();
	void SavePreviousCameraMode();
	void RestorePreviousCameraMode();

	// Input
	void KeyPressed(int key, int scancode, int mods);
	void KeyReleased(int key, int scancode, int mods);
	void CharacterEntered(int keyCode);
	void MouseLeftPressed();
	void MouseLeftReleased();
	void MouseRightPressed();
	void MouseRightReleased();
	void MouseMiddlePressed();
	void MouseMiddleReleased();
	void MouseScroll(double x, double y);
	void WrapCameraZoomValue();

	// Mouse controls
	void MouseCameraRotate();

	// Joystick controls
	void JoystickCameraMove(float dt);
	void JoystickCameraRotate(float dt);
	void JoystickCameraZoom(float dt);

	// Game functions
	void QuitToFrontEnd();
	void SetupDataForGame();
	void SetupDataForFrontEnd();
	void StartGameFromFrontEnd();
	void SetGameMode(GameMode mode);
	GameMode GetGameMode();
	void SetCameraMode(CameraMode mode);
	CameraMode GetCameraMode();

	// Interactions
	bool CheckInteractions();
	Item* GetInteractItem();

	// Updating
	void Update();
	void UpdatePlayerAlpha(float dt);
	void UpdateLights(float dt);
	void UpdateGUI(float dt);
	void UpdateGameGUI(float dt);

	// Rendering
	void PreRender();
	void BeginShaderRender();
	void EndShaderRender();
	void Render();
	void RenderSkybox();
	void RenderShadows();
	void RenderDeferredLighting();
	void RenderTransparency();
	void RenderSSAOTexture();
	void RenderFXAATexture();
	void RenderFirstPassFullScreen();
	void RenderSecondPassFullScreen();
	void RenderGUI();
	void RenderCinematicLetterBox();
	void RenderCrosshair();
	void RenderPaperdollViewport();
	void RenderDeferredRenderingPaperDoll();
	void RenderDebugInformation();

	// GUI Helper functions
	bool IsGUIWindowStillDisplayed();
	void CloseAllGUIWindows();
	void TurnCursorOn(bool resetCursorPosition);
	void TurnCursorOff();

	// GUI
	void CreateGUI();
	void SetupGUI();
	void SkinGUI();
	void UnSkinGUI();
	void GUITurnOffCursor();
	void ShowGUI();
	void HideGUI();
	void UpdateCharactersPulldown();
	void UpdateWeaponsPulldown();
	void UpdateAnimationsPulldown();
	void UpdateGUIThemePulldown();
	void AddConsoleLabel(string message);
	void ClearConsoleLabels();
	void UpdateConsoleLabels();

	// Accessors
	unsigned int GetDefaultViewport();
	FrontendManager* GetFrontendManager();
	BlockParticleManager* GetBlockParticleManager();
	CharacterGUI* GetCharacterGUI();

protected:
	/* Protected methods */
	VoxGame() {};
	VoxGame(const VoxGame&) {};
	VoxGame &operator=(const VoxGame&) {};

	// GUI callbacks
	static void _ToggleFullScreenPressed(void *apData);
	void ToggleFullScreenPressed();

	static void _PlayAnimationPressed(void *apData);
	void PlayAnimationPressed();

	static void _AnimationPullDownChanged(void *apData);
	void AnimationPullDownChanged();

	static void _WeaponPullDownChanged(void *apData);
	void WeaponPullDownChanged();

	static void _CharacterPullDownChanged(void *apData);
	void CharacterPullDownChanged();

	static void _GameModeChanged(void *apData);
	void GameModeChanged();

	static void _CameraModeChanged(void *apData);
	void CameraModeChanged();

	static void _GUIThemePullDownChanged(void *apData);
	void GUIThemePullDownChanged();

	static void _FaceMergeCheckboxChanged(void *apData);
	void FaceMergeCheckboxChanged();

	static void _StepUpdatePressed(void *apData);
	void StepUpdatePressed();

	static void _ConsoleReturnPressed(void *apData);
	void ConsoleReturnPressed();
	
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
	VoxSettings* m_pVoxSettings;

	// Renderer
	Renderer* m_pRenderer;

	// GUI
	OpenGLGUI* m_pGUI;

	// Game camera
	Camera* m_pGameCamera;

	// Qubicle binary manager
	QubicleBinaryManager* m_pQubicleBinaryManager;

	// Player
	Player* m_pPlayer;

	// Inventory
	InventoryManager* m_pInventoryManager;

	// Items
	ItemManager* m_pItemManager;

	// Projectile manager
	ProjectileManager* m_pProjectileManager;

	// Chunk manager
	ChunkManager* m_pChunkManager;

	// Scenery
	SceneryManager* m_pSceneryManager;

	// Lighting manager
	LightingManager* m_pLightingManager;

	// Skybox
	Skybox* m_pSkybox;

	// Block particle manager
	BlockParticleManager* m_pBlockParticleManager;

	// Text effects
	TextEffectsManager* m_pTextEffectsManager;

	// Instance manager
	InstanceManager* m_pInstanceManager;

	// Frontend manager
	FrontendManager* m_pFrontendManager;

	// Game mode
	GameMode m_gameMode;
	bool m_allowToChangeToGame;
	bool m_allowToChangeToFrontend;

	// Camera mode
	CameraMode m_cameraMode;
	CameraMode m_previousCameraMode;
	bool m_shouldRestorePreviousCameraMode;

	// Interacting item
	mutex m_interactItemMutex;
	Item* m_pInteractItem;

	// Window width and height
	int m_windowWidth;
	int m_windowHeight;

	// Quit message
	bool m_bGameQuit;

	// Paused
	bool m_bPaused;

	// View ports
	unsigned int m_defaultViewport;
	unsigned int m_paperdollViewport;

	// Fonts
	unsigned int m_defaultFont;

	// Lights
	unsigned int m_defaultLight;
	vec3 m_defaultLightPosition;
	vec3 m_defaultLightView;

	// Materials
	unsigned int m_defaultMaterial;

	// Frame buffers
	unsigned int m_SSAOFrameBuffer;
	unsigned int m_shadowFrameBuffer;
	unsigned int m_lightingFrameBuffer;
	unsigned int m_transparencyFrameBuffer;
	unsigned int m_FXAAFrameBuffer;
	unsigned int m_firstPassFullscreenBuffer;
	unsigned int m_secondPassFullscreenBuffer;
	unsigned int m_paperdollBuffer;
	unsigned int m_paperdollSSAOTextureBuffer;

	// Shaders
	unsigned int m_defaultShader;
	unsigned int m_phongShader;
	unsigned int m_SSAOShader;
	unsigned int m_shadowShader;
	unsigned int m_lightingShader;
	unsigned int m_cubeMapShader;
	unsigned int m_textureShader;
	unsigned int m_fxaaShader;
	unsigned int m_blurVerticalShader;
	unsigned int m_blurHorizontalShader;
	unsigned int m_paperdollShader;

	// Paperdoll viewport
	int m_paperdollViewportX;
	int m_paperdollViewportY;
	int m_paperdollViewportWidth;
	int m_paperdollViewportHeight;

	// FPS and deltatime
#ifdef _WIN32
	LARGE_INTEGER m_fpsPreviousTicks;
	LARGE_INTEGER m_fpsCurrentTicks;
	LARGE_INTEGER m_fpsTicksPerSecond;
#else
	double m_fpsPreviousTicks;
	double m_fpsCurrentTicks;
#endif //_WIN32
	float m_deltaTime;
	float m_fps;

	// Initial starting wait timer
	float m_initialWaitTimer;
	float m_initialWaitTime;
	bool m_initialStartWait;

	// Keyboard flags
	bool m_bKeyboardForward;
	bool m_bKeyboardBackward;
	bool m_bKeyboardStrafeLeft;
	bool m_bKeyboardStrafeRight;
	bool m_bKeyboardLeft;
	bool m_bKeyboardRight;
	bool m_bKeyboardUp;
	bool m_bKeyboardDown;
	bool m_bKeyboardSpace;
	bool m_bKeyboardMenu;

	// Joystick flags
	bool m_bJoystickJump;

	// Combat flags
	bool m_bAttackPressed_Mouse;
	bool m_bAttackReleased_Mouse;
	bool m_bCanDoAttack_Mouse;
	bool m_bAttackPressed_Joystick;
	bool m_bAttackReleased_Joystick;
	bool m_bCanDoAttack_Joystick;

	// Camera movement
	bool m_bCameraRotate;
	int m_pressedX;
	int m_pressedY;
	int m_currentX;
	int m_currentY;
	float m_cameraDistance;
	float m_maxCameraDistance;

	// Auto camera mode
	vec3 m_cameraPosition_AutoModeCached;
	vec3 m_cameraPosition_AutoMode;
	vec3 m_cameraBehindPlayerPosition;
	float m_autoCameraMovingModifier;

	// NPC dialog camera mode
	vec3 m_targetCameraView_NPCDialog;
	vec3 m_targetCameraPosition_NPCDialog;

	// Camera clipping
	vec3 m_targetCameraPositionBeforeClipping;
	vec3 m_cameraPositionAfterClipping;

	// Player movement
	bool m_keyboardMovement;
	bool m_gamepadMovement;
	vec3 m_movementDirection;
	float m_movementSpeed;
	float m_movementDragTime;
	float m_movementIncreaseTime;
	float m_maxMovementSpeed;
	float m_movementStopThreshold;

	// Blur
	float m_globalBlurAmount;

	// Cinematic letterbox mode
	float m_letterBoxRatio;

	// Paperdoll rendering
	float m_paperdollRenderRotation;

	// Game GUI
	InventoryGUI* m_pInventoryGUI;
	CharacterGUI* m_pCharacterGUI;
	LootGUI* m_pLootGUI;
	CraftingGUI* m_pCraftingGUI;
	ActionBar* m_pActionBar;

	// GUI Components
	bool m_GUICreated;
	GUIWindow* m_pMainWindow;
	CheckBox* m_pShadowsCheckBox;
	CheckBox* m_pSSAOCheckBox;
	CheckBox* m_pDynamicLightingCheckBox;
	CheckBox* m_pWireframeCheckBox;
	CheckBox* m_pMSAACheckBox;
	CheckBox* m_pDeferredCheckBox;
	CheckBox* m_pUpdateCheckBox;
	CheckBox* m_pBlurCheckBox;
	CheckBox* m_pDebugRenderCheckBox;
	CheckBox* m_pInstanceRenderCheckBox;
	Button* m_pFullscreenButton;
	Button* m_pPlayAnimationButton;
	PulldownMenu* m_pAnimationsPulldown;
	PulldownMenu* m_pWeaponsPulldown;
	PulldownMenu* m_pCharacterPulldown;
	GUIWindow* m_pGameWindow;
	OptionBox* m_pGameOptionBox;
	OptionBox* m_pDebugOptionBox;
	OptionBox* m_pFrontEndOptionBox;
	OptionController* m_pGameModeOptionController;
	PulldownMenu* m_pGUIThemePulldown;
	OptionBox* m_pDebugCameraOptionBox;
	OptionBox* m_pMouseRotateCameraOptionBox;
	OptionBox* m_pAutoCameraOptionBox;
	OptionBox* m_pFirstPersonCameraOptionBox;
	OptionController* m_pCameraModeOptionController;
	CheckBox* m_pFaceMergingCheckbox;
	CheckBox* m_pStepUpdateCheckbox;
	Button* m_pStepUpdateButton;
	GUIWindow* m_pConsoleWindow;
	TextBox* m_pConsoleTextbox;
	ScrollBar* m_pConsoleScrollbar;
	vector<Label*> m_vpConsoleLabels;
	vector<Label*> m_vpConsoleLabels_Add;
	vector<string> m_vStringCache;

	// Toggle flags
	bool m_deferredRendering;
	bool m_modelWireframe;
	int m_modelAnimationIndex;
	bool m_multiSampling;
	bool m_ssao;
	bool m_blur;
	bool m_shadows;
	bool m_dynamicLighting;
	bool m_animationUpdate;
	bool m_fullscreen;
	bool m_debugRender;
	bool m_instanceRender;

	// Singleton instance
	static VoxGame *c_instance;
};
