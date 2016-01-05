// ******************************************************************************
// Filename:	Player.cpp
// Project:	Vox
// Author:	Steven Ball
//
// Revision History:
//   Initial Revision - 27/10/15
//
// Copyright (c) 2005-2015, Steven Ball
// ******************************************************************************

#include "Player.h"
#include "../utils/Random.h"
#include "../blocks/Chunk.h"
#include <glm/detail/func_geometric.hpp>

const vec3 Player::PLAYER_CENTER_OFFSET = vec3(0.0f, 1.525f, 0.0f);


Player::Player(Renderer* pRenderer, ChunkManager* pChunkManager, QubicleBinaryManager* pQubicleBinaryManager, LightingManager* pLightingManager, BlockParticleManager* pBlockParticleManager)
{
	m_pRenderer = pRenderer;
	m_pChunkManager = pChunkManager;
	m_pQubicleBinaryManager = pQubicleBinaryManager;
	m_pLightingManager = pLightingManager;
	m_pBlockParticleManager = pBlockParticleManager;

	m_forward = vec3(0.0f, 0.0f, 1.0f);
	m_right = vec3(1.0f, 0.0f, 0.0f);
	m_up = vec3(0.0f, 1.0f, 0.0f);

	m_targetForward = m_forward;

	m_position = vec3(8.0f, 8.0f, 8.0f);
	m_gravityDirection = vec3(0.0f, -1.0f, 0.0f);

	m_gridPositionX = 0;
	m_gridPositionY = 0;
	m_gridPositionZ = 0;

	m_pCachedGridChunk = NULL;

	m_bCanJump = true;
	m_jumpTimer = 0.0f;

	m_bIsIdle = true;

	m_bCanAttackLeft = true;
	m_bCanAttackRight = true;
	m_bCanInteruptCombatAnim = true;

	for (int i = 0; i < AnimationSections_NUMSECTIONS; i++)
	{
		m_animationFinished[i] = false;
	}

	// Create voxel character
	m_pVoxelCharacter = new VoxelCharacter(m_pRenderer, m_pQubicleBinaryManager);
	 
	// Load default character model
	LoadCharacter("Steve");
}

Player::~Player()
{
}

// Get voxel character pointer
VoxelCharacter* Player::GetVoxelCharacter()
{
	return m_pVoxelCharacter;
}

// Accessors / Setters
vec3 Player::GetCenter()
{
	vec3 center = m_position + m_up * m_radius;

	return center;
}

vec3 Player::GetForwardVector()
{
	return m_forward;
}

vec3 Player::GetRightVector()
{
	return m_right;
}

vec3 Player::GetUpVector()
{
	return m_up;
}

float Player::GetRadius()
{
	return m_radius;
}

void Player::UpdateRadius()
{
	m_radius = m_pVoxelCharacter->GetCharacterScale() / 0.14f;
}

void Player::SetForwardVector(vec3 forward)
{
	m_forward = normalize(forward);
	m_right = normalize(cross(m_forward, m_up));

	m_targetForward = m_forward;
	m_targetForward.y = 0.0f;
	m_targetForward = normalize(m_targetForward);
}

// Loading
void Player::LoadCharacter(string characterName)
{
	m_pVoxelCharacter->UnloadCharacter();
	m_pVoxelCharacter->Reset();

	char characterBaseFolder[128];
	char qbFilename[128];
	char ms3dFilename[128];
	char animListFilename[128];
	char facesFilename[128];
	char characterFilename[128];
	string modelName = characterName;
	string typeName = "Human";
	snprintf(characterBaseFolder, 128, "media/gamedata/models");
	snprintf(qbFilename, 128, "media/gamedata/models/%s/%s.qb", typeName.c_str(), modelName.c_str());
	snprintf(ms3dFilename, 128, "media/gamedata/models/%s/%s.ms3d", typeName.c_str(), typeName.c_str());
	snprintf(animListFilename, 128, "media/gamedata/models/%s/%s.animlist", typeName.c_str(), typeName.c_str());
	snprintf(facesFilename, 128, "media/gamedata/models/%s/%s.faces", typeName.c_str(), modelName.c_str());
	snprintf(characterFilename, 128, "media/gamedata/models/%s/%s.character", typeName.c_str(), modelName.c_str());
	m_pVoxelCharacter->LoadVoxelCharacter(typeName.c_str(), qbFilename, ms3dFilename, animListFilename, facesFilename, characterFilename, characterBaseFolder);

	m_pVoxelCharacter->SetBreathingAnimationEnabled(true);
	m_pVoxelCharacter->SetWinkAnimationEnabled(true);
	m_pVoxelCharacter->SetTalkingAnimationEnabled(false);
	m_pVoxelCharacter->SetRandomMouthSelection(true);
	m_pVoxelCharacter->SetRandomLookDirection(true);
	m_pVoxelCharacter->SetWireFrameRender(false);
	m_pVoxelCharacter->SetCharacterScale(0.08f);

	UpdateRadius();
}

// Unloading
void Player::UnloadWeapon(bool left)
{
	VoxelWeapon* pWeapon = NULL;
	bool isWeaponLoaded = false;
	if (left)  // Left side
	{
		pWeapon = m_pVoxelCharacter->GetLeftWeapon();
		isWeaponLoaded = m_pVoxelCharacter->IsLeftWeaponLoaded();
	}
	else  // Right side
	{
		pWeapon = m_pVoxelCharacter->GetRightWeapon();
		isWeaponLoaded = m_pVoxelCharacter->IsRightWeaponLoaded();
	}

	if (pWeapon != NULL)
	{
		if (isWeaponLoaded)
		{
			// Lights
			for (int i = 0; i < pWeapon->GetNumLights(); i++)
			{
				unsigned int lightId;
				vec3 lightPos;
				float lightRadius;
				float lightDiffuseMultiplier;
				Colour lightColour;
				bool connectedToSegment;
				pWeapon->GetLightParams(i, &lightId, &lightPos, &lightRadius, &lightDiffuseMultiplier, &lightColour, &connectedToSegment);

				if (lightId != -1)
				{
					m_pLightingManager->RemoveLight(lightId);
					pWeapon->SetLightingId(i, -1);

					if (connectedToSegment == false)
					{
						// Rotate due to characters forward vector
						float rotationAngle = acos(dot(vec3(0.0f, 0.0f, 1.0f), m_forward));
						if (m_forward.x < 0.0f)
						{
							rotationAngle = -rotationAngle;
						}
						Matrix4x4 rotationMatrix;
						rotationMatrix.SetRotation(0.0f, rotationAngle, 0.0f);
						lightPos = rotationMatrix * lightPos;

						// Translate to position
						lightPos += m_position;
					}

					float scale = m_pVoxelCharacter->GetCharacterScale();
					unsigned int lId;
					m_pLightingManager->AddDyingLight(vec3(lightPos.x, lightPos.y, lightPos.z), lightRadius * scale, lightDiffuseMultiplier, lightColour, 2.0f, &lId);
				}
			}

			// Particle Effects
			for (int i = 0; i < pWeapon->GetNumParticleEffects(); i++)
			{
				unsigned int particleEffectId;
				vec3 ParticleEffectPos;
				string effectName;
				bool connectedToSegment;
				pWeapon->GetParticleEffectParams(i, &particleEffectId, &ParticleEffectPos, &effectName, &connectedToSegment);

				if (particleEffectId != -1)
				{
					m_pBlockParticleManager->DestroyParticleEffect(particleEffectId);
					pWeapon->SetParticleEffectId(i, -1);
				}
			}
		}

		pWeapon->UnloadWeapon();

		if (left)  // Left side
		{
			m_pVoxelCharacter->UnloadLeftWeapon();
		}
		else  // Right side
		{
			m_pVoxelCharacter->UnloadRightWeapon();
		}
	}
}

// Collision
bool Player::CheckCollisions(vec3 positionCheck, vec3 previousPosition, vec3 *pNormal, vec3 *pMovement)
{
	vec3 movementCache = *pMovement;

	// World collision
	bool worldCollision = false;
	float radius = GetRadius();

	int blockX, blockY, blockZ;
	vec3 blockPos;
	int blockXAbove, blockYAbove, blockZAbove;
	vec3 blockPosAbove;
	int numChecks = 1 + (int)(radius / (Chunk::BLOCK_RENDER_SIZE* 2.0f));
	bool canAllStepUp = false;
	bool firstStepUp = true;
	for (int x = -numChecks; x <= numChecks; x++)
	{
		for (int y = -numChecks; y <= numChecks; y++)
		{
			for (int z = -numChecks; z <= numChecks; z++)
			{
				bool isStepUp = false;
				*pNormal = vec3(0.0f, 0.0f, 0.0f);

				Chunk* pChunk = GetCachedGridChunkOrFromPosition(positionCheck + vec3((Chunk::BLOCK_RENDER_SIZE*2.0f)*x, (Chunk::BLOCK_RENDER_SIZE*2.0f)*y, (Chunk::BLOCK_RENDER_SIZE*2.0f)*z));
				bool active = m_pChunkManager->GetBlockActiveFrom3DPosition(positionCheck.x + ((Chunk::BLOCK_RENDER_SIZE*2.0f)*x), positionCheck.y + ((Chunk::BLOCK_RENDER_SIZE*2.0f)*y), positionCheck.z + ((Chunk::BLOCK_RENDER_SIZE*2.0f)*z), &blockPos, &blockX, &blockY, &blockZ, &pChunk);
				bool activeAbove = false;
				bool activeAbove2 = false;

				if (active == false)
				{
					if (pChunk == NULL || pChunk->IsSetup() == false)
					{
						*pMovement = vec3(0.0f, 0.0f, 0.0f);
						//m_movementVelocity = vec3(0.0f, 0.0f, 0.0f);
						worldCollision = true;
					}
				}
				else if (active == true)
				{
					Plane3D planes[6];
					planes[0] = Plane3D(vec3(-1.0f, 0.0f, 0.0f), vec3(Chunk::BLOCK_RENDER_SIZE, 0.0f, 0.0f));
					planes[1] = Plane3D(vec3(1.0f, 0.0f, 0.0f), vec3(-Chunk::BLOCK_RENDER_SIZE, 0.0f, 0.0f));
					planes[2] = Plane3D(vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, Chunk::BLOCK_RENDER_SIZE, 0.0f));
					planes[3] = Plane3D(vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, -Chunk::BLOCK_RENDER_SIZE, 0.0f));
					planes[4] = Plane3D(vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 0.0f, Chunk::BLOCK_RENDER_SIZE));
					planes[5] = Plane3D(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, -Chunk::BLOCK_RENDER_SIZE));

					float distance;
					int inside = 0;
					bool insideCache[6];

					for (int i = 0; i < 6; i++)
					{
						vec3 pointToCheck = blockPos - previousPosition;
						distance = planes[i].GetPointDistance(pointToCheck);

						if (distance < -radius)
						{
							// Outside...
							insideCache[i] = false;
						}
						else if (distance < radius)
						{
							// Intersecting..
							insideCache[i] = true;
						}
						else
						{
							// Inside...
							insideCache[i] = true;
						}
					}

					for (int i = 0; i < 6; i++)
					{
						vec3 pointToCheck = blockPos - positionCheck;
						distance = planes[i].GetPointDistance(pointToCheck);

						if (distance < -radius)
						{
							// Outside...
						}
						else if (distance < radius)
						{
							// Intersecting..
							inside++;
							if (insideCache[i] == false)
							{
								*pNormal += planes[i].mNormal;
							}
						}
						else
						{
							// Inside...
							inside++;
							if (insideCache[i] == false)
							{
								*pNormal += planes[i].mNormal;
							}
						}
					}

					if (inside == 6)
					{
						if (y == 0) // We only want to check on the same y-level as the players position.
						{
							vec3 posCheck1 = vec3(positionCheck.x + ((Chunk::BLOCK_RENDER_SIZE*2.0f)*x), positionCheck.y + (Chunk::BLOCK_RENDER_SIZE*2.0f), positionCheck.z + ((Chunk::BLOCK_RENDER_SIZE*2.0f)*z));
							vec3 posCheck2 = vec3(positionCheck.x + ((Chunk::BLOCK_RENDER_SIZE*2.0f)*x), positionCheck.y + (Chunk::BLOCK_RENDER_SIZE*4.0f), positionCheck.z + ((Chunk::BLOCK_RENDER_SIZE*2.0f)*z));

							Chunk* pChunkAbove = GetCachedGridChunkOrFromPosition(vec3(posCheck1.x, posCheck1.y, posCheck1.z));
							activeAbove = m_pChunkManager->GetBlockActiveFrom3DPosition(posCheck1.x, posCheck1.y, posCheck1.z, &blockPosAbove, &blockXAbove, &blockYAbove, &blockZAbove, &pChunkAbove);
							Chunk* pChunkAbove2 = GetCachedGridChunkOrFromPosition(vec3(posCheck2.x, posCheck2.y, posCheck2.z));
							activeAbove2 = m_pChunkManager->GetBlockActiveFrom3DPosition(posCheck2.x, posCheck2.y, posCheck2.z, &blockPosAbove, &blockXAbove, &blockYAbove, &blockZAbove, &pChunkAbove2);

							if ((activeAbove == false) && (activeAbove2 == false))
							{
								if (firstStepUp)
								{
									canAllStepUp = true;
								}

								isStepUp = true;
							}
							else
							{
								canAllStepUp = false;
							}

							firstStepUp = false;
						}

						if (length(*pNormal) <= 1.0f)
						{
							if (length(*pNormal) > 0.0f)
							{
								*pNormal = normalize(*pNormal);
							}

							float dotResult = dot(*pNormal, *pMovement);
							*pNormal *= dotResult;

							*pMovement -= *pNormal;

							//if (fabs((*pNormal).x) > 0.01f || fabs((*pNormal).z) > 0.01f)
							//{
							//	m_movementVelocity = vec3(0.0f, 0.0f, 0.0f);
							//}

							worldCollision = true;
						}
					}
				}
			}
		}
	}

	if (worldCollision)
		return true;

	*pMovement = movementCache;

	return false;
}

// World
void Player::UpdateGridPosition()
{
	int gridPositionX = (int)((m_position.x + Chunk::BLOCK_RENDER_SIZE) / Chunk::CHUNK_SIZE);
	int gridPositionY = (int)((m_position.y + Chunk::BLOCK_RENDER_SIZE) / Chunk::CHUNK_SIZE);
	int gridPositionZ = (int)((m_position.z + Chunk::BLOCK_RENDER_SIZE) / Chunk::CHUNK_SIZE);

	if (m_position.x <= -0.5f)
		gridPositionX -= 1;
	if (m_position.y <= -0.5f)
		gridPositionY -= 1;
	if (m_position.z <= -0.5f)
		gridPositionZ -= 1;

	if (gridPositionX != m_gridPositionX || gridPositionY != m_gridPositionY || gridPositionZ != m_gridPositionZ || m_pCachedGridChunk == NULL)
	{
		m_gridPositionX = gridPositionX;
		m_gridPositionY = gridPositionY;
		m_gridPositionZ = gridPositionZ;

		m_pCachedGridChunk = m_pChunkManager->GetChunk(m_gridPositionX, m_gridPositionY, m_gridPositionZ);
	}
}

Chunk* Player::GetCachedGridChunkOrFromPosition(vec3 pos)
{
	// First check if the position is in the same grid as the cached chunk
	int gridPositionX = (int)((pos.x + Chunk::BLOCK_RENDER_SIZE) / Chunk::CHUNK_SIZE);
	int gridPositionY = (int)((pos.y + Chunk::BLOCK_RENDER_SIZE) / Chunk::CHUNK_SIZE);
	int gridPositionZ = (int)((pos.z + Chunk::BLOCK_RENDER_SIZE) / Chunk::CHUNK_SIZE);

	if (pos.x <= -0.5f)
		gridPositionX -= 1;
	if (pos.y <= -0.5f)
		gridPositionY -= 1;
	if (pos.z <= -0.5f)
		gridPositionZ -= 1;

	if (gridPositionX != m_gridPositionX || gridPositionY != m_gridPositionY || gridPositionZ != m_gridPositionZ)
	{
		return NULL;
	}
	else
	{
		return m_pCachedGridChunk;
	}
}

void Player::ClearChunkCacheForChunk(Chunk* pChunk)
{
	if (m_pCachedGridChunk == pChunk)
	{
		m_pCachedGridChunk = NULL;
	}
}

// Movement
vec3 Player::GetPositionMovementAmount()
{
	return m_positionMovementAmount;
}

vec3 Player::MoveAbsolute(vec3 direction, const float speed, bool shouldChangeForward)
{
	if (shouldChangeForward)
	{
		m_forward = normalize(direction);
	}

	m_targetForward = m_forward;
	m_targetForward.y = 0.0f;
	m_targetForward = normalize(m_targetForward);

	vec3 totalAmountMoved;

	vec3 movement = direction;
	vec3 movementAmount = direction*speed;
	vec3 pNormal;
	int numberDivision = 1;
	while (length(movementAmount) >= 1.0f)
	{
		numberDivision++;
		movementAmount = direction*(speed / numberDivision);
	}
	for (int i = 0; i < numberDivision; i++)
	{
		float speedToUse = (speed / numberDivision) + ((speed / numberDivision) * i);
		vec3 posToCheck = GetCenter() + movement*speedToUse;
		if (CheckCollisions(posToCheck, m_previousPosition, &pNormal, &movement))
		{
		}

		m_position += (movement * speedToUse);

		totalAmountMoved += (movement * speedToUse);
	}

	// Change to run animation
	m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_FullBody, true, AnimationSections_FullBody, "Run", 0.01f);

	m_bIsIdle = false;

	return totalAmountMoved;
}

void Player::Move(const float speed)
{
}

void Player::Strafe(const float speed)
{
}

void Player::Levitate(const float speed)
{
	m_force += vec3(0.0f, 60.0f, 0.0f);
}

void Player::StopMoving()
{
	if (m_bIsIdle == false)
	{
		m_bIsIdle = true;

		if (CanAttackLeft() && CanAttackRight())
		{
			m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_FullBody, false, AnimationSections_FullBody, "BindPose", 0.15f);
		}
		if (m_bCanInteruptCombatAnim)
		{
			m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Legs_Feet, false, AnimationSections_Legs_Feet, "BindPose", 0.15f);
		}
	}
}

void Player::Jump()
{
	if (m_bCanJump == false)
	{
		return;
	}

	if (m_jumpTimer >= 0.0f)
	{
		return;
	}

	m_bCanJump = false;
	m_jumpTimer = 0.3f;

	m_velocity += m_up * 14.0f;

	// Change to jump animation
	if (CanAttackLeft() && CanAttackRight())
	{
		m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_FullBody, false, AnimationSections_FullBody, "Jump", 0.01f);
	}
	if (m_bCanInteruptCombatAnim)
	{
		m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Legs_Feet, false, AnimationSections_Legs_Feet, "Jump", 0.01f);
	}
}

bool Player::CanJump()
{
	return m_bCanJump;
}

// Combat
void Player::PressAttack()
{
	if (CanAttackRight())
	{
		int randomAttack = GetRandomNumber(1, 3);
		if (randomAttack == 1)
		{
			m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_FullBody, true, AnimationSections_FullBody, "SwordAttack1", 0.01f);
			m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Head_Body, false, AnimationSections_Head_Body, "SwordAttack1", 0.01f);
			m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Right_Arm_Hand, false, AnimationSections_Right_Arm_Hand, "SwordAttack1", 0.01f);
			m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Left_Arm_Hand, false, AnimationSections_Left_Arm_Hand, "SwordAttack1", 0.01f);

			m_bCanInteruptCombatAnim = true;
		}
		else if (randomAttack == 2)
		{
			m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_FullBody, true, AnimationSections_FullBody, "SwordAttack2", 0.01f);
			m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Head_Body, false, AnimationSections_Head_Body, "SwordAttack2", 0.01f);
			m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Right_Arm_Hand, false, AnimationSections_Right_Arm_Hand, "SwordAttack2", 0.01f);
			m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Left_Arm_Hand, false, AnimationSections_Left_Arm_Hand, "SwordAttack1", 0.01f);

			m_bCanInteruptCombatAnim = true;
		}
		else if (randomAttack == 3)
		{
			m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_FullBody, false, AnimationSections_FullBody, "SwordAttack3", 0.01f);

			m_bCanInteruptCombatAnim = false;
		}

		m_bCanAttackRight = false;
	}
}

void Player::ReleaseAttack()
{

}

bool Player::CanAttackLeft()
{
	return m_bCanAttackLeft;
}

bool Player::CanAttackRight()
{
	return m_bCanAttackRight;
}

// Rendering modes
void Player::SetWireFrameRender(bool wireframe)
{
	m_pVoxelCharacter->SetWireFrameRender(wireframe);
}

void Player::SetPlayerAlpha(float alpha)
{
	m_pVoxelCharacter->SetMeshAlpha(alpha);
}

// Rendering Helpers
void Player::CalculateWorldTransformMatrix()
{
	m_right = normalize(cross(m_up, m_forward));
	m_forward = normalize(cross(m_right, m_up));

	float lMatrix[16] =
	{
		m_right.x, m_right.y, m_right.z, 0.0f,
		m_up.x, m_up.y, m_up.z, 0.0f,
		m_forward.x, m_forward.y, m_forward.z, 0.0f,
		m_position.x, m_position.y, m_position.z, 1.0f
	};

	m_worldMatrix.SetValues(lMatrix);
}

void Player::RebuildVoxelCharacter(bool faceMerge)
{
	m_pVoxelCharacter->RebuildVoxelModel(faceMerge);
}

// Updating
void Player::Update(float dt)
{
	// Update grid position
	UpdateGridPosition();

	// Update the voxel model
	float animationSpeeds[AnimationSections_NUMSECTIONS] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	m_pVoxelCharacter->Update(dt, animationSpeeds);
	m_pVoxelCharacter->SetWeaponTrailsOriginMatrix(dt, m_worldMatrix);

	// Update animations
	UpdateAnimations(dt);

	// Update / Create weapon lights and particle effects
	UpdateWeaponLights(dt);
	UpdateWeaponParticleEffects(dt);

	// Update timers
	UpdateTimers(dt);

	// Physics update
	UpdatePhysics(dt);
}

void Player::UpdateAnimations(float dt)
{
	if (m_pVoxelCharacter != NULL)
	{
		for (int i = 0; i < AnimationSections_NUMSECTIONS; i++)
		{
			m_animationFinished[i] = m_pVoxelCharacter->HasAnimationFinished((AnimationSections)i);
		}
	}

	if (m_bCanInteruptCombatAnim == false && m_animationFinished[AnimationSections_FullBody] == true)
	{
		m_bCanInteruptCombatAnim = true;
	}
	if (m_bCanAttackLeft == false && m_animationFinished[AnimationSections_Left_Arm_Hand] == true)
	{
		m_bCanAttackLeft = true;
	}
	if (m_bCanAttackRight == false && m_animationFinished[AnimationSections_Right_Arm_Hand] == true)
	{
		m_bCanAttackRight = true;
	}
}

void Player::UpdatePhysics(float dt)
{
	m_positionMovementAmount = vec3(0.0f, 0.0f, 0.0f);

	// Integrate velocity
	vec3 acceleration = m_force + (m_gravityDirection * 9.81f)*4.0f;
	m_velocity += acceleration * dt;

	// Check collision
	{
		vec3 velocityToUse = m_velocity;
		vec3 velAmount = velocityToUse*dt;
		vec3 pNormal;
		int numberDivision = 1;
		while (length(velAmount) >= 1.0f)
		{
			numberDivision++;
			velAmount = velocityToUse*(dt / numberDivision);
		}
		for (int i = 0; i < numberDivision; i++)
		{
			float dtToUse = (dt / numberDivision) + ((dt / numberDivision) * i);
			vec3 posToCheck = GetCenter() + velocityToUse*dtToUse;
			if (CheckCollisions(posToCheck, m_previousPosition, &pNormal, &velAmount))
			{
				if (velocityToUse.y <= 0.0f)
				{
					// Reset velocity, we don't have any bounce
					m_velocity = vec3(0.0f, 0.0f, 0.0f);
					velocityToUse = vec3(0.0f, 0.0f, 0.0f);

					if (m_bCanJump == false)
					{
						m_bCanJump = true;
					}
				}
			}
		}

		// Integrate position
		m_position += velocityToUse * dt;

		m_positionMovementAmount += velocityToUse * dt;
	}

	// Store previous position
	m_previousPosition = m_position;
}

void Player::UpdateTimers(float dt)
{
	// Jump timer
	if (m_jumpTimer >= 0.0f)
	{
		m_jumpTimer -= dt;
	}
}

void Player::UpdateWeaponLights(float dt)
{
	for (int i = 0; i < 2; i++)
	{
		VoxelWeapon* pWeapon = NULL;
		bool isWeaponLoaded = false;
		if (i == 0)  // Right side
		{
			pWeapon = m_pVoxelCharacter->GetRightWeapon();
			isWeaponLoaded = m_pVoxelCharacter->IsRightWeaponLoaded();
		}
		else  // Left side
		{
			pWeapon = m_pVoxelCharacter->GetLeftWeapon();
			isWeaponLoaded = m_pVoxelCharacter->IsLeftWeaponLoaded();
		}

		if (pWeapon != NULL)
		{
			if (isWeaponLoaded)
			{
				for (int i = 0; i < pWeapon->GetNumLights(); i++)
				{
					unsigned int lightId;
					vec3 lightPos;
					float lightRadius;
					float lightDiffuseMultiplier;
					Colour lightColour;
					bool connectedToSegment;
					pWeapon->GetLightParams(i, &lightId, &lightPos, &lightRadius, &lightDiffuseMultiplier, &lightColour, &connectedToSegment);

					if (lightId == -1)
					{
						m_pLightingManager->AddLight(vec3(0.0f), 0.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f, 1.0f), &lightId);
						pWeapon->SetLightingId(i, lightId);
					}

					if (connectedToSegment == false)
					{
						// Rotate due to characters forward vector
						float rotationAngle = acos(dot(vec3(0.0f, 0.0f, 1.0f), m_forward));
						if (m_forward.x < 0.0f)
						{
							rotationAngle = -rotationAngle;
						}
						Matrix4x4 rotationMatrix;
						rotationMatrix.SetRotation(0.0f, rotationAngle, 0.0f);
						lightPos = rotationMatrix * lightPos;

						// Translate to position
						lightPos += m_position;
					}

					float scale = m_pVoxelCharacter->GetCharacterScale();

					m_pLightingManager->UpdateLightPosition(lightId, vec3(lightPos.x, lightPos.y, lightPos.z));
					m_pLightingManager->UpdateLightRadius(lightId, lightRadius * scale);
					m_pLightingManager->UpdateLightDiffuseMultiplier(lightId, lightDiffuseMultiplier);
					m_pLightingManager->UpdateLightColour(lightId, lightColour);
				}
			}
		}
	}
}

void Player::UpdateWeaponParticleEffects(float dt)
{
	// Create/update
	for (int i = 0; i < 2; i++)
	{
		VoxelWeapon* pWeapon = NULL;
		bool isWeaponLoaded = false;
		if (i == 0)  // Right side
		{
			pWeapon = m_pVoxelCharacter->GetRightWeapon();
			isWeaponLoaded = m_pVoxelCharacter->IsRightWeaponLoaded();
		}
		else  // Left side
		{
			pWeapon = m_pVoxelCharacter->GetLeftWeapon();
			isWeaponLoaded = m_pVoxelCharacter->IsLeftWeaponLoaded();
		}

		if (pWeapon != NULL)
		{
			if (isWeaponLoaded)
			{
				for (int i = 0; i < pWeapon->GetNumParticleEffects(); i++)
				{
					unsigned int particleEffectId;
					vec3 ParticleEffectPos;
					string effectName;
					bool connectedToSegment;
					pWeapon->GetParticleEffectParams(i, &particleEffectId, &ParticleEffectPos, &effectName, &connectedToSegment);

					if (particleEffectId == -1)
					{
						m_pBlockParticleManager->ImportParticleEffect(effectName, vec3(ParticleEffectPos.x, ParticleEffectPos.y, ParticleEffectPos.z), &particleEffectId);
						pWeapon->SetParticleEffectId(i, particleEffectId);
					}

					if (connectedToSegment == false)
					{
						// Rotate due to characters forward vector
						float rotationAngle = acos(dot(vec3(0.0f, 0.0f, 1.0f), m_forward));
						if (m_forward.x < 0.0f)
						{
							rotationAngle = -rotationAngle;
						}
						Matrix4x4 rotationMatrix;
						rotationMatrix.SetRotation(0.0f, rotationAngle, 0.0f);
						ParticleEffectPos = rotationMatrix * ParticleEffectPos;

						// Translate to position
						ParticleEffectPos += m_position;
					}

					m_pBlockParticleManager->UpdateParticleEffectPosition(particleEffectId, vec3(ParticleEffectPos.x, ParticleEffectPos.y, ParticleEffectPos.z));
				}
			}
		}
	}
}

// Rendering
void Player::Render()
{
	Colour OulineColour(1.0f, 1.0f, 0.0f, 1.0f);
	m_pRenderer->PushMatrix();
		m_pRenderer->MultiplyWorldMatrix(m_worldMatrix);
		m_pVoxelCharacter->RenderWeapons(false, false, false, OulineColour);
		m_pVoxelCharacter->Render(false, false, false, OulineColour, false);
	m_pRenderer->PopMatrix();
}

void Player::RenderFirstPerson()
{
	m_pVoxelCharacter->SetMeshAlpha(1.0f);

	Colour OulineColour(1.0f, 1.0f, 0.0f, 1.0f);
	m_pRenderer->PushMatrix();
		m_pRenderer->MultiplyWorldMatrix(m_worldMatrix);
		m_pVoxelCharacter->RenderWeapons(false, false, false, OulineColour);
	m_pRenderer->PopMatrix();
}

void Player::RenderWeaponTrails()
{
	m_pRenderer->PushMatrix();
		m_pVoxelCharacter->RenderWeaponTrails();
	m_pRenderer->PopMatrix();
}

void Player::RenderFace()
{
	m_pRenderer->PushMatrix();
		m_pRenderer->MultiplyWorldMatrix(m_worldMatrix);
		m_pRenderer->EmptyTextureIndex(0);
		m_pVoxelCharacter->RenderFace();
	m_pRenderer->PopMatrix();
}

void Player::RenderDebug()
{
	m_pRenderer->PushMatrix();
		m_pRenderer->TranslateWorldMatrix(GetCenter().x, GetCenter().y, GetCenter().z);

		// Radius
		m_pRenderer->PushMatrix();
			m_pRenderer->SetLineWidth(1.0f);
			m_pRenderer->SetRenderMode(RM_WIREFRAME);
			m_pRenderer->ImmediateColourAlpha(1.0f, 1.0f, 1.0f, 1.0f);

			m_pRenderer->RotateWorldMatrix(90.0f, 0.0f, 0.0f);
			m_pRenderer->DrawSphere(m_radius, 20, 20);
		m_pRenderer->PopMatrix();

		// Forwards
		m_pRenderer->PushMatrix();
			m_pRenderer->ScaleWorldMatrix(m_pVoxelCharacter->GetCharacterScale(), m_pVoxelCharacter->GetCharacterScale(), m_pVoxelCharacter->GetCharacterScale());

			m_pRenderer->SetRenderMode(RM_SOLID);
			m_pRenderer->SetLineWidth(3.0f);
			m_pRenderer->EnableImmediateMode(IM_LINES);
				// Target forwards
				m_pRenderer->ImmediateColourAlpha(0.0f, 1.0f, 1.0f, 1.0f);
				m_pRenderer->ImmediateVertex(0.0f, 0.0f, 0.0f);
				m_pRenderer->ImmediateVertex(m_targetForward.x*15.0f, m_targetForward.y*15.0f, m_targetForward.z*15.0f);

				// Right
				m_pRenderer->ImmediateColourAlpha(1.0f, 0.0f, 0.0f, 1.0f);
				m_pRenderer->ImmediateVertex(0.0f, 0.0f, 0.0f);
				m_pRenderer->ImmediateVertex(m_right.x*15.0f, m_right.y*15.0f, m_right.z*15.0f);

				// Up
				m_pRenderer->ImmediateColourAlpha(0.0f, 1.0f, 0.0f, 1.0f);
				m_pRenderer->ImmediateVertex(0.0f, 0.0f, 0.0f);
				m_pRenderer->ImmediateVertex(m_up.x*15.0f, m_up.y*15.0f, m_up.z*15.0f);	

				// Forwards
				m_pRenderer->ImmediateColourAlpha(0.0f, 0.0f, 1.0f, 1.0f);
				m_pRenderer->ImmediateVertex(0.0f, 0.0f, 0.0f);
				m_pRenderer->ImmediateVertex(m_forward.x*15.0f, m_forward.y*15.0f, m_forward.z*15.0f);	
			m_pRenderer->DisableImmediateMode();
		m_pRenderer->PopMatrix();
	m_pRenderer->PopMatrix();
}
