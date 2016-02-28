// ******************************************************************************
// Filename:    Player.h
// Project:     Vox
// Author:      Steven Ball
//
// Purpose:
//   The player class contains all the functioanlity of a player, an entity that
//   is controlled and can act within the world and game.
// 
// Revision History:
//   Initial Revision - 27/10/15
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#pragma once

#include "../Renderer/Renderer.h"
#include "../blocks/ChunkManager.h"
#include "../models/VoxelCharacter.h"
#include "../Lighting/LightingManager.h"
#include "../Particles/BlockParticleManager.h"
#include "../Inventory/InventoryManager.h"
#include "../Items/ItemManager.h"

class ProjectileManager;
class Projectile;
enum eProjectileHitboxType;

static const int PlayerEquippedProperties_None = 0;
static const int PlayerEquippedProperties_Sword = 1;
static const int PlayerEquippedProperties_Axe = 2;
static const int PlayerEquippedProperties_Hammer = 4;
static const int PlayerEquippedProperties_Mace = 8;
static const int PlayerEquippedProperties_Dagger = 16;
static const int PlayerEquippedProperties_Sickle = 32;
static const int PlayerEquippedProperties_2HandedSword = 64;
static const int PlayerEquippedProperties_Shield = 128;
static const int PlayerEquippedProperties_Boomerang = 256;
static const int PlayerEquippedProperties_Bow = 512;
static const int PlayerEquippedProperties_Staff = 1024;
static const int PlayerEquippedProperties_Wand = 2048;
static const int PlayerEquippedProperties_Pickaxe = 4096;
static const int PlayerEquippedProperties_Torch = 8192;
static const int PlayerEquippedProperties_PlacementItem = 16384;
static const int PlayerEquippedProperties_PlacementScenery = 32768;
static const int PlayerEquippedProperties_PlacementBlock = 65536;
static const int PlayerEquippedProperties_Consumable = 131072;
static const int PlayerEquippedProperties_Bomb = 262144;
static const int PlayerEquippedProperties_SpellHands = 524288;

class Player
{
public:
	/* Public methods */
	Player(Renderer* pRenderer, ChunkManager* pChunkManager, QubicleBinaryManager* pQubicleBinaryManager, LightingManager* pLightingManager, BlockParticleManager* pBlockParticleManager);
	~Player();

	// Linkage
	void SetInventoryManager(InventoryManager* pInventoryManager);
	void SetItemManager(ItemManager* pItemManager);
	void SetProjectileManager(ProjectileManager* pProjectileManager);

	// Get voxel character pointer
	VoxelCharacter* GetVoxelCharacter();

	// Accessors / Setters
	void SetName(string name);
	string GetName();
	vec3 GetCenter();
	vec3 GetForwardVector();
	vec3 GetRightVector();
	vec3 GetUpVector();
	float GetRadius();
	void UpdateRadius();
	void SetForwardVector(vec3 forward);
	void SetRotation(float rot);
	float GetRotation();

	// Loading
	void LoadCharacter(string characterName);

	// Unloading
	void LoadWeapon(bool left, string weaponFile);
	void UnloadWeapon(bool left);

	// Equipping items
	void EquipItem(InventoryItem* pItem);
	void UnequipItem(EquipSlot equipSlot);

	// Stat modifier values
	void RefreshStatModifierCacheValues();

	// Collision
	bool CheckCollisions(vec3 positionCheck, vec3 previousPosition, vec3 *pNormal, vec3 *pMovement, bool *pStepUpBlock);

	// World
	void UpdateGridPosition();
	int GetGridX() const;
	int GetGridY() const;
	int GetGridZ() const;
	Chunk* GetCachedGridChunkOrFromPosition(vec3 pos);
	void ClearChunkCacheForChunk(Chunk* pChunk);

	// Camera
	void SetCameraPosition(vec3 cameraPos);
	void SetCameraForward(vec3 cameraForward);

	// Movement
	vec3 GetPositionMovementAmount();
	vec3 MoveAbsolute(vec3 direction, const float speed, bool shouldChangeForward = true);
	void Move(const float speed);
	void Strafe(const float speed);
	void Levitate(const float speed);
	void StopMoving();
	void Jump();
	bool CanJump();

	// Dead
	bool IsDead();

	// Combat
	void PressAttack();
	void ReleaseAttack();
	bool CanAttackLeft();
	bool CanAttackRight();
	void CheckProjectileDamageRadius(Projectile* pProjectile);

	// Projectile hitbox
	eProjectileHitboxType GetProjectileHitboxType();
	float GetProjectileHitboxRadius();
	float GetProjectileHitboxXLength();
	float GetProjectileHitboxYLength();
	float GetProjectileHitboxZLength();
	vec3 GetProjectileHitboxCenter();

	// Player equipped attributes
	void SetNormal();
	void SetSword(bool s);
	void SetAxe(bool s);
	void SetHammer(bool s);
	void SetMace(bool s);
	void SetDagger(bool s);
	void SetSickle(bool s);
	void Set2HandedSword(bool s);
	void SetShield(bool s);
	void SetBoomerang(bool s);
	void SetBow(bool s);
	void SetStaff(bool s);
	void SetWand(bool s);
	void SetPickaxe(bool s);
	void SetTorch(bool s);
	void SetItemPlacing(bool s);
	void SetSceneryPlacing(bool s);
	void SetBlockPlacing(bool s);
	void SetConsumable(bool s);
	void SetBomb(bool s);
	void SetSpellHands(bool s);

	bool IsNormal();
	bool IsSword();
	bool IsAxe();
	bool IsHammer();
	bool IsMace();
	bool IsDagger();
	bool IsSickle();
	bool Is2HandedSword();
	bool IsShield();
	bool IsBoomerang();
	bool IsBow();
	bool IsStaff();
	bool IsWand();
	bool IsPickaxe();
	bool IsTorch();
	bool IsItemPlacing();
	bool IsSceneryPlacing();
	bool IsBlockPlacing();
	bool IsConsumable();
	bool IsBomb();
	bool IsSpellHands();

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
	void UpdateChargingAttack(float dt);
	void UpdateCombat(float dt);

	// Rendering
	void Render();
	void RenderFirstPerson();
	void RenderWeaponTrails();
	void RenderFace();
	void RenderDebug();
	void RenderProjectileHitboxDebug();

protected:
	/* Protected methods */
	static void _StepUpAnimationFinished(void *apData);
	void StepUpAnimationFinished();

	static void _AttackAnimationTimerFinished(void *apData);
	void AttackAnimationTimerFinished();

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
	InventoryManager* m_pInventoryManager;
	ItemManager* m_pItemManager;
	ProjectileManager* m_pProjectileManager;

	// Player position and movement variables
	vec3 m_position;
	vec3 m_velocity;
	vec3 m_force;

	// Store previous position each frame after we have worked out the new position
	// Used for collision and other movement calculations
	vec3 m_previousPosition;

	// The direction of gravity for the player
	vec3 m_gravityDirection;

	// Local axis
	vec3 m_forward;
	vec3 m_right;
	vec3 m_up;

	// Target forward / looking vector
	vec3 m_targetForward;

	// Keep track of how much we have changed position in the update, based on physics, etc.
	// So that the fake camera position can be updated, if we are in some kind of follow camera mode.
	vec3 m_positionMovementAmount;

	// Player name
	string m_name;

	// Player type and model name
	string m_type;
	string m_modelName;

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

	// Camera variables
	vec3 m_cameraPosition;
	vec3 m_cameraForward;

	// Ground flag
	bool m_bIsOnGround;
	float m_groundCheckTimer;

	// Flag to control if we are allowed to jump or not, reset when landing
	bool m_bCanJump;
	float m_jumpTimer;

	// Idle flag
	bool m_bIsIdle;

	// Dead flag
	bool m_dead;

	// Combat
	bool m_bCanAttackLeft;
	bool m_bCanAttackRight;
	bool m_bCanInteruptCombatAnim;
	bool m_bCanThrowWeapon;
	float m_bowAttackDelay;

	// Projectile hitbox
	eProjectileHitboxType m_eProjectileHitboxType;
	float m_projectileHitboxRadius;
	float m_projectileHitboxXLength;
	float m_projectileHitboxYLength;
	float m_projectileHitboxZLength;
	vec3 m_projectileHitboxCenterOffset;

	// Charging attacks
	bool m_bIsChargingAttack;
	float m_chargeAmount;
	float m_chargeTime;
	vec3 m_chargeSpawnVelocity;
	vec3 m_chargeSpawnPosition;

	// Bitfield flag to hold the equipped properties for a player
	unsigned int m_equippedProperties;

	// Cached values for stat modifications, cache is refreshed whenever we equip or unequip a new item
	int m_strengthModifier;
	int m_dexterityModifier;
	int m_intelligenceModifier;
	int m_vitalityModifier;
	int m_armorModifier;
	int m_luckModifier;

	// Player radius
	float m_radius;

	// Animation params
	bool m_animationFinished[AnimationSections_NUMSECTIONS];
	float m_animationTimer;

	// Players world matrix
	Matrix4x4 m_worldMatrix;

	// Voxel character
	VoxelCharacter* m_pVoxelCharacter;
	QubicleBinary* m_pCharacterBackup;
};