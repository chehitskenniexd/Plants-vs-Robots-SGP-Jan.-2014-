#include "MultiPState.h"
#include "GameplayState.h"

#include "../MainInfo/Game.h"
#include "../MainInfo/BMFont.h"

#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/SGD_String.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//#define MENU_ITEMS_X 100 
//#define MENU_ITEMS_Y 100

//Height of each line
#define MENU_ITEMS_YOFF 25 

//Back the cursor off from the X by these many pixels
#define CURSOR_XOFF 50 


/*static*/ CMultiPState* CMultiPState::GetInstance(void)
{
	static CMultiPState s_Instance;
	return &s_Instance;
}

CMultiPState::CMultiPState(void)
{
	selectionChangedSFX = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/SelectionChange.wav"));
	selectionMadeSFX = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/SelectionMade.wav"));
}


CMultiPState::~CMultiPState(void)
{
}

/*virtual*/ void CMultiPState::Enter ( void ) /*override*/
{
	MENU_ITEMS_X = int(CGame::GetInstance()->GetScreenWidth() * .29f);
	MENU_ITEMS_Y = int( CGame::GetInstance()->GetScreenWidth() * 0.15f);

	BG = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/EmptyMenu.png"));
	indicatorIMG = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/PlantAP.png"));

	HINSTANCE hInst = CGame::GetInstance()->GethInstance();
	m_nCursor = 0;
	m_nTimer = 60;
	turnOrderOption = 0;
	m_nSelectedMap = 0;

	m_bP1Ready = false;
	m_bP2Ready = false;

	LoadStringA(hInst, IDS_MPSETUP, szTitleString, 32);

	LoadStringA(hInst, IDS_REGULARMODE, szRegularString, 32);
	vChoices.push_back(szRegularString);

	LoadStringA(hInst, IDS_DM, szDeathmatchString, 32);
	vChoices.push_back(szDeathmatchString);

	LoadStringA(hInst, IDS_DOMINATION, szDominationString, 32);
	vChoices.push_back(szDominationString);

	LoadStringA(hInst, IDS_KOTH, szKotHString, 32);
	vChoices.push_back(szKotHString);

	/*LoadStringA(hInst, IDS_TOW, szToWString, 32);
	vChoices.push_back(szToWString);*/

	LoadStringA(hInst, IDS_MAPSELECTOR, szMapString, 32);

	for (unsigned int i = 0; i < 4; i++)
		LoadStringA(hInst, IDS_MAP1 + i, szMapNames[i], 32);

	LoadStringA(hInst, IDS_USECOINFLIP, szFirstTurnString, 32);
	LoadStringA(hInst, IDS_PLANTS, szPlantString, 32);
	LoadStringA(hInst, IDS_ROBOTS, szRobotString, 32);
	LoadStringA(hInst, IDS_COINFLIP, szCoinFlipString, 32);
	

	LoadStringA(hInst, IDS_TIMER, szTimerString, 32);

	LoadStringA(hInst, IDS_EXIT, szExitString, 32);
}

/*virtual*/ void CMultiPState::Exit	( void ) /*override*/
{				 
	vChoices.clear();
}				 
				 
/*virtual*/ bool CMultiPState::Input ( void ) /*override*/
{
	CSGD_TextureManager::GetInstance()->Draw(BG, 0, 0);

	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
	CGameplayState* pGS = CGameplayState::GetInstance();

	if (pDI->KeyPressed(DIK_ESCAPE) || pDI->JoystickButtonPressed(1) || pDI->KeyPressed(DIK_NUMPAD6))
		CGame::GetInstance()->PopState();

	else if ((pDI->KeyPressed(DIK_RETURN) || pDI->JoystickButtonPressed(0)))
	{
		if (m_nCursor < 4)
		{
			CurrentLevel nSelectedLevel;

			switch (m_nSelectedMap)
			{
			case 0:
				nSelectedLevel = MPLEVEL1;
				break;
			case 1:
				nSelectedLevel = MPLEVEL2;
				break;
			case 2:
				nSelectedLevel = MPLEVEL3;
				break;
			case 3:
				nSelectedLevel = MPLEVEL4;
				break;
			}

			CGame::GetInstance()->SetCurrentLevel(nSelectedLevel);

			CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionMadeSFX);
			CGameplayState::GetInstance()->SetTurnTimer(static_cast<float>(m_nTimer));
			CGameplayState::GetInstance()->SetMode(static_cast<GameMode>(m_nCursor));

			CGame::GetInstance()->ChangeState(CGameplayState::GetInstance());

			switch (turnOrderOption)
			{
			case PLANTS:
				pGS->SetFirstTurn(OWNER_PLANTS);
				break;
			case ROBOTS:
				pGS->SetFirstTurn(OWNER_ROBOTS);
				break;
			case FLIP:
				pGS->SetFirstTurn(OWNER_NEUTRAL);
				break;
			}
		}
		else if (m_nCursor == 7)
		{
			CGame::GetInstance()->PopState();
			return true;
		}
	}

	if ((pDI->KeyPressed(DIK_DOWN) || pDI->JoystickDPadPressed(DIR_DOWN)) && m_nCursor < static_cast<int>(vChoices.size() + 3))
	{
		CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionChangedSFX);
		m_nCursor++;
	}
	else if ((pDI->KeyPressed(DIK_UP) || pDI->JoystickDPadPressed(DIR_UP)) && m_nCursor > 0)
	{
		CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionChangedSFX);
		m_nCursor--;
	}

	if (pDI->JoystickButtonPressed(0, 0))
		m_bP1Ready = true;
	if (pDI->JoystickButtonPressed(0, 1))
		m_bP2Ready = true;
	if (pDI->JoystickButtonPressed(1, 0))
		m_bP1Ready = false;
	if (pDI->JoystickButtonPressed(1, 1))
		m_bP2Ready = false;

	ScrollHorizField(m_nSelectedMap, vChoices.size(), 0, 3);
	ScrollHorizField(turnOrderOption, vChoices.size() + 1, 0, 2);
	ScrollHorizField(m_nTimer, vChoices.size() + 2, 10, INT_MAX, 10);

	return true;
}		

void CMultiPState::ScrollHorizField(int& field, const int cursorVertPos, const int min, const int max, const int interval)
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	if (cursorVertPos == m_nCursor)
	{
		if (field > min && (pDI->KeyPressed(DIK_LEFT) || pDI->JoystickDPadPressed(DIR_LEFT)))
		{
			CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionChangedSFX);
			field -= interval;
		}
		else if (field < max && (pDI->KeyPressed(DIK_RIGHT) || pDI->JoystickDPadPressed(DIR_RIGHT)))
		{
			CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionChangedSFX);
			field += interval;
		}
	}
}

/*virtual*/ void CMultiPState::Update ( float fElapsedTime ) /*override*/
{				 
	if (m_bP1Ready && m_bP2Ready)
	{
		CGame::GetInstance()->SetGameType(MULTIPLAYER);
		CGameplayState::GetInstance()->SetTurnTimer(static_cast<float>(m_nTimer));
		CGameplayState::GetInstance()->SetMode(static_cast<GameMode>(m_nCursor));
		CGame::GetInstance()->ChangeState(CGameplayState::GetInstance());
	}
}				 

/*virtual*/ void CMultiPState::Render ( void ) /*override*/
{
	DWORD dwColor = D3DCOLOR_XRGB(12, 104, 0);
	CBMFont* pF = CGame::GetInstance()->GetFont();

	CSGD_TextureManager::GetInstance()->Draw(BG, 0, 0);


	ostringstream oss;

	pF->Write(oss.str().c_str(), 0, 0, 1.0, dwColor);
	CSGD_TextureManager::GetInstance()->Draw(indicatorIMG, MENU_ITEMS_X - CURSOR_XOFF, MENU_ITEMS_Y + m_nCursor * MENU_ITEMS_YOFF);

	oss.str("");

	for (unsigned int i = 0; i < vChoices.size(); i++)
	{
		pF->Write(vChoices[i], MENU_ITEMS_X, MENU_ITEMS_Y + i * MENU_ITEMS_YOFF, 1.0, dwColor);
	}
	
	oss << "< " << szMapString << szMapNames[m_nSelectedMap] << " >";
	pF->Write(oss.str().c_str(), MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * vChoices.size(), 1.0f, dwColor);
	oss.str("");

	oss << "< " << szFirstTurnString;

	switch (turnOrderOption)
	{
	case PLANTS:
		oss << szPlantString;
		break;
	case ROBOTS:
		oss << szRobotString;
		break;
	case FLIP:
		oss << szCoinFlipString;
		break;
	}

	oss << " >";

	pF->Write(oss.str().c_str(), MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * (vChoices.size() + 1), 1.0f, dwColor);
	oss.str("");

	oss << "< " << szTimerString << m_nTimer << " >";
	pF->Write(oss.str().c_str(), MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * (vChoices.size() + 2), 1.0f, dwColor);
	pF->Write(szExitString, MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * (vChoices.size() + 3), 1.0f, dwColor);

}