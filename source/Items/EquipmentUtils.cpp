// ******************************************************************************
// Filename:    EquipmentUtils.cpp
// Project:     Vox
// Author:      Steven Ball
//
// Purpose:
//   Utility functionality for equipment, mostly a way to associate equipment
//   enums with data like the friendly names and display strings.
//
// Revision History:
//   Initial Revision - 12/04/14
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "Item.h"

string GetEquipmentTitleForType(eEquipment type)
{
	switch(type)
	{
		case eEquipment_None:				{ return ""; } break;
		case eEquipment_NormalPickaxe:		{ return "Normal Pickaxe"; } break;
		case eEquipment_Torch:				{ return "Torch"; } break;
		case eEquipment_Hammer:				{ return "Hammer"; } break;
		case eEquipment_MageStaff:			{ return "Mage Staff"; } break;
		case eEquipment_NecroStaff:			{ return "Necro Staff"; } break;
		case eEquipment_PriestStaff:		{ return "Priest Staff"; } break;
		case eEquipment_DruidStaff:			{ return "Druid Staff"; } break;
		case eEquipment_2HandedSword:		{ return "Two Handed Sword"; } break;
		case eEquipment_Boomerang:			{ return "Boomerang"; } break;
		case eEquipment_Bomb:				{ return "Bomb"; } break;
		case eEquipment_WoodenBow:			{ return "Wooden Bow"; } break;
		case eEquipment_IronSword:			{ return "Iron Sword"; } break;
		case eEquipment_IronShield:			{ return "Iron Shield"; } break;
	}

	return "";
}

string GetEquipmentDescriptionForType(eEquipment type)
{
	switch(type)
	{
		case eEquipment_None:				{ return ""; } break;
		case eEquipment_NormalPickaxe:		{ return "Used for mining and digging the world."; } break;
		case eEquipment_Torch:				{ return "A torch to light up the darkness."; } break;
		case eEquipment_Hammer:				{ return "A hammer to smite your foes."; } break;
		case eEquipment_MageStaff:			{ return "A staff that contains powerful magic."; } break;
		case eEquipment_NecroStaff:			{ return "A necromancer staff once used by powerful mages of undeath."; } break;
		case eEquipment_PriestStaff:		{ return "A holy staff of pure energy."; } break;
		case eEquipment_DruidStaff:			{ return "A staff that is engraved with nature."; } break;
		case eEquipment_2HandedSword:		{ return "A sword that requires both hands to wield."; } break;
		case eEquipment_Boomerang:			{ return "Once thrown, the boomerang returns to you."; } break;
		case eEquipment_Bomb:				{ return "An explosive bomb, can destroy enemies and the landscape."; } break;
		case eEquipment_WoodenBow:			{ return "A wooden bow, made for shooting arrows."; } break;
		case eEquipment_IronSword:			{ return "A sturdy sword made of iron."; } break;
		case eEquipment_IronShield:			{ return "A shield of pure iron."; } break;
	}

	return "";
}

string GetEquipmentFilenameForType(eEquipment type)
{
	switch(type)
	{
		case eEquipment_None:				{ return ""; } break;
		case eEquipment_NormalPickaxe:		{ return "media/gamedata/weapons/Pickaxe/Pickaxe.weapon"; } break;
		case eEquipment_Torch:				{ return "media/gamedata/weapons/Torch/Torch.weapon"; } break;
		case eEquipment_Hammer:				{ return "media/gamedata/weapons/Hammer/Hammer.weapon"; } break;
		case eEquipment_MageStaff:			{ return "media/gamedata/weapons/Staff/Staff.weapon"; } break;
		case eEquipment_NecroStaff:			{ return "media/gamedata/weapons/NecroStaff/NecroStaff.weapon"; } break;
		case eEquipment_PriestStaff:		{ return "media/gamedata/weapons/PriestStaff/PriestStaff.weapon"; } break;
		case eEquipment_DruidStaff:			{ return "media/gamedata/weapons/DruidStaff/DruidStaff.weapon"; } break;
		case eEquipment_2HandedSword:		{ return "media/gamedata/weapons/2HandedSword/2HandedSword.weapon"; } break;
		case eEquipment_Boomerang:			{ return "media/gamedata/weapons/Boomerang/Boomerang.weapon"; } break;
		case eEquipment_Bomb:				{ return "media/gamedata/items/Bomb/Bomb.item"; } break;
		case eEquipment_WoodenBow:			{ return "media/gamedata/weapons/WoodenBow/WoodenBow.weapon"; } break;
		case eEquipment_IronSword:			{ return "media/gamedata/weapons/IronSword/IronSword.weapon"; } break;
		case eEquipment_IronShield:			{ return "media/gamedata/weapons/IronShield/IronShield.weapon"; } break;
	}

	return "";
}

string GetEquipmentTextureForType(eEquipment type)
{
	switch(type)
	{
		case eEquipment_None:				{ return ""; } break;
		case eEquipment_NormalPickaxe:		{ return "media/textures/items/pickaxe.tga"; } break;
		case eEquipment_Torch:				{ return "media/textures/items/torch.tga"; } break;
		case eEquipment_Hammer:				{ return "media/textures/items/hammer.tga"; } break;
		case eEquipment_MageStaff:			{ return "media/textures/items/staff.tga"; } break;
		case eEquipment_NecroStaff:			{ return "media/textures/items/staff.tga"; } break;
		case eEquipment_PriestStaff:		{ return "media/textures/items/staff.tga"; } break;
		case eEquipment_DruidStaff:			{ return "media/textures/items/staff.tga"; } break;
		case eEquipment_2HandedSword:		{ return "media/textures/items/2handed_sword.tga"; } break;
		case eEquipment_Boomerang:			{ return "media/textures/items/boomerang.tga"; } break;
		case eEquipment_Bomb:				{ return "media/textures/items/bomb.tga"; } break;
		case eEquipment_WoodenBow:			{ return "media/textures/items/wooden_bow.tga"; } break;
		case eEquipment_IronSword:			{ return "media/textures/items/iron_sword.tga"; } break;
		case eEquipment_IronShield:			{ return "media/textures/items/iron_shield.tga"; } break;
	}

	return "";
}

InventoryType GetInventoryTypeForEquipment(eEquipment type)
{
	switch(type)
	{
		case eEquipment_None:				{ return InventoryType_Nothing; } break;
		case eEquipment_NormalPickaxe:		{ return InventoryType_Weapon_Pickaxe; } break;
		case eEquipment_Torch:				{ return InventoryType_Weapon_Torch; } break;
		case eEquipment_Hammer:				{ return InventoryType_Weapon_Hammer; } break;
		case eEquipment_MageStaff:			{ return InventoryType_Weapon_Staff; } break;
		case eEquipment_NecroStaff:			{ return InventoryType_Weapon_Staff; } break;
		case eEquipment_PriestStaff:		{ return InventoryType_Weapon_Staff; } break;
		case eEquipment_DruidStaff:			{ return InventoryType_Weapon_Staff; } break;
		case eEquipment_2HandedSword:		{ return InventoryType_Weapon_2HandedSword; } break;
		case eEquipment_Boomerang:			{ return InventoryType_Weapon_Boomerang; } break;
		case eEquipment_Bomb:				{ return InventoryType_Weapon_Bomb; } break;
		case eEquipment_WoodenBow:			{ return InventoryType_Weapon_Bow; } break;
		case eEquipment_IronSword:			{ return InventoryType_Weapon_Sword; } break;
		case eEquipment_IronShield:			{ return InventoryType_Weapon_Shield; } break;
	}

	return InventoryType_Nothing;
}

EquipSlot GetEquipSlotForEquipment(eEquipment type)
{
	switch (type)
	{
		case eEquipment_None:				{ return EquipSlot_NoSlot; } break;
		case eEquipment_NormalPickaxe:		{ return EquipSlot_RightHand; } break;
		case eEquipment_Torch:				{ return EquipSlot_LeftHand; } break;
		case eEquipment_Hammer:				{ return EquipSlot_RightHand; } break;
		case eEquipment_MageStaff:			{ return EquipSlot_RightHand; } break;
		case eEquipment_NecroStaff:			{ return EquipSlot_RightHand; } break;
		case eEquipment_PriestStaff:		{ return EquipSlot_RightHand; } break;
		case eEquipment_DruidStaff:			{ return EquipSlot_RightHand; } break;
		case eEquipment_2HandedSword:		{ return EquipSlot_RightHand; } break;
		case eEquipment_Boomerang:			{ return EquipSlot_RightHand; } break;
		case eEquipment_Bomb:				{ return EquipSlot_RightHand; } break;
		case eEquipment_WoodenBow:			{ return EquipSlot_LeftHand; } break;
		case eEquipment_IronSword:			{ return EquipSlot_RightHand; } break;
		case eEquipment_IronShield:			{ return EquipSlot_LeftHand; } break;
	}

	return EquipSlot_NoSlot;
}

ItemQuality GetItemQualityForEquipment(eEquipment type)
{
	switch (type)
	{
		case eEquipment_None:				{ return ItemQuality_Common; } break;
		case eEquipment_NormalPickaxe:		{ return ItemQuality_Common; } break;
		case eEquipment_Torch:				{ return ItemQuality_Common; } break;
		case eEquipment_Hammer:				{ return ItemQuality_Common; } break;
		case eEquipment_MageStaff:			{ return ItemQuality_Common; } break;
		case eEquipment_NecroStaff:			{ return ItemQuality_Common; } break;
		case eEquipment_PriestStaff:		{ return ItemQuality_Common; } break;
		case eEquipment_DruidStaff:			{ return ItemQuality_Common; } break;
		case eEquipment_2HandedSword:		{ return ItemQuality_Common; } break;
		case eEquipment_Boomerang:			{ return ItemQuality_Common; } break;
		case eEquipment_Bomb:				{ return ItemQuality_Common; } break;
		case eEquipment_WoodenBow:			{ return ItemQuality_Common; } break;
		case eEquipment_IronSword:			{ return ItemQuality_Common; } break;
		case eEquipment_IronShield:			{ return ItemQuality_Common; } break;
	}

	return ItemQuality_Common;
}

void GetItemSidesForEquipment(eEquipment type, bool *left, bool *right)
{
	switch (type)
	{
		case eEquipment_None:				{ *left = false; *right = false; } break;
		case eEquipment_NormalPickaxe:		{ *left = false; *right = false; } break;
		case eEquipment_Torch:				{ *left = false; *right = false; } break;
		case eEquipment_Hammer:				{ *left = false; *right = false; } break;
		case eEquipment_MageStaff:			{ *left = false; *right = false; } break;
		case eEquipment_NecroStaff:			{ *left = false; *right = false; } break;
		case eEquipment_PriestStaff:		{ *left = false; *right = false; } break;
		case eEquipment_DruidStaff:			{ *left = false; *right = false; } break;
		case eEquipment_2HandedSword:		{ *left = false; *right = false; } break;
		case eEquipment_Boomerang:			{ *left = false; *right = false; } break;
		case eEquipment_Bomb:				{ *left = false; *right = false; } break;
		case eEquipment_WoodenBow:			{ *left = false; *right = false; } break;
		case eEquipment_IronSword:			{ *left = false; *right = false; } break;
		case eEquipment_IronShield:			{ *left = false; *right = false; } break;
	}
}