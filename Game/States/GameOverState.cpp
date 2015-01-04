#include "GameOverState.h"
#include "../MainInfo/Game.h"
#include "../MainInfo/BMFont.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../States/GameplayState.h"
#include "../States/MainMenuState.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../Systems/AnimationSystem.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"

#define MENU_ITEMS 4

/*static*/ CGameOverState* CGameOverState::GetInstance( void )
{
	static CGameOverState s_Instance;
	return &s_Instance;
}

CGameOverState::CGameOverState(void)
{
	//LoadStringA(CGame::GetInstance()->GethInstance(), IDS_MAINMENU, mainMenu, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_MAINMENU, mainMenuOption, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_EXITGAME, exitGameOption, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_PLANTSWIN, plantsWin, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_ROBOTSWIN, robotsWin, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_REPLAY, replayOption, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CONTINUE, continueOption, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CAMPAIGN1FINISH, campaign1, 50);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CAMPAIGNFINISH, campaign3, 50);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TIMEGAME, timeElapsed, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_ROUNDSGAME, roundsElapsed, 20);

	m_nCursorPos = 0;
	m_nScreenWidth = CGame::GetInstance()->GetScreenWidth();
	m_nScreenHeight = CGame::GetInstance()->GetScreenHeight();
}


CGameOverState::~CGameOverState(void)
{
}

/*virtual*/ void CGameOverState::Enter ( void ) /*override*/
{
	selectionChangedSFX = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/SelectionChange.wav"));
	selectionMadeSFX = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/SelectionMade.wav"));
	victorySFX = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/Victory.wav"));
	loseSFX = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/Lose.wav"));
	plantsWinImg = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/PVRWinScreen.png"));
	robotWinImg = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/PVRRobotsWinScreen.png"));

	switch (CGameplayState::GetInstance()->GetWinningFaction())
	{
	case OWNER_PLANTS:
		{
			CSGD_XAudio2::GetInstance()->SFXPlaySound(victorySFX);
		}
		break;
	case OWNER_ROBOTS:
		{
			CSGD_XAudio2::GetInstance()->SFXPlaySound(victorySFX);
		}
		break;
	default:
		CSGD_XAudio2::GetInstance()->SFXPlaySound(loseSFX);

		break;
	}

	float gameTimeElapsed = CGameplayState::GetInstance()->GetTotalGameTime();
	m_nHours = int(gameTimeElapsed / 3600);
	m_nMinutes = int((gameTimeElapsed - (m_nHours * 3600.0f))/60);
	m_nSeconds = int(gameTimeElapsed - (m_nMinutes * 60.0f));

	if (CGame::GetInstance()->GetCurrentLevel() == CAMPAIGN_LEVEL3)
		m_nCursorPos = 1;
}

/*virtual*/ void CGameOverState::Exit	( void ) /*override*/
{				 
	CSGD_XAudio2::GetInstance()->SFXUnloadSound(selectionChangedSFX);
	CSGD_XAudio2::GetInstance()->SFXUnloadSound(selectionMadeSFX);
	CSGD_XAudio2::GetInstance()->SFXUnloadSound(victorySFX);
	CSGD_XAudio2::GetInstance()->SFXUnloadSound(loseSFX);
	CSGD_TextureManager::GetInstance()->UnloadTexture(plantsWinImg);
	CSGD_TextureManager::GetInstance()->UnloadTexture(robotWinImg);
}				 

/*virtual*/ bool CGameOverState::Input ( void ) /*override*/
{	
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	// Move the cursor
	if ((pDI->KeyPressed(DIK_DOWN) || pDI->JoystickDPadPressed(DIR_DOWN)) && m_nCursorPos < MENU_ITEMS - 1)
	{
		CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionChangedSFX);
		m_nCursorPos++;
	}
	else if ((pDI->KeyPressed(DIK_UP) || pDI->JoystickDPadPressed(DIR_UP)) && m_nCursorPos > 0)
	{
		CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionChangedSFX);
		if (CGame::GetInstance()->GetCurrentLevel() != CAMPAIGN_LEVEL3
			|| (CGame::GetInstance()->GetCurrentLevel() == CAMPAIGN_LEVEL3 && m_nCursorPos > 1))
			m_nCursorPos--;
	}

	if (pDI->KeyPressed(DIK_RETURN) || pDI->JoystickButtonPressed(0))
	{
		CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionMadeSFX);

		if(CSGD_XAudio2::GetInstance()->SFXIsSoundPlaying(victorySFX))
			CSGD_XAudio2::GetInstance()->SFXStopSound(victorySFX);
		else if(CSGD_XAudio2::GetInstance()->SFXIsSoundPlaying(loseSFX))
			CSGD_XAudio2::GetInstance()->SFXStopSound(loseSFX);

		switch(m_nCursorPos)
		{
		case CONTINUE_CAMPAIGN:
			if(CGame::GetInstance()->GetCurrentLevel() != CAMPAIGN_LEVEL3)
			{
				currOption = CONTINUE_CAMPAIGN;

				switch(CGame::GetInstance()->GetCurrentLevel())
				{
				case CurrentLevel::TUTORIAL:
					CGame::GetInstance()->SetCurrentLevel(CAMPAIGN_LEVEL1);
					break;
				case CurrentLevel::CAMPAIGN_LEVEL1:
					CGame::GetInstance()->SetCurrentLevel(CAMPAIGN_LEVEL2);
					break;
				case CurrentLevel::CAMPAIGN_LEVEL2:
					CGame::GetInstance()->SetCurrentLevel(CAMPAIGN_LEVEL3);
					break;
				case CurrentLevel::CAMPAIGN_LEVEL3:
					//CGame::GetInstance()->SetCurrentLevel(CAMPAIGN_LEVEL1);
					break;
				}

				CGameplayState::GetInstance()->Enter();
				CGame::GetInstance()->PopState();
			}
			break;
		case REPLAY_LEVEL:
			CGameplayState::GetInstance()->Enter();
			CGame::GetInstance()->PopState();
			break;
		case MAIN_MENU:
			{
				CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
			}
			break;
		case EXIT_GAME:
			return false;
			break;
		default:
			break;
		}
	}

	return true;
}				 

/*virtual*/ void CGameOverState::Update ( float fElapsedTime ) /*override*/
{				 
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_MAINMENU, mainMenuOption, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_EXITGAME, exitGameOption, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_PLANTSWIN, plantsWin, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_ROBOTSWIN, robotsWin, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_REPLAY, replayOption, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CONTINUE, continueOption, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CAMPAIGN1FINISH, campaign1, 50);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CAMPAIGN2FINISH, campaign2, 50);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CAMPAIGNFINISH, campaign3, 50);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TIMEGAME, timeElapsed, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_ROUNDSGAME, roundsElapsed, 20);
}				 

/*virtual*/ void CGameOverState::Render ( void ) /*override*/
{
	int m_nOptions = 0;
	CBMFont* pF = CGame::GetInstance()->GetFont();
	DWORD dwColor = D3DCOLOR_XRGB(255, 255, 255);


#pragma region Win Render
	switch (CGame::GetInstance()->GetGameType())
	{
	case SINGLE_PLAYER:
		{
			CSGD_TextureManager::GetInstance()->Draw(plantsWinImg, 0, 0, 1.0f, 1.0f);
			switch(CGame::GetInstance()->GetCurrentLevel())
			{
			case CAMPAIGN_LEVEL1:
				pF->Write(campaign1, m_nScreenWidth/2 - 55, m_nScreenHeight/3 - 50, 1.0f, dwColor);
				break;
			case CAMPAIGN_LEVEL2:
				pF->Write(campaign2, m_nScreenWidth/2 - 55, m_nScreenHeight/3 - 50, 1.0f, dwColor);
				break;
			case CAMPAIGN_LEVEL3:
				pF->Write(campaign3, m_nScreenWidth/2 - 55, m_nScreenHeight/3 - 50, 1.0f, dwColor);
				break;
			default:
				break;
			}
		}
		break;
	case MULTIPLAYER:
		{
			switch (CGameplayState::GetInstance()->GetWinningFaction())
			{
			case OWNER_PLANTS:
				{
					CSGD_TextureManager::GetInstance()->Draw(plantsWinImg, 0, 0, 1.0f, 1.0f);
					pF->Write(plantsWin, m_nScreenWidth/2 - 55, m_nScreenHeight/3 - 50, 1.0f, dwColor);
				}
				break;
			case OWNER_ROBOTS:
				{
					CSGD_TextureManager::GetInstance()->Draw(robotWinImg, 0, 0, 1.0f, 1.0f);
					pF->Write(robotsWin, m_nScreenWidth/2 - 55, m_nScreenHeight/3, 1.0f, dwColor);
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
#pragma endregion

#pragma region Stats Render
	char m_cRoundsElapsed[40];
	char m_cTimeElapsed[40];
	int j;
	j = sprintf_s(m_cTimeElapsed, 40, "%s", timeElapsed);
	j += sprintf_s(m_cTimeElapsed + j, 40 - j, "%dh: ", m_nHours);
	j += sprintf_s(m_cTimeElapsed + j, 40 - j, "%dm: ", m_nMinutes);
	j += sprintf_s(m_cTimeElapsed + j, 40 - j, "%ds", m_nSeconds);

	j = 0;
	j = sprintf_s(m_cRoundsElapsed, 40, "%s", roundsElapsed);
	j += sprintf_s(m_cRoundsElapsed + j, 40 - j, "%d", CGameplayState::GetInstance()->GetRoundsStat());

	pF->Write(m_cTimeElapsed, m_nScreenWidth/5, 0, 1.0f, dwColor);
	pF->Write(m_cRoundsElapsed, m_nScreenWidth/5, 50, 1.0f, dwColor);
#pragma endregion

#pragma region Options Render
	pF->Write("-->", m_nScreenWidth/2 - 50, m_nScreenHeight/3 - 100 + (m_nCursorPos*50), 1.0f, dwColor);

	if(CGame::GetInstance()->GetCurrentLevel() != CAMPAIGN_LEVEL3)
		pF->Write(continueOption, m_nScreenWidth/2 - 10, m_nScreenHeight/3 - 100, 1.0f, dwColor);

	pF->Write(replayOption, m_nScreenWidth/2 - 10, m_nScreenHeight/3 - 50, 1.0f, dwColor);
	pF->Write(mainMenuOption, m_nScreenWidth/2 - 10, m_nScreenHeight/3, 1.0f, dwColor);
	pF->Write(exitGameOption, m_nScreenWidth/2 - 10, m_nScreenHeight/3 + 50, 1.0f, dwColor);
#pragma endregion

}