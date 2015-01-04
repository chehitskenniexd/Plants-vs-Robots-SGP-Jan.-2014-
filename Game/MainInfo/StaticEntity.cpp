#include "StaticEntity.h"
#include "CreateEntityMessage.h"
#include "KOTHMessage.h"
#include "../States/GameplayState.h"

#include "../SGD Wrappers/CSGD_MessageSystem.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../MainInfo/AIEntity.h"

CStaticEntity::CStaticEntity(void)
{
	this->m_nCreateRange = 1;
}

CStaticEntity::~CStaticEntity(void)
{
}

void CStaticEntity::Initialize()
{
	SetUnitType(UNIT_BASE);

	//Set base stats here

}

void CStaticEntity::ShowCreateRange (CGrid &pMap)
{
	for(int y = 0; y < pMap.GetMapHeight(); y++)
	{
		for(int x = 0; x < pMap.GetMapWidth(); x++)
		{	
			if((abs(x - GetPosX()/pMap.GetTileWidth()) + abs(y - GetPosY()/pMap.GetTileHeight()) <= m_nCreateRange))
				pMap.GetTile(x,y)->Highlight(D3DCOLOR_ARGB(128, 255, 255, 255));
		}
	}
}

bool CStaticEntity::CheckCreateRange (CTile* pTile, CGrid& pMap)
{
	if(abs(pTile->GetPosX()/pMap.GetTileWidth() - GetPosX()/pMap.GetTileWidth()) + abs(pTile->GetPosY()/pMap.GetTileHeight() - GetPosY()/pMap.GetTileHeight()) <= m_nCreateRange
		&& pTile->GetOccupyingUnit() == nullptr)
	{
		CSGD_XAudio2::GetInstance()->SFXPlaySound(createSFX, false);
		return true;
	}

	return false;
}


/*virtual*/ void CStaticEntity::Update(float dT) /*override*/
{
	//Test for unit creation
	CEntity::Update(dT);
}

void CStaticEntity::CreateUnit(UnitType nType, OwnerFaction nFaction, int nX, int nY)
{
	CSGD_MessageSystem::GetInstance()->QueueMessage(new CCreateEntityMessage(nType, nFaction, nX, nY));
	//////////////////////////////////////////////////////////
	// BUG FIX: 54|84, 58|84
	// Fixed By: Kenneth Moy
	//////////////////////////////////////////////////////////
	// Begin Fix
	CGameplayState::GetInstance()->SetUnitCreating(true);
	// End Fix
	if (this->GetFaction() == OWNER_AI)
	{
		AIEntity::GetInstance()->SetTimesLooped(0);
	}
	SetUsedAction(true);
}

void CStaticEntity::CreateUnit(UnitType nType, OwnerFaction nFaction, CTile* targetTile)
{
	if (targetTile->GetOccupyingUnit() == nullptr)
	{
		CSGD_MessageSystem::GetInstance()->QueueMessage(new CCreateEntityMessage(nType, nFaction, targetTile));
		//////////////////////////////////////////////////////////
		// BUG FIX: 54|84, 58|84
		// Fixed By: Kenneth Moy
		//////////////////////////////////////////////////////////
		// Begin Fix
		CGameplayState::GetInstance()->SetUnitCreating(true);
		// End Fix
		if (this->GetFaction() == OWNER_AI)
		{
			AIEntity::GetInstance()->SetTimesLooped(0);
		}
		SetUsedAction(true);
	}
}

void CStaticEntity::HandleEvent(const CEvent* pEvent)
{
	if (pEvent->GetEventID() == "KOTHENDTURN")
		if (GetFaction() != OWNER_NEUTRAL)
			CSGD_MessageSystem::GetInstance()->QueueMessage(new CKOTHMessage(GetFaction()));
}

void CStaticEntity::SetUsedAction(bool hasUsed) 
{	
	CEntity::SetUsedAction(hasUsed);

	//if (hasUsed)
	//	CGameplayState::GetInstance()->IncreaseAP();
}