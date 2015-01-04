#pragma once
#include "Message.h"
#include "../MainInfo/Entity.h"
#include "../MainInfo/Game.h"
#include "../Grid/Tile.h"

//Enumerated faction and unit types
//enum UnitType {UNIT_BASE, UNIT_SCOUT, UNIT_INFANTRY, UNIT_TANK};
//enum OwnerFaction {OWNER_PLANTS, OWNER_ROBOTS};

class CCreateEntityMessage : public CMessage
{
private:
	//What type of unit to create, for who
	UnitType m_nType;
	OwnerFaction m_nOwner;

	//And where to put it
	int m_nX, m_nY, m_nHealth;
	bool m_bMoved, m_bActed;
	CTile *tile;

public:

	CCreateEntityMessage(UnitType nType, OwnerFaction nOwner, int nX, int nY, int nHealth =  -1, bool bMoved = false, bool bActed = false) : CMessage(MSG_CREATE_ENTITY)
	{
		m_nType = nType;
		m_nOwner = nOwner;

		if (CGame::GetInstance()->GetGameType() == SINGLE_PLAYER
			&& nOwner == OwnerFaction::OWNER_ROBOTS)
			m_nOwner = OwnerFaction::OWNER_AI;

		m_nX = nX;
		m_nY = nY;
		m_nHealth = nHealth;
		m_bMoved = bMoved;
		m_bActed = bActed;

		this->tile = nullptr;
	}

		CCreateEntityMessage(UnitType nType, OwnerFaction nOwner, CTile *tile) : CMessage(MSG_CREATE_ENTITY)
	{
		m_nType = nType;
		m_nOwner = nOwner;

		this->tile = tile;
	}

	~CCreateEntityMessage(void)
	{

	}

	UnitType GetUnitType(void) 
	{
		return m_nType;
	}

	OwnerFaction GetUnitOwner(void)
	{
		return m_nOwner;
	}

	CTile *GetTile(void) const {return tile;}
	int GetX(void) const {return m_nX;}
	int GetY(void) const {return m_nY;}
	int GetHP(void) const{return m_nHealth;}
	bool GetMoved(void) const{return m_bMoved;}
	bool GetActed(void) const{return m_bActed;}
};

