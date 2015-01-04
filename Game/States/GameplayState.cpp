/*************************************************************
|	File:		GameplayState.cpp
|	Purpose:	To initialize & run the gameplay
|
|	Author:		Doug Monroe (dmonroe@fullsail.com)
|	Course:		SGD 1312
*************************************************************/
#include "GameplayState.h"
#include "../MainInfo/Game.h"
#include "MainMenuState.h"
#include "ProfileState.h"
// SGD Wrappers
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"
#include "../SGD Wrappers/CSGD_MessageSystem.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_Math.h"
#include "../MainInfo/BMFont.h"
// Messages
#include "../MainInfo/Message.h"
#include "../MainInfo/CreateEntityMessage.h"
#include "../MainInfo/DestroyEntityMessage.h"
#include "../MainInfo/UnitConvertTile.h"
#include "../MainInfo/UnitMoveMessage.h"
#include "../MainInfo/KOTHMessage.h"
// Entities
#include "../MainInfo/EntityManager.h"
#include "../MainInfo/AIEntity.h"
//#include "../MainInfo/Entity.h"
#include "../Systems/AnimationSystem.h"
#include "../MainInfo/AnimTimeStamps.h"
#include "../MainInfo/MovingEntity.h"
#include "../MainInfo/StaticEntity.h"
#include "../Grid/Tile.h"
#include "../MainInfo/BMFont.h"
#include "../SGD Wrappers/tinyxml.h"

#include "../States/SaveState.h"
#include "../States/OptionState.h"
#include "../States/GameOverState.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ShlObj.h>
#include <cassert>
#include <ctime>

//************************************************************
// Entity Manager Buckets
enum EntityBucket {  };

//************************************************************
// GetInstance
//	- allocate static global instance
//	- return THE instance
/*static*/ CGameplayState* CGameplayState::GetInstance( void )
{
	static CGameplayState s_Instance;
	return &s_Instance;
}

//************************************************************
// CONSTRUCTOR
//	- set ALL data members to starting values
CGameplayState::CGameplayState( void )
{	
	m_unFPS			= 0;
	m_unFrames		= 0;
	m_fFPSTimer		= 0.0f;	 

	m_fCamX = 0.0f;
	m_fCamY = 0.0f;

	m_nCamDestX = 0;
	m_nCamDestY = 0;
	m_nCamStartX = 0;
	m_nCamStartY = 0;

	m_bCamLocked = false;
	m_bBaseCreate = false;

	m_nCursorX = 0;
	m_nCursorY = 0;
	m_nSelectedAction = NONE;
	m_pSelectedUnit = nullptr;
	m_aCoinFlip.SetCurrentAnimation("Coin Flip");
	m_aAPExplosion.SetCurrentAnimation("Explosion AP");

	m_fSecsPerTurn = -1;
	m_nFirstTurn = OWNER_PLANTS;
	m_nCurrFaction = m_nFirstTurn;
	m_nWinner = OWNER_NEUTRAL;
	m_nAP = 5;

	/*m_nRoundsStat = 1;
	m_fPlantsTime = 0.0f;
	m_fRobotsTime = 0.0f;*/

	// -1 == INVALID INDEX

	m_pEM			= nullptr;

	m_pES			= nullptr;
	m_pMS			= nullptr;
}

//************************************************************
// DESTRUCTOR
CGameplayState::~CGameplayState( void )
{
}

//************************************************************
// Enter
//	- reset game
//	- load resources
//	- set up entities
/*virtual*/ void CGameplayState::Enter( void )		
{
	// Reset the fps
	m_unFPS			= 60;
	m_unFrames		= 0;
	m_fFPSTimer		= 0.0f;	
	m_fTurnTimer = m_fSecsPerTurn;
	m_fEndGameTimer = 3.0f;
	m_fAiUpdateTimer = AIEntity::GetInstance()->GetUpdateAiTimer();
	m_pSelectedUnit = nullptr;

	if (CGame::GetInstance()->GetGameType() == MULTIPLAYER)
	{
		m_nCoin = UNFLIPPED;
	}
	else if (CGame::GetInstance()->GetGameType() == SINGLE_PLAYER)
	{
		m_nCoin = UNFLIPPED;
		m_nStatus = COINFLIP;
	}
	//m_bWinnerChosen = false;
	m_fCoinFlipTimer = 2.0f;

	// Load the assets
	//	- RELATIVE TO THE PROJECT FILE!!!
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	m_pXA = CSGD_XAudio2::GetInstance();
	// Allocate the entity manager
	m_pEM = new CEntityManager();

	turnChangePosX = -500;

	// Initialize the Message System
	m_pMS = CSGD_MessageSystem::GetInstance();
	m_pMS->Initialize( &MessageProc );

	// Initialize the Event System
	m_pES = CSGD_EventSystem::GetInstance();

	tileSetImgID = pTM->LoadTexture(_T("Resources/Images/PVRtemp.bmp"));
	hud.SetHudImgId(pTM->LoadTexture(_T("Resources/Images/HUD.png")));
	hud.SetActionsImgId(pTM->LoadTexture(_T("Resources/Images/Actions.png")));

	storyBoard = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/EmptyMenu.png"));
	turnChange  = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/TurnChange.png"));
	tutorialBox = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/TurnChange.png"));
	BG = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/BG.png"));

	convertNormalSFX = m_pXA->SFXLoadSound(_T("Resources/Sounds/ConvertNormal.wav"));
	convertSpecialSFX = m_pXA->SFXLoadSound(_T("Resources/Sounds/ConvertSpecial.wav"));
	convertWaterSFX = m_pXA->SFXLoadSound(_T("Resources/Sounds/ConvertWater.wav"));
	plantDeselectSFX = m_pXA->SFXLoadSound(_T("Resources/Sounds/PlantDeselect.wav"));
	plantSelectSFX = m_pXA->SFXLoadSound(_T("Resources/Sounds/PlantSelect.wav"));
	robotDeselectSFX = m_pXA->SFXLoadSound(_T("Resources/Sounds/RobotDeselect.wav"));
	robotSelectSFX = m_pXA->SFXLoadSound(_T("Resources/Sounds/RobotSelect.wav"));
	turnEndSFX = m_pXA->SFXLoadSound(_T("Resources/Sounds/TurnEnd.wav"));
	roundEndSFX = m_pXA->SFXLoadSound(_T("Resources/Sounds/RoundEnd.wav"));
	timerLowSFX = m_pXA->SFXLoadSound(_T("Resources/Sounds/TimerLow.wav"));


	inTutorial = false;
	storyTime = false;

	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_PLANTS_TURN, turnChangeString, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_COINDIRECTIONS, coinString, 32);

	if(CSGD_DirectInput::GetInstance()->JoystickIsUnplugged())
		LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CONTINUE_CONFIRM_KEYBOARD, continueConfirm, 32);
	else
		LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CONTINUE_CONFIRM, continueConfirm, 32);

	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_COINDIRECTIONS, coinString, 32);

	if (m_fSecsPerTurn <= 0)
		m_fSecsPerTurn = 60.0f;

	m_fTurnTimer = m_fSecsPerTurn;
	m_nCoin = UNFLIPPED;

	if (m_bLoadingSavedGame)
	{
		//LoadXml();
		gameMap.Update(0);
	}
	else
	{
		if(CGame::GetInstance()->GetCurrentLevel() == TUTORIAL || CGame::GetInstance()->GetCurrentLevel() == JUST_TUTORIAL)
		{
			inTutorial = true;
			if(CSGD_DirectInput::GetInstance()->JoystickIsUnplugged(0) == true)
			{
				LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_1_KEYBOARD, tutorialInstruction, 200);
			}
			else
				LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_1, tutorialInstruction, 200);

			LoadStringA(CGame::GetInstance()->GethInstance(), IDS_STORYTIME0, storyTimeString, 250);

			tutorialStep = 1;
			string temp = "TUTORIAL";
			gameMap.LoadMap(temp);
			m_nStatus = GameplayStatus::PLAYING;
			m_fTurnTimer = 60.0f;
			storyTime = true;
		}
		else if (m_nStatus != OVER)
		{
			if (CGame::GetInstance()->GetCurrentLevel() == NO_LEVEL)
			{
				string temp = "TESTING";
				gameMap.LoadMap(temp);
				CGame::GetInstance()->SetCurrentLevel(TESTING);
			}
			else if (CGame::GetInstance()->GetCurrentLevel() == CAMPAIGN_LEVEL1)
			{
				gameMap.LoadMap("Level1");
				LoadStringA(CGame::GetInstance()->GethInstance(), IDS_STORYTIME1, storyTimeString, 250);
				storyTime = true;
			}
			else if (CGame::GetInstance()->GetCurrentLevel() == CAMPAIGN_LEVEL2)
			{
				gameMap.LoadMap("Level2");
				LoadStringA(CGame::GetInstance()->GethInstance(), IDS_STORYTIME2, storyTimeString, 250);
				storyTime = true;
			}
			else if (CGame::GetInstance()->GetCurrentLevel() == CAMPAIGN_LEVEL3)
			{
				gameMap.LoadMap("Level3");
				LoadStringA(CGame::GetInstance()->GethInstance(), IDS_STORYTIME3, storyTimeString, 250);
				storyTime = true;
			}
			else if (CGame::GetInstance()->GetCurrentLevel() == MPLEVEL1)
			{
				gameMap.LoadMap("Multiplayer1");
			}
			else if (CGame::GetInstance()->GetCurrentLevel() == MPLEVEL2)
			{
				gameMap.LoadMap("Multiplayer2");
			}
			else if (CGame::GetInstance()->GetCurrentLevel() == MPLEVEL3)
			{
				gameMap.LoadMap("Multiplayer3");
			}
			else if (CGame::GetInstance()->GetCurrentLevel() == MPLEVEL4)
			{
				gameMap.LoadMap("MultiplayerKOTH");
			}

			m_nStatus = COINFLIP;
		}
		else if (m_nStatus == OVER)
		{
			if (CGame::GetInstance()->GetCurrentLevel() == TESTING)
			{
				gameMap.LoadMap("Level1");
				CGame::GetInstance()->SetCurrentLevel(CAMPAIGN_LEVEL1);
			}
			else if (CGame::GetInstance()->GetCurrentLevel() == CAMPAIGN_LEVEL1)
			{
				gameMap.LoadMap("Level2");
				CGame::GetInstance()->SetCurrentLevel(CAMPAIGN_LEVEL2);
			}
			else if (CGame::GetInstance()->GetCurrentLevel() == CAMPAIGN_LEVEL2)
			{
				gameMap.LoadMap("Level3");
				CGame::GetInstance()->SetCurrentLevel(CAMPAIGN_LEVEL3);
			}
			m_nStatus = COINFLIP;
		}
	}

	AIEntity::GetInstance()->SetAiUnits()->clear();

	m_nCursor = RESUME;

	m_nAP = 5;

	m_pMS->ProcessMessages();
	m_nActiveUnits = (OWNER_PLANTS == m_nCurrFaction ? m_nPlantUnits : m_nRobotUnits);
	step1 = step2 = step3 = true;
	m_nRoundsStat = 0;
	m_fPlantsTime = 0.0f;
	m_fRobotsTime = 0.0f;

	m_bBeingPlayed = true;

	m_nKotHPlants = 0;
	m_nKotHRobots = 0;

	selectionChangedSFX = m_pXA->SFXLoadSound(_T("Resources/Sounds/SelectionChange.wav"));
	selectionMadeSFX = m_pXA->SFXLoadSound(_T("Resources/Sounds/SelectionMade.wav"));
	selectionBackSFX = m_pXA->SFXLoadSound(_T("Resources/Sounds/SelectionBack.wav"));
}

//************************************************************
// Exit
//	- deallocate entities
//	- unload resources
/*virtual*/ void CGameplayState::Exit( void )		
{
	m_bBeingPlayed = false;
	CPlayerProfile* pProf = CGame::GetInstance()->GetProfile();
	ostringstream oss;

	m_fSecsPerTurn = -1;

	m_fTotalTime = m_fPlantsTime + m_fRobotsTime;
	pProf->SetRounds(pProf->GetRounds() + m_nRoundsStat);
	pProf->SetTime(pProf->GetTime() + m_fTotalTime);
	pProf->SetCampaignProgress(CGame::GetInstance()->GetCurrentLevel());
	//oss << pProf->GetProfName() << "Save" << std::time(0) << ".sav";

	//oss << "Resources/" << pProf->GetProfName() << "Save" << std::time(0) << ".sav";
	/*SaveXml(oss.str().c_str());*/

	// Terminate the Event System
	m_pES->Terminate();
	m_pES = nullptr;

	// Terminate the Message System
	m_pMS->Terminate();
	m_pMS = nullptr;

	// Deallocate the entity manager
	m_pEM->RemoveAll();
	delete m_pEM;
	m_pEM = nullptr;

	// Unload assets
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	CSGD_XAudio2* pXA = CSGD_XAudio2::GetInstance();

	pTM->UnloadTexture(tileSetImgID);

	m_nSelectedAction = NONE;
	pCurUnit = nullptr;
	m_pSelectedUnit = nullptr;
	gameMap.Unhighlight();
	m_fTurnTimer = m_fSecsPerTurn;

	//TODO: Replace this with campaign advancement checks
	//CGame::GetInstance()->SetCurrentLevel(CurrentLevel::NO_LEVEL);
}

/*virtual*/ bool CGameplayState::Input( void )
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	if(pDI->KeyPressed(DIK_NUMPAD7))
	{
		m_nStatus = OVER;
		m_nWinner = OWNER_PLANTS;
		return true;
	}

	if(storyTime == true)
	{
		if(pDI->JoystickButtonPressed(0) || pDI->KeyPressed(DIK_NUMPAD2) || pDI->KeyPressed(DIK_RETURN))
		{
			storyTime = false;
			m_pXA->SFXPlaySound(selectionMadeSFX);
		}
		return true;
	}	


	// Press 'Escape' to return to main menu
	if((pDI->KeyPressed(DIK_ESCAPE) || pDI->JoystickButtonPressed(7, m_nCurrFaction)) && PLAYING == m_nStatus)
	{
		if (m_nStatus != PLAYING && pDI->KeyPressed(DIK_ESCAPE))
		{
			CGame::GetInstance()->SetCurrentLevel(CurrentLevel::NO_LEVEL);
			CGame::GetInstance()->ChangeState( CMainMenuState::GetInstance() );
		}
		else 
		{
			m_pXA->SFXPlaySound(selectionMadeSFX);
			m_nStatus = PAUSED;
		}
		// GTFO!
		return true;
	}

	if (m_nStatus == PAUSED)
	{
		if (pDI->KeyPressed(DIK_DOWN) || pDI->KeyPressed(DIK_S) || pDI->JoystickDPadPressed(DIR_DOWN))
		{
			if (m_nCursor < QUITGAME)
				m_nCursor++;
		}
		else if (pDI->KeyPressed(DIK_UP) || pDI->KeyPressed(DIK_W) || pDI->JoystickDPadPressed(DIR_UP))
		{
			if (m_nCursor > RESUME)
				m_nCursor--;
		}

		if (pDI->KeyPressed(DIK_RETURN) || pDI->JoystickButtonPressed(0)) //TODO: ENTER ON GAMEPAD
		{
			switch (m_nCursor)
			{
			case RESUME:
				m_nStatus = PLAYING;
				break;
			case SAVE:
				CGame::GetInstance()->PushState(CSaveState::GetInstance());
				break;
			case OPTIONS:
				CGame::GetInstance()->PushState(COptionState::GetInstance());
				break;
			case QUITTOMENU:
				CGame::GetInstance()->SetCurrentLevel(CurrentLevel::NO_LEVEL);
				CGame::GetInstance()->PopState();
				break;
			case QUITGAME:
				return false;
				break;
			}
		}
	}
	else if (m_nStatus == ENDROUND)
	{
		if (pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0, 0))
		{
			m_nStatus = END_TURN;
			m_pXA->SFXPlaySound(selectionMadeSFX);
		}
	}

	if(m_nStatus == END_TURN)
	{
		if(pDI->JoystickButtonPressed(0) || pDI->KeyPressed(DIK_NUMPAD2))
		{
			m_nStatus = PLAYING;
			turnChangePosX = -500;
		}
		return true;
	}

	if (m_nStatus == PLAYING && ((CGame::GetInstance()->GetGameType() == SINGLE_PLAYER && m_nCurrFaction == OWNER_PLANTS) || CGame::GetInstance()->GetGameType() == MULTIPLAYER) && !m_bCamLocked && (pDI->KeyPressed(DIK_BACKSPACE)|| pDI->JoystickButtonPressed(6, m_nCurrFaction)))
		EndTurn();

	if(SINGLE_PLAYER == CGame::GetInstance()->GetGameType())
	{
		if(inTutorial == true)
			if(TutorialInput() == false)
				return true;

		if(m_nCurrFaction != OWNER_PLANTS)
			return true;
	}







#pragma region Coin Flip
	if (m_nStatus == COINFLIP)
	{
		if (pDI->KeyDown(DIK_SPACE) || pDI->JoystickButtonDown(0))
		{
			if (UNFLIPPED == m_nCoin)
			{
				m_nCoin = FLIPPING;
			}
		}
		else if (m_nCoin == HEADS)
		{
			if (pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(2, 0))
			{
				m_nFirstTurn = OWNER_PLANTS;
				m_nCurrFaction = m_nFirstTurn;
				m_nStatus = PLAYING;
				/*m_bCoinFlipped = true;*/
			}
			else if (pDI->KeyPressed(DIK_NUMPAD6) || pDI->JoystickButtonPressed(1, 0))
			{
				if (CGame::GetInstance()->GetGameType() != SINGLE_PLAYER)
				{
					m_nFirstTurn = OWNER_ROBOTS;
					m_nCurrFaction = m_nFirstTurn;
				}
				else if (CGame::GetInstance()->GetGameType() == SINGLE_PLAYER)
				{
					m_nFirstTurn = OWNER_AI;
					m_nCurrFaction = m_nFirstTurn;
				}
				m_nStatus = PLAYING;
				m_bCoinFlipped = true;
			}
		}
		else if (m_nCoin == TAILS)
		{
			if (pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(2, 1))
			{
				m_nFirstTurn = OWNER_PLANTS;
				m_nCurrFaction = m_nFirstTurn;
				m_nStatus = PLAYING;

			}
			else if (pDI->KeyPressed(DIK_NUMPAD6) || pDI->JoystickButtonPressed(1, 1))
			{
				m_nFirstTurn = OWNER_ROBOTS;
				m_nCurrFaction = m_nFirstTurn;
				m_nStatus = PLAYING;

			}
		}
	}
#pragma endregion

	else if (m_nStatus == PLAYING /*&& m_bCoinFlipped == true*/)
	{
		if (!m_bCamLocked)
		{

			//Cursor scrolling
			if (0 < m_nCursorX && (pDI->KeyPressed(DIK_A) || pDI->JoystickDPadPressed(DIR_LEFT, m_nCurrFaction)))
				m_nCursorX--;
			else if(m_nCursorX + 1 < gameMap.GetMapWidth() && (pDI->KeyPressed(DIK_D) || pDI->JoystickDPadPressed(DIR_RIGHT, m_nCurrFaction)))
				m_nCursorX++;

			if (0 < m_nCursorY && (pDI->KeyPressed(DIK_W) || pDI->JoystickDPadPressed(DIR_UP, m_nCurrFaction)))
				m_nCursorY--;
			else if (m_nCursorY + 1 < gameMap.GetMapHeight() && (pDI->KeyPressed(DIK_S) || pDI->JoystickDPadPressed(DIR_DOWN, m_nCurrFaction)))
				m_nCursorY++;
		}


		CStaticEntity* pSelectedStatic = dynamic_cast<CStaticEntity*>(m_pSelectedUnit);

		//Unit selection and deselection
		if (pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0, m_nCurrFaction)) // Move
		{
			if (pCurTile->GetOccupyingUnit() != nullptr || CGame::GetInstance()->GetCurrentUnit() != nullptr || m_nSelectedAction != NONE)
			{
#pragma region m_pSelectedUnit == nullptr
				if(m_pSelectedUnit == nullptr && pCurUnit != nullptr)
				{
					if (m_nCurrFaction == pCurUnit->GetFaction() && pCurUnit->GetUsedAction() == false)
					{
						m_pSelectedUnit = pCurUnit;

						if(m_pSelectedUnit->GetFaction() == OWNER_PLANTS)	
							m_pXA->SFXPlaySound(plantSelectSFX);
						else
							m_pXA->SFXPlaySound(robotSelectSFX);

						if (CGame::GetInstance()->GetCurrentUnit() == nullptr)
							CGame::GetInstance()->SetCurrentUnit(pCurUnit);

						if(m_pSelectedUnit->GetUnitType() == UnitType::UNIT_BASE)
							m_nSelectedAction = CREATE;

						float fX = pCurTile->GetPosX() + 32 - CGame::GetInstance()->GetScreenWidth() / 2.0f;
						float fY = pCurTile->GetPosY() + 32 - CGame::GetInstance()->GetScreenHeight() / 2.0f;
						PanTo(fX, fY);

						m_fCamTimer = 0.0f;
					}
				}
#pragma endregion

#pragma region m_pSelectedUnits->GetUnitType() == UnitType::UNIT_BASE
				else if(m_pSelectedUnit != nullptr && m_pSelectedUnit->GetUnitType() == UnitType::UNIT_BASE && CREATE == m_nSelectedAction)
				{
					m_nSelectedAction = CREATE_SCOUT;
					pSelectedStatic->ShowCreateRange(gameMap);
					m_pXA->SFXPlaySound(selectionMadeSFX);
				}
#pragma endregion
#pragma region m_pSelectedUnit Actions
				else if (m_pSelectedUnit != nullptr)
				{
					CMovingEntity* pSelectedMover = dynamic_cast<CMovingEntity*>(m_pSelectedUnit);

					switch (m_nSelectedAction)
					{
					case NONE:
						{
							if (m_pSelectedUnit->GetUnitType() != UNIT_BASE)
							{
								if (pSelectedMover->GetMoved() == false)
								{
									m_nSelectedAction = MOVE;
									CGame::GetInstance()->SetCurrentUnit(pCurUnit);
									pSelectedMover->ShowMoveRange(gameMap);
									m_pXA->SFXPlaySound(selectionMadeSFX);
								}
								else if (pSelectedMover->GetMoved() && pSelectedMover->GetCurrTile() != pSelectedMover->GetPreviousTile())
								{
									m_nAP++;
									pSelectedMover->UndoMove();
								}
							}

						}
						break;
					case MOVE:
						{
							if (pSelectedMover->GetMoving() == false && pCurTile->isHighlighted() == true && pCurTile->GetOccupyingUnit() == nullptr)
							{							
								pSelectedMover->SetPreviousTile(m_pSelectedUnit->GetCurrTile());
								pSelectedMover->SetTargetTile(pCurTile);
								pSelectedMover->SetMoving(true);
								pSelectedMover->MoveToTile(pCurTile, gameMap);
							}
						}
						break;
					case CONVERT:
						{
							pSelectedMover->SetTargetTile(pCurTile);
							pSelectedMover->SetCurrentAnimStamp(pSelectedMover->GetUnitConvertStamps());
							pSelectedMover->SetConvertingTile(true);

							if(m_pSelectedUnit->GetUsedAction() == true)
								Deselect();
						}
						break;
					case ATTACK:
						{
							if (m_pSelectedUnit && pCurUnit && m_pSelectedUnit->GetFaction() != pCurUnit->GetFaction()) //If there is a selected unit AND a unit under the cursor
							{
								pSelectedMover->SetTargetUnit(pCurUnit);
								if (pSelectedMover->AttackUnit(pCurUnit, pCurTile, gameMap) == true)
									Deselect();
							}
						}
						break;
					case CREATE_SCOUT:
						if (pSelectedStatic->CheckCreateRange(pCurTile, gameMap) == true)
						{
							//////////////////////////////////////////////////////////
							// BUG FIX: 54|84, 58|84
							// Fixed By: Kenneth Moy
							//////////////////////////////////////////////////////////
							// Begin Fix
							CGameplayState::GetInstance()->SetUnitCreating(true);
							// End Fix
							m_pMS->QueueMessage(new CCreateEntityMessage(UnitType::UNIT_SCOUT, m_pSelectedUnit->GetFaction(), pCurTile));
							pSelectedStatic->SetUsedAction(true);
							Deselect();
						}
						break;
					case CREATE_INFANTRY:
						if (pSelectedStatic->CheckCreateRange(pCurTile, gameMap) == true)
						{
							CGameplayState::GetInstance()->SetUnitCreating(true);
							m_pMS->QueueMessage(new CCreateEntityMessage(UnitType::UNIT_INFANTRY, m_pSelectedUnit->GetFaction(), pCurTile));
							pSelectedStatic->SetUsedAction(true);
							Deselect();
						}
						break;
					case CREATE_TANK:
						if (pSelectedStatic->CheckCreateRange(pCurTile, gameMap) == true)
						{
							CGameplayState::GetInstance()->SetUnitCreating(true);
							m_pMS->QueueMessage(new CCreateEntityMessage(UnitType::UNIT_TANK, m_pSelectedUnit->GetFaction(), pCurTile));
							pSelectedStatic->SetUsedAction(true);
							Deselect();
						}
						break;
					default:
						break;
					}
				}
#pragma endregion
#pragma region Set MOVE if not highlighted
				else if (CGame::GetInstance()->GetCurrentUnit() != nullptr && m_nSelectedAction == MOVE)
				{
					m_nSelectedAction = MOVE;
					CGame::GetInstance()->SetCurrentUnit(pCurUnit);
					dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->ShowMoveRange(gameMap);
				}
#pragma endregion
			}
		}

		else if (pDI->KeyPressed(DIK_NUMPAD4) || pDI->JoystickButtonPressed(2, m_nCurrFaction)) // Attack
		{
			switch (m_nSelectedAction)
			{
#pragma region NONE
			case NONE:
				if (m_pSelectedUnit && m_pSelectedUnit->GetType() != UNIT_BASE) //If a unit which is not a base is selected...
				{
					m_nSelectedAction = ATTACK; //Set to attack mode so user can pick a target
					dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->ShowAttackRange(gameMap);
					m_pXA->SFXPlaySound(selectionMadeSFX);
				}
				break;
#pragma endregion
#pragma region ATTACK
			case ATTACK:
				if (m_pSelectedUnit && pCurUnit && m_pSelectedUnit->GetFaction() != pCurUnit->GetFaction()) //If there is a selected unit AND a unit under the cursor
				{
					dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->SetTargetUnit(pCurUnit);
					if (static_cast<CMovingEntity*>(m_pSelectedUnit)->AttackUnit(pCurUnit, pCurTile, gameMap) == true)
						Deselect();
				}
				break;
#pragma endregion
#pragma region CREATE
			case CREATE:
				{
					m_nSelectedAction = CREATE_INFANTRY;
					pSelectedStatic->ShowCreateRange(gameMap);
					m_pXA->SFXPlaySound(selectionMadeSFX);
				}
				break;
			case CREATE_INFANTRY:
				if (pSelectedStatic->CheckCreateRange(pCurTile, gameMap) == true)
				{
					CGameplayState::GetInstance()->SetUnitCreating(true);
					m_pMS->QueueMessage(new CCreateEntityMessage(UnitType::UNIT_INFANTRY, m_pSelectedUnit->GetFaction(), pCurTile));
					pSelectedStatic->SetUsedAction(true);
					Deselect();
				}
				break;
#pragma endregion
			default:
				break;
			}
		}
		else if (pDI->KeyPressed(DIK_NUMPAD6) || pDI->JoystickButtonPressed(1, m_nCurrFaction)) // Cancel
		{
			if((m_nSelectedAction == CurrentCommand::NONE || m_nSelectedAction == CurrentCommand::CREATE) && m_pSelectedUnit != nullptr)
			{
				if(m_pSelectedUnit->GetFaction() == OWNER_PLANTS)	
					m_pXA->SFXPlaySound(plantDeselectSFX);
				else
					m_pXA->SFXPlaySound(robotDeselectSFX);

				Deselect();
			}
			else if(m_pSelectedUnit != nullptr)
			{
				if(m_pSelectedUnit->GetType() == UnitType::UNIT_BASE)
					m_nSelectedAction = CurrentCommand::CREATE;
				else
					m_nSelectedAction = CurrentCommand::NONE;

				m_pXA->SFXPlaySound(selectionBackSFX);

				gameMap.Unhighlight();
			}
		}
		else if (pDI->KeyPressed(DIK_NUMPAD8) || pDI->JoystickButtonPressed(3, m_nCurrFaction)) // Convert
		{
#pragma region NONE
			if(m_pSelectedUnit != nullptr && m_nSelectedAction == NONE)
			{
				if(m_pSelectedUnit->GetUnitType() != UnitType::UNIT_BASE)
				{
					m_nSelectedAction = CONVERT;
					m_pXA->SFXPlaySound(selectionMadeSFX);
					dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->ShowConvertRange(gameMap);
				}
			}
#pragma endregion
			else if(m_nSelectedAction == CONVERT)
			{
				if(pCurTile->GetTileType() != WATER && pCurTile->GetTileType() != SWAMP && pCurTile->GetTileType() != SLUDGE)
				{

					dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->SetTargetTile(pCurTile);
					dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->SetCurrentAnimStamp(
						dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->GetUnitConvertStamps());
					dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->SetConvertingTile(true);
					if(m_pSelectedUnit->GetUsedAction() == true)
						Deselect();
				}
			}
			else if (m_nSelectedAction == CREATE)
			{
				m_nSelectedAction = CREATE_TANK;
				m_pXA->SFXPlaySound(selectionMadeSFX);
				dynamic_cast<CStaticEntity*>(m_pSelectedUnit)->ShowCreateRange(gameMap);
			}
			else if (m_nSelectedAction == CREATE_TANK)
				if (dynamic_cast<CStaticEntity*>(m_pSelectedUnit)->CheckCreateRange(pCurTile, gameMap) == true)
				{
					CGameplayState::GetInstance()->SetUnitCreating(true);
					m_pMS->QueueMessage(new CCreateEntityMessage(UnitType::UNIT_TANK, m_pSelectedUnit->GetFaction(), pCurTile));
					dynamic_cast<CStaticEntity*>(m_pSelectedUnit)->SetUsedAction(true);
					Deselect();
				}
		}
	}
	else if (m_nStatus == PAUSED)
	{
		if (pDI->KeyPressed(DIK_BACKSPACE) || pDI->JoystickButtonPressed(1, m_nCurrFaction))
		{
			m_nStatus = PLAYING;
			m_pXA->SFXPlaySound(selectionMadeSFX);
		}
	}
	return true;	// keep playing
}

bool CGameplayState::TutorialInput()
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	if(m_nCurrFaction == OWNER_PLANTS)
		m_fTurnTimer = 60.9f;

	if(tutorialStep == 23)
	{
		for(int y = 0; y < gameMap.GetMapHeight(); y++)
		{
			for(int x = 0; x < gameMap.GetMapWidth(); x++)
			{
				if(gameMap.GetTile(x, y)->GetOccupyingUnit() != nullptr && gameMap.GetTile(x, y)->GetOccupyingUnit()->GetFaction() == OWNER_AI)
					return true;
			}
		}

		tutorialStep = 99;
	}

	if(m_nStatus == GameplayStatus::ENDROUND)
	{
		if(pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0))
			m_nStatus = GameplayStatus::END_TURN;
		return true;
	}

	if(pDI->KeyDown(DIK_LSHIFT) || pDI->JoystickButtonPressed(4))
	{
		if(CGame::GetInstance()->GetCurrentLevel() != JUST_TUTORIAL)
		{
			CGame::GetInstance()->SetGameType(SINGLE_PLAYER);
			CGame::GetInstance()->SetCurrentLevel(CAMPAIGN_LEVEL1);
		}

		m_nRoundsStat = 0;
		tutorialStep = 99;
	}

	//Cursor scrolling
	if (0 < m_nCursorX && (pDI->KeyPressed(DIK_A) || pDI->JoystickDPadPressed(DIR_LEFT, m_nCurrFaction)))
		m_nCursorX--;
	else if(m_nCursorX + 1 < gameMap.GetMapWidth() && (pDI->KeyPressed(DIK_D) || pDI->JoystickDPadPressed(DIR_RIGHT, m_nCurrFaction)))
		m_nCursorX++;

	if (0 < m_nCursorY && (pDI->KeyPressed(DIK_W) || pDI->JoystickDPadPressed(DIR_UP)))
		m_nCursorY--;
	else if (m_nCursorY + 1 < gameMap.GetMapHeight() && (pDI->KeyPressed(DIK_S) || pDI->JoystickDPadPressed(DIR_DOWN, m_nCurrFaction)))
		m_nCursorY++;

	if(pDI->KeyDown(DIK_NUMPADMINUS))
	{
		tutorialStep = 23;
		LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_20, tutorialInstruction, 200);
	}

	switch(tutorialStep)
	{
	case 1: // Welcome
		{
			if((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0)))
			{
				tutorialStep++;
				if(CSGD_DirectInput::GetInstance()->JoystickIsUnplugged(0) == true)
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_2_KEYBOARD, tutorialInstruction, 200);
				else
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_2, tutorialInstruction, 200);
			}
		}
		break;
	case 2: // Move camera
		{
			if(pDI->JoystickGetRStickDirDown(DIR_DOWN, OWNER_PLANTS) || pDI->JoystickGetRStickDirDown(DIR_UP, OWNER_PLANTS) 
				|| pDI->JoystickGetRStickDirDown(DIR_LEFT, OWNER_PLANTS) || pDI->JoystickGetRStickDirDown(DIR_RIGHT, OWNER_PLANTS)
				|| pDI->KeyPressed(DIK_UP) || pDI->KeyPressed(DIK_DOWN) || pDI->KeyPressed(DIK_LEFT) || pDI->KeyPressed(DIK_RIGHT))
			{
				tutorialStep++;
				if(CSGD_DirectInput::GetInstance()->JoystickIsUnplugged(0) == true)
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_3_KEYBOARD, tutorialInstruction, 200);
				else
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_3, tutorialInstruction, 200);
			}			
		}
		break;
	case 3: // Move Cursor
		{
			if(pDI->JoystickDPadPressed(DIR_DOWN, OWNER_PLANTS) || pDI->JoystickDPadPressed(DIR_UP, OWNER_PLANTS)
				|| pDI->JoystickDPadPressed(DIR_LEFT, OWNER_PLANTS) || pDI->JoystickDPadPressed(DIR_RIGHT, OWNER_PLANTS)
				|| pDI->KeyPressed(DIK_W) || pDI->KeyPressed(DIK_A) || pDI->KeyPressed(DIK_S) || pDI->KeyPressed(DIK_D))
			{
				tutorialStep++;
				tutorialHighlight.top = 30;
				tutorialHighlight.left = 265;
				tutorialHighlight.bottom = 70;
				tutorialHighlight.right = 730;

				if(CSGD_DirectInput::GetInstance()->JoystickIsUnplugged(0) == true)
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_HUD1_KEYBOARD, tutorialInstruction, 200);
				else
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_HUD1, tutorialInstruction, 200);
			}
		}
		break;
	case 4: // Percentage
		{
			if((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0)))
			{
				tutorialStep++;
				tutorialHighlight.top = 5;
				tutorialHighlight.left = 265;
				tutorialHighlight.bottom = 45;
				tutorialHighlight.right = 730;

				if(CSGD_DirectInput::GetInstance()->JoystickIsUnplugged(0) == true)
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_HUD2_KEYBOARD, tutorialInstruction, 200);
				else
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_HUD2, tutorialInstruction, 200);
			}
		}
		break;
	case 5: // Visual
		{
			if((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0)))
			{
				tutorialStep++;
				tutorialHighlight.top = 65;
				tutorialHighlight.left = 255;
				tutorialHighlight.bottom = 110;
				tutorialHighlight.right = 485;
				if(CSGD_DirectInput::GetInstance()->JoystickIsUnplugged(0) == true)
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_HUD3_KEYBOARD, tutorialInstruction, 200);
				else
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_HUD3, tutorialInstruction, 200);
			}
		}
		break;
	case 6: // AP
		{
			if((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0)))
			{
				tutorialStep++;
				tutorialHighlight.top = 70;
				tutorialHighlight.left = 475;
				tutorialHighlight.bottom = 115;
				tutorialHighlight.right = 535;
				if(CSGD_DirectInput::GetInstance()->JoystickIsUnplugged(0) == true)
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_HUD4_KEYBOARD, tutorialInstruction, 200);
				else
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_HUD4, tutorialInstruction, 200);
			}
		}
		break;
	case 7: // Action Menu
		{
			if((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0)))
			{
				tutorialStep++;
				if(CSGD_DirectInput::GetInstance()->JoystickIsUnplugged(0) == true)
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_4_KEYBOARD, tutorialInstruction, 200);
				else
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_4, tutorialInstruction, 200);

				tutorialHighlight.top = 120 + OffsetY();
				tutorialHighlight.left = 0 + OffsetX();
				tutorialHighlight.bottom = tutorialHighlight.top + 80;
				tutorialHighlight.right = tutorialHighlight.left + 80;
			}
		}
		break;
	case 8: // Select base
		{
			if(step1 == true)
			{
				tutorialHighlight.top = 120 + OffsetY();
				tutorialHighlight.left = -10 + OffsetX();
				tutorialHighlight.bottom = tutorialHighlight.top + 80;
				tutorialHighlight.right = tutorialHighlight.left + 80;
			}
			else if(step2 == true)
			{				
				tutorialHighlight.top = 30 + OffsetY();
				tutorialHighlight.left = -160 + OffsetX();
				tutorialHighlight.bottom = tutorialHighlight.top + 270;
				tutorialHighlight.right = tutorialHighlight.left + 370;
			}
			else if(step3 == true)
			{				
				tutorialHighlight.top = 130 + OffsetY();
				tutorialHighlight.left = -165 + OffsetX();
				tutorialHighlight.bottom = tutorialHighlight.top + 55;
				tutorialHighlight.right = tutorialHighlight.left + 120;
			}
			else
			{
				tutorialHighlight.top = 55 + OffsetY();
				tutorialHighlight.left = -5 + OffsetX();
				tutorialHighlight.bottom = tutorialHighlight.top + 75;
				tutorialHighlight.right = tutorialHighlight.left + 75;
			}

			if(m_pSelectedUnit != nullptr && m_pSelectedUnit->GetUnitType() == UNIT_BASE && ((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0)) || (pDI->KeyPressed(DIK_NUMPAD4) || pDI->JoystickButtonPressed(2, m_nCurrFaction)))
				&& pCurTile->GetPosX() == m_pSelectedUnit->GetPosX()&& pCurTile->GetPosY() == m_pSelectedUnit->GetPosY() - gameMap.GetTileHeight() && m_nSelectedAction == CREATE_INFANTRY)
			{
				m_pMS->QueueMessage(new CCreateEntityMessage(UnitType::UNIT_INFANTRY, m_pSelectedUnit->GetFaction(), pCurTile));
				m_pSelectedUnit->SetUsedAction(true);
				Deselect();
				tutorialStep++;
				tutorialStep++;
				tutorialHighlight.top = -1;
				if(CSGD_DirectInput::GetInstance()->JoystickIsUnplugged(0) == true)
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_7_KEYBOARD, tutorialInstruction, 200);
				else
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_7, tutorialInstruction, 200);
				step1 = step2 = step3 = true;
			}
			else if((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0)) && pCurUnit != nullptr && pCurUnit->GetUnitType() == UnitType::UNIT_BASE && pCurUnit->GetFaction() == OWNER_PLANTS && m_pSelectedUnit == nullptr)
			{
				m_pSelectedUnit = pCurUnit;
				m_nSelectedAction = CREATE;
				if(CSGD_DirectInput::GetInstance()->JoystickIsUnplugged(0) == true)
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_HUD5_KEYBOARD, tutorialInstruction, 200);
				else
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_HUD5, tutorialInstruction, 200);
				step1 = false;
			}
			else if((pDI->KeyPressed(DIK_NUMPAD4) || pDI->JoystickButtonPressed(2, m_nCurrFaction)) && m_pSelectedUnit != nullptr && m_pSelectedUnit->GetUnitType() == UNIT_BASE)
			{
				dynamic_cast<CStaticEntity*>(m_pSelectedUnit)->ShowCreateRange(gameMap);
				m_nSelectedAction = CurrentCommand::CREATE_INFANTRY;
				if(CSGD_DirectInput::GetInstance()->JoystickIsUnplugged(0) == true)
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_6_KEYBOARD, tutorialInstruction, 200);
				else
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_6, tutorialInstruction, 200);
				step3 = false;
				step2 = false;
			}
			else if(pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0) || pDI->KeyPressed(DIK_NUMPAD4) || pDI->JoystickButtonPressed(2) && m_pSelectedUnit != nullptr)
			{
				if(CSGD_DirectInput::GetInstance()->JoystickIsUnplugged(0) == true)
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_5_KEYBOARD, tutorialInstruction, 200);
				else
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_5, tutorialInstruction, 200);
				step2 = false;
			}
		}
		break;
	case 10: // Note turn end
		{
			if((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0)))
			{
				tutorialStep++;
				EndTurn();
				LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_7_5, tutorialInstruction, 200);
			}
		}
		break;
	case 11: // Note Stats
		{
			tutorialHighlight.top = 100 + OffsetY();
			tutorialHighlight.left = -25 + OffsetX();
			tutorialHighlight.bottom = tutorialHighlight.top + 40;
			tutorialHighlight.right = tutorialHighlight.left + 120;

			if((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0)))
			{
				tutorialStep++;
				if(CSGD_DirectInput::GetInstance()->JoystickIsUnplugged(0) == true)
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_8_KEYBOARD, tutorialInstruction, 200);
				else
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_8, tutorialInstruction, 200);
				tutorialHighlight.top = -1;
			}
		}
		break;
	case 12: // Select unit and convert
		{
			if((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0)) && pCurUnit != nullptr && pCurUnit->GetUnitType() == UNIT_INFANTRY && pCurUnit->GetFaction() == OWNER_PLANTS && m_pSelectedUnit == nullptr)
			{
				m_pSelectedUnit = pCurUnit;
				m_nSelectedAction = CurrentCommand::NONE;
			}
			else if((pDI->KeyPressed(DIK_NUMPAD8) || pDI->JoystickButtonPressed(3, m_nCurrFaction)) && m_pSelectedUnit != nullptr && m_pSelectedUnit->GetUnitType() == UNIT_INFANTRY && m_nSelectedAction == CurrentCommand::NONE)
			{
				m_nSelectedAction = CurrentCommand::CONVERT;
				dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->ShowConvertRange(gameMap);
				if(CSGD_DirectInput::GetInstance()->JoystickIsUnplugged(0) == true)
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_9_KEYBOARD, tutorialInstruction, 200);
				else
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_9, tutorialInstruction, 200);
			}
			else if(((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0)) || (pDI->KeyPressed(DIK_NUMPAD8) || pDI->JoystickButtonPressed(3, m_nCurrFaction))) && m_pSelectedUnit != nullptr && pCurTile->GetPosX() == m_pSelectedUnit->GetPosX()
				&& pCurTile->GetPosY() == m_pSelectedUnit->GetPosY() && m_nSelectedAction == CurrentCommand::CONVERT)
			{
				tutorialStep++;
				//dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->ConvertTile(pCurTile, gameMap);
				dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->SetTargetTile(pCurTile);
				dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->SetCurrentAnimStamp(
					dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->GetUnitConvertStamps());
				dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->SetConvertingTile(true);
				Deselect();
				LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_8_5, tutorialInstruction, 200);
			}
		}
		break;
	case 13: // Note Stat change
		{
			tutorialHighlight.top = 100 + OffsetY();
			tutorialHighlight.left = -25 + OffsetX();
			tutorialHighlight.bottom = tutorialHighlight.top + 40;
			tutorialHighlight.right = tutorialHighlight.left + 120;

			if((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0)))
			{
				tutorialStep++;
				//dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->ConvertTile(pCurTile, gameMap);
				LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_10, tutorialInstruction, 200);
				tutorialHighlight.top = -1;
			}
		}
		break;
	case 14: // Create Tank
		{
			if(m_pSelectedUnit != nullptr && m_pSelectedUnit->GetUnitType() == UNIT_BASE && ((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0)) || (pDI->KeyPressed(DIK_NUMPAD8) || pDI->JoystickButtonPressed(3, m_nCurrFaction))) && pCurTile->GetPosX() == m_pSelectedUnit->GetPosX() + gameMap.GetTileWidth() && pCurTile->GetPosY() == m_pSelectedUnit->GetPosY())
			{
				m_pMS->QueueMessage(new CCreateEntityMessage(UnitType::UNIT_TANK, m_pSelectedUnit->GetFaction(), pCurTile));
				m_pSelectedUnit->SetUsedAction(true);
				Deselect();
				tutorialStep++;
				if(CSGD_DirectInput::GetInstance()->JoystickIsUnplugged(0) == true)
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_12_KEYBOARD, tutorialInstruction, 200);
				else
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_12, tutorialInstruction, 200);
			}
			else if((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0)) && pCurUnit != nullptr && pCurUnit->GetUnitType() == UnitType::UNIT_BASE && pCurUnit->GetFaction() == OWNER_PLANTS)
			{
				m_pSelectedUnit = pCurUnit;
				m_nSelectedAction = CREATE;
			}
			else if((pDI->KeyPressed(DIK_NUMPAD8) || pDI->JoystickButtonPressed(3, m_nCurrFaction)) && m_pSelectedUnit != nullptr && m_pSelectedUnit->GetUnitType() == UNIT_BASE)
			{
				dynamic_cast<CStaticEntity*>(m_pSelectedUnit)->ShowCreateRange(gameMap);
				m_nSelectedAction = CurrentCommand::CREATE_TANK;
			}
		}
		break;
	case 15: // Move tank to volcano
		{
			if(m_nCurrFaction != OWNER_PLANTS)
				EndTurn();

			tutorialHighlight.top = 120 + OffsetY();
			tutorialHighlight.left = 119 + OffsetX();
			tutorialHighlight.bottom = tutorialHighlight.top + 80;
			tutorialHighlight.right = tutorialHighlight.left + 80;

			if(m_nSelectedAction == CurrentCommand::MOVE && m_pSelectedUnit != nullptr && (pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0))
				&& pCurTile->GetPosX() == m_pSelectedUnit->GetPosX() + gameMap.GetTileWidth() && pCurTile->GetPosY() == m_pSelectedUnit->GetPosY())
			{
				dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->MoveToTile(pCurTile, gameMap);
				Deselect();
				tutorialStep++;
				LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_14, tutorialInstruction, 200);
			}
			else if((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0)) && m_pSelectedUnit != nullptr)
			{
				m_nSelectedAction = CurrentCommand::MOVE;
				dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->ShowMoveRange(gameMap);
			}
			else if(pCurUnit != nullptr && (pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0)) && pCurUnit->GetUnitType() == UNIT_TANK && m_pSelectedUnit == nullptr)
			{
				m_pSelectedUnit = pCurUnit;
				m_nSelectedAction = CurrentCommand::NONE;
			}
		}
		break;
	case 16: // Convert Volcano
		{
			tutorialHighlight.top = 120 + OffsetY();
			tutorialHighlight.left = 183 + OffsetX();
			tutorialHighlight.bottom = tutorialHighlight.top + 80;
			tutorialHighlight.right = tutorialHighlight.left + 80;

			if((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0)) && pCurUnit != nullptr && pCurUnit->GetUnitType() == UNIT_TANK && pCurUnit->GetFaction() == OWNER_PLANTS)
			{
				m_pSelectedUnit = pCurUnit;
				m_nSelectedAction = CurrentCommand::NONE;
			}
			else if((pDI->KeyPressed(DIK_NUMPAD8) || pDI->JoystickButtonPressed(3, m_nCurrFaction)) && m_pSelectedUnit != nullptr && m_pSelectedUnit->GetUnitType() == UNIT_TANK && m_nSelectedAction == CurrentCommand::NONE)
			{
				m_nSelectedAction = CurrentCommand::CONVERT;
				dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->ShowConvertRange(gameMap);
			}
			else if(((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0)) || (pDI->KeyPressed(DIK_NUMPAD8) || pDI->JoystickButtonPressed(3, m_nCurrFaction))) && pCurTile->GetPosX() == m_pSelectedUnit->GetPosX() + gameMap.GetTileWidth()
				&& pCurTile->GetPosY() == m_pSelectedUnit->GetPosY() && m_nSelectedAction == CurrentCommand::CONVERT)
			{
				tutorialStep++;
				dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->ConvertTile(pCurTile, gameMap);
				Deselect();
				LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_15, tutorialInstruction, 200);
			}
		}
		break;
	case 17: // Move infantry to water
		{
			tutorialHighlight.top = 50 + OffsetY();
			tutorialHighlight.left = 183 + OffsetX();
			tutorialHighlight.bottom = tutorialHighlight.top + 80;
			tutorialHighlight.right = tutorialHighlight.left + 80;

			if(m_nSelectedAction == CurrentCommand::MOVE && m_pSelectedUnit != nullptr && (pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0))
				&& pCurTile->GetPosX() == m_pSelectedUnit->GetPosX() + gameMap.GetTileWidth()*3 && pCurTile->GetPosY() == m_pSelectedUnit->GetPosY())
			{
				dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->MoveToTile(pCurTile, gameMap);
				Deselect();
				tutorialStep++;
				tutorialHighlight.top = -1;
				LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_16, tutorialInstruction, 200);
			}
			else if((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0, OWNER_PLANTS)) && m_pSelectedUnit != nullptr)
			{
				m_nSelectedAction = CurrentCommand::MOVE;
				dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->ShowMoveRange(gameMap);
			}
			else if(pCurUnit != nullptr && (pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0, OWNER_PLANTS)) && pCurUnit->GetUnitType() == UNIT_INFANTRY && m_pSelectedUnit == nullptr)
			{
				m_pSelectedUnit = pCurUnit;
				m_nSelectedAction = CurrentCommand::NONE;
			}
		}
		break;
	case 18: // Convert Mountain
		{
			if((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0, OWNER_PLANTS)) && pCurUnit != nullptr && pCurUnit->GetUnitType() == UNIT_INFANTRY && pCurUnit->GetFaction() == OWNER_PLANTS && !pCurUnit->GetUsedAction())
			{
				m_pSelectedUnit = pCurUnit;
				m_nSelectedAction = CurrentCommand::NONE;
			}
			else if((pDI->KeyPressed(DIK_NUMPAD8) || pDI->JoystickButtonPressed(3, OWNER_PLANTS)) && m_pSelectedUnit != nullptr && m_pSelectedUnit->GetUnitType() == UNIT_INFANTRY && m_nSelectedAction == CurrentCommand::NONE)
			{
				m_nSelectedAction = CurrentCommand::CONVERT;
				dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->ShowConvertRange(gameMap);
			}
			else if(((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0)) || (pDI->KeyPressed(DIK_NUMPAD8) || pDI->JoystickButtonPressed(3, OWNER_PLANTS))) && m_pSelectedUnit != nullptr && pCurTile->GetPosX() == m_pSelectedUnit->GetPosX()
				&& pCurTile->GetPosY() == m_pSelectedUnit->GetPosY() + gameMap.GetTileHeight() && m_nSelectedAction == CurrentCommand::CONVERT)
			{
				tutorialStep++;
				dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->ConvertTile(pCurTile, gameMap);
				Deselect();
				if(CSGD_DirectInput::GetInstance()->JoystickIsUnplugged(0) == true)
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_17_KEYBOARD, tutorialInstruction, 200);
				else
					LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_17, tutorialInstruction, 200);
			}
		}
		break;
	case 19: 
		{
			if((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0, OWNER_PLANTS)))
			{
				tutorialStep = 23;
				LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_18, tutorialInstruction, 200);
			}
		}
		break;
	case 20: // Move tank down one
		{
			tutorialHighlight.top = 184 + OffsetY();
			tutorialHighlight.left = 183 + OffsetX();
			tutorialHighlight.bottom = tutorialHighlight.top + 80;
			tutorialHighlight.right = tutorialHighlight.left + 80;

			if(m_nSelectedAction == CurrentCommand::MOVE && m_pSelectedUnit != nullptr && (pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0, OWNER_PLANTS))
				&& pCurTile->GetPosX() == m_pSelectedUnit->GetPosX()&& pCurTile->GetPosY() == m_pSelectedUnit->GetPosY() + gameMap.GetTileHeight() )
			{
				dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->MoveToTile(pCurTile, gameMap);
				Deselect();
				tutorialStep++;
			}
			else if((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0, OWNER_PLANTS)) && m_pSelectedUnit != nullptr)
			{
				m_nSelectedAction = CurrentCommand::MOVE;
				dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->ShowMoveRange(gameMap);
			}
			else if(pCurUnit != nullptr && (pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0, OWNER_PLANTS)) && pCurUnit->GetUnitType() == UNIT_TANK && m_pSelectedUnit == nullptr  && !pCurUnit->GetUsedAction())
			{
				m_pSelectedUnit = pCurUnit;
				m_nSelectedAction = CurrentCommand::NONE;
			}
		}
		break;
	case 21: // Convert castle
		{
			tutorialHighlight.top = 184 + OffsetY();
			tutorialHighlight.left = 183 + OffsetX();
			tutorialHighlight.bottom = tutorialHighlight.top + 80;
			tutorialHighlight.right = tutorialHighlight.left + 80;

			if(((pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0, OWNER_PLANTS))) && pCurUnit != nullptr && pCurUnit->GetUnitType() == UNIT_TANK && pCurUnit->GetFaction() == OWNER_PLANTS  && !pCurUnit->GetUsedAction()) 
			{
				m_pSelectedUnit = pCurUnit;
				m_nSelectedAction = CurrentCommand::NONE;
			}
			else if((pDI->KeyPressed(DIK_NUMPAD8) || pDI->JoystickButtonPressed(3, OWNER_PLANTS)) && m_pSelectedUnit != nullptr && m_pSelectedUnit->GetUnitType() == UNIT_TANK && m_nSelectedAction == CurrentCommand::NONE)
			{
				m_nSelectedAction = CurrentCommand::CONVERT;
				dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->ShowConvertRange(gameMap);
			}
			else if((pDI->KeyPressed(DIK_NUMPAD2) || (pDI->KeyPressed(DIK_NUMPAD8) || pDI->JoystickButtonPressed(3, OWNER_PLANTS))) && m_pSelectedUnit != nullptr && pCurTile->GetPosX() == m_pSelectedUnit->GetPosX() + gameMap.GetTileWidth()
				&& pCurTile->GetPosY() == m_pSelectedUnit->GetPosY() && m_nSelectedAction == CurrentCommand::CONVERT)
			{
				tutorialStep++;
				tutorialStep++;
				tutorialHighlight.top = -1;
				dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->ConvertTile(pCurTile, gameMap);
				pCurTile->GetOccupyingUnit()->SetFaction(OWNER_PLANTS);
				Deselect();
				LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TUTORIAL_STEP_20, tutorialInstruction, 200);
			}
		}
		break;
	case 23: // Convert castle
		{
			return true;
		}
		break;
	default:
		{
			if(CGame::GetInstance()->GetCurrentLevel() == JUST_TUTORIAL)
			{
				CGame::GetInstance()->SetCurrentLevel(CurrentLevel::NO_LEVEL);
				CGame::GetInstance()->ChangeState( CMainMenuState::GetInstance() );
				Deselect();
				return true;
			}
			else
			{
				inTutorial = false;
				m_nAP = 5;

				m_fTurnTimer = m_fSecsPerTurn;
				m_fTurnTimer = 60.0f;
				m_nCoin = CoinStatus::UNFLIPPED;
				m_nStatus = GameplayStatus::COINFLIP;

				float fX = gameMap.GetTileWidth() * gameMap.GetMapWidth() / 2 + 32 - CGame::GetInstance()->GetScreenWidth() / 2.0f;
				float fY = gameMap.GetTileHeight()* gameMap.GetMapHeight() / 2 + 32 - CGame::GetInstance()->GetScreenHeight() / 2.0f;
				PanTo(fX, fY);
				Deselect();
				AIEntity::GetInstance()->SetAiBases()->clear();
				AIEntity::GetInstance()->SetAiUnits()->clear();
				m_pEM->RemoveAll();

				string temp = "Level1";
				gameMap.LoadMap(temp);
			}
		}
		break;
	}

	return false;
}

void CGameplayState::Deselect()
{
	if (m_pSelectedUnit != nullptr)
	{
		if (m_pSelectedUnit->GetUnitType() != UNIT_BASE)
		{
			if (dynamic_cast<CMovingEntity*>(m_pSelectedUnit)->GetConvertingTile() == false)
				gameMap.Unhighlight();
		}
	}
	//gameMap.Unhighlight();
	m_pSelectedUnit = nullptr;
	m_nSelectedAction = NONE;
	CGame::GetInstance()->SetCurrentUnit(nullptr);
}

//************************************************************
// Update
//	- update game entities
/*virtual*/ void CGameplayState::Update( float fElapsedTime )	
{
	if (m_nStatus == PLAYING)
	{
		m_nActiveUnits = 0;
		for (unsigned int i = 0; i < m_pEM->GetBucket(0).size(); ++i)
		{
			CEntity* useEntity = dynamic_cast<CEntity*>(m_pEM->GetBucket(0)[i]);
			if (useEntity->GetFaction() == m_nCurrFaction
				|| (useEntity->GetFaction() == OWNER_ROBOTS && m_nCurrFaction == OWNER_AI))
			{
				if (useEntity->GetUsedAction() == false)
					++m_nActiveUnits;
			}
		}
	}

	if(CGame::GetInstance()->IsPaused() && m_nStatus == PLAYING)
		m_nStatus = PAUSED;

	if (m_nCoin == FLIPPING)
	{
		if (m_fCoinFlipTimer <= 2.0 && m_fCoinFlipTimer > 1.0f)
			coinPos += int(500 * fElapsedTime);
		else if (m_fCoinFlipTimer <= 1.0f && m_fCoinFlipTimer > 0.0f)
			coinPos -= int(500 * fElapsedTime);
	}

	if (m_nStatus == PLAYING && !m_bCamLocked)
	{
		CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

		if((pDI->KeyDown(DIK_DOWN) || pDI->JoystickGetRStickDirDown(DIR_DOWN, m_nCurrFaction)) && m_fCamY <= (gameMap.GetMapHeight() - 1) * 64)
			m_fCamY += int(500 * fElapsedTime);
		else if((pDI->KeyDown(DIK_UP) || pDI->JoystickGetRStickDirDown(DIR_UP, m_nCurrFaction)) && m_fCamY >= -CGame::GetInstance()->GetScreenHeight() + 64)
			m_fCamY -= int(500 * fElapsedTime);

		if ((pDI->KeyDown(DIK_LEFT) || pDI->JoystickGetRStickDirDown(DIR_LEFT, m_nCurrFaction)) && m_fCamX >= -CGame::GetInstance()->GetScreenWidth() + 64)
			m_fCamX -= int(500 * fElapsedTime);
		else if ((pDI->KeyDown(DIK_RIGHT) || pDI->JoystickGetRStickDirDown(DIR_RIGHT, m_nCurrFaction)) && m_fCamX <= (gameMap.GetMapWidth() - 1) * 64)
			m_fCamX += int(500 * fElapsedTime);
	}

	if(m_nStatus == END_TURN || m_nStatus == ENDROUND )
	{
		if(turnChangePosX < (CGame::GetInstance()->GetScreenWidth() * 0.50f) - 235)
			turnChangePosX += int(500 * fElapsedTime);
	}

	if(m_nStatus == PLAYING /*&& m_bCoinFlipped == true*/)
	{
		m_fTurnTimer -= fElapsedTime;
		(OWNER_PLANTS == m_nCurrFaction ? m_fPlantsTime : m_fRobotsTime) += fElapsedTime;
		m_fTotalTime += fElapsedTime;

		if(m_fTurnTimer <= 10.0f && hasBeenWarned == false)
		{
			hasBeenWarned = true;
			m_pXA->SFXPlaySound(timerLowSFX);
		}

	}

	if (m_bAPDec == true)
		m_aAPExplosion.Update(fElapsedTime);

	AIEntity::GetInstance()->AiTimerUpdate(m_nRobotUnits);

	pCurTile = gameMap.GetTile(m_nCursorX, m_nCursorY);
	pCurUnit = pCurTile->GetOccupyingUnit();

	gameMap.Update(fElapsedTime);

	// Update the Entity Manager
	m_pEM->UpdateAll( fElapsedTime );
	if (m_nCurrFaction == OWNER_AI
		&& m_nStatus != ENDROUND)
		AIEntity::GetInstance()->UpdateStates(fElapsedTime);
	//m_pEM->UpdateAll( fElapsedTime );
	AnimationSystem::GetInstance()->Update(fElapsedTime);

	//////////////////////////////////////////////////////////
	// BUG FIX: 53|84
	// Fixed By: Kenneth Moy
	//////////////////////////////////////////////////////////
	// Begin Fix
	if (m_nCurrFaction == OWNER_AI
		&& m_nStatus != ENDROUND
		&& m_nStatus == PLAYING)
	{
		m_fAiUpdateTimer -= fElapsedTime;

		if (m_fAiUpdateTimer <= 0)
		{
			AIEntity::GetInstance()->AiChooseActions();
			m_fAiUpdateTimer = AIEntity::GetInstance()->GetUpdateAiTimer();
		}
	}
	// End Fix

	// Process the events & messages

	m_pMS->ProcessMessages();
	m_pES->ProcessEvents();

	// Update the FPS 
	m_fFPSTimer += fElapsedTime;
	m_unFrames++;

	if( m_fFPSTimer >= 1.0f )		// 1 second
	{
		m_fFPSTimer = 0.0f;
		m_unFPS		= m_unFrames;
		m_unFrames	= 0;
	}

	//Update the camera if it's scrolling somewhere
	if (m_bCamLocked)
	{
		if (m_nCamStartX == m_nCamDestX && m_nCamStartY == m_nCamDestY)
			m_bCamLocked = false;

		if (m_fCamTimer >= 0.5f)
		{
			m_bCamLocked = false;
			m_fCamTimer = 0.5f;
		}
		else
			m_fCamTimer += fElapsedTime;

		m_fCamX = Lerp((float)m_nCamStartX, (float)m_nCamDestX, 2 * m_fCamTimer);
		m_fCamY = Lerp((float)m_nCamStartY, (float)m_nCamDestY, 2 * m_fCamTimer);


	}

	m_nPlantTiles = gameMap.GetPlantTileCount();
	m_nNeutralTiles = gameMap.GetNeutralTileCount();
	m_nRobotTiles = gameMap.GetRobotTileCount();

	if (m_nMode != DEATHMATCH)
	{
		if ((float)m_nPlantTiles / (float)gameMap.GetTileCount() >= .65f)
		{
			m_nStatus = OVER;
			m_nWinner = OWNER_PLANTS;
		}
		else if ((float)m_nRobotTiles / (float)gameMap.GetTileCount() >= .65f)
		{
			m_nStatus = OVER;
			m_nWinner = OWNER_ROBOTS;
		}
	}

	if(m_fTurnTimer <= 0.0f || m_nActiveUnits <= 0 || m_nAP <= 0)
		EndTurn();

	hud.Update(fElapsedTime);

	if (m_nStatus == ENDROUND)
	{

	}

	if(m_nStatus == OVER)
	{
		m_fEndGameTimer -= fElapsedTime;
		if (m_fEndGameTimer <= 0.0f)
		{
			m_fEndGameTimer = 3.0f;


			CGame::GetInstance()->PushState(CGameOverState::GetInstance());
		}
	}

	if (FLIPPING == m_nCoin)
	{
		m_fCoinFlipTimer -= fElapsedTime;

		if (m_fCoinFlipTimer > 0.0f)
			m_aCoinFlip.Update(fElapsedTime);
		else if (m_fCoinFlipTimer <= 0.0f)
		{
			int winner = rand()%2;
			if (winner == 0)
			{
				m_aCoinFlip.SetCurrentFrame(0);
				m_nCoin = HEADS;
			}
			else if (winner == 1)
			{
				m_aCoinFlip.SetCurrentFrame(4);
				m_nCoin = TAILS;
			}
			/*m_bWinnerChosen = true;*/
		}
	}

	if(m_nPlantUnits <= 0)
	{
		m_nWinner = OWNER_ROBOTS;
		m_nStatus = OVER;
	}
	else if (m_nRobotUnits <= 0)
	{
		m_nWinner = OWNER_PLANTS;
		m_nStatus = OVER;
	}
	if(m_nCoin == TAILS && CGame::GetInstance()->GetGameType() == SINGLE_PLAYER)
	{
		m_fCoinAiTimer -= fElapsedTime;
		if (m_fCoinAiTimer <= 0.0f)
		{
			int choice = rand()%2;
			if (choice == 0)
			{
				m_nFirstTurn = OWNER_PLANTS;
				m_nCurrFaction = m_nFirstTurn;
			}
			else if (choice == 1)
			{
				m_nFirstTurn = OWNER_AI;
				m_nCurrFaction = m_nFirstTurn;
			}
			m_nCoin = FLIPPED;
			m_bCoinFlipped = true;
			m_fCoinAiTimer = 1.0f;
			m_nStatus = END_TURN;
		}
	}

	//////////////////////////////////////////////////////////
	// BUG FIX: 57|84
	// Fixed By: Kenneth Moy
	//////////////////////////////////////////////////////////
	// Begin Fix
	if (m_nRoundsStat != 0
		&& CGame::GetInstance()->GetCurrentLevel() != JUST_TUTORIAL
		&& CGame::GetInstance()->GetCurrentLevel() != TUTORIAL
		&& CGame::GetInstance()->GetGameType() == SINGLE_PLAYER)
	{
		if (AIEntity::GetInstance()->GetAiUnits().size() == 0
			&& AIEntity::GetInstance()->GetAiBases().size() == 0)
		{
			m_nWinner = OWNER_PLANTS;
			m_nStatus = OVER;
		}
		else if (AIEntity::GetInstance()->GetPlayerUnits().size() == 0
			&& AIEntity::GetInstance()->GetPlayerBases().size() == 0)
		{
			m_nWinner = OWNER_PLANTS;
			m_nStatus = OVER;
		}
	}
	// End Fix
	m_bEndTurn = false;

}

//************************************************************
// Render
//	- render the game entities
/*virtual*/ void CGameplayState::Render( void )	
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	CBMFont* pBF = CGame::GetInstance()->GetFont();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	D3DCOLOR color = D3DCOLOR_XRGB(
		(int)((float)(m_nRobotTiles) / gameMap.GetTileCount() * 255) + 89, 
		(int)((float)(m_nPlantTiles) / gameMap.GetTileCount() * 255) + 89, 
		(int)((float)(m_nRobotTiles) / gameMap.GetTileCount() * 255) + 89
		);


	CSGD_Direct3D::GetInstance()->Clear(color);

	CGame* pG = CGame::GetInstance();

	if(storyTime == true)
	{
		pTM->Draw(storyBoard, 0, 0);
		pBF->Write(storyTimeString, int(CGame::GetInstance()->GetScreenWidth()*0.25f), int(CGame::GetInstance()->GetScreenHeight()*0.25f), 1.5f, D3DCOLOR_ARGB(255, 255, 0, 0));
		pBF->Write(continueConfirm, int(CGame::GetInstance()->GetScreenWidth()*0.30f), int(CGame::GetInstance()->GetScreenHeight()*0.75f), 1.0f, D3DCOLOR_ARGB(255, 255, 0, 0));
		return;
	}

	int nCursorPosX = m_nCursorX * 64 + OffsetX();
	int nCursorPosY = m_nCursorY * 64 + OffsetY();

	RECT rCursor = {nCursorPosX, nCursorPosY, nCursorPosX + 64, nCursorPosY + 64};
	DWORD dwCursor;

	switch (m_nSelectedAction)
	{
	case NONE:
		dwCursor = D3DCOLOR_XRGB(255, 255, 255);
		break;
	case ATTACK:
		dwCursor = D3DCOLOR_XRGB(0, 0, 255);
		break;
	case MOVE:
		dwCursor = D3DCOLOR_XRGB(0, 255, 0);	
		break;
	case CREATE:
		dwCursor = D3DCOLOR_XRGB(255, 255, 0);
		break;
	case CONVERT:
		dwCursor = D3DCOLOR_XRGB(255, 255, 0);
		break;
	case CREATE_SCOUT:
	case CREATE_INFANTRY:
	case CREATE_TANK:
		dwCursor = D3DCOLOR_XRGB(0, 255, 255);
		break;
	}

	// Render the entity manager
	gameMap.Render();
	m_pEM->RenderAll();

	if (m_pSelectedUnit)
	{
		int nSelectedPosX = m_pSelectedUnit->GetCurrTile()->GetPosX() + OffsetX();
		int nSelectedPosY = m_pSelectedUnit->GetCurrTile()->GetPosY() + OffsetY();
		RECT rSelected = {nSelectedPosX, nSelectedPosY, nSelectedPosX + 64, nSelectedPosY+ 64};

		pD3D->DrawRect(rSelected, D3DCOLOR_ARGB(128, 255, 255, 255));
	}

	pD3D->DrawHollowRect(rCursor, dwCursor);

	hud.Render();

	if(inTutorial == true)
	{
		pTM->Draw(tutorialBox, int(CGame::GetInstance()->GetScreenWidth()*0.39f), int(CGame::GetInstance()->GetScreenHeight()*0.67f), 1.05f, 1.0f, nullptr, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(175, 0, 0, 255));
		pBF->Write(tutorialInstruction, int(CGame::GetInstance()->GetScreenWidth()*0.39f), int(CGame::GetInstance()->GetScreenHeight()*0.67f), 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255));

		if(tutorialHighlight.top != -1)
			pD3D->DrawHollowRect(tutorialHighlight, D3DCOLOR_ARGB(255, 255, 255, 0), 5);
	}

	if (m_nStatus == COINFLIP
		&& m_nCoin != FLIPPED)
	{
		pTM->Draw(turnChange, (int)(pG->GetScreenWidth() * 0.50 - 234), (int)(pG->GetScreenHeight() * 0.7f), 1, 1.25f, nullptr, 0, 0, 0, D3DCOLOR_ARGB(200, 255, 255, 255));

		if (m_nCoin == UNFLIPPED) 
			pBF->Write(coinString, (int)(pG->GetScreenWidth() * 0.50f - 100), (int)(pG->GetScreenHeight() * 0.7f + 140), 1.0, D3DCOLOR_XRGB(0, 200, 0));
		else if (HEADS == m_nCoin)
		{
			m_aCoinFlip.SetCurrentFrame(0);
			pBF->Write("X for Plants\nB for Robots to go first", (int)(pG->GetScreenWidth() * 0.50f - 150), (int)(pG->GetScreenHeight() * 0.7f + 120), 1.0, D3DCOLOR_XRGB(0, 200, 0));
		}
		else if (TAILS == m_nCoin)
		{
			m_aCoinFlip.SetCurrentFrame(4);
			pBF->Write("X for Plants\nB for Robots to go first", (int)(pG->GetScreenWidth() * 0.50f - 150), (int)(pG->GetScreenHeight() * 0.7f + 120), 1.0, D3DCOLOR_XRGB(0, 200, 0));
		}

		AnimationSystem::GetInstance()->Render(int(pG->GetScreenWidth()*0.5f - 100), int(((pG->GetScreenHeight()) * 0.666f) - (52 + coinPos)), m_aCoinFlip);
	}
	else if (m_nStatus == PAUSED)
	{
		pTM->Draw(turnChange, (int)(pG->GetScreenWidth() * 0.25), (int)(pG->GetScreenHeight() * 0.35), 1, 1.25f, nullptr, 0, 0, 0, D3DCOLOR_ARGB(128, 0, 0, 255));
		pBF->Write("PAUSED", (int)(pG->GetScreenWidth()*0.40), (int)(pG->GetScreenHeight()*0.40) - 25, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
		pBF->Write("Resume", (int)(pG->GetScreenWidth()*0.40), (int)(pG->GetScreenHeight()*0.40) + 25 * RESUME, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
		pBF->Write("Save/Load", (int)(pG->GetScreenWidth()*0.40), (int)(pG->GetScreenHeight()*0.40) + 25 * SAVE, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
		pBF->Write("Options", (int)(pG->GetScreenWidth()*0.40), (int)(pG->GetScreenHeight()*0.40) + 25 * OPTIONS, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
		pBF->Write("Exit to Menu", (int)(pG->GetScreenWidth()*0.40), (int)(pG->GetScreenHeight()*0.40) + 25 * QUITTOMENU, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
		pBF->Write("Exit Game", (int)(pG->GetScreenWidth()*0.40), (int)(pG->GetScreenHeight()*0.40) + 25 * QUITGAME, 1, D3DCOLOR_ARGB(255, 255, 255, 255));

		pBF->Write("-->", (int)(pG->GetScreenWidth()*0.40) - 50, (int)(pG->GetScreenHeight()*0.40) + 25 * m_nCursor, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else if (m_nStatus == OVER)
	{
		pTM->Draw(turnChange, pG->GetScreenWidth() / 2 - 100, pG->GetScreenHeight() / 2 - 50, 0.75f, 0.6f);
		pBF->Write((m_nWinner == OWNER_PLANTS ? "PLANTS WIN" : "ROBOTS WIN"), pG->GetScreenWidth() / 2 - 50, pG->GetScreenHeight() / 2 - 25, 1.5f, D3DCOLOR_XRGB(255, 0, 0));
	}

	//////////////////////////////////////////////////////////
	// BUG FIX: 21|84
	// Fixed By: Kenneth Moy
	//////////////////////////////////////////////////////////
	// Begin Fix
	if (m_bAPDec == true
		&& m_bEndTurn == false)
	{
		if (m_nCurrFaction == OWNER_PLANTS)
			AnimationSystem::GetInstance()->Render(int(CGame::GetInstance()->GetScreenWidth()*0.50f - 290) + 35 * (m_nAP), 20, m_aAPExplosion);
		else if (m_nCurrFaction == OWNER_AI || m_nCurrFaction == OWNER_ROBOTS)
			AnimationSystem::GetInstance()->Render(int(CGame::GetInstance()->GetScreenWidth()*0.50f - 20) + 35 * (m_nAP), 20, m_aAPExplosion);
	}
	// End Fix

	AnimationSystem::GetInstance()->Render();

	if (m_nStatus == ENDROUND)
	{
		pTM->Draw(turnChange, turnChangePosX, int(CGame::GetInstance()->GetScreenHeight() * 0.50f - 75));

		pBF->Write("End of  Round", turnChangePosX + 115, int(CGame::GetInstance()->GetScreenHeight() * 0.50f - 20), 1.5f, D3DCOLOR_ARGB(255, 255, 0, 0));
	}

	if(m_nStatus == END_TURN)
	{
		pTM->Draw(turnChange, turnChangePosX, int(CGame::GetInstance()->GetScreenHeight() * 0.50f - 75));

		D3DCOLOR color  = D3DCOLOR_ARGB(255, 255, 0, 255);

		if(m_nCurrFaction == OWNER_PLANTS)
			color = D3DCOLOR_ARGB(255, 0, 255, 0);

		pBF->Write(turnChangeString, turnChangePosX + 100, int(CGame::GetInstance()->GetScreenHeight() * 0.50f - 20), 1.5f, color);
	}
}

void CGameplayState::PanTo(float fX, float fY)
{
	m_fCamTimer = 0.0f;
	m_bCamLocked = true;

	m_nCamStartX = (int)m_fCamX;
	m_nCamStartY = (int)m_fCamY;

	m_nCamDestX = (int)fX;
	m_nCamDestY = (int)fY;
}

int CGameplayState::OffsetX(void)
{
	return (int)-m_fCamX;
}

int CGameplayState::OffsetY(void)
{
	return (int)-m_fCamY;
}

void CGameplayState::EndTurn(void)
{
	//float fX = gameMap.GetTileWidth() * gameMap.GetMapWidth() / 2 + 32 - CGame::GetInstance()->GetScreenWidth() / 2.0f;
	//float fY = gameMap.GetTileHeight()* gameMap.GetMapHeight() / 2 + 32 - CGame::GetInstance()->GetScreenHeight() / 2.0f;
	m_bEndTurn = true;
	float fY = 0.0f;
	float fX = 0.0f;
	if (m_nCurrFaction != OWNER_PLANTS)
	{
		for (unsigned int i = 0; i < m_pEM->GetBucket(0).size(); ++i)
		{

			if (dynamic_cast<CEntity*>(m_pEM->GetBucket(0)[i])->GetFaction() == OWNER_PLANTS)
				//&& dynamic_cast<CEntity*>(m_pEM->GetBucket(0)[i])->GetUnitType() != UNIT_BASE)
			{
				fX = (float)dynamic_cast<CEntity*>(m_pEM->GetBucket(0)[i])->GetPosX() + 32 - CGame::GetInstance()->GetScreenWidth() / 2.0f;
				fY = (float)dynamic_cast<CEntity*>(m_pEM->GetBucket(0)[i])->GetPosY() + 32 - CGame::GetInstance()->GetScreenHeight() / 2.0f;
				m_pSelectedUnit = dynamic_cast<CEntity*>(m_pEM->GetBucket(0)[i]);
				break;
			}
		}
	}
	else if (m_nCurrFaction == OWNER_PLANTS)
	{
		for (unsigned int i = 0; i < m_pEM->GetBucket(0).size(); ++i)
		{
			if (dynamic_cast<CEntity*>(m_pEM->GetBucket(0)[i])->GetFaction() != OWNER_PLANTS)
				//&& dynamic_cast<CEntity*>(m_pEM->GetBucket(0)[i])->GetUnitType() != UNIT_BASE)
			{
				fX = (float)dynamic_cast<CEntity*>(m_pEM->GetBucket(0)[i])->GetPosX() + 32 - CGame::GetInstance()->GetScreenWidth() / 2.0f;
				fY = (float)dynamic_cast<CEntity*>(m_pEM->GetBucket(0)[i])->GetPosY() + 32 - CGame::GetInstance()->GetScreenHeight() / 2.0f;
				m_pSelectedUnit = dynamic_cast<CEntity*>(m_pEM->GetBucket(0)[i]);
				break;
			}
		}
	}

	m_nCursorX = m_pSelectedUnit->GetPosX() / gameMap.GetTileWidth();
	m_nCursorY = m_pSelectedUnit->GetPosY() / gameMap.GetTileHeight();

	PanTo(fX, fY);
	Deselect();
	hasBeenWarned = false;
	m_nStatus = END_TURN;
	m_pXA->SFXPlaySound(turnEndSFX);

	if (m_nRoundsStat <= 0)
	{
		if (m_nFirstTurn == OWNER_PLANTS)
			LoadStringA(CGame::GetInstance()->GethInstance(), IDS_ROBOTS_TURN, turnChangeString, 20);
		else
			LoadStringA(CGame::GetInstance()->GethInstance(), IDS_PLANTS_TURN, turnChangeString, 20);
	}
	else if (m_nRoundsStat > 0)
	{
		if(m_nCurrFaction == OWNER_PLANTS)
			LoadStringA(CGame::GetInstance()->GethInstance(), IDS_ROBOTS_TURN, turnChangeString, 20);
		else
			LoadStringA(CGame::GetInstance()->GethInstance(), IDS_PLANTS_TURN, turnChangeString, 20);
	}

	for (int j = 0; j < gameMap.GetMapWidth(); j++)
	{
		for (int i = 0; i < gameMap.GetMapHeight(); i++)
		{	
			CEntity* pTemp = gameMap.GetTile(j, i)->GetOccupyingUnit();
			if (pTemp)
			{
				pTemp->SetUsedAction(false);

				if(dynamic_cast<CMovingEntity*>(pTemp) != nullptr)
				{
					dynamic_cast<CMovingEntity*>(pTemp)->SetHasMoved(false);
					if (dynamic_cast<CMovingEntity*>(pTemp)->GetJustCreated() == true)
					{
						dynamic_cast<CMovingEntity*>(pTemp)->SetJustCreated(false);
					}
				}
			}
		}
	}

	if (CGame::GetInstance()->GetGameType() != SINGLE_PLAYER)
	{
		m_nCurrFaction = (m_nCurrFaction == OWNER_PLANTS ? OWNER_ROBOTS : OWNER_PLANTS);
		m_nActiveUnits = (OWNER_PLANTS == m_nCurrFaction ? m_nPlantUnits : m_nRobotUnits);
	}
	else if (CGame::GetInstance()->GetGameType() == SINGLE_PLAYER)
	{
		m_nCurrFaction = (m_nCurrFaction == OWNER_PLANTS ? OWNER_AI : OWNER_PLANTS);
		m_nActiveUnits = (OWNER_PLANTS == m_nCurrFaction ? m_nPlantUnits : m_nRobotUnits);
	}

	m_nAP = 5;

	if (m_nCurrFaction == OWNER_PLANTS && .35 <= static_cast<float>(m_nPlantTiles) / gameMap.GetTileCount() )
		m_nAP++;
	else if ((m_nCurrFaction == OWNER_ROBOTS || m_nCurrFaction == OWNER_AI) && .35 <= static_cast<float>(m_nRobotTiles) / gameMap.GetTileCount())
		m_nAP++;

	if (m_nCurrFaction == m_nFirstTurn) //If a round (pair of turns) has ended
	{
		m_nRoundsStat++;
		convertedTiles = gameMap.ConvertAll();
		m_nStatus = ENDROUND;
		m_pXA->SFXPlaySound(roundEndSFX);
	}

	m_fTurnTimer = 60.0f;

	if (m_nMode == KOTH)
	{
		m_pES->SendEventNow("KOTHENDTURN");

		m_pMS->ProcessMessages();

		if (m_nKotHPlants > KOTHWINAMOUNT)
			m_nWinner = OWNER_PLANTS;
		else if (m_nKotHRobots > KOTHWINAMOUNT)
			m_nWinner = OWNER_ROBOTS;
	}
}

//************************************************************
// MessageProc
//	- called for each message queued in the Message System
//	- static method DOES NOT HAVE INVOKING OBJECT!!!
//		- must use the singleton object to access data members
//
//	- MSG_DESTROY_ENTITY	- remove the entity from Entity Manager
CEntity* CGameplayState::CreateMovingObject(CCreateEntityMessage* pMsg)
{
	CMovingEntity* pMovObj = new CMovingEntity();

	//Configure entity stats here based on info in pMsg
	switch (pMsg->GetUnitType())
	{
	case UNIT_INFANTRY:
		//TEMP
		pMovObj->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/InfantryImg.png")));
		pMovObj->SetUnitType(UNIT_INFANTRY);
		pMovObj->SetBaseHealth(8);
		pMovObj->SetCurrHealth(8);
		pMovObj->SetBaseDamage(2);
		pMovObj->SetCurrDamage(2);
		pMovObj->SetMoveRange(8);
		pMovObj->SetAttackRange(3);
		pMovObj->SetConvertRange(1);
		pMovObj->SetJustCreated(true);
		if (pMsg->GetTile() == nullptr)
		{
			pMovObj->SetX(pMsg->GetX()*gameMap.GetTileWidth());
			pMovObj->SetY(pMsg->GetY()*gameMap.GetTileHeight());
		}
		else if (pMsg->GetTile() != nullptr)
		{
			pMovObj->SetX(pMsg->GetTile()->GetPosX());
			pMovObj->SetY(pMsg->GetTile()->GetPosY());
		}
		if (m_nStatus == PLAYING && (CGame::GetInstance()->GetGameType() == MULTIPLAYER || m_nCurrFaction == OWNER_PLANTS))
			CGame::GetInstance()->GetProfile()->SetInfantry(CGame::GetInstance()->GetProfile()->GetInfantry() + 1);
		break;
	case UNIT_SCOUT:
		pMovObj->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/ScoutImg.png")));
		pMovObj->SetUnitType(UNIT_SCOUT);
		pMovObj->SetBaseHealth(3);
		pMovObj->SetCurrHealth(3);
		pMovObj->SetBaseDamage(1);
		pMovObj->SetCurrDamage(1);
		pMovObj->SetMoveRange(12);
		pMovObj->SetAttackRange(5);
		pMovObj->SetConvertRange(1);
		pMovObj->SetJustCreated(true);
		if (pMsg->GetTile() == nullptr)
		{
			pMovObj->SetX(pMsg->GetX()*gameMap.GetTileWidth());
			pMovObj->SetY(pMsg->GetY()*gameMap.GetTileHeight());
		}
		else if (pMsg->GetTile() != nullptr)
		{
			pMovObj->SetX(pMsg->GetTile()->GetPosX());
			pMovObj->SetY(pMsg->GetTile()->GetPosY());
		}

		if (m_nStatus == PLAYING && (CGame::GetInstance()->GetGameType() == MULTIPLAYER || m_nCurrFaction == OWNER_PLANTS))
			CGame::GetInstance()->GetProfile()->SetScouts(CGame::GetInstance()->GetProfile()->GetScouts() + 1);
		break;
	case UNIT_TANK:
		pMovObj->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/TankImg.png")));
		pMovObj->SetUnitType(UNIT_TANK);
		pMovObj->SetBaseHealth(12);
		pMovObj->SetCurrHealth(12);
		pMovObj->SetBaseDamage(4);
		pMovObj->SetCurrDamage(4);
		pMovObj->SetMoveRange(6);
		pMovObj->SetAttackRange(2);
		pMovObj->SetConvertRange(1);
		pMovObj->SetJustCreated(true);
		if (pMsg->GetTile() == nullptr)
		{
			pMovObj->SetX(pMsg->GetX()*gameMap.GetTileWidth());
			pMovObj->SetY(pMsg->GetY()*gameMap.GetTileHeight());
		}
		else if (pMsg->GetTile() != nullptr)
		{
			pMovObj->SetX(pMsg->GetTile()->GetPosX());
			pMovObj->SetY(pMsg->GetTile()->GetPosY());
		}

		if (m_nStatus == PLAYING && (CGame::GetInstance()->GetGameType() == MULTIPLAYER || m_nCurrFaction == OWNER_PLANTS))
			CGame::GetInstance()->GetProfile()->SetTanks(CGame::GetInstance()->GetProfile()->GetTanks() + 1);
		break;
	}
	pMovObj->SetHasMoved(pMsg->GetMoved());
	pMovObj->SetUsedAction(pMsg->GetActed());

	return pMovObj;
}

CEntity* CGameplayState::CreateStaticObject(CCreateEntityMessage* pMsg)
{
	CStaticEntity* pEnt = new CStaticEntity;
	pEnt->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/BaseImg.png")));
	pEnt->SetX(pMsg->GetX()*gameMap.GetTileWidth());
	pEnt->SetY(pMsg->GetY()*gameMap.GetTileHeight());
	pEnt->SetTile(pMsg->GetTile());
	pEnt->SetBaseHealth(10);
	pEnt->SetCurrHealth(10);
	pEnt->SetUsedAction(pMsg->GetActed());

	return pEnt;
}

/*static*/ void CGameplayState::MessageProc( IMessage* pMsg )
{
	CGame* pG = CGame::GetInstance();
	CGameplayState* pGS = CGameplayState::GetInstance();
	// What type of message?
	switch( pMsg->GetMessageID() )
	{
	case MSG_KOTH:
		{
			OwnerFaction senderFaction = static_cast<CKOTHMessage*>(pMsg)->GetFaction();
			if (senderFaction == OWNER_PLANTS)
				pGS->m_nKotHPlants++;
			else if (senderFaction == OWNER_ROBOTS)
				pGS->m_nKotHRobots++;
		}
		break;
#pragma region UNIT_MOVE_MESSAGE
	case UNIT_MOVE:
		{
			CUnitMoveMessage* pMoveMsg = static_cast<CUnitMoveMessage*>(pMsg);

			if (dynamic_cast<CMovingEntity*>(pMoveMsg->GetEntity())->MoveToTile(
				pMoveMsg->GetTile(), pGS->GetCurrentMap()) == true)
				pMoveMsg->GetPrevious()->SetOccupyingUnit(nullptr);

			dynamic_cast<CMovingEntity*>(pMoveMsg->GetEntity())->SetMoving(false);
			if (pMoveMsg->GetEntity()->GetFaction() == OWNER_AI)
				AIEntity::GetInstance()->AiChooseActions(pMoveMsg->GetEntity());
		}
		break;
#pragma endregion
#pragma region UNIT_CONVERT MESSAGE
	case UNIT_CONVERT:
		{
			CUnitConvertTileMessage* pConvertMsg = static_cast<CUnitConvertTileMessage*>(pMsg);

			if(pConvertMsg->GetTile()->GetTileType() == WATER
				|| pConvertMsg->GetTile()->GetTileType() == SWAMP
				|| pConvertMsg->GetTile()->GetTileType() == SLUDGE)
			{
				dynamic_cast<CMovingEntity*>(pConvertMsg->GetEntity())->SetConvertingTile(false);
				CGameplayState::GetInstance()->GetGameMap()->Unhighlight();
				return;
			}

			dynamic_cast<CMovingEntity*>(pConvertMsg->GetEntity())->ConvertTile(
				pConvertMsg->GetTile(),
				CGameplayState::GetInstance()->GetCurrentMap());

			if(pConvertMsg->GetTile()->GetTileType() == TileType::GRASS || pConvertMsg->GetTile()->GetTileType() == TileType::NEUTRAL || pConvertMsg->GetTile()->GetTileType() == TileType::NANO)
				CGameplayState::GetInstance()->m_pXA->SFXPlaySound(CGameplayState::GetInstance()->convertNormalSFX);
			else if(pConvertMsg->GetTile()->GetTileType() == TileType::FOREST || pConvertMsg->GetTile()->GetTileType() == TileType::MOUNTAIN || pConvertMsg->GetTile()->GetTileType() == TileType::VOLCANO)
				CGameplayState::GetInstance()->m_pXA->SFXPlaySound(CGameplayState::GetInstance()->convertSpecialSFX);

			dynamic_cast<CMovingEntity*>(pConvertMsg->GetEntity())->SetConvertingTile(false);
		}
		break;
#pragma endregion
#pragma region MSG_CREATE_ENTITY
	case MSG_CREATE_ENTITY:
		{
			//Downcast so we can access the UnitType and CreatorType

			CCreateEntityMessage* pCreateMsg = static_cast<CCreateEntityMessage*>(pMsg);
			CEntity* pEnt = nullptr; //Created unit;

			switch (pCreateMsg->GetUnitType()) //This switch will select the factory method to call;
			{
			case UNIT_BASE:
				pEnt = pGS->CreateStaticObject(pCreateMsg);
				break;
			case UNIT_SCOUT:
			case UNIT_INFANTRY:
			case UNIT_TANK:
				pEnt = pGS->CreateMovingObject(pCreateMsg);
				break;
			default:
				OutputDebugString(_T("Invalid unit creation type"));
				break;
			}

			//////////////////////////////////////////////////////////
			// BUG FIX: 54|84, 58|84
			// Fixed By: Kenneth Moy
			//////////////////////////////////////////////////////////
			// Begin Fix
			if (CGameplayState::GetInstance()->GetUnitCreating() == true)
			{
				pEnt->SetUsedAction(true);
			}
			else if (CGameplayState::GetInstance()->GetUnitCreating() == false)
			{
				pEnt->SetUsedAction(false);
			}
			CGameplayState::GetInstance()->SetUnitCreating(false);
			// End Fix

			//If the unit is created successfully, add it to the EntityManager and get rid of local reference
			if (pEnt)
			{
				CTile* pTile = nullptr;
				if (pCreateMsg->GetTile() == nullptr)
					pTile = pGS->gameMap.GetTile(pCreateMsg->GetX(), pCreateMsg->GetY());
				else if (pCreateMsg->GetTile() != nullptr)
					pTile = pCreateMsg->GetTile();

				pEnt->SetFaction(pCreateMsg->GetUnitOwner());

				(pEnt->GetFaction() == OWNER_PLANTS ? pGS->m_nPlantUnits : pGS->m_nRobotUnits)++;


				if (pEnt->GetFaction() == OWNER_AI)
				{
					pEnt->SetCurrentAiState(NORMAL_AI);
				}

				CSGD_XAudio2* pXA = CSGD_XAudio2::GetInstance();

				if (pEnt->GetType() != UNIT_BASE)
				{
					switch(pEnt->GetFaction())
					{
#pragma region PLANTS
					case OWNER_PLANTS:
						{
							// Set Attack Animation
							dynamic_cast<CMovingEntity*>(pEnt)->GetUnitAttackStamp()->SetCurrentAnimation("Plant Attack");
							//Set Idle Render Animation
							switch(pEnt->GetUnitType())
							{
							case UNIT_INFANTRY:
								{
									dynamic_cast<CEntity*>(pEnt)->GetIdleAnimationStamp()->SetCurrentAnimation("P Infantry Idle");
									dynamic_cast<CMovingEntity*>(pEnt)->GetUnitConvertStamp()->SetCurrentAnimation("P Infantry Convert");
									dynamic_cast<CMovingEntity*>(pEnt)->GetUnitMoveStamp()->SetCurrentAnimation("P Infantry Move");
								}
								break;
							case UNIT_SCOUT:
								{
									dynamic_cast<CEntity*>(pEnt)->GetIdleAnimationStamp()->SetCurrentAnimation("P Scout Idle");
									dynamic_cast<CMovingEntity*>(pEnt)->GetUnitConvertStamp()->SetCurrentAnimation("P Scout Convert");
									dynamic_cast<CMovingEntity*>(pEnt)->GetUnitMoveStamp()->SetCurrentAnimation("P Scout Move");
								}
								break;
							case UNIT_TANK:
								{
									dynamic_cast<CEntity*>(pEnt)->GetIdleAnimationStamp()->SetCurrentAnimation("P Tank Idle");
									dynamic_cast<CMovingEntity*>(pEnt)->GetUnitConvertStamp()->SetCurrentAnimation("P Tank Convert");
									dynamic_cast<CMovingEntity*>(pEnt)->GetUnitMoveStamp()->SetCurrentAnimation("P Tank Move");
								}
								break;
							default:
								break;
							}
							dynamic_cast<CMovingEntity*>(pEnt)->GetUnitAttackStamp()->SetCurrentAnimation("Plant Attack");
							dynamic_cast<CMovingEntity*>(pEnt)->SetAttackSFX(pXA->SFXLoadSound(_T("Resources/Sounds/PlantAttack.wav")));
							dynamic_cast<CMovingEntity*>(pEnt)->SetDeathSFX(pXA->SFXLoadSound(_T("Resources/Sounds/PlantDeath.wav")));
							dynamic_cast<CMovingEntity*>(pEnt)->SetMoveSFX(pXA->SFXLoadSound(_T("Resources/Sounds/PlantMove.wav")));
							dynamic_cast<CMovingEntity*>(pEnt)->SetSelectedSFX(pXA->SFXLoadSound(_T("Resources/Sounds/PlantSelect.wav")));
							dynamic_cast<CMovingEntity*>(pEnt)->SetInjuredSFX(pXA->SFXLoadSound(_T("Resources/Sounds/PlantInjured.wav")));
						}
						break;
#pragma endregion
#pragma region ROBOTS
					case OWNER_AI:
					case OWNER_ROBOTS:
						{
							// Set Attack Animation
							dynamic_cast<CMovingEntity*>(pEnt)->GetUnitAttackStamp()->SetCurrentAnimation("Robot Attack");
							// Set Idle Render Animation
							switch(pEnt->GetUnitType())
							{
							case UNIT_INFANTRY:
								{
									dynamic_cast<CEntity*>(pEnt)->GetIdleAnimationStamp()->SetCurrentAnimation("R Infantry Idle");
									dynamic_cast<CMovingEntity*>(pEnt)->GetUnitConvertStamp()->SetCurrentAnimation("R Infantry Convert");
									dynamic_cast<CMovingEntity*>(pEnt)->GetUnitMoveStamp()->SetCurrentAnimation("R Infantry Move");
								}
								break;
							case UNIT_SCOUT:
								{
									dynamic_cast<CEntity*>(pEnt)->GetIdleAnimationStamp()->SetCurrentAnimation("R Scout Idle");
									dynamic_cast<CMovingEntity*>(pEnt)->GetUnitConvertStamp()->SetCurrentAnimation("R Scout Convert");
									dynamic_cast<CMovingEntity*>(pEnt)->GetUnitMoveStamp()->SetCurrentAnimation("R Scout Move");
								}
								break;
							case UNIT_TANK:
								{
									dynamic_cast<CEntity*>(pEnt)->GetIdleAnimationStamp()->SetCurrentAnimation("R Tank Idle");
									dynamic_cast<CMovingEntity*>(pEnt)->GetUnitConvertStamp()->SetCurrentAnimation("R Tank Convert");
									dynamic_cast<CMovingEntity*>(pEnt)->GetUnitMoveStamp()->SetCurrentAnimation("R Tank Move");
								}
								break;
							default:
								break;
							}
							dynamic_cast<CMovingEntity*>(pEnt)->SetAttackSFX(pXA->SFXLoadSound(_T("Resources/Sounds/RobotAttack.wav")));
							dynamic_cast<CMovingEntity*>(pEnt)->SetDeathSFX(pXA->SFXLoadSound(_T("Resources/Sounds/RobotDeath.wav")));
							dynamic_cast<CMovingEntity*>(pEnt)->SetMoveSFX(pXA->SFXLoadSound(_T("Resources/Sounds/RobotMove.wav")));
							dynamic_cast<CMovingEntity*>(pEnt)->SetSelectedSFX(pXA->SFXLoadSound(_T("Resources/Sounds/RobotSelect.wav")));
							dynamic_cast<CMovingEntity*>(pEnt)->SetInjuredSFX(pXA->SFXLoadSound(_T("Resources/Sounds/RobotInjured.wav")));
						}
						break;
					default:
						break;
#pragma endregion
					}
				}
				else if (pEnt->GetUnitType() == UNIT_BASE)
				{
					switch(pEnt->GetFaction())
					{
					case OWNER_PLANTS:
						{
							//Set Idle Render Animation
							dynamic_cast<CEntity*>(pEnt)->GetIdleAnimationStamp()->SetCurrentAnimation("P Base Idle");
							dynamic_cast<CStaticEntity*>(pEnt)->SetCreateSFX(pXA->SFXLoadSound(_T("Resources/Sounds/PlantCreate.wav")));
							dynamic_cast<CEntity*>(pEnt)->SetCurrentAnimStamp(*pEnt->GetIdleAnimationStamp());
						}
						break;
					case OWNER_AI:
					case OWNER_ROBOTS:
						{
							// Set Idle Render Animation
							dynamic_cast<CEntity*>(pEnt)->GetIdleAnimationStamp()->SetCurrentAnimation("R Base Idle");
							dynamic_cast<CStaticEntity*>(pEnt)->SetCreateSFX(pXA->SFXLoadSound(_T("Resources/Sounds/RobotCreate.wav")));
							dynamic_cast<CEntity*>(pEnt)->SetCurrentAnimStamp(*pEnt->GetIdleAnimationStamp());
						}
						break;
					case OWNER_NEUTRAL:
						//Set Idle Render Animation
						dynamic_cast<CEntity*>(pEnt)->GetIdleAnimationStamp()->SetCurrentAnimation("N Base Idle");
						//dynamic_cast<CStaticEntity*>(pEnt)->SetCreateSFX(pXA->SFXLoadSound(_T("Resources/Sounds/PlantCreate.wav")));
						dynamic_cast<CEntity*>(pEnt)->SetCurrentAnimStamp(*pEnt->GetIdleAnimationStamp());
						break;
					default:
						break;
					}
				}
				if (pTile != nullptr)
					pEnt->SetTile(pTile); //Place the unit into the game map

				pTile->SetOccupyingUnit(pEnt);

				pGS->m_pEM->AddEntity(pEnt, 0);
				pEnt->Release();
				pEnt = nullptr;
				CGameplayState::GetInstance()->GetGameMap()->Unhighlight();
			}
		}
		break;
#pragma endregion
#pragma region MSG_DESTROY_ENTITY
	case MSG_DESTROY_ENTITY:
		{
			CDestroyEntityMessage* pDestroyMsg = static_cast<CDestroyEntityMessage*>(pMsg);
			(pDestroyMsg->GetSender()->GetFaction() == OWNER_PLANTS ? pGS->m_nPlantUnits : pGS->m_nRobotUnits)--;

			if (pG->GetGameType() == MULTIPLAYER || (pG->GetGameType() == SINGLE_PLAYER && pGS->m_nCurrFaction == OWNER_PLANTS))
				pG->GetProfile()->SetKills(pG->GetProfile()->GetKills() + 1);

			pDestroyMsg->GetSender()->GetCurrTile()->SetOccupyingUnit(nullptr);

			if(pGS->pCurUnit = pDestroyMsg->GetSender())
				pGS->pCurUnit = nullptr;

			if(pGS->m_pSelectedUnit = pDestroyMsg->GetSender())
				pGS->m_pSelectedUnit = nullptr;

			for(unsigned int i = 0; i < AnimationSystem::GetInstance()->GetStampsToRender().size(); ++i)
			{
				if (AnimationSystem::GetInstance()->GetStampsToRender()[i] == pDestroyMsg->GetSender()->GetDeathAnimation())
					AnimationSystem::GetInstance()->RemoveAnimTimeStamp(i);
			}

			pGS->m_pEM->RemoveEntity(pDestroyMsg->GetSender(), 0);

			if (pGS->m_nMode != DOMINATION)
			{
				if (!pGS->m_nPlantUnits)
				{
					pGS->m_nWinner = OWNER_ROBOTS;
					pGS->m_nStatus = OVER;
				}
				else if (!pGS->m_nRobotUnits)
				{
					pGS->m_nWinner = OWNER_PLANTS;
					pGS->m_nStatus = OVER;
				}
			}

			if (!pGS->m_nRobotUnits)
			{
				pGS->m_nWinner = OWNER_PLANTS;
				pGS->m_nStatus = OVER;
			}

		}
		break;
#pragma endregion
	default:
		OutputDebugString( _T("CGame::MessageProc - unknown message ID \n") );
		break;
	}
}

bool CGameplayState::SaveXml(const char* szFilename)
{
	//Set up the structure of the document
	TiXmlDocument doc;
	doc.LinkEndChild(new TiXmlDeclaration("1.0", "utf-8", ""));

	TiXmlElement* pRoot = new TiXmlElement("save_data");
	doc.LinkEndChild(pRoot);

	TiXmlElement* pTiles = new TiXmlElement("tile_data");
	pRoot->LinkEndChild(pTiles);

	TiXmlElement* pUnits = new TiXmlElement("unit_data");
	pRoot->LinkEndChild(pUnits);

	//General attributes for the map 
	pRoot->SetAttribute("name", gameMap.GetName());
	pRoot->SetAttribute("gfx", gameMap.GetTileSetPath().c_str());
	pRoot->SetAttribute("width", gameMap.GetMapWidth());
	pRoot->SetAttribute("height",gameMap.GetMapHeight());
	pRoot->SetAttribute("tile_width", gameMap.GetTileWidth());
	pRoot->SetAttribute("tile_height", gameMap.GetTileHeight());
	pRoot->SetAttribute("curr_player", m_nCurrFaction);
	pRoot->SetAttribute("ap", m_nAP);
	pRoot->SetAttribute("mode", m_nMode);
	pRoot->SetAttribute("players", CGame::GetInstance()->GetGameType());

	for (int i = 0; i < gameMap.GetTileCount(); i++)
	{
		CTile* pCurTile = const_cast<CTile*>(gameMap[i]);
		TiXmlElement* pTile = new TiXmlElement("tile");

		//Set attributes here
		pTile->SetAttribute("type", pCurTile->GetTileType());
		pTile->SetAttribute("event", pCurTile->GetTileEvent().c_str());
		pTile->SetAttribute("posX", pCurTile->GetPosX());
		pTile->SetAttribute("posY", pCurTile->GetPosY());
		pTile->SetAttribute("srcX", pCurTile->GetSrcRect().left);
		pTile->SetAttribute("srcY", pCurTile->GetSrcRect().top);

		pTiles->LinkEndChild(pTile);

		//If there's a unit on that tile, put it in an element and push it into pUnits
		if (pCurTile->GetOccupyingUnit())
		{
			CEntity* pCurUnit = pCurTile->GetOccupyingUnit();
			TiXmlElement* pUnit = new TiXmlElement("unit");

			//Attributes go here
			pUnit->SetAttribute("type", pCurUnit->GetType());

			if (CGame::GetInstance()->GetGameType() == SINGLE_PLAYER && pCurUnit->GetFaction() == OWNER_AI)
			{
				pCurUnit->SetFaction(OWNER_ROBOTS);
			}

			pUnit->SetAttribute("faction", pCurUnit->GetFaction());
			pUnit->SetAttribute("x", pCurTile->GetPosX() / gameMap.GetTileWidth());
			pUnit->SetAttribute("y", pCurTile->GetPosY() / gameMap.GetTileHeight());
			pUnit->SetAttribute("hp",pCurUnit->GetCurrHealth());
			pUnit->SetAttribute("moved", static_cast<CMovingEntity*>(pCurUnit)->GetMoved());
			pUnit->SetAttribute("acted", pCurUnit->GetUsedAction());
			pUnits->LinkEndChild(pUnit);
		}
	}

	TiXmlElement* pTileSet = new TiXmlElement("tileset");
	pTileSet->SetAttribute("width", 3);
	pTileSet->SetAttribute("height", 3);

	pRoot->LinkEndChild(pTileSet);

	std::map<TileType, RECT> tempTileset = gameMap.GetTileSet();

	for (unsigned int i = 0; i < tempTileset.size(); i++)
	{
		TiXmlElement* pTile = new TiXmlElement("tileinfo");
		RECT tempRect = tempTileset[static_cast<TileType>(i)];

		pTile->SetAttribute("type", i);
		pTile->SetAttribute("left", tempRect.left);
		pTile->SetAttribute("top", tempRect.top);
		pTileSet->LinkEndChild(pTile);
	}

	std::ostringstream stringstream;
	char path[MAX_PATH];
	LPWSTR wszPath = NULL;
	size_t   size;

	HRESULT hr = SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, 0, &wszPath);

	wcstombs_s(&size,path,MAX_PATH,wszPath,MAX_PATH);

	// Convert char types
	if(hr == S_OK)
		stringstream << path;

	std::string pathtowrite = stringstream.str();

	// Add the company and game information
	pathtowrite += "\\AllInGames\\PvR\\";

	SHCreateDirectoryExA(NULL, pathtowrite.c_str(), 0);

	pathtowrite += szFilename;

	pathtowrite.c_str();

	if (!doc.SaveFile(pathtowrite.c_str()))
		return false;

	return CProfileState::GetInstance()->SaveProfiles();
}

bool CGameplayState::LoadXml(const char* szFilename)
{
	TOSTRINGSTREAM toss;
	TiXmlDocument doc;

	if (!doc.LoadFile(szFilename))
		return false;

	TiXmlElement* pRoot = doc.RootElement();

	if (!pRoot)
		return false;

	m_pEM->RemoveAll();



	TiXmlElement* pData = pRoot->FirstChildElement("save_data");

	string tempStr = pRoot->Attribute("name");
	gameMap.SetName(tempStr.c_str());

	tempStr = pRoot->Attribute("gfx");
	gameMap.SetTileSetName(tempStr.c_str());

	toss << tempStr.c_str();
	tileSetImgID = CSGD_TextureManager::GetInstance()->LoadTexture(toss.str().c_str());

	int tempInt = 0, nWidth, nHeight;

	pRoot->Attribute("width", &tempInt);
	gameMap.SetWidth(tempInt);
	pRoot->Attribute("height", &tempInt);
	gameMap.SetHeight(tempInt);

	pRoot->Attribute("tile_width", &nWidth);
	gameMap.SetTileWidth(nWidth);
	pRoot->Attribute("tile_height", &nHeight);
	gameMap.SetTileHeight(nHeight);

	pRoot->Attribute("ap", &m_nAP);
	pRoot->Attribute("mode", &tempInt);
	m_nMode = static_cast<GameMode>(tempInt);

	pRoot->Attribute("curr_player", &tempInt);
	m_nCurrFaction = static_cast<OwnerFaction>(tempInt);

	pRoot->Attribute("players", &tempInt);
	CGame::GetInstance()->SetGameType(static_cast<GameType>(tempInt));

	TiXmlElement* pTile = pRoot->FirstChildElement("tile_data")->FirstChildElement("tile");
	int nType, nX, nY;
	vector<CTile> vTiles;
	CTile tempTile;
	RECT tempRect;

	while (pTile)
	{
		pTile->Attribute("type", &nType);
		tempTile.SetTileType((TileType)nType);

		if (nType == FOREST || nType == GRASS || nType == SWAMP)
			tempTile.SetFactionType(OWNER_PLANTS);
		else if (nType == VOLCANO || nType == NANO || nType == SLUDGE)
			tempTile.SetFactionType(OWNER_ROBOTS);
		else
			tempTile.SetFactionType(OWNER_NEUTRAL);

		tempStr = pTile->Attribute("event");
		tempTile.SetTileEvent(tempStr);

		pTile->Attribute("posX", &nX);
		pTile->Attribute("posY", &nY);
		tempTile.SetPosX(nX);
		tempTile.SetPosY(nY);

		//Source rect
		pTile->Attribute("srcX", (int*)&tempRect.left);
		pTile->Attribute("srcY", (int*)&tempRect.top);
		tempRect.right = tempRect.left + gameMap.GetTileWidth();
		tempRect.bottom = tempRect.top + gameMap.GetTileHeight();

		tempTile.SetSrcRect(tempRect);
		vTiles.push_back(tempTile);

		pTile = pTile->NextSiblingElement("tile");
	}

	vTiles.shrink_to_fit();

	gameMap.SetTiles(vTiles);

	TiXmlElement* pUnit = pRoot->FirstChildElement("unit_data")->FirstChildElement("unit");
	CCreateEntityMessage* pMsg = nullptr;
	int nFaction, nHP;
	int tempActed, tempMoved;

	while (pUnit)
	{
		pUnit->Attribute("type", &nType);
		pUnit->Attribute("faction", &nFaction);
		pUnit->Attribute("x", &nX);
		pUnit->Attribute("y", &nY);
		pUnit->Attribute("hp",&nHP);
		pUnit->Attribute("moved", &tempMoved);
		pUnit->Attribute("acted", &tempActed);

		bool bMoved = (tempMoved == 0 ? false : true);
		bool bActed = (tempActed == 0 ? false : true);
		m_pMS->QueueMessage(new CCreateEntityMessage((UnitType)nType, (OwnerFaction)nFaction, nX, nY, nHP, bMoved, bActed));
		pUnit = pUnit->NextSiblingElement("unit");
	}

	TileSet tempTileset;

	TiXmlElement* pTileset = pRoot->FirstChildElement("tileset");
	TiXmlElement* pSrcRect = pTileset->FirstChildElement("tileinfo");

	RECT srcRect;
	int tempLeft, tempTop;

	for (unsigned int i = 0; i < 9; i++)
	{
		pSrcRect->Attribute("left", &tempLeft);
		pSrcRect->Attribute("top", &tempTop);

		srcRect.left = static_cast<LONG>(tempLeft);
		srcRect.right = srcRect.left + gameMap.GetTileWidth();
		srcRect.top = static_cast<LONG>(tempTop);
		srcRect.bottom = srcRect.top + gameMap.GetTileHeight();

		tempTileset[static_cast<TileType>(i)] = srcRect;

		pSrcRect = pSrcRect->NextSiblingElement("tileinfo");
	}
	m_pMS->ProcessMessages();
	gameMap.SetTileSet(tempTileset);

	gameMap.Update(0);
	m_nStatus = PLAYING;

	return false;
}

void CGameplayState::DecrementAP(void)
{
	if (m_nAP > 1)
		m_bAPDec = true;
	--m_nAP;
}

void CGameplayState::IncreaseAP(void)
{
	++m_nAP;
}

void CGameplayState::SetFirstTurn (OwnerFaction nFaction)
{
	if (OWNER_PLANTS == nFaction || OWNER_ROBOTS == nFaction)
	{
		m_nFirstTurn = nFaction;
		m_nCurrFaction = m_nFirstTurn;
		m_nStatus = PLAYING;
	}
	else
		m_nStatus = COINFLIP;
}