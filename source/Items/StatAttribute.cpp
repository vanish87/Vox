// ******************************************************************************
// Filename:    StatAttribute.cpp
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

#include "StatAttribute.h"


StatAttribute::StatAttribute(AttributeType type, int modifyAmount)
{
	m_type = type;
	m_modifyAmount = modifyAmount;
}

StatAttribute::~StatAttribute()
{
}

AttributeType StatAttribute::GetType()
{
	return m_type;
}

void StatAttribute::SetType(AttributeType type)
{
	m_type = type;
}

int StatAttribute::GetModifyAmount()
{
	return m_modifyAmount;
}

void StatAttribute::SetModifyAmount(int amount)
{
	m_modifyAmount = amount;
}

string StatAttribute::GetString()
{
	char returnString[64];
	char signModifier[2];
	char colourModifier[16];
	if(m_modifyAmount > 0)
	{
		sprintf(signModifier, "+");
		sprintf(colourModifier, "Green");
	}
	else
	{
		sprintf(signModifier, "");
		sprintf(colourModifier, "Red");
	}
	switch (m_type)
	{
	case AttributeType_Strength:		{ sprintf(returnString, "[C=%s]%s%i Strength", colourModifier, signModifier, m_modifyAmount); break; }
	case AttributeType_Dexterity:		{ sprintf(returnString, "[C=%s]%s%i Dexterity", colourModifier, signModifier, m_modifyAmount); break; }
	case AttributeType_Intelligence:	{ sprintf(returnString, "[C=%s]%s%i Magic", colourModifier, signModifier, m_modifyAmount); break; }
	case AttributeType_Vitality:		{ sprintf(returnString, "[C=%s]%s%i Vitality", colourModifier, signModifier, m_modifyAmount); break; }
	case AttributeType_Armor:			{ sprintf(returnString, "[C=%s]%s%i Armor", colourModifier, signModifier, m_modifyAmount); break; }
	case AttributeType_Luck:			{ sprintf(returnString, "[C=%s]%s%i Luck", colourModifier, signModifier, m_modifyAmount); break; }
	}

	return returnString;
}