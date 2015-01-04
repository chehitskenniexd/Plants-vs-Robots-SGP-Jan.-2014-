#pragma once
#include "entity.h"
#include "CreateEntityMessage.h"
#include "../Grid/Grid.h"
#include "../Grid/Tile.h"

#include "../SGD Wrappers/CSGD_MessageSystem.h"
#include "../SGD Wrappers/IListener.h"
#include "../SGD Wrappers/CEvent.h"

class CStaticEntity :
	public CEntity
{
private:
	int m_nCreateRange;
	int createSFX;

public:
	CStaticEntity(void);
	virtual ~CStaticEntity(void);

	void Initialize(void);
	void SetCreateSFX(int sfxID) { createSFX = sfxID; }
	void CreateUnit(UnitType nType, OwnerFaction nFaction, int nX, int nY);
	void CreateUnit(UnitType nType, OwnerFaction nFaction, CTile* targetTile);
	void ShowCreateRange (CGrid &pMap);
	bool CheckCreateRange (CTile* pTile, CGrid& pMap);
	virtual void SetUsedAction(bool hasUsed);

	virtual void Update(float dT) override;
	//virtual void Render() override;

	virtual void HandleEvent(const CEvent* pEvent) override;
};

