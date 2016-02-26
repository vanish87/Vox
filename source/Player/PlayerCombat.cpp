// ******************************************************************************
// Filename:    PlayerCombat.cpp
// Project:     Vox
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 27/10/15
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "Player.h"
#include "../utils/Random.h"
#include "../Projectile/ProjectileManager.h"
#include "../Projectile/Projectile.h"

// Combat
void Player::PressAttack()
{
	if (IsDead())
	{
		return;
	}

	if (IsBow())
	{
		if (m_bIsChargingAttack == false)
		{
			m_bIsChargingAttack = true;
			m_chargeAmount = 0.0f;

			LoadWeapon(false, "media/gamedata/items/Arrow/ArrowHold.item");

			if (m_bIsIdle)
			{
				m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_FullBody, false, AnimationSections_FullBody, "BowCharge", 0.2f);
			}
			else
			{
				m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Head_Body, false, AnimationSections_Head_Body, "BowCharge", 0.2f);
				m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Left_Arm_Hand, false, AnimationSections_Left_Arm_Hand, "BowCharge", 0.2f);
				m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Right_Arm_Hand, false, AnimationSections_Right_Arm_Hand, "BowCharge", 0.2f);
			}
		}
	}
	else if (IsBoomerang())
	{
	}
	else if (IsStaff())
	{
		m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Right_Arm_Hand, false, AnimationSections_Right_Arm_Hand, "StaffAttack", 0.01f);
		m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Left_Arm_Hand, false, AnimationSections_Left_Arm_Hand, "StaffAttack", 0.01f);
		m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Head_Body, false, AnimationSections_Head_Body, "StaffAttack", 0.01f);
	}
	else if (IsSpellHands())
	{
	}
	else if (IsWand())
	{
	}
	else if (IsBomb())
	{
	}
	else if (IsConsumable())
	{
	}
	else if (IsDagger())
	{
		m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_FullBody, true, AnimationSections_FullBody, "SwordAttack2", 0.01f);
		m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Right_Arm_Hand, false, AnimationSections_Right_Arm_Hand, "SwordAttack2", 0.01f);
		m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Left_Arm_Hand, false, AnimationSections_Left_Arm_Hand, "SwordAttack2", 0.01f);
	}
	else if (IsHammer())
	{
	}
	else if (IsMace())
	{
	}
	else if (IsSickle())
	{
	}
	else if (IsPickaxe())
	{
		m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_FullBody, true, AnimationSections_FullBody, "Mine", 0.01f);
		m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Right_Arm_Hand, false, AnimationSections_Right_Arm_Hand, "Mine", 0.01f);
	}
	else if (IsTorch())
	{
	}
	else if (IsAxe())
	{
	}
	else if (Is2HandedSword())
	{
		m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_FullBody, true, AnimationSections_FullBody, "2HandedSwordAttack", 0.01f);
		m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Right_Arm_Hand, false, AnimationSections_Right_Arm_Hand, "2HandedSwordAttack", 0.01f);
		m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Left_Arm_Hand, false, AnimationSections_Left_Arm_Hand, "2HandedSwordAttack", 0.01f);
	}
	else if (IsSword())
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
	else if (IsBlockPlacing())
	{
	}
	else if (IsItemPlacing())
	{
	}
	else if (IsSceneryPlacing())
	{
	}
}

void Player::ReleaseAttack()
{
	if (IsDead())
	{
		return;
	}

	if (IsBow())
	{
		if (m_bIsChargingAttack == true)
		{
			UnloadWeapon(false);

			Projectile* pProjectile = m_pProjectileManager->CreateProjectile(m_chargeSpawnPosition, m_chargeSpawnVelocity, 0.0f, "media/gamedata/items/Arrow/Arrow.item", 0.08f);
			pProjectile->SetProjectileType(true, false, false);
			// TODO : Add me back in
			//pProjectile->SetOwner(this, NULL, NULL);

			m_bIsChargingAttack = false;
			m_chargeAmount = 0.0f;

			if (m_bIsIdle)
			{
				m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_FullBody, false, AnimationSections_FullBody, "BindPose", 0.2f);
			}
			else
			{
				m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Head_Body, false, AnimationSections_Head_Body, "BindPose", 0.2f);
				m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Left_Arm_Hand, false, AnimationSections_Left_Arm_Hand, "BindPose", 0.2f);
				m_pVoxelCharacter->BlendIntoAnimation(AnimationSections_Right_Arm_Hand, false, AnimationSections_Right_Arm_Hand, "BindPose", 0.2f);
			}
		}
	}
}

bool Player::CanAttackLeft()
{
	return m_bCanAttackLeft;
}

bool Player::CanAttackRight()
{
	return m_bCanAttackRight;
}