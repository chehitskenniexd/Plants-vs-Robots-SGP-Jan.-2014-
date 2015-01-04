#pragma once
#include "Tile.h"
#include "../MainInfo/Entity.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

enum WindDirection{WIND_NONE, WIND_NORTH, WIND_EAST, WIND_SOUTH, WIND_WEST};

typedef std::map<TileType, RECT> TileSet;

class CGrid
{
public:
	CGrid(void);
	~CGrid(void);

	int GetPlantTileCount() { return plantTileCount;}
	int GetRobotTileCount() { return robotTileCount;}
	int GetNeutralTileCount() { return neutraltilecount;}
	int GetTileCount() { return tileCount;}

	int GetMapHeight() { return mapHeight;}
	int GetMapWidth() { return mapWidth;}
	int GetTileHeight() { return tileHeight;}
	int GetTileWidth() { return tileWidth;}
	const char* GetName() const {return mapName.c_str();}

	void SetName(const char* szName) {mapName = szName;}
	void SetTileSetName(const char* szName) {tileSetName = szName;}
	void SetWidth(int nWidth) {mapWidth = nWidth;}
	void SetHeight(int nHeight) {mapHeight = nHeight;}

	void SetTileWidth(int nWidth) {tileWidth = nWidth;}
	void SetTileHeight(int nHeight){tileHeight = nHeight;}

	void SetTiles(vector<CTile> vTiles);

	void Render();
	void Update(float dT);

	vector<CTile*> ConvertAll();
	void ConvertNormal(int x, int y);
	void ConvertWater(int x, int y);
	void ConvertImpassable(int x, int y);

	TileSet GetTileSet() const { return tileSet;}
	void SetTileSet (TileSet mTileSet) {tileSet = mTileSet;}
	string GetTileSetPath() { return tileSetName;}

	void LoadMap(string mapName);

	CTile* GetTile(int x, int y);
	CTile GetTileCpy(int x, int y);

	const CTile* operator[](int i) const;
	vector<CNode*> FindPath (int startX, int startY, int endX, int endY, int maxDist, CMovingEntity currentEntity, OwnerFaction pathingUnitFaction = OWNER_NEUTRAL);
	CNode* AdjNode(CNode* myNode, CNode* myGoal, CNode* myParent);
	float DistFormula (CNode* start, CNode* end);
	void RecursiveHelper(vector<CNode*>* finalPath, CNode* goalNode);
	void Unhighlight();

private:
	//Pathfinding cache
	//vector<CNode*> searchGraph;

	vector<CTile> tiles;
	int plantTileCount;
	int robotTileCount;
	int neutraltilecount;
	int tileCount;

	int mapHeight;
	int mapWidth;
	int tileHeight;
	int tileWidth;

	string tileSetName;
	string mapName;
	int tileSetImageID;

	TileSet tileSet;

	WindDirection windDirection;
};

