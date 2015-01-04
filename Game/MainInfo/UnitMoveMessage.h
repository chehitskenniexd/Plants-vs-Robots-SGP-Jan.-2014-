#pragma once
#include "Message.h"
#include "../MainInfo/Entity.h"
#include "../MainInfo/Game.h"
#include "../Grid/Tile.h"
#include "../Grid/Grid.h"

class CUnitMoveMessage : public CMessage
{
private:
	CTile* targetTile;
	CTile* prevTile;
	CEntity* selectedEntity;

public:

	CUnitMoveMessage(CEntity* pEntity,CTile* pTile, CTile* previousTile) : CMessage(UNIT_MOVE)
	{
		prevTile = previousTile;
		targetTile = pTile;
		selectedEntity = pEntity;
	}

	~CUnitMoveMessage(void)
	{

	}

	CTile *GetTile(void) {return targetTile;}
	CEntity* GetEntity(void) {return selectedEntity;}
	CTile* GetPrevious (void) {return prevTile;}
};

