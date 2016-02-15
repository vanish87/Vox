// ******************************************************************************
// Filename:	Player.h
// Project:	Vox
// Author:	Steven Ball
//
// Purpose:
//   The player class contains all the functioanlity of a player, an entity that
//   is controlled and can act within the world and game.
// 
// Revision History:
//   Initial Revision - 27/10/15
//
// Copyright (c) 2005-2015, Steven Ball
// ******************************************************************************

#pragma once

#include "../Renderer/Renderer.h"
#include "../blocks/ChunkManager.h"
#include "../models/VoxelCharacter.h"
#include "../Lighting/LightingManager.h"
#include "../Particles/BlockParticleManager.h"


class Player
{
public:
	/* Public methods */
	Player(Renderer* pRenderer, ChunkManager* pChunkManager, QubicleBinaryManager* pQubicleBinaryManager, LightingManager* pLightingManager, BlockParticleManager* pBlockParticleManager);
	~Player();

	// Get voxel character pointer
	VoxelCharacter* GetVoxelCharacter();

	// Accessors / Setters
	vec3 GetCenter();
	vec3 GetForwardVector();
	vec3 GetRightVector();
	vec3 GetUpVector();
	float GetRadius();
	void UpdateRadius();
	void SetForwardVector(vec3 forward);

	// Loading
	void LoadCharacter(string characterName);

	// Unloading
	void UnloadWeapon(bool left);

	// Collision
	bool CheckCollisions(vec3 positionCheck, vec3 previousPosition, vec3 *pNormal, vec3 *pMovement, bool *pStepUpBlock);

	// World
	void UpdateGridPosition();
	int GetGridX() const;
	int GetGridY() const;
	int GetGridZ() const;
	Chunk* GetCachedGridChunkOrFromPosition(vec3 pos);
	void ClearChunkCacheForChunk(Chunk* pChunk);

	// Movement
	vec3 GetPositionMovementAmount();
	vec3 MoveAbsolute(vec3 direction, const float speed, bool shouldChangeForward = true);
	void Move(const float speed);
	void Strafe(const float speed);
	void Levitate(const float speed);
	void StopMoving();
	void Jump();
	bool CanJump();

	// Combat
	void PressAttack();
	void ReleaseAttack();
	bool CanAttackLeft();
	bool CanAttackRight();

	// Rendering modes
	void SetWireFrameRender(bool wireframe);
	void SetPlayerAlpha(float alpha);

	// Rendering Helpers
	void CalculateWorldTransformMatrix();
	void RebuildVoxelCharacter(bool faceMerge);

	// Updating
	void Update(float dt);
	void UpdateAnimations(float dt);
	void UpdatePhysics(float dt);
	void UpdateTimers(float dt);
	void UpdateWeaponLights(float dt);
	void UpdateWeaponParticleEffects(float dt);

	// Rendering
	void Render();
	void RenderFirstPerson();
	void RenderWeaponTrails();
	void RenderFace();
	void RenderDebug();

protected:
	/* Protected methods */
	static void _StepUpAnimationFinished(void *apData);
	void StepUpAnimationFinished();

private:
	/* Private methods */

public:
	/* Public members */
	static const vec3 PLAYER_CENTER_OFFSET;

protected:
	/* Protected members */

private:
	/* Private members */
	Renderer* m_pRenderer;
	ChunkManager* m_pChunkManager;
	QubicleBinaryManager* m_pQubicleBinaryManager;
	LightingManager* m_pLightingManager;
	BlockParticleManager* m_pBlockParticleManager;

	// Player position and movement variables
	vec3 m_position;
	vec3 m_velocity;
	vec3 m_force;

	// Store previous position each frame after we have worked out the new position
	// Used for collision and other movement calculations
	vec3 m_previousPosition;

	// The direction of gravity for the player
	vec3 m_gravityDirection;

	// Keep track of how much we have changed position in the update, based on physics, etc.
	// So that the fake camera position can be updated, if we are in some kind of follow camera mode.
	vec3 m_positionMovementAmount;

	// Stepping up single world blocks by walking into them
	bool m_bDoStepUpAnimation;
	float m_stepUpAnimationYAmount;
	float m_stepUpAnimationPrevious;
	float m_stepUpAnimationYOffset;
	float m_stepUpAdditionYAmountChangedCache;

	// Grid position
	int m_gridPositionX;
	int m_gridPositionY;
	int m_gridPositionZ;

	// Cached chunk from grid position
	Chunk* m_pCachedGridChunk;

	// Flag to control if we are allowed to jump or not, reset when landing
	bool m_bCanJump;
	float m_jumpTimer;

	// Idle flag
	bool m_bIsIdle;

	// Combat
	bool m_bCanAttackLeft;
	bool m_bCanAttackRight;
	bool m_bCanInteruptCombatAnim;

	// Player radius
	float m_radius;

	// Local axis
	vec3 m_forward;
	vec3 m_right;
	vec3 m_up;

	// Target forward / looking vector
	vec3 m_targetForward;

	// Animation params
	bool m_animationFinished[AnimationSections_NUMSECTIONS];

	// Players world matrix
	Matrix4x4 m_worldMatrix;

	// Voxel character
	VoxelCharacter* m_pVoxelCharacter;
};
