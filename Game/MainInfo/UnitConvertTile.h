#pragma once
#include "Message.h"
#include "../MainInfo/Entity.h"
#include "../Grid/Tile.h"
#include "../Grid/Grid.h"

class CUnitConvertTileMessage : public CMessage
{
private:
	CGrid* gameMap;
	CTile* targetTile;
	CEntity* selectedEntity;

public:

	CUnitConvertTileMessage(CEntity* pEntity,CTile* pTile, CGrid* pMap) : CMessage(UNIT_CONVERT)
	{
		targetTile = pTile;
		gameMap = pMap;
		selectedEntity = pEntity;
	}

	~CUnitConvertTileMessage(void)
	{

	}

	CTile *GetTile(void) {return targetTile;}
	CGrid* GetMap (void) {return gameMap;}
	CEntity* GetEntity(void) {return selectedEntity;}
};

