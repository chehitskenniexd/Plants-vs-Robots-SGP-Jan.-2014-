#pragma once
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../MainInfo/AnimTimeStamps.h"
#include "../Systems/AnimationSystem.h"

class CBMFont;
class CGameplayState;
class CEntity;
class CSGD_TextureManager;

class CHUD
{
public:
	CHUD(void);
	~CHUD(void);

	void Render();
	void Update(float dt);

	int GetHudImgId() {return hudImgID;}
	void SetHudImgId(int id) {hudImgID = id;}

	int GetActionsImgId() {return actionsImgID;}
	void SetActionsImgId(int id) {actionsImgID = id;}

	char* GetPlantString() { return plantString;}
	char* GetNeutralString() { return neutralString;}
	char* GetRobotString() { return robotString;}
private:
	int hudImgID;
	int actionsImgID;
	bool flash;
	float flashTimer;

	RECT plantRect;
	RECT neutralRect;
	RECT robotRect;

	AnimTimeStamps m_aChangedPercent;
	float previous;
	bool m_bFireworks, m_bPlants, m_bRobot, m_bFirstTurn, m_bExplosion;

	CSGD_TextureManager* TM;

	int shieldID;
	int swordID;
	int cogID;
	int flowerID;
	int radialHpID;

	CEntity* selectedUnit;
	CEntity* currUnit;

	CBMFont* pFont;
	CGameplayState* pGamePlay;

	float tileCount;
	float plantCount;
	float neutralCount;
	float robotCount;

	char plantString[20];
	char neutralString[20];
	char robotString[20];
	char attackString[20];
	char moveString[20];
	char cancelString[20];
	char convertString[20];
	char createString[20];
	char tankString[20];
	char scoutString[20];
	char infantryString[20];

};

