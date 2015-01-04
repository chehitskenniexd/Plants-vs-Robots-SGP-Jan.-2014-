#include "Grid.h"
#include "Tile.h"
#include "../MainInfo/MovingEntity.h"
#include "../MainInfo/Entity.h"
#include "../SGD Wrappers/tinystr.h"
#include "../SGD Wrappers/tinyxml.h"
#include "../SGD Wrappers/SGD_Math.h"
#include "../MainInfo/Game.h"
#include "../States/GameplayState.h"
#include "../SGD Wrappers/CSGD_MessageSystem.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../MainInfo/CreateEntityMessage.h"
#include "../SGD Wrappers/SGD_String.h"
#include <fstream>
#include <string>
#include"ittnotify.h"


class IsGreater
{
public:
	bool operator() (CNode* lhs, CNode* rhs)
	{
		return (lhs->moveCost > rhs->moveCost);
	}
};

CGrid::CGrid(void)
{
	mapHeight = 4;
	mapWidth = 5;
	tileHeight = 64;
	tileWidth = 64;
	tileCount = mapHeight * mapWidth;

	CTile testTile;
	for(int y = 0; y < mapHeight; y++)
	{
		for(int x = 0; x < mapWidth; x++)
		{
			tiles.push_back(testTile);
			testTile.SetPosX(testTile.GetPosX() + tileWidth) ;		
		}
		testTile.SetPosY(testTile.GetPosY() + tileHeight);
		testTile.SetPosX(0);
		if(y == 0)
			testTile.SetTileType(TileType::NEUTRAL) ;
		else
			testTile.SetTileType(TileType::NANO);
	}

	vector<CTile> tiles;
	int plantTileCount = 0;
	int robotTileCount = 0;
	int neutraltilecount = 0;

	string tileSetName;
	string mapName;
	//int tileSetImageID;

	windDirection = WindDirection::WIND_NONE;
}

CGrid::~CGrid(void)
{
}

void CGrid::Render()
{
	int screenW = CGame::GetInstance()->GetScreenWidth() + tileWidth;
	int screenH = CGame::GetInstance()->GetScreenHeight() + tileHeight;
	int offsetX = CGameplayState::GetInstance()->OffsetX();
	int offsetY = CGameplayState::GetInstance()->OffsetY();

	int minX = -offsetX / tileWidth;
	int minY = -offsetY / tileHeight;
	int maxX = (screenW - offsetX) / tileWidth;
	int maxY = (screenH - offsetY) / tileHeight;

	if(maxX > mapWidth)
		maxX = mapWidth;

	if(maxY > mapHeight)
		maxY = mapHeight;

	if(minX < 0)
		minX = 0;

	if(minY < 0)
		minY = 0;

	if(minX == maxX || minY == maxY)
		return;

	for(int y = minY; y < maxY; y++)
	{
		for(int x = minX; x < maxX; x++)
		{	
			tiles[y * mapWidth + x].Render(tileWidth, tileHeight);
		}
	}
}

void CGrid::Update(float dT)
{
	plantTileCount = robotTileCount = neutraltilecount = 0;

	for(int i = 0; i < tileCount; i++)
	{
		switch(tiles[i].GetFactionType())
		{
		case OwnerFaction::OWNER_PLANTS:
			plantTileCount++;
			break;
		case OwnerFaction::OWNER_NEUTRAL:
			neutraltilecount++;
			break;
		case OwnerFaction::OWNER_ROBOTS:
			robotTileCount++;
			break;
		}

		if (tiles[i].GetIsConverting() == true)
			tiles[i].GetConvertRenderStamp()->Update(dT, *tiles[i].GetConverting());
	}
}

void CGrid::LoadMap(string mapName)
{
	this->mapName = mapName;
	tiles.clear();

	TiXmlDocument doc;

	string filename = "Resources/XML/" + mapName + ".xml";

	if(doc.LoadFile(filename.c_str()) == false)
		return;

	TiXmlElement* pRoot = doc.RootElement();

	if(pRoot == nullptr)
		return;

	TiXmlElement* currElement = pRoot->FirstChildElement("ImageName");

	tileSetName = "Resources/Images/";
	tileSetName += currElement->Attribute("Name");

	TOSTRINGSTREAM tossTileSet;
	tossTileSet << "Resources/Images/" << currElement->Attribute("Name");

	tileSetImageID = CSGD_TextureManager::GetInstance()->LoadTexture(tossTileSet.str().c_str());

	currElement = pRoot->FirstChildElement("BattleMap");

	currElement->Attribute("MapWidth", &mapWidth);
	currElement->Attribute("MapHeight", &mapHeight);
	currElement->Attribute("TileWidth", &tileWidth);
	currElement->Attribute("TileHeight", &tileHeight);
	string wind = currElement->Attribute("WindDirection");

	if(wind == "NONE")
		windDirection = WindDirection::WIND_NONE;
	else if(wind == "NORTH")
		windDirection = WindDirection::WIND_NORTH;
	else if(wind == "EAST")
		windDirection = WindDirection::WIND_EAST;
	else if(wind == "SOUTH")
		windDirection = WindDirection::WIND_SOUTH;
	else if(wind == "WEST")
		windDirection = WindDirection::WIND_WEST;

	tileCount = mapWidth * mapHeight;

	TiXmlElement* tileElement = currElement->FirstChildElement("Tiles");
	tileElement = tileElement->FirstChildElement("Tile");

	for(int i = 0; i < tileCount; i++)
	{
		string type = tileElement->Attribute("TileType");
		string trigger = tileElement->Attribute("TileTrigger");
		int x;
		int y;
		int posX;
		int posY;
		string flow = "NULL";

		tileElement->Attribute("MapPosX", &x);
		tileElement->Attribute("MapPosY", &y);

		tileElement->Attribute("TilesetX", &posX);
		tileElement->Attribute("TilesetY", &posY);

		CTile tile;

		if(type == "GRASS")
		{
			tile.SetTileType(TileType::GRASS);	
			tile.SetFactionType(OwnerFaction::OWNER_PLANTS);
		}
		else if(type == "NANO")
		{
			tile.SetTileType(TileType::NANO);
			tile.SetFactionType(OwnerFaction::OWNER_ROBOTS);
		}
		else if(type == "NEUTRAL")
		{
			tile.SetTileType(TileType::NEUTRAL);
			tile.SetFactionType(OwnerFaction::OWNER_NEUTRAL);
		}
		else if(type == "WATER")
		{
			tile.SetTileType(TileType::WATER);
			tile.SetFactionType(OwnerFaction::OWNER_NEUTRAL);
			flow = tileElement->Attribute("WaterFlow");
		}
		else if(type == "MOUNTAIN")
		{
			tile.SetTileType(TileType::MOUNTAIN);
			tile.SetFactionType(OwnerFaction::OWNER_NEUTRAL);
		}
		else if(type == "VOLCANO")
		{
			tile.SetTileType(TileType::VOLCANO);
			tile.SetFactionType(OwnerFaction::OWNER_ROBOTS);
		}
		else if(type == "SLUDGE")
		{
			tile.SetTileType(TileType::SLUDGE);
			tile.SetFactionType(OwnerFaction::OWNER_ROBOTS);
			flow = tileElement->Attribute("WaterFlow");
		}
		else if(type == "SWAMP")
		{
			tile.SetTileType(TileType::SWAMP);
			tile.SetFactionType(OwnerFaction::OWNER_PLANTS);
			flow = tileElement->Attribute("WaterFlow");
		}
		else if(type == "FOREST")
		{
			tile.SetTileType(TileType::FOREST);
			tile.SetFactionType(OwnerFaction::OWNER_PLANTS);
		}

		if(flow != "NULL")
		{
			if(flow == "NONE")
				tile.SetFlow(WaterFlow::NONE);
			else if(flow == "ALL")
				tile.SetFlow(WaterFlow::ALL);
			else if(flow == "NORTH")
				tile.SetFlow(WaterFlow::NORTH);
			else if(flow == "EAST")
				tile.SetFlow(WaterFlow::EAST);
			else if(flow == "SOUTH")
				tile.SetFlow(WaterFlow::SOUTH);
			else if(flow == "WEST")
				tile.SetFlow(WaterFlow::WEST);		
		}

		tile.SetPosX(x * tileWidth);
		tile.SetPosY(y * tileHeight);
		tile.SetTileEvent(trigger);

		RECT rect;
		rect.left = posX;
		rect.right = posX + tileWidth;
		rect.top = posY;
		rect.bottom = posY + tileHeight;

		tile.SetSrcRect(rect);

		tiles.push_back(tile);	

		tileElement = tileElement->NextSiblingElement("Tile");
	}

	TiXmlElement* spawnElement = currElement->FirstChildElement("Spawns");
	spawnElement = spawnElement->FirstChildElement("Spawn");

	while(spawnElement != nullptr)
	{
		string type = spawnElement->Attribute("UnitType");
		string faction = spawnElement->Attribute("Faction");
		OwnerFaction owner;
		UnitType unitType;
		int x;
		int y;
		int posX;
		int posY;

		spawnElement->Attribute("MapPosX", &x);
		spawnElement->Attribute("MapPosY", &y);

		spawnElement->Attribute("TilesetX", &posX);
		spawnElement->Attribute("TilesetY", &posY);

		if(type == "BASE")
			unitType = UnitType::UNIT_BASE;
		else if(type == "INFANTRY")
			unitType = UnitType::UNIT_INFANTRY;
		else if(type == "SCOUT")
			unitType = UnitType::UNIT_SCOUT;
		else if(type == "TANK")
			unitType = UnitType::UNIT_TANK;

		if(faction == "PLANT")
			owner = OwnerFaction::OWNER_PLANTS;
		else if(faction == "ROBOT")
			owner = OwnerFaction::OWNER_ROBOTS;
		else if(faction == "NEUTRAL")
			owner = OwnerFaction::OWNER_NEUTRAL;

		CSGD_MessageSystem::GetInstance()->QueueMessage(new CCreateEntityMessage(unitType, owner, x, y));

		spawnElement = spawnElement->NextSiblingElement("Spawn");
	}

	TiXmlElement* tilesetElement = currElement->FirstChildElement("TileSet");
	tileElement = tilesetElement->FirstChildElement("Tile");

	while(tileElement != nullptr)
	{
		string type = tileElement->Attribute("TileType");
		TileType tileType;
		RECT src;
		int left;
		int top;
		int width;
		int height;

		if(type == "GRASS")
			tileType = TileType::GRASS;	
		else if(type == "FOREST")
			tileType = TileType::FOREST;
		else if(type == "SWAMP")
			tileType = TileType::SWAMP;
		else if(type == "NEUTRAL")
			tileType = TileType::NEUTRAL;	
		else if(type == "MOUNTAIN")
			tileType = TileType::MOUNTAIN;
		else if(type == "WATER")
			tileType = TileType::WATER;
		else if(type == "NANO")
			tileType = TileType::NANO;	
		else if(type == "VOLCANO")
			tileType = TileType::VOLCANO;
		else if(type == "SLUDGE")
			tileType = TileType::SLUDGE;

		tileElement->Attribute("SourceRectLeft", &left);
		tileElement->Attribute("SourceRectTop", &top);
		tileElement->Attribute("SourceRectWidth", &width);
		tileElement->Attribute("SourceRectHeight", &height);

		src.left = left;
		src.top = top;
		src.right = src.left + width;
		src.bottom = src.top + height;

		tileSet[tileType] = src;
		tileElement = tileElement->NextSiblingElement("Tile");
	}

	//for(int i = 0; i < tileCount; i++)
	//{
	//	for(auto iter = tileSet.begin(); iter != tileSet.end(); ++iter)
	//	{
	//		tiles[i].SetTileSetMap(iter->first, iter->second);
	//	}
	//}
}

CTile* CGrid::GetTile(int x, int y)
{
	return &tiles[y * mapWidth + x];
}

CTile CGrid::GetTileCpy(int x, int y)
{
	return tiles[y*mapWidth + x];
}


vector<CTile*> CGrid::ConvertAll()
{
	vector<CTile*> convertedTiles;

	for(int y = 0; y < mapHeight; y++)
	{
		for(int x = 0; x < mapWidth; x++)
		{
			if(GetTile(x,y)->GetTileType() == TileType::GRASS || GetTile(x,y)->GetTileType() == TileType::NEUTRAL || GetTile(x,y)->GetTileType() == TileType::NANO)
				ConvertNormal(x, y);
			else if(GetTile(x,y)->GetTileType() == TileType::SWAMP || GetTile(x,y)->GetTileType() == TileType::WATER || GetTile(x,y)->GetTileType() == TileType::SLUDGE)
				ConvertWater(x, y);
			else if(GetTile(x,y)->GetTileType() == TileType::VOLCANO || GetTile(x,y)->GetTileType() == TileType::FOREST)
				ConvertImpassable(x, y);
		}
	}

	for(int i = 0; i < tileCount; i++)
	{
		if(tiles[i].ToUpdate() == true)
		{

			int robotChance = 0, plantChance = 0, convert = 0;

			tiles[i].GetConvertChance(robotChance, plantChance);

			convert = rand() % 100;

			if(convert < robotChance)
			{
				tiles[i].Convert(OWNER_ROBOTS);
				tiles[i].GetConvertRenderStamp()->SetCurrentAnimation("R Sparkle");
				convertedTiles.push_back(&tiles[i]);
			}
			else if(convert < robotChance + plantChance)
			{
				tiles[i].Convert(OWNER_PLANTS);
				tiles[i].GetConvertRenderStamp()->SetCurrentAnimation("P Sparkle");
				convertedTiles.push_back(&tiles[i]);
			}
			else
			{
				if(tiles[i].GetTileType() == TileType::SWAMP || tiles[i].GetTileType() == TileType::WATER || tiles[i].GetTileType() == TileType::SLUDGE)
				{
					TileType preType = tiles[i].GetTileType();
					tiles[i].GetConvertRenderStamp()->SetCurrentAnimation("P Sparkle");
					tiles[i].Convert(OWNER_NEUTRAL);

					if (tiles[i].GetTileType() != preType)
						convertedTiles.push_back(&tiles[i]);
				}
			}

			for( unsigned int j = 0; j < convertedTiles.size(); ++j)
			{
				convertedTiles[j]->SetIsConverting(true);
			}
			tiles[i].ResetUpdate();
		}
	}

	return convertedTiles;
}

void CGrid::ConvertNormal(int x, int y)
{
	if(x > 0) // Check tiles on the left
	{
		if(GetTile(x - 1, y)->GetTileType() == TileType::GRASS || GetTile(x - 1, y)->GetTileType() == TileType::NANO)
			GetTile(x, y)->SetToUpdate(false, GetTile(x - 1, y)->GetFactionType());

		if(y > 0 && (GetTile(x - 1, y - 1)->GetTileType() == TileType::GRASS || GetTile(x, y - 1)->GetTileType() == TileType::NANO))
		{
			GetTile(x, y)->SetToUpdate(false, GetTile(x - 1, y - 1)->GetFactionType());
		}

		if(y + 1 < mapHeight && (GetTile(x - 1, y + 1)->GetTileType() == TileType::GRASS || GetTile(x - 1, y + 1)->GetTileType() == TileType::NANO))
		{
			GetTile(x, y)->SetToUpdate(false, GetTile(x - 1, y + 1)->GetFactionType());
		}
	}

	if(x + 1 < mapWidth) // Check tiles on the right
	{
		if(GetTile(x + 1, y)->GetTileType() == TileType::GRASS || GetTile(x + 1, y)->GetTileType() == TileType::NANO)
			GetTile(x, y)->SetToUpdate(false, GetTile(x + 1, y)->GetFactionType());

		if(y > 0 && (GetTile(x + 1, y - 1)->GetTileType() == TileType::GRASS || GetTile(x + 1, y - 1)->GetTileType() == TileType::NANO))
		{
			GetTile(x, y)->SetToUpdate(false, GetTile(x + 1, y - 1)->GetFactionType());
		}

		if(y + 1 < mapHeight && (GetTile(x + 1, y + 1)->GetTileType() == TileType::GRASS || GetTile(x + 1, y + 1)->GetTileType() == TileType::NANO))
		{
			GetTile(x, y)->SetToUpdate(false, GetTile(x + 1, y + 1)->GetFactionType());
		}
	}

	if(y > 0 && (GetTile(x, y - 1)->GetTileType() == TileType::GRASS || GetTile(x, y - 1)->GetTileType() == TileType::NANO)) // Check tile on the top
	{
		GetTile(x, y)->SetToUpdate(false, GetTile(x, y - 1)->GetFactionType());
	}

	if(y + 1 < mapHeight && (GetTile(x, y + 1)->GetTileType() == TileType::GRASS || GetTile(x, y + 1)->GetTileType() == TileType::NANO)) // Check tile on the bottom
	{
		GetTile(x, y)->SetToUpdate(false, GetTile(x, y + 1)->GetFactionType());
	}
}

void CGrid::ConvertWater(int x, int y)
{
	switch(GetTile(x, y)->GetFlow())
	{
	case WaterFlow::NONE:
		return;
		break;
	case WaterFlow::ALL:
		if(y > 0 && GetTile(x, y - 1)->GetFlow() != WaterFlow::SOUTH) // Flow up
			GetTile(x, y - 1)->SetToUpdate(false, GetTile(x, y)->GetFactionType());
		if(x + 1 < mapWidth && GetTile(x + 1, y)->GetFlow() != WaterFlow::WEST)// Flow right
			GetTile(x + 1, y)->SetToUpdate(false, GetTile(x, y)->GetFactionType());
		if(y + 1 < mapHeight && GetTile(x, y + 1)->GetFlow() != WaterFlow::NORTH)// Flow down
			GetTile(x, y + 1)->SetToUpdate(false, GetTile(x, y)->GetFactionType());
		if(x > 0 && GetTile(x - 1, y)->GetFlow() != WaterFlow::EAST)// Flow left
			GetTile(x - 1, y)->SetToUpdate(false, GetTile(x, y)->GetFactionType());
		break;
	case WaterFlow::NORTH:
		if(y > 0)
			GetTile(x, y - 1)->SetToUpdate(false, GetTile(x, y)->GetFactionType());
		break;
	case WaterFlow::EAST:
		if(x + 1 < mapWidth)
			GetTile(x + 1, y)->SetToUpdate(false, GetTile(x, y)->GetFactionType());
		break;
	case WaterFlow::SOUTH:
		if(y + 1 < mapHeight)
			GetTile(x, y + 1)->SetToUpdate(false, GetTile(x, y)->GetFactionType());
		break;
	case WaterFlow::WEST:
		if(x > 0)
			GetTile(x - 1, y)->SetToUpdate(false, GetTile(x, y)->GetFactionType());
		break;
	}
}

void CGrid::ConvertImpassable(int x, int y)
{
	switch(windDirection)
	{
	case WindDirection::WIND_NORTH:
		{
			if(y > 0)
				GetTile(x, y - 1)->SetToUpdate(true, GetTile(x, y)->GetFactionType());

			if(y - 1 > 0)
			{
				GetTile(x, y - 2)->SetToUpdate(true, GetTile(x, y)->GetFactionType());

				if(x > 0)
				{
					GetTile(x - 1, y - 2)->SetToUpdate(true, GetTile(x, y)->GetFactionType());
				}

				if(x + 1 < mapWidth)
				{
					GetTile(x + 1, y - 2)->SetToUpdate(true, GetTile(x, y)->GetFactionType());
				}
			}
		}
		break;
	case WindDirection::WIND_EAST:
		{
			if(x + 1 < mapWidth)
				GetTile(x + 1, y)->SetToUpdate(true, GetTile(x, y)->GetFactionType());

			if(x + 2 < mapWidth)
			{
				GetTile(x + 2, y)->SetToUpdate(true, GetTile(x, y)->GetFactionType());

				if(y > 0)
				{
					GetTile(x + 2, y - 1)->SetToUpdate(true, GetTile(x, y)->GetFactionType());
				}

				if(y - 1 < mapHeight)
				{
					GetTile(x + 2, y + 1)->SetToUpdate(true, GetTile(x, y)->GetFactionType());
				}
			}
		}
		break;
	case WindDirection::WIND_SOUTH:
		{
			if(y + 1 < mapHeight)
				GetTile(x, y + 1)->SetToUpdate(true, GetTile(x, y)->GetFactionType());

			if(y + 2 < mapHeight)
			{
				GetTile(x, y + 2)->SetToUpdate(true, GetTile(x, y)->GetFactionType());

				if(x > 0)
				{
					GetTile(x - 1, y + 2)->SetToUpdate(true, GetTile(x, y)->GetFactionType());
				}

				if(x + 1 < mapWidth)
				{
					GetTile(x + 1, y + 2)->SetToUpdate(true, GetTile(x, y)->GetFactionType());
				}
			}
		}
		break;
	case WindDirection::WIND_WEST:
		{
			if(x > 0)
				GetTile(x - 1, y)->SetToUpdate(true, GetTile(x, y)->GetFactionType());

			if(x - 1 > 0)
			{
				GetTile(x - 2, y)->SetToUpdate(true, GetTile(x, y)->GetFactionType());

				if(y > 0)
				{
					GetTile(x - 2, y - 1)->SetToUpdate(true, GetTile(x, y)->GetFactionType());
				}

				if(y - 1 < mapHeight)
				{
					GetTile(x - 2, y + 1)->SetToUpdate(true, GetTile(x, y)->GetFactionType());
				}
			}
		}
		break;
	}
}

void CGrid::Unhighlight()
{
	for(int i = 0; i < tileCount; i++)
	{
		tiles[i].UnHighlight();
	}
}

const CTile* CGrid::operator[](int i) const
{
	return &tiles[i];
}

void CGrid::SetTiles(vector<CTile> vTiles) 
{
	tiles = vTiles;
	tileCount = vTiles.size();
}

vector<CNode*> CGrid::FindPath (int startX, int startY, int endX, int endY, int maxDist, CMovingEntity currentEntity, OwnerFaction pathingUnitFaction)
{
	__itt_resume();
	std::priority_queue<CNode*, vector<CNode*>, IsGreater> myPath;
	vector<CNode*> searchGraph;

	CNode* tempNode;
	int graphRows = mapWidth;
	int graphCols = mapHeight;


	for (int i = currentEntity.GetCurrTile()->GetPosY()/this->GetMapHeight() - currentEntity.GetMoveRange(); i < currentEntity.GetCurrTile()->GetPosY()/this->GetMapHeight() + currentEntity.GetMoveRange(); i++)
	{
		for (int j = currentEntity.GetCurrTile()->GetPosX()/this->GetMapWidth() - currentEntity.GetMoveRange(); j < currentEntity.GetCurrTile()->GetPosX()/this->GetMapWidth() + currentEntity.GetMoveRange(); j++)
		{
			if (i < 0)
				i = 0;
			if (i > this->GetMapHeight())
				i = this->GetMapHeight();
			if (j < 0)
				j = 0;
			if( j > this->GetMapWidth())
				j = this->GetMapWidth();
			tempNode = new CNode;
			tempNode->parentTile = GetTile(j, i);

			tempNode->x = tempNode->parentTile->GetPosX() / GetTileWidth();
			tempNode->y = tempNode->parentTile->GetPosY() / GetTileHeight();

			if (tempNode->parentTile->GetTileType() != MOUNTAIN && tempNode->parentTile->GetTileType() != VOLCANO && tempNode->parentTile->GetTileType() != FOREST)
			{
				if (tempNode->parentTile->GetTileType() == WATER)
					tempNode->moveCost = 2;
				if (tempNode->parentTile->GetTileType() == SLUDGE)
					tempNode->moveCost = (OWNER_PLANTS == pathingUnitFaction ? 2 : 1);
				if (tempNode->parentTile->GetTileType() == SWAMP)
					tempNode->moveCost = (OWNER_PLANTS == pathingUnitFaction ? 1 : 2);
				searchGraph.push_back(tempNode);
			}
			else
			{
				searchGraph.push_back(nullptr);
				delete tempNode;
			}
		}
	}

	CNode* goalNode = searchGraph[endY * graphRows + endX];

	tempNode = searchGraph[startY * graphRows + startX];

	if (tempNode)
	{
		tempNode->moveCost = 0;
		tempNode->beenSearched = true;
	}

	myPath.push(tempNode);

	bool isFound = false;

	while (!myPath.empty())
	{
		tempNode = myPath.top();
		myPath.pop();

		if (tempNode->moveCost > maxDist)
		{
			isFound = false;
			break;
		}
		if (tempNode->x == endX && tempNode->y == endY)
		{
			isFound = true;
			break;
		}
		else
		{
			CNode* tempAdjNode;
			//Enqueue adjacent nodes
			if (tempNode->x > 0) //Left adjacent
			{
				tempAdjNode = searchGraph[tempNode->y * graphRows + tempNode->x - 1];
				if (tempAdjNode && goalNode && !tempAdjNode->beenSearched)
					myPath.push(AdjNode(tempAdjNode, goalNode, tempNode));
			}
			if (tempNode->y > 0) //Top Adjacent
			{
				tempAdjNode = searchGraph[(tempNode->y - 1) * graphRows + tempNode->x];
				if (tempAdjNode && goalNode && !tempAdjNode->beenSearched)
					myPath.push(AdjNode(tempAdjNode, goalNode, tempNode));
			}
			if (tempNode->x < graphRows - 1) //Right Adjacent
			{
				tempAdjNode = searchGraph[tempNode->y * graphRows + tempNode->x + 1];
				if (tempAdjNode && goalNode && !tempAdjNode->beenSearched)
					myPath.push(AdjNode(tempAdjNode, goalNode, tempNode));
			}
			if (tempNode->y < graphCols - 1) //Bottom Adjacent
			{
				tempAdjNode = searchGraph[(tempNode->y + 1) * graphRows + tempNode->x];
				if (tempAdjNode && goalNode && !tempAdjNode->beenSearched)
					myPath.push(AdjNode(tempAdjNode, goalNode, tempNode));
			}
		}		
	}

	vector<CNode*> finalPath;
	if (isFound)
		RecursiveHelper(&finalPath, goalNode);

	for (unsigned int i = 0; i <searchGraph.size(); i++)
		delete searchGraph[i];

	return finalPath;
}

void CGrid::RecursiveHelper(vector<CNode*>* finalPath, CNode* goalNode)
{
	if (goalNode)
	{
		RecursiveHelper(finalPath, goalNode->parentNode);
		finalPath->push_back(new CNode(*goalNode));
	}
}

CNode* CGrid::AdjNode(CNode* myNode, CNode* myGoal, CNode* myParent)
{
	myNode->parentNode = myParent;
	myNode->beenSearched = true;
	myNode->moveCost += myParent->moveCost + static_cast<int>(DistFormula(myNode, myGoal));

	return myNode;
}

float CGrid::DistFormula (CNode* start, CNode* end)
{
	float x2 = static_cast<float>(pow(end->x - start->x, 2));
	float y2 = static_cast<float>(pow(end->y - start->y, 2));

	return sqrt(x2 + y2);
}