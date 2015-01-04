#pragma once
#include <string>
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../MainInfo/Entity.h"
#include "../MainInfo/AnimTimeStamps.h"
#include <map>
using namespace std;

enum TileType{GRASS, NEUTRAL, NANO, SWAMP, WATER, SLUDGE, FOREST, MOUNTAIN, VOLCANO};
enum WaterFlow{NONE, ALL, NORTH, EAST, SOUTH, WEST};

struct CNode
{
	int x, y, moveCost;
	bool beenSearched;
	CNode* parentNode;
	CTile* parentTile;

	CNode() : x(0), y(0), moveCost(1), parentNode(nullptr), parentTile(nullptr), beenSearched(false) {}

	static bool isGreater(CNode* lhs, CNode* rhs) {return (lhs->moveCost > rhs->moveCost);}
	int GetPosX(void) {return this->x;}
	int GetPosY(void) {return this->y;}
	CTile* GetParentTile(void) {return this->parentTile;}
};

class CTile
{
public:
	CTile(void);
	~CTile(void);

	void Highlight(D3DCOLOR color);
	void UnHighlight();
	bool Convert(OwnerFaction newFaction);

	void Render(int tileW, int tileH);

	void GetPos(int &x, int &y);

	int GetPosX() { return posX;}
	int GetPosY() { return posY;}
	RECT GetSrcRect() { return srcRect;}

	int GetHpModifier(void) {return hpModifier;}

	void SetPosX(int x) { posX = x;}
	void SetPosY(int y) { posY = y;}
	void SetSrcRect(RECT rect) { srcRect = rect;}

	CEntity* GetOccupyingUnit() { return occupyingUnit;}
	void SetOccupyingUnit(CEntity* unit);
	//void SetTileSetMap(TileType key, RECT src) {tileSet[key] = src;}

	void SetTileEvent(string trig) { tileEvent = trig;}
	string GetTileEvent() { return tileEvent;};

	TileType GetTileType() { return tileType;}
	void SetTileType(TileType type) { tileType = type;}

	WaterFlow GetFlow() {return waterFlow;}
	void SetFlow(WaterFlow flow) {waterFlow = flow;}

	OwnerFaction GetFactionType() { return faction;}
	void SetFactionType(OwnerFaction owner) { faction = owner;}

	AnimTimeStamps* GetConvertRenderStamp () {return &convertRender;}

	bool GetIsConverting () {return this->isConverting;}
	bool *GetConverting() {return &isConverting;}
	void SetIsConverting (bool bConverting) {this->isConverting = bConverting;}

	bool ToUpdate() { return toUpdate;}
	void SetToUpdate(bool update, OwnerFaction owner);
	void ResetUpdate();

	bool isHighlighted(void) const {return highlight;}

	void GetConvertChance(int& robot, int& plant) { robot = robotConvertChance; plant = plantConvertChance;}

private:
	TileType tileType;
	int tileID, hpModifier, dmgModifier, posX, posY, plantConvertChance, robotConvertChance;
	string tileEvent;
	bool impassable, highlight, toUpdate, isConverting;
	CEntity* occupyingUnit;
	D3DCOLOR highlightColor;
	RECT srcRect;
	OwnerFaction faction;
	//map<TileType, RECT> tileSet;
	WaterFlow waterFlow;
	AnimTimeStamps convertRender;

	int convertSFX;

};

