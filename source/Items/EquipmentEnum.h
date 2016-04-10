// ******************************************************************************
// Filename:    EquipmentEnum.h
// Project:     Vox
// Author:      Steven Ball
//
// Purpose:
//   An enum list of all of the equipment in the game, used for equipment
//   tracking and equipment specific functionality.
//
// Revision History:
//   Initial Revision - 12/04/14
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#pragma once

enum eEquipment
{
	eEquipment_None = 0,

	eEquipment_NormalPickaxe,
	eEquipment_Torch,
	eEquipment_Hammer,
	eEquipment_MageStaff,
	eEquipment_NecroStaff,
	eEquipment_PriestStaff,
	eEquipment_DruidStaff,
	eEquipment_2HandedSword,
	eEquipment_Boomerang,
	eEquipment_Bomb,
	eEquipment_KnifeLeft,
	eEquipment_KnifeRight,
	eEquipment_FireballHandLeft,
	eEquipment_FireballHandRight,

	// Wooden
	eEquipment_WoodenSword,
	eEquipment_WoodenShield,
	eEquipment_WoodenBow,
	eEquipment_WoodenHelm,
	eEquipment_WoodenArmor,
	eEquipment_WoodenPants,
	eEquipment_WoodenGloves,
	eEquipment_WoodenBoots,
	eEquipment_WoodenShoulders,

	// Copper

	// Iron
	eEquipment_IronSword,
	eEquipment_IronShield,
	eEquipment_IronHelm,
	eEquipment_IronArmor,
	eEquipment_IronPants,
	eEquipment_IronGloves,
	eEquipment_IronBoots,
	eEquipment_IronShoulders,

	// Silver

	// Gold

	eEquipment_NUM_EQUIPMENT,
};