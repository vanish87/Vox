// ******************************************************************************
// Filename:    StatAttribute.h
// Project:     Game
// Author:      Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 12/04/14
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#pragma once

#include <vector>
#include <string>
using namespace std;


enum AttributeType
{
	AttributeType_Strength = 0,
	AttributeType_Dexterity,
	AttributeType_Intelligence,
	AttributeType_Vitality,
	AttributeType_Armor,
	AttributeType_Luck,
};

class StatAttribute
{
public:
	/* Public methods */
	StatAttribute(AttributeType type, int modifyAmount);
	~StatAttribute();

	AttributeType GetType();
	void SetType(AttributeType type);

	int GetModifyAmount();
	void SetModifyAmount(int amount);

	string GetString();

protected:
	/* Protected methods */

private:
	/* Private methods */

public:
	/* Public members */

protected:
	/* Protected members */

private:
	/* Private members */
	AttributeType m_type;
	int m_modifyAmount;
};
