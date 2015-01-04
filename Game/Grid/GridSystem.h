#pragma once
#include "Grid.h"
#include "Tile.h"

class CGridSystem
{
public:
	CGridSystem(void);
	~CGridSystem(void);

	void LoadMap();
	void Render();

	CTile* GetTile(int x, int y);

	int GetWidth();
	int GetHeight();

	int GetPlantTiles(void) {return gameMap.GetPlantTileCount();}
	int GetNeutralTiles(void) {return gameMap.GetNeutralTileCount();}
	int GetRobotTiles(void) {return gameMap.GetRobotTileCount();}
	int GetTileCount(void) {return gameMap.GetTileCount();}

	void Update(float dT) {gameMap.Update(dT);}

private:
	CGrid gameMap;
	string mapName;
};

