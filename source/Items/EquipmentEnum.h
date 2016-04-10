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

	eEquipment_WoodenBow,

	eEquipment_IronSword,
	eEquipment_IronShield,

	eEquipment_NUM_EQUIPMENT,
};