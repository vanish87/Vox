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
#include "../utils/Interpolator.h"
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
	m_pInventoryManager = NULL;
	m_pItemManager = NULL;

	m_forward = vec3(0.0f, 0.0f, 1.0f);
	m_right = vec3(1.0f, 0.0f, 0.0f);
	m_up = vec3(0.0f, 1.0f, 0.0f);

	m_targetForward = m_forward;

	m_position = vec3(8.0f, 8.0f, 8.0f);
	m_gravityDirection = vec3(0.0f, -1.0f, 0.0f);

	// Stepping up single world blocks by walking into them
	m_bDoStepUpAnimation = false;
	m_stepUpAnimationYAmount = 0.0f;
	m_stepUpAnimationPrevious = 0.0f;
	m_stepUpAnimationYOffset = 0.0f;
	m_stepUpAdditionYAmountChangedCache = 0.0f;

	m_gridPositionX = 0;
	m_gridPositionY = 0;
	m_gridPositionZ = 0;

	m_pCachedGridChunk = NULL;

	m_bIsOnGround = false;
	m_groundCheckTimer = 0.0f;

	m_bCanJump = true;
	m_jumpTimer = 0.0f;

	m_bIsIdle = true;

	m_bCanAttackLeft = true;
	m_bCanAttackRight = true;
	m_bCanInteruptCombatAnim = true;

	// Player stats
	m_strengthModifier = 0;
	m_dexterityModifier = 0;
	m_intelligenceModifier = 0;
	m_vitalityModifier = 0;
	m_armorModifier = 0;
	m_luckModifier = 0;

	// Initial equipped state
	m_equippedProperties = 0;
	SetNormal();

	for (int i = 0; i < AnimationSections_NUMSECTIONS; i++)
	{
		m_animationFinished[i] = false;
	}

	// Create voxel character
	m_pVoxelCharacter = new VoxelCharacter(m_pRenderer, m_pQubicleBinaryManager);
	m_pCharacterBackup = new QubicleBinary(m_pRenderer);

	// Load default character model
	SetName("Steve");
	LoadCharacter("Steve");
}

Player::~Player()
{
}

// Linkage
void Player::SetInventoryManager(InventoryManager* pInventoryManager)
{
	m_pInventoryManager = pInventoryManager;
}

void Player::SetItemManager(ItemManager* pItemManager)
{
	m_pItemManager = pItemManager;
}

// Get voxel character pointer
VoxelCharacter* Player::GetVoxelCharacter()
{
	return m_pVoxelCharacter;
}

// Accessors / Setters
void Player::SetName(string name)
{
	m_name = name;
}

string Player::GetName()
{
	return m_name;
}

vec3 Player::GetCenter()
{
	vec3 center = m_position + m_up * m_radius;
	center -= vec3(0.0f, (m_bDoStepUpAnimation ? 0.0f : m_stepUpAnimationYOffset), 0.0f);

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

	m_type = "Human";
	m_modelName = characterName;

	char characterBaseFolder[128];
	char qbFilename[128];
	char ms3dFilename[128];
	char animListFilename[128];
	char facesFilename[128];
	char characterFilename[128];
	snprintf(characterBaseFolder, 128, "media/gamedata/models");
	snprintf(qbFilename, 128, "media/gamedata/models/%s/%s.qb", m_type.c_str(), m_modelName.c_str());
	snprintf(ms3dFilename, 128, "media/gamedata/models/%s/%s.ms3d", m_type.c_str(), m_type.c_str());
	snprintf(animListFilename, 128, "media/gamedata/models/%s/%s.animlist", m_type.c_str(), m_type.c_str());
	snprintf(facesFilename, 128, "media/gamedata/models/%s/%s.faces", m_type.c_str(), m_modelName.c_str());
	snprintf(characterFilename, 128, "media/gamedata/models/%s/%s.character", m_type.c_str(), m_modelName.c_str());
	m_pVoxelCharacter->LoadVoxelCharacter(m_type.c_str(), qbFilename, ms3dFilename, animListFilename, facesFilename, characterFilename, characterBaseFolder);

	m_pVoxelCharacter->SetBreathingAnimationEnabled(true);
	m_pVoxelCharacter->SetWinkAnimationEnabled(true);
	m_pVoxelCharacter->SetTalkingAnimationEnabled(false);
	m_pVoxelCharacter->SetRandomMouthSelection(true);
	m_pVoxelCharacter->SetRandomLookDirection(true);
	m_pVoxelCharacter->SetWireFrameRender(false);
	m_pVoxelCharacter->SetCharacterScale(0.08f);

	if (m_pCharacterBackup != NULL)
	{
		delete m_pCharacterBackup;
		m_pCharacterBackup = new QubicleBinary(m_pRenderer);
	}

	m_pCharacterBackup->Import(qbFilename, true);

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

// Equipping items
void Player::EquipItem(InventoryItem* pItem)
{
	switch (pItem->m_equipSlot)
	{
	case EquipSlot_LeftHand:
	{
		//LoadWeapon(true, pItem->m_filename);

		switch (pItem->m_itemType)
		{
		case InventoryType_Weapon_Dagger: { SetDagger(true); } break;
		case InventoryType_Weapon_Shield: { SetShield(true); } break;
		case InventoryType_Weapon_Bow: { SetBow(true); } break;
		case InventoryType_Weapon_Torch: { SetTorch(true); } break;
		case InventoryType_Weapon_SpellHands:
			{
				SetSpellHands(true);
				m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Left_Arm_Hand, false, AnimationSections_Left_Arm_Hand, "HandSpellCastPose", 0.25f);
				m_pVoxelCharacter->SetQubicleMatrixRender("Left_Hand", false);
				break;
			}
		}

		if (pItem->m_itemType == InventoryType_Weapon_Bow)
		{
			VoxelWeapon* pNewEquipment = new VoxelWeapon(m_pRenderer, m_pQubicleBinaryManager);
			pNewEquipment->SetVoxelCharacterParent(NULL);
			pNewEquipment->LoadWeapon("media/gamedata/items/Quiver/Quiver.item", false);

			pNewEquipment->GetAnimatedSection(0)->m_pVoxelObject->GetQubicleModel()->SetScaleAndOffsetForMatrix("Quiver", pNewEquipment->GetAnimatedSection(0)->m_renderScale, pNewEquipment->GetAnimatedSection(0)->m_renderOffset.x, pNewEquipment->GetAnimatedSection(0)->m_renderOffset.y, pNewEquipment->GetAnimatedSection(0)->m_renderOffset.z);
			QubicleMatrix* pQuiverMatrix = pNewEquipment->GetAnimatedSection(0)->m_pVoxelObject->GetQubicleModel()->GetQubicleMatrix("Quiver");
			pQuiverMatrix->m_boneIndex = m_pVoxelCharacter->GetBodyBoneIndex();
			m_pVoxelCharacter->AddQubicleMatrix(pQuiverMatrix, false);
		}
	}
	break;
	case EquipSlot_RightHand:
	{
		//LoadWeapon(false, pItem->m_filename);

		switch (pItem->m_itemType)
		{
		case InventoryType_Item: { SetItemPlacing(true); } break;
		case InventoryType_Scenery: { SetSceneryPlacing(true); } break;
		case InventoryType_Block: { SetBlockPlacing(true); } break;
		case InventoryType_Weapon_Sword: { SetSword(true); } break;
		case InventoryType_Weapon_Dagger: { SetDagger(true); /*SetStealth(true);*/ } break;
		case InventoryType_Weapon_Axe: { SetAxe(true); } break;
		case InventoryType_Weapon_Hammer: { SetHammer(true); } break;
		case InventoryType_Weapon_Mace: { SetMace(true); } break;
		case InventoryType_Weapon_Sickle: { SetSickle(true); } break;
		case InventoryType_Weapon_2HandedSword:
			{
				Set2HandedSword(true);
				m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_FullBody, false, AnimationSections_FullBody, "2HandedSwordPose", 0.25f);
				break;
			}
		case InventoryType_Weapon_Boomerang: { SetBoomerang(true); /*m_bCanThrowWeapon = true;*/ } break;
		case InventoryType_Weapon_Bomb: { SetBomb(true); } break;
		case InventoryType_Weapon_Staff:
			{
				SetStaff(true);
				m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Right_Arm_Hand, false, AnimationSections_Right_Arm_Hand, "StaffPose", 0.25f);
				break;
			}
		case InventoryType_Weapon_Wand: { SetWand(true); } break;
		case InventoryType_Weapon_Pickaxe: { SetPickaxe(true); } break;
		case InventoryType_Weapon_SpellHands:
			{
				SetSpellHands(true);
				m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Right_Arm_Hand, false, AnimationSections_Right_Arm_Hand, "HandSpellCastPose", 0.25f);
				m_pVoxelCharacter->SetQubicleMatrixRender("Right_Hand", false);
				break;
			}
		}
	}
	break;
	case EquipSlot_Head:
	{
		if (m_pVoxelCharacter != NULL)
		{
			VoxelWeapon* pNewEquipment = new VoxelWeapon(m_pRenderer, m_pQubicleBinaryManager);
			pNewEquipment->SetVoxelCharacterParent(NULL);
			pNewEquipment->LoadWeapon(pItem->m_filename.c_str(), false);

			pNewEquipment->GetAnimatedSection(0)->m_pVoxelObject->GetQubicleModel()->SetScaleAndOffsetForMatrix("Helm", pNewEquipment->GetAnimatedSection(0)->m_renderScale, pNewEquipment->GetAnimatedSection(0)->m_renderOffset.x, pNewEquipment->GetAnimatedSection(0)->m_renderOffset.y, pNewEquipment->GetAnimatedSection(0)->m_renderOffset.z);
			QubicleMatrix* pHelmMatrix = pNewEquipment->GetAnimatedSection(0)->m_pVoxelObject->GetQubicleModel()->GetQubicleMatrix("Helm");
			pHelmMatrix->m_boneIndex = m_pVoxelCharacter->GetHeadBoneIndex();
			m_pVoxelCharacter->AddQubicleMatrix(pHelmMatrix, false);
		}
	}
	break;
	case EquipSlot_Shoulders:
	{
		if (m_pVoxelCharacter != NULL)
		{
			VoxelWeapon* pNewEquipment = new VoxelWeapon(m_pRenderer, m_pQubicleBinaryManager);
			pNewEquipment->SetVoxelCharacterParent(NULL);
			pNewEquipment->LoadWeapon(pItem->m_filename.c_str(), false);

			if (pItem->m_right)
			{
				pNewEquipment->GetAnimatedSection(0)->m_pVoxelObject->GetQubicleModel()->SetScaleAndOffsetForMatrix("Right_Shoulder_Armor", pNewEquipment->GetAnimatedSection(0)->m_renderScale, pNewEquipment->GetAnimatedSection(0)->m_renderOffset.x, pNewEquipment->GetAnimatedSection(0)->m_renderOffset.y, pNewEquipment->GetAnimatedSection(0)->m_renderOffset.z);
				QubicleMatrix* pRightShoulderMatrix = pNewEquipment->GetAnimatedSection(0)->m_pVoxelObject->GetQubicleModel()->GetQubicleMatrix("Right_Shoulder_Armor");
				pRightShoulderMatrix->m_boneIndex = m_pVoxelCharacter->GetRightShoulderBoneIndex();
				m_pVoxelCharacter->AddQubicleMatrix(pRightShoulderMatrix, false);
			}

			if (pItem->m_left)
			{
				pNewEquipment->GetAnimatedSection(1)->m_pVoxelObject->GetQubicleModel()->SetScaleAndOffsetForMatrix("Left_Shoulder_Armor", pNewEquipment->GetAnimatedSection(1)->m_renderScale, pNewEquipment->GetAnimatedSection(1)->m_renderOffset.x, pNewEquipment->GetAnimatedSection(1)->m_renderOffset.y, pNewEquipment->GetAnimatedSection(1)->m_renderOffset.z);
				QubicleMatrix* pLeftShoulderMatrix = pNewEquipment->GetAnimatedSection(1)->m_pVoxelObject->GetQubicleModel()->GetQubicleMatrix("Left_Shoulder_Armor");
				pLeftShoulderMatrix->m_boneIndex = m_pVoxelCharacter->GetLeftShoulderBoneIndex();
				m_pVoxelCharacter->AddQubicleMatrix(pLeftShoulderMatrix, false);
			}
		}
	}
	break;
	case EquipSlot_Body:
	{
		if (m_pVoxelCharacter != NULL)
		{
			VoxelWeapon* pNewEquipment = new VoxelWeapon(m_pRenderer, m_pQubicleBinaryManager);
			pNewEquipment->SetVoxelCharacterParent(NULL);
			pNewEquipment->LoadWeapon(pItem->m_filename.c_str(), false);

			pNewEquipment->GetAnimatedSection(0)->m_pVoxelObject->GetQubicleModel()->SetScaleAndOffsetForMatrix("Body", pNewEquipment->GetAnimatedSection(0)->m_renderScale, pNewEquipment->GetAnimatedSection(0)->m_renderOffset.x, pNewEquipment->GetAnimatedSection(0)->m_renderOffset.y, pNewEquipment->GetAnimatedSection(0)->m_renderOffset.z);
			QubicleMatrix* pBodyMatrix = pNewEquipment->GetAnimatedSection(0)->m_pVoxelObject->GetQubicleModel()->GetQubicleMatrix("Body");
			pBodyMatrix->m_boneIndex = m_pVoxelCharacter->GetBodyBoneIndex();
			m_pVoxelCharacter->AddQubicleMatrix(pBodyMatrix, false);
		}
	}
	break;
	case EquipSlot_Legs:
	{
		if (m_pVoxelCharacter != NULL)
		{
			VoxelWeapon* pNewEquipment = new VoxelWeapon(m_pRenderer, m_pQubicleBinaryManager);
			pNewEquipment->SetVoxelCharacterParent(NULL);
			pNewEquipment->LoadWeapon(pItem->m_filename.c_str(), false);

			pNewEquipment->GetAnimatedSection(0)->m_pVoxelObject->GetQubicleModel()->SetScaleAndOffsetForMatrix("Legs", pNewEquipment->GetAnimatedSection(0)->m_renderScale, pNewEquipment->GetAnimatedSection(0)->m_renderOffset.x, pNewEquipment->GetAnimatedSection(0)->m_renderOffset.y, pNewEquipment->GetAnimatedSection(0)->m_renderOffset.z);
			QubicleMatrix* pLegsMatrix = pNewEquipment->GetAnimatedSection(0)->m_pVoxelObject->GetQubicleModel()->GetQubicleMatrix("Legs");
			pLegsMatrix->m_boneIndex = m_pVoxelCharacter->GetLegsBoneIndex();
			m_pVoxelCharacter->AddQubicleMatrix(pLegsMatrix, false);
		}
	}
	break;
	case EquipSlot_Hand:
	{
		if (m_pVoxelCharacter != NULL)
		{
			VoxelWeapon* pNewEquipment = new VoxelWeapon(m_pRenderer, m_pQubicleBinaryManager);
			pNewEquipment->SetVoxelCharacterParent(NULL);
			pNewEquipment->LoadWeapon(pItem->m_filename.c_str(), false);

			if (pItem->m_right)
			{
				pNewEquipment->GetAnimatedSection(0)->m_pVoxelObject->GetQubicleModel()->SetScaleAndOffsetForMatrix("Right_Hand", pNewEquipment->GetAnimatedSection(0)->m_renderScale, pNewEquipment->GetAnimatedSection(0)->m_renderOffset.x, pNewEquipment->GetAnimatedSection(0)->m_renderOffset.y, pNewEquipment->GetAnimatedSection(0)->m_renderOffset.z);
				QubicleMatrix* pRightHandMatrix = pNewEquipment->GetAnimatedSection(0)->m_pVoxelObject->GetQubicleModel()->GetQubicleMatrix("Right_Hand");
				pRightHandMatrix->m_boneIndex = m_pVoxelCharacter->GetRightHandBoneIndex();
				m_pVoxelCharacter->AddQubicleMatrix(pRightHandMatrix, false);
			}

			if (pItem->m_left)
			{
				pNewEquipment->GetAnimatedSection(1)->m_pVoxelObject->GetQubicleModel()->SetScaleAndOffsetForMatrix("Left_Hand", pNewEquipment->GetAnimatedSection(1)->m_renderScale, pNewEquipment->GetAnimatedSection(1)->m_renderOffset.x, pNewEquipment->GetAnimatedSection(1)->m_renderOffset.y, pNewEquipment->GetAnimatedSection(1)->m_renderOffset.z);
				QubicleMatrix* pLeftHandMatrix = pNewEquipment->GetAnimatedSection(1)->m_pVoxelObject->GetQubicleModel()->GetQubicleMatrix("Left_Hand");
				pLeftHandMatrix->m_boneIndex = m_pVoxelCharacter->GetLeftHandBoneIndex();
				m_pVoxelCharacter->AddQubicleMatrix(pLeftHandMatrix, false);
			}
		}
	}
	break;
	case EquipSlot_Feet:
	{
		if (m_pVoxelCharacter != NULL)
		{
			VoxelWeapon* pNewEquipment = new VoxelWeapon(m_pRenderer, m_pQubicleBinaryManager);
			pNewEquipment->SetVoxelCharacterParent(NULL);
			pNewEquipment->LoadWeapon(pItem->m_filename.c_str(), false);

			if (pItem->m_right)
			{
				pNewEquipment->GetAnimatedSection(0)->m_pVoxelObject->GetQubicleModel()->SetScaleAndOffsetForMatrix("Right_Foot", pNewEquipment->GetAnimatedSection(0)->m_renderScale, pNewEquipment->GetAnimatedSection(0)->m_renderOffset.x, pNewEquipment->GetAnimatedSection(0)->m_renderOffset.y, pNewEquipment->GetAnimatedSection(0)->m_renderOffset.z);
				QubicleMatrix* pRightFootMatrix = pNewEquipment->GetAnimatedSection(0)->m_pVoxelObject->GetQubicleModel()->GetQubicleMatrix("Right_Foot");
				pRightFootMatrix->m_boneIndex = m_pVoxelCharacter->GetRightFootBoneIndex();
				m_pVoxelCharacter->AddQubicleMatrix(pRightFootMatrix, false);
			}

			if (pItem->m_left)
			{
				pNewEquipment->GetAnimatedSection(1)->m_pVoxelObject->GetQubicleModel()->SetScaleAndOffsetForMatrix("Left_Foot", pNewEquipment->GetAnimatedSection(1)->m_renderScale, pNewEquipment->GetAnimatedSection(1)->m_renderOffset.x, pNewEquipment->GetAnimatedSection(1)->m_renderOffset.y, pNewEquipment->GetAnimatedSection(1)->m_renderOffset.z);
				QubicleMatrix* pLeftFootMatrix = pNewEquipment->GetAnimatedSection(1)->m_pVoxelObject->GetQubicleModel()->GetQubicleMatrix("Left_Foot");
				pLeftFootMatrix->m_boneIndex = m_pVoxelCharacter->GetLeftFootBoneIndex();
				m_pVoxelCharacter->AddQubicleMatrix(pLeftFootMatrix, false);
			}
		}
	}
	break;
	case EquipSlot_Accessory1:
	{
	}
	break;
	case EquipSlot_Accessory2:
	{
	}
	break;
	}

	// Refresh stat modifier cache since we have equipped an item
	RefreshStatModifierCacheValues();
}

void Player::UnequipItem(EquipSlot equipSlot)
{
	switch (equipSlot)
	{
	case EquipSlot_LeftHand:
	{
		UnloadWeapon(true);

		SetBow(false);
		SetShield(false);
		SetTorch(false);

		InventoryItem* pRightHand = m_pInventoryManager->GetInventoryItemForEquipSlot(EquipSlot_RightHand);
		if (pRightHand == NULL || pRightHand->m_itemType != InventoryType_Weapon_SpellHands)
		{
			SetSpellHands(false);
		}

		m_pVoxelCharacter->SetQubicleMatrixRender("Left_Hand", true);

		m_pVoxelCharacter->RemoveQubicleMatrix("Quiver");
	}
	break;
	case EquipSlot_RightHand:
	{
		UnloadWeapon(false);

		SetSword(false);
		SetAxe(false);
		SetHammer(false);
		SetMace(false);
		SetDagger(false);
		SetSickle(false);
		Set2HandedSword(false);
		SetBoomerang(false);
		SetBomb(false);
		SetStaff(false);
		SetWand(false);
		SetPickaxe(false);
		SetItemPlacing(false);
		SetSceneryPlacing(false);
		SetBlockPlacing(false);
		SetConsumable(false);

		InventoryItem* pLeftHand = m_pInventoryManager->GetInventoryItemForEquipSlot(EquipSlot_LeftHand);
		if (pLeftHand == NULL || pLeftHand->m_itemType != InventoryType_Weapon_SpellHands)
		{
			SetSpellHands(false);
		}

		m_pVoxelCharacter->SetQubicleMatrixRender("Right_Hand", true);

		/*SetStealth(false);*/

		m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Right_Arm_Hand, false, AnimationSections_Right_Arm_Hand, "BindPose", 0.25f);
	}
	break;
	case EquipSlot_Head:
	{
		m_pVoxelCharacter->RemoveQubicleMatrix("Helm");
	}
	break;
	case EquipSlot_Shoulders:
	{
		m_pVoxelCharacter->RemoveQubicleMatrix("Right_Shoulder_Armor");
		m_pVoxelCharacter->RemoveQubicleMatrix("Left_Shoulder_Armor");
	}
	break;
	case EquipSlot_Body:
	{
		QubicleMatrix* pBodyMatrix = m_pCharacterBackup->GetQubicleMatrix("Body");
		pBodyMatrix->m_boneIndex = m_pVoxelCharacter->GetBodyBoneIndex();
		m_pVoxelCharacter->AddQubicleMatrix(pBodyMatrix, false);

		char characterFilename[128];
		sprintf(characterFilename, "media/gamedata/characters/%s/%s.character", m_type.c_str(), m_modelName.c_str());
		m_pVoxelCharacter->ResetMatrixParamsFromCharacterFile(characterFilename, "Body");
	}
	break;
	case EquipSlot_Legs:
	{
		QubicleMatrix* pLegsMatrix = m_pCharacterBackup->GetQubicleMatrix("Legs");
		pLegsMatrix->m_boneIndex = m_pVoxelCharacter->GetLegsBoneIndex();
		m_pVoxelCharacter->AddQubicleMatrix(pLegsMatrix, false);

		char characterFilename[128];
		sprintf(characterFilename, "media/gamedata/characters/%s/%s.character", m_type.c_str(), m_modelName.c_str());
		m_pVoxelCharacter->ResetMatrixParamsFromCharacterFile(characterFilename, "Legs");
	}
	break;
	case EquipSlot_Hand:
	{
		QubicleMatrix* pRightHandMatrix = m_pCharacterBackup->GetQubicleMatrix("Right_Hand");
		pRightHandMatrix->m_boneIndex = m_pVoxelCharacter->GetRightHandBoneIndex();
		m_pVoxelCharacter->AddQubicleMatrix(pRightHandMatrix, false);

		QubicleMatrix* pLeftHandMatrix = m_pCharacterBackup->GetQubicleMatrix("Left_Hand");
		pLeftHandMatrix->m_boneIndex = m_pVoxelCharacter->GetLeftHandBoneIndex();
		m_pVoxelCharacter->AddQubicleMatrix(pLeftHandMatrix, false);

		char characterFilename[128];
		sprintf(characterFilename, "media/gamedata/characters/%s/%s.character", m_type.c_str(), m_modelName.c_str());
		m_pVoxelCharacter->ResetMatrixParamsFromCharacterFile(characterFilename, "Right_Hand");
		m_pVoxelCharacter->ResetMatrixParamsFromCharacterFile(characterFilename, "Left_Hand");
	}
	break;
	case EquipSlot_Feet:
	{
		QubicleMatrix* pRightFootMatrix = m_pCharacterBackup->GetQubicleMatrix("Right_Foot");
		pRightFootMatrix->m_boneIndex = m_pVoxelCharacter->GetRightFootBoneIndex();
		m_pVoxelCharacter->AddQubicleMatrix(pRightFootMatrix, false);

		QubicleMatrix* pLeftFootMatrix = m_pCharacterBackup->GetQubicleMatrix("Left_Foot");
		pLeftFootMatrix->m_boneIndex = m_pVoxelCharacter->GetLeftFootBoneIndex();
		m_pVoxelCharacter->AddQubicleMatrix(pLeftFootMatrix, false);

		char characterFilename[128];
		sprintf(characterFilename, "media/gamedata/characters/%s/%s.character", m_type.c_str(), m_modelName.c_str());
		m_pVoxelCharacter->ResetMatrixParamsFromCharacterFile(characterFilename, "Right_Foot");
		m_pVoxelCharacter->ResetMatrixParamsFromCharacterFile(characterFilename, "Left_Foot");
	}
	break;
	case EquipSlot_Accessory1:
	{
	}
	break;
	case EquipSlot_Accessory2:
	{
	}
	break;
	}

	// Refresh stat modifier cache since we have un-equipped an item
	RefreshStatModifierCacheValues();
}

void Player::RefreshStatModifierCacheValues()
{
	m_strengthModifier = 0;
	m_dexterityModifier = 0;
	m_intelligenceModifier = 0;
	m_vitalityModifier = 0;
	m_armorModifier = 0;
	m_luckModifier = 0;

	for (int i = 0; i < EquipSlot_NumSlots; i++)
	{
		InventoryItem *pEquippedItem = m_pInventoryManager->GetInventoryItemForEquipSlot((EquipSlot)i);

		if (pEquippedItem != NULL)
		{
			for (int j = 0; j < (int)pEquippedItem->m_vpStatAttributes.size(); j++)
			{
				switch (pEquippedItem->m_vpStatAttributes[j]->GetType())
				{
				case AttributeType_Strength: { m_strengthModifier += pEquippedItem->m_vpStatAttributes[j]->GetModifyAmount(); break; }
				case AttributeType_Dexterity: { m_dexterityModifier += pEquippedItem->m_vpStatAttributes[j]->GetModifyAmount(); break; }
				case AttributeType_Intelligence: { m_intelligenceModifier += pEquippedItem->m_vpStatAttributes[j]->GetModifyAmount(); break; }
				case AttributeType_Vitality: { m_vitalityModifier += pEquippedItem->m_vpStatAttributes[j]->GetModifyAmount(); break; }
				case AttributeType_Armor: { m_armorModifier += pEquippedItem->m_vpStatAttributes[j]->GetModifyAmount(); break; }
				case AttributeType_Luck: { m_luckModifier += pEquippedItem->m_vpStatAttributes[j]->GetModifyAmount(); break; }
				}
			}
		}
	}

	//m_pCharacterGUI->UpdatePlayerStats();
}

// Collision
bool Player::CheckCollisions(vec3 positionCheck, vec3 previousPosition, vec3 *pNormal, vec3 *pMovement, bool *pStepUpBlock)
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

							worldCollision = true;
						}
					}
				}
			}
		}
	}

	*pStepUpBlock = canAllStepUp;

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

int Player::GetGridX() const
{
	return m_gridPositionX;
}

int Player::GetGridY() const
{
	return m_gridPositionY;
}

int Player::GetGridZ() const
{
	return m_gridPositionZ;
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

		if (m_bDoStepUpAnimation == false)
		{
			bool stepUp = false;
			if (CheckCollisions(posToCheck, m_previousPosition, &pNormal, &movement, &stepUp))
			{
				if (m_bIsOnGround == true && m_bCanJump == true)  // Only allow step up block animations when we are on the ground and also when we can jump. i.e not in air.
				{
					if (stepUp)
					{
						if (m_bDoStepUpAnimation == false)
						{
							m_bDoStepUpAnimation = true;

							m_stepUpAnimationYAmount = 0.0f;
							m_stepUpAnimationPrevious = 0.0f;
							m_stepUpAnimationYOffset = 0.0f;
							Interpolator::GetInstance()->AddFloatInterpolation(&m_stepUpAnimationYAmount, 0.0f, (Chunk::BLOCK_RENDER_SIZE*2.2f), 0.1f, 0.0f, NULL, _StepUpAnimationFinished, this);
							Interpolator::GetInstance()->AddFloatInterpolation(&m_stepUpAnimationYOffset, (Chunk::BLOCK_RENDER_SIZE*2.2f), 0.0f, 0.125f, -100.0f);
						}
					}
				}
			}
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

	if (m_bIsOnGround == false)
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

// Player equipped attributes
void Player::SetNormal()
{
	m_equippedProperties = PlayerEquippedProperties_None;
}

void Player::SetSword(bool s)
{
	if (s)
	{
		m_equippedProperties |= PlayerEquippedProperties_Sword;
	}
	else
	{
		m_equippedProperties &= ~PlayerEquippedProperties_Sword;
	}
}

void Player::SetAxe(bool s)
{
	if (s)
	{
		m_equippedProperties |= PlayerEquippedProperties_Axe;
	}
	else
	{
		m_equippedProperties &= ~PlayerEquippedProperties_Axe;
	}
}

void Player::SetHammer(bool s)
{
	if (s)
	{
		m_equippedProperties |= PlayerEquippedProperties_Hammer;
	}
	else
	{
		m_equippedProperties &= ~PlayerEquippedProperties_Hammer;
	}
}

void Player::SetMace(bool s)
{
	if (s)
	{
		m_equippedProperties |= PlayerEquippedProperties_Mace;
	}
	else
	{
		m_equippedProperties &= ~PlayerEquippedProperties_Mace;
	}
}

void Player::SetDagger(bool s)
{
	if (s)
	{
		m_equippedProperties |= PlayerEquippedProperties_Dagger;
	}
	else
	{
		m_equippedProperties &= ~PlayerEquippedProperties_Dagger;
	}
}

void Player::SetSickle(bool s)
{
	if (s)
	{
		m_equippedProperties |= PlayerEquippedProperties_Sickle;
	}
	else
	{
		m_equippedProperties &= ~PlayerEquippedProperties_Sickle;
	}
}

void Player::Set2HandedSword(bool s)
{
	if (s)
	{
		m_equippedProperties |= PlayerEquippedProperties_2HandedSword;
	}
	else
	{
		m_equippedProperties &= ~PlayerEquippedProperties_2HandedSword;
	}
}

void Player::SetShield(bool s)
{
	if (s)
	{
		m_equippedProperties |= PlayerEquippedProperties_Shield;
	}
	else
	{
		m_equippedProperties &= ~PlayerEquippedProperties_Shield;
	}
}

void Player::SetBoomerang(bool s)
{
	if (s)
	{
		m_equippedProperties |= PlayerEquippedProperties_Boomerang;
	}
	else
	{
		m_equippedProperties &= ~PlayerEquippedProperties_Boomerang;
	}
}

void Player::SetBow(bool s)
{
	if (s)
	{
		m_equippedProperties |= PlayerEquippedProperties_Bow;
	}
	else
	{
		m_equippedProperties &= ~PlayerEquippedProperties_Bow;
	}
}

void Player::SetStaff(bool s)
{
	if (s)
	{
		m_equippedProperties |= PlayerEquippedProperties_Staff;
	}
	else
	{
		m_equippedProperties &= ~PlayerEquippedProperties_Staff;
	}
}

void Player::SetWand(bool s)
{
	if (s)
	{
		m_equippedProperties |= PlayerEquippedProperties_Wand;
	}
	else
	{
		m_equippedProperties &= ~PlayerEquippedProperties_Wand;
	}
}

void Player::SetPickaxe(bool s)
{
	if (s)
	{
		m_equippedProperties |= PlayerEquippedProperties_Pickaxe;
	}
	else
	{
		m_equippedProperties &= ~PlayerEquippedProperties_Pickaxe;
	}
}

void Player::SetTorch(bool s)
{
	if (s)
	{
		m_equippedProperties |= PlayerEquippedProperties_Torch;
	}
	else
	{
		m_equippedProperties &= ~PlayerEquippedProperties_Torch;
	}
}

void Player::SetItemPlacing(bool s)
{
	if (s)
	{
		m_equippedProperties |= PlayerEquippedProperties_PlacementItem;
	}
	else
	{
		m_equippedProperties &= ~PlayerEquippedProperties_PlacementItem;
	}
}

void Player::SetSceneryPlacing(bool s)
{
	if (s)
	{
		m_equippedProperties |= PlayerEquippedProperties_PlacementScenery;
	}
	else
	{
		m_equippedProperties &= ~PlayerEquippedProperties_PlacementScenery;
	}
}

void Player::SetBlockPlacing(bool s)
{
	if (s)
	{
		m_equippedProperties |= PlayerEquippedProperties_PlacementBlock;
	}
	else
	{
		m_equippedProperties &= ~PlayerEquippedProperties_PlacementBlock;
	}
}

void Player::SetConsumable(bool s)
{
	if (s)
	{
		m_equippedProperties |= PlayerEquippedProperties_Consumable;
	}
	else
	{
		m_equippedProperties &= ~PlayerEquippedProperties_Consumable;
	}
}

void Player::SetBomb(bool s)
{
	if (s)
	{
		m_equippedProperties |= PlayerEquippedProperties_Bomb;
	}
	else
	{
		m_equippedProperties &= ~PlayerEquippedProperties_Bomb;
	}
}

void Player::SetSpellHands(bool s)
{
	if (s)
	{
		m_equippedProperties |= PlayerEquippedProperties_SpellHands;
	}
	else
	{
		m_equippedProperties &= ~PlayerEquippedProperties_SpellHands;
	}
}

bool Player::IsNormal()
{
	return (m_equippedProperties & PlayerEquippedProperties_None) == PlayerEquippedProperties_None;
}

bool Player::IsSword()
{
	return (m_equippedProperties & PlayerEquippedProperties_Sword) == PlayerEquippedProperties_Sword;
}

bool Player::IsAxe()
{
	return (m_equippedProperties & PlayerEquippedProperties_Axe) == PlayerEquippedProperties_Axe;
}

bool Player::IsHammer()
{
	return (m_equippedProperties & PlayerEquippedProperties_Hammer) == PlayerEquippedProperties_Hammer;
}

bool Player::IsMace()
{
	return (m_equippedProperties & PlayerEquippedProperties_Mace) == PlayerEquippedProperties_Mace;
}

bool Player::IsDagger()
{
	return (m_equippedProperties & PlayerEquippedProperties_Dagger) == PlayerEquippedProperties_Dagger;
}

bool Player::IsSickle()
{
	return (m_equippedProperties & PlayerEquippedProperties_Sickle) == PlayerEquippedProperties_Sickle;
}

bool Player::Is2HandedSword()
{
	return (m_equippedProperties & PlayerEquippedProperties_2HandedSword) == PlayerEquippedProperties_2HandedSword;
}

bool Player::IsShield()
{
	return (m_equippedProperties & PlayerEquippedProperties_Shield) == PlayerEquippedProperties_Shield;
}

bool Player::IsBoomerang()
{
	return (m_equippedProperties & PlayerEquippedProperties_Boomerang) == PlayerEquippedProperties_Boomerang;
}

bool Player::IsBow()
{
	return (m_equippedProperties & PlayerEquippedProperties_Bow) == PlayerEquippedProperties_Bow;
}

bool Player::IsStaff()
{
	return (m_equippedProperties & PlayerEquippedProperties_Staff) == PlayerEquippedProperties_Staff;
}

bool Player::IsWand()
{
	return (m_equippedProperties & PlayerEquippedProperties_Wand) == PlayerEquippedProperties_Wand;
}

bool Player::IsPickaxe()
{
	return (m_equippedProperties & PlayerEquippedProperties_Pickaxe) == PlayerEquippedProperties_Pickaxe;
}

bool Player::IsTorch()
{
	return (m_equippedProperties & PlayerEquippedProperties_Torch) == PlayerEquippedProperties_Torch;
}

bool Player::IsItemPlacing()
{
	return (m_equippedProperties & PlayerEquippedProperties_PlacementItem) == PlayerEquippedProperties_PlacementItem;
}

bool Player::IsSceneryPlacing()
{
	return (m_equippedProperties & PlayerEquippedProperties_PlacementScenery) == PlayerEquippedProperties_PlacementScenery;
}

bool Player::IsBlockPlacing()
{
	return (m_equippedProperties & PlayerEquippedProperties_PlacementBlock) == PlayerEquippedProperties_PlacementBlock;
}

bool Player::IsConsumable()
{
	return (m_equippedProperties & PlayerEquippedProperties_Consumable) == PlayerEquippedProperties_Consumable;
}

bool Player::IsBomb()
{
	return (m_equippedProperties & PlayerEquippedProperties_Bomb) == PlayerEquippedProperties_Bomb;
}

bool Player::IsSpellHands()
{
	return (m_equippedProperties & PlayerEquippedProperties_SpellHands) == PlayerEquippedProperties_SpellHands;
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
		m_position.x, m_position.y - (m_bDoStepUpAnimation ? 0.0f : m_stepUpAnimationYOffset), m_position.z, 1.0f
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

	if (m_groundCheckTimer <= 0.0f)
	{
		m_bIsOnGround = false;
	}

	// Step up animation
	float stepUpAddition = 0.0f;	
	if (m_bDoStepUpAnimation)
	{
		stepUpAddition = m_stepUpAnimationYAmount - m_stepUpAnimationPrevious;
		m_position.y += stepUpAddition;

		m_stepUpAnimationPrevious = m_stepUpAnimationYAmount;
	}

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
			bool stepUp = false;
			if (CheckCollisions(posToCheck, m_previousPosition, &pNormal, &velAmount, &stepUp))
			{
				// Reset velocity, we don't have any bounce
				m_velocity = vec3(0.0f, 0.0f, 0.0f);
				velocityToUse = vec3(0.0f, 0.0f, 0.0f);

				if (velocityToUse.y <= 0.0f)
				{
					m_bIsOnGround = true;
					m_groundCheckTimer = 0.1f;

					if (m_bCanJump == false)
					{
						m_bCanJump = true;
					}
				}
			}
		}

		// Integrate position
		m_position += velocityToUse * dt;

		m_positionMovementAmount += vec3(0.0f, stepUpAddition + m_stepUpAdditionYAmountChangedCache, 0.0f);
		m_positionMovementAmount += velocityToUse * dt;
	}

	m_stepUpAdditionYAmountChangedCache = 0.0f;

	// Store previous position
	m_previousPosition = GetCenter();
}

void Player::UpdateTimers(float dt)
{
	// Jump timer
	if (m_jumpTimer >= 0.0f)
	{
		m_jumpTimer -= dt;
	}

	// Ground check timer
	if (m_groundCheckTimer >= 0.0f)
	{
		m_groundCheckTimer -= dt;
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

void Player::_StepUpAnimationFinished(void *apData)
{
	Player* lpPlayer = (Player*)apData;
	lpPlayer->StepUpAnimationFinished();
}

void Player::StepUpAnimationFinished()
{
	// Final addition
	m_stepUpAdditionYAmountChangedCache = m_stepUpAnimationYAmount - m_stepUpAnimationPrevious;
	m_position.y += m_stepUpAdditionYAmountChangedCache;

	m_bDoStepUpAnimation = false;
}