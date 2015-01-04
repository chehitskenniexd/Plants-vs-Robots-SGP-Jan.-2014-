#include "GridSystem.h"


CGridSystem::CGridSystem(void)
{

}


CGridSystem::~CGridSystem(void)
{

}

void CGridSystem::LoadMap()
{
	gameMap.LoadMap(mapName);
}

void CGridSystem::Render()
{
	gameMap.Render();
}

CTile* CGridSystem::GetTile(int x, int y)
{
	return gameMap.GetTile(x, y);
}

int CGridSystem::GetWidth()
{
	return gameMap.GetMapWidth();
}

int CGridSystem::GetHeight()
{
	return gameMap.GetMapHeight();
}

