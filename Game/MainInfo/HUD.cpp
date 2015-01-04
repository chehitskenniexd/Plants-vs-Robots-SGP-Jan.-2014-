#include "HUD.h"
#include "BMFont.h"
#include "MovingEntity.h"
#include "../MainInfo/Game.h"
#include "../States/GameplayState.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/SGD_String.h"

CHUD::CHUD(void)
{
	pFont = CGame::GetInstance()->GetFont();

	plantRect.top = 10;
	plantRect.bottom = 25;
	plantRect.left = int(CGame::GetInstance()->GetScreenWidth()*0.50f - 195);

	neutralRect.top = 10;
	neutralRect.bottom = 25;

	robotRect.top = 10;
	robotRect.bottom = 25;
	m_bFireworks = false;
	m_bFirstTurn = true;
	m_bExplosion = false;

	pGamePlay = CGameplayState::GetInstance();
	TM = TM->GetInstance();

	swordID = TM->LoadTexture(_T("Resources/Images/Swords.png"));
	shieldID = TM->LoadTexture(_T("Resources/Images/Shield.png"));
	flowerID = TM->LoadTexture(_T("Resources/Images/PlantAP.png"));
	cogID = TM->LoadTexture(_T("Resources/Images/RobotAP.png"));
	radialHpID = TM->LoadTexture(_T("Resources/Images/RadialHealth.png"));
	m_aChangedPercent.SetCurrentAnimation("AFireworks");

	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_PLANTS, plantString, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_ROBOTS, robotString, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_NEUTRAL, neutralString, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_ATTACK, attackString, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_MOVE, moveString, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CANCEL, cancelString, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CONVERT, convertString, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CREATE, createString, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CREATE_TANK, tankString, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CREATE_SCOUT, scoutString, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CREATE_INFANTRY, infantryString, 20);
}																   

CHUD::~CHUD(void)
{
}

void CHUD::Render()
{
	CSGD_TextureManager::GetInstance()->Draw(hudImgID, int(CGame::GetInstance()->GetScreenWidth()*0.50f - 256), -10, 1.0f, 1.0f, (RECT*)0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(200, 255, 255, 255));

	if(CGameplayState::GetInstance()->GetMode() == GameMode::KOTH)
	{
		CSGD_TextureManager::GetInstance()->Draw(actionsImgID, int(CGame::GetInstance()->GetScreenWidth()*0.50f - 115),  100, 1.0f, 1.0f, (RECT*)0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(200, 255, 255, 255));		
		CSGD_TextureManager::GetInstance()->Draw(actionsImgID, int(CGame::GetInstance()->GetScreenWidth()*0.50f - 15),  100, 1.0f, 1.0f, (RECT*)0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(200, 255, 255, 255));		

		ostringstream ossPlantScore;
		ossPlantScore << CGameplayState::GetInstance()->GetPlantKotH() << "/25";
		ostringstream ossRobotScore;
		ossRobotScore << CGameplayState::GetInstance()->GetRobotKotH() << "/25";

		pFont->Write(ossPlantScore.str().c_str(), int(CGame::GetInstance()->GetScreenWidth()*0.50f - 75),  117, 0.8f, D3DCOLOR_XRGB(0, 255, 0));
		pFont->Write(ossRobotScore.str().c_str(), int(CGame::GetInstance()->GetScreenWidth()*0.50f + 25),  117, 0.8f, D3DCOLOR_XRGB(255, 0, 255));
	}

	if(pGamePlay->GetSelectedUnit() != nullptr)
	{
		int x = pGamePlay->GetSelectedUnit()->GetCurrTile()->GetPosX();
		int y = pGamePlay->GetSelectedUnit()->GetCurrTile()->GetPosY();

		RECT hpRECT;
		hpRECT.bottom = 172;
		hpRECT.left = 0;
		hpRECT.right = 200;
		hpRECT.top = (LONG)(172 - (pGamePlay->GetSelectedUnit()->GetCurrHealth() * 17.2f));

		CSGD_TextureManager::GetInstance()->Draw(radialHpID, x - 60 + pGamePlay->OffsetX(), y - 55 + pGamePlay->OffsetY() + int(172 - (pGamePlay->GetSelectedUnit()->GetCurrHealth() * 17.2f)), 1.0f, 1.0f, &hpRECT, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(200, 255, 255, 255));

		// Cancel
		CSGD_TextureManager::GetInstance()->Draw(actionsImgID, x + 95 + pGamePlay->OffsetX(),  y - 1 + pGamePlay->OffsetY(), 1.0f, 1.0f, (RECT*)0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(200, 255, 255, 255));		
		pFont->Write(cancelString, x + 127 + pGamePlay->OffsetX(), y + 16 + pGamePlay->OffsetY(), 0.8f, D3DCOLOR_XRGB(255, 0, 0));

		if(pGamePlay->GetSelectedUnit()->GetUnitType() != UnitType::UNIT_BASE)
		{

			if(pGamePlay->GetSelectedAction()==pGamePlay->NONE || pGamePlay->GetSelectedAction() == pGamePlay->ATTACK) // Attack
			{
				CSGD_TextureManager::GetInstance()->Draw(actionsImgID, x - 168 + pGamePlay->OffsetX(),  y - 1 + pGamePlay->OffsetY(), 1.0f, 1.0f, (RECT*)0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(200, 255, 255, 255));
				pFont->Write(attackString, x - 135 + pGamePlay->OffsetX(), y + 16 + pGamePlay->OffsetY(), 0.8f, D3DCOLOR_XRGB(0, 0, 255));
			}
			if(pGamePlay->GetSelectedAction()==pGamePlay->NONE || pGamePlay->GetSelectedAction() == pGamePlay->CONVERT) // Convert
			{
				CSGD_TextureManager::GetInstance()->Draw(actionsImgID, x - 34 + pGamePlay->OffsetX(),  y - 103 + pGamePlay->OffsetY(), 1.0f, 1.0f, (RECT*)0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(200, 255, 255, 255));
				pFont->Write(convertString, x + pGamePlay->OffsetX() - 7, y - 86 + pGamePlay->OffsetY(), 0.8f, D3DCOLOR_XRGB(255, 255, 0));
			}
			if(pGamePlay->GetSelectedAction()==pGamePlay->NONE || pGamePlay->GetSelectedAction() == pGamePlay->MOVE) // Move
			{
				if (dynamic_cast<CMovingEntity*>(pGamePlay->GetSelectedUnit())->GetMoved() == false)
				{
					CSGD_TextureManager::GetInstance()->Draw(actionsImgID, x - 34 + pGamePlay->OffsetX(),  y + 103 + pGamePlay->OffsetY(), 1.0f, 1.0f, (RECT*)0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(200, 255, 255, 255));
					pFont->Write(moveString, x + 2 + pGamePlay->OffsetX(), y + 120 + pGamePlay->OffsetY(), 0.8f, D3DCOLOR_XRGB(0, 255, 0));
				}
				else
				{
					CSGD_TextureManager::GetInstance()->Draw(actionsImgID, x - 34 + pGamePlay->OffsetX(),  y + 103 + pGamePlay->OffsetY(), 1.0f, 1.0f, (RECT*)0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(200, 255, 255, 255));
					pFont->Write(" Undo", x + 2 + pGamePlay->OffsetX(), y + 120 + pGamePlay->OffsetY(), 0.8f, D3DCOLOR_XRGB(0, 255, 0));
				}

			}
		}
		else
		{
			if(pGamePlay->GetSelectedAction()==pGamePlay->CREATE || pGamePlay->GetSelectedAction() == pGamePlay->CREATE_INFANTRY) // INFANTRY
			{
				CSGD_TextureManager::GetInstance()->Draw(actionsImgID, x - 168 + pGamePlay->OffsetX(),  y - 1 + pGamePlay->OffsetY(), 1.0f, 1.0f, (RECT*)0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(200, 255, 255, 255));
				pFont->Write(infantryString, x - 135 + pGamePlay->OffsetX(), y + 16 + pGamePlay->OffsetY(), 0.8f, D3DCOLOR_XRGB(0, 0, 255));
			}
			if(pGamePlay->GetSelectedAction()==pGamePlay->CREATE || pGamePlay->GetSelectedAction() == pGamePlay->CREATE_TANK) // TANK
			{
				CSGD_TextureManager::GetInstance()->Draw(actionsImgID, x - 34 + pGamePlay->OffsetX(),  y - 103 + pGamePlay->OffsetY(), 1.0f, 1.0f, (RECT*)0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(200, 255, 255, 255));
				pFont->Write(tankString, x + 5 + pGamePlay->OffsetX(), y - 86 + pGamePlay->OffsetY(), 0.8f, D3DCOLOR_XRGB(255, 255, 0));
			}
			if(pGamePlay->GetSelectedAction()==pGamePlay->CREATE || pGamePlay->GetSelectedAction() == pGamePlay->CREATE_SCOUT) // SCOUT
			{
				CSGD_TextureManager::GetInstance()->Draw(actionsImgID, x - 34 + pGamePlay->OffsetX(),  y + 103 + pGamePlay->OffsetY(), 1.0f, 1.0f, (RECT*)0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(200, 255, 255, 255));
				pFont->Write(scoutString, x + 2 + pGamePlay->OffsetX(), y + 120 + pGamePlay->OffsetY(), 0.8f, D3DCOLOR_XRGB(0, 255, 0));
			}
		}
	}

	ostringstream ossPlantCount;
	ossPlantCount << plantString << ": " << (int)(plantCount*100) << "%";
	ostringstream ossNeutralCount;
	ossNeutralCount << neutralString << ": " << (int)(neutralCount*100) << "%";
	ostringstream ossRobotsCount;
	ossRobotsCount << robotString << ": " << (int)(robotCount*100) << "%";

	pFont->Write(ossPlantCount.str().c_str(), int(CGame::GetInstance()->GetScreenWidth()*0.50f - 206), int(CGame::GetInstance()->GetScreenHeight()*0.05f), 0.8f, D3DCOLOR_XRGB(0, 255, 0));
	pFont->Write(ossNeutralCount.str().c_str(), int(CGame::GetInstance()->GetScreenWidth()*0.50f - 46), int(CGame::GetInstance()->GetScreenHeight()*0.05f), 0.8f, D3DCOLOR_XRGB(200, 200, 200));
	pFont->Write(ossRobotsCount.str().c_str(), int(CGame::GetInstance()->GetScreenWidth()*0.50f + 114), int(CGame::GetInstance()->GetScreenHeight()*0.05f), 0.8f, D3DCOLOR_XRGB(255, 0, 255));

	CSGD_Direct3D::GetInstance()->DrawRect(plantRect, D3DCOLOR_ARGB(245, 0, 255, 0));
	CSGD_Direct3D::GetInstance()->DrawRect(neutralRect, D3DCOLOR_ARGB(245, 150, 150, 150));
	CSGD_Direct3D::GetInstance()->DrawRect(robotRect, D3DCOLOR_ARGB(245, 255, 0, 255));


	if (CGameplayState::GameplayStatus::COINFLIP != CGameplayState::GetInstance()->GetStatus())
	{
		if(CGameplayState::GetInstance()->GetCurrentFaction() == OWNER_PLANTS)
		{
			for(int i = 0; i < CGameplayState::GetInstance()->GetAP(); i++)
			{
				TM->Draw(flowerID, int(CGame::GetInstance()->GetScreenWidth()*0.50f - 240) + 35 * i, 75);
			}
		}
		else
		{
			for(int i = 0; i < CGameplayState::GetInstance()->GetAP(); i++)
			{
				TM->Draw(cogID, int(CGame::GetInstance()->GetScreenWidth()*0.50f + 30) + 35 * i, 75);
			}
		}
	}

	ostringstream ossTurnTimer;
	ossTurnTimer << (int)CGameplayState::GetInstance()->GetTimer();

	if(CGameplayState::GetInstance()->GetTimer() > 10.0f)
		pFont->Write(ossTurnTimer.str().c_str(), int(CGame::GetInstance()->GetScreenWidth()*0.50f - 15), 75, 1, D3DCOLOR_XRGB(255, 255, 255));
	else if(flash == true)
		pFont->Write(ossTurnTimer.str().c_str(), int(CGame::GetInstance()->GetScreenWidth()*0.50f - 15), 75, 1, D3DCOLOR_XRGB(255, 0, 0));

	if (m_bFireworks == true)
	{
		if (m_bPlants == true)
		{
			AnimationSystem::GetInstance()->Render(plantRect.right, plantRect.top, m_aChangedPercent);
		}
		else if (m_bRobot == true)
		{
			AnimationSystem::GetInstance()->Render(robotRect.left, robotRect.top, m_aChangedPercent);
		}
	}

	if(currUnit != nullptr)
	{
		if (currUnit->GetFaction() == OWNER_AI)
			currUnit->SetFaction(OWNER_ROBOTS);

		stringstream hp, dmg;
		hp << currUnit->GetCurrHealth();
		TM->Draw(shieldID, currUnit->GetCurrTile()->GetPosX() - 20 + pGamePlay->OffsetX(), currUnit->GetCurrTile()->GetPosY() + 40 + pGamePlay->OffsetY());

		if(currUnit->GetCurrTile()->GetFactionType() == currUnit->GetFaction())
			pFont->Write(hp.str().c_str(), currUnit->GetCurrTile()->GetPosX() - 10 + pGamePlay->OffsetX(), currUnit->GetCurrTile()->GetPosY() + 40 + pGamePlay->OffsetY(), 0.9f, D3DCOLOR_ARGB(255, 0, 255, 0));
		else if(currUnit->GetCurrTile()->GetFactionType() == OWNER_NEUTRAL)
			pFont->Write(hp.str().c_str(), currUnit->GetCurrTile()->GetPosX() - 10 + pGamePlay->OffsetX(), currUnit->GetCurrTile()->GetPosY() + 40 + pGamePlay->OffsetY(), 0.9f, D3DCOLOR_ARGB(255, 255, 255, 0));
		else
			pFont->Write(hp.str().c_str(), currUnit->GetCurrTile()->GetPosX() - 10 + pGamePlay->OffsetX(), currUnit->GetCurrTile()->GetPosY() + 40 + pGamePlay->OffsetY(), 0.9f, D3DCOLOR_ARGB(255, 255, 0, 0));

		if(dynamic_cast<CMovingEntity*>(currUnit))
		{
			dmg << dynamic_cast<CMovingEntity*>(currUnit)->GetCurrDamage();
			TM->Draw(swordID, currUnit->GetCurrTile()->GetPosX() + 50 + pGamePlay->OffsetX(), currUnit->GetCurrTile()->GetPosY() + 40 + pGamePlay->OffsetY());
			if(currUnit->GetCurrTile()->GetFactionType() == currUnit->GetFaction())
				pFont->Write(dmg.str().c_str(), currUnit->GetCurrTile()->GetPosX() + 60 + pGamePlay->OffsetX(), currUnit->GetCurrTile()->GetPosY() + 40 + pGamePlay->OffsetY(), 0.9f, D3DCOLOR_ARGB(255, 0, 255, 0));	
			else if(currUnit->GetCurrTile()->GetFactionType() == OWNER_NEUTRAL)
				pFont->Write(dmg.str().c_str(), currUnit->GetCurrTile()->GetPosX() + 60 + pGamePlay->OffsetX(), currUnit->GetCurrTile()->GetPosY() + 40 + pGamePlay->OffsetY(), 0.9f, D3DCOLOR_ARGB(255, 255, 255, 100));
			else
				pFont->Write(dmg.str().c_str(), currUnit->GetCurrTile()->GetPosX() + 60 + pGamePlay->OffsetX(), currUnit->GetCurrTile()->GetPosY() + 40 + pGamePlay->OffsetY(), 0.9f, D3DCOLOR_ARGB(255, 255, 0, 0));

			if (CGame::GetInstance()->GetGameType() == SINGLE_PLAYER
				&&currUnit->GetFaction() == OWNER_ROBOTS )
				currUnit->SetFaction(OWNER_AI);
		}
	}
	m_bFirstTurn = false;
}

void CHUD::Update(float dt)
{
	flashTimer -= dt;

	if(flashTimer <= 0)
	{
		flash = !flash;
		flashTimer = 0.35f;
	}
	if (CGameplayState::GetInstance()->GetSelectedUnit() != nullptr)
		selectedUnit = CGameplayState::GetInstance()->GetSelectedUnit();
	currUnit = CGameplayState::GetInstance()->GetCurrUnit();

	if(selectedUnit != nullptr)
	{
		if(selectedUnit->GetCurrHealth() <= 0)
			selectedUnit = nullptr;
	}

	if(currUnit != nullptr)
	{
		if(currUnit->GetCurrHealth() <= 0)
			currUnit = nullptr;
	}

	previous = (float)(plantRect.left + 400 * plantCount);
	if (plantRect.right != previous)
	{
		plantRect.right = (LONG)(plantRect.left + 400 * plantCount);
	}

	neutralRect.left = plantRect.right;
	neutralRect.right = (LONG)(neutralRect.left + 400 * neutralCount);

	previous = (float)(neutralRect.right);
	if (robotRect.left != previous)
	{
		robotRect.left = neutralRect.right;
		robotRect.right = (LONG)(robotRect.left + 400 * robotCount);
	}

	if (m_bFireworks == true)
	{
		if (m_bPlants == true)
			m_aChangedPercent.Update(dt, m_bFireworks, m_bPlants);
		else if (m_bRobot == true)
			m_aChangedPercent.Update(dt, m_bFireworks, m_bRobot);
	}

	//////////////////////////////////////////////////////////
	// BUG FIX: 22|84
	// Fixed By: Kenneth Moy
	//////////////////////////////////////////////////////////
	// Begin Fix
	tileCount = (float)pGamePlay->GetGameMap()->GetTileCount();

	float currentPCount = plantCount;
	if (currentPCount != pGamePlay->GetGameMap()->GetPlantTileCount() / tileCount)
	{
		if (m_bFirstTurn == false)
			m_bFireworks = true;
		m_bPlants = true;
		plantCount = pGamePlay->GetGameMap()->GetPlantTileCount() / tileCount;
	}

	neutralCount = pGamePlay->GetGameMap()->GetNeutralTileCount() / tileCount;

	float currentRCount = robotCount;
	if (currentRCount != pGamePlay->GetGameMap()->GetRobotTileCount() / tileCount)
	{
		if (m_bFirstTurn == false)
			m_bFireworks = true;
		//////////////////////////////////////////////////////////
		// BUG FIX: 64|84
		// Fixed By: Kenneth Moy
		//////////////////////////////////////////////////////////
		// Begin Fix
		m_bRobot = true;
		// End Fix
		robotCount = pGamePlay->GetGameMap()->GetRobotTileCount() / tileCount;
	}
	// End Fix

	char testString[20];
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_PLANTS, testString, 20);
	if(_stricmp(plantString, testString) != 0)
	{
		LoadStringA(CGame::GetInstance()->GethInstance(), IDS_PLANTS, plantString, 20);
		LoadStringA(CGame::GetInstance()->GethInstance(), IDS_ROBOTS, robotString, 20);
		LoadStringA(CGame::GetInstance()->GethInstance(), IDS_NEUTRAL, neutralString, 20);
		LoadStringA(CGame::GetInstance()->GethInstance(), IDS_ATTACK, attackString, 20);
		LoadStringA(CGame::GetInstance()->GethInstance(), IDS_MOVE, moveString, 20);
		LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CANCEL, cancelString, 20);
		LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CONVERT, convertString, 20);
		LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CREATE, createString, 20);
		LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CREATE_TANK, tankString, 20);
		LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CREATE_SCOUT, scoutString, 20);
		LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CREATE_INFANTRY, infantryString, 20);
	}
}