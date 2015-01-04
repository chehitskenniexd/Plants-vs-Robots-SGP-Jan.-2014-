/*************************************************************
|	File:		MainMenuState.cpp
|	Purpose:	To handle the main menu
|
|	Author:		Doug Monroe (dmonroe@fullsail.com)
|	Course:		SGD 1312
*************************************************************/

#include "MainMenuState.h"

#include "../MainInfo/Game.h"
#include "../MainInfo/BMFont.h"

//States
#include "GameplayState.h"
#include "OptionState.h"
#include "InstructionState.h"
#include "CreditState.h"
#include "MultiPState.h"
#include "SaveState.h"
#include "../SGD Wrappers/SGD_String.h"
#include <time.h>

#include "../SGD Wrappers/CSGD_Direct3D.h"	// for color macro
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"

enum MAIN_MENU_CHOICES {NEW, LOAD, CONTINUE, MENU_MULTIPLAYER, HELP, MENU_TUTORIAL, OPTIONS, CREDITS, EXIT, MENU_ITEMS};

//************************************************************
// SINGLETON
/*static*/ CMainMenuState* CMainMenuState::GetInstance( void )
{
	static CMainMenuState s_Instance;
	return &s_Instance;
}


//************************************************************
// CONSTRUCTOR
//	- set all data members to clean values
CMainMenuState::CMainMenuState( void )
{
	m_nCursor = 0;
}

//************************************************************
// DESTRUCTOR
CMainMenuState::~CMainMenuState( void )
{
}


//************************************************************
// Enter
//	- load assets / initialize
//	- reset the state
/*virtual*/ void CMainMenuState::Enter( void )	/*override*/
{
	// Reset the cursor to the top of the menu
	//m_nCursor = 0;
	BG = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/EmptyMenu.png"));
	indicatorIMG = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/PlantAP.png"));

	selectionChangedSFX = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/SelectionChange.wav"));
	selectionMadeSFX = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/SelectionMade.wav"));

	MENU_ITEMS_X = int(CGame::GetInstance()->GetScreenWidth() * .29f);
	MENU_ITEMS_Y = int( CGame::GetInstance()->GetScreenWidth() * 0.15f);
	MENU_ITEMS_YOFF = 42; 
	CURSOR_XOFF = 50;
}

//************************************************************
// Exit
//	- unload assets
/*virtual*/ void CMainMenuState::Exit( void )	/*override*/
{
}

//************************************************************
// Input
//	- handle user input
/*virtual*/ bool CMainMenuState::Input( void )	/*override*/
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
	CGame* pG = CGame::GetInstance();

	// Press 'Escape' to quit
	if (pDI->KeyPressed( DIK_ESCAPE ) && m_nCursor == EXIT)
		return false;
	else if (pDI->KeyPressed( DIK_ESCAPE ))
		m_nCursor = EXIT;

	// Move the cursor
	if ((pDI->KeyPressed(DIK_DOWN) || pDI->JoystickDPadPressed(DIR_DOWN)) && m_nCursor < MENU_ITEMS - 1)
	{
		CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionChangedSFX);
		m_nCursor++;
	}
	else if ((pDI->KeyPressed(DIK_UP) || pDI->JoystickDPadPressed(DIR_UP)) && m_nCursor > 0)
	{
		CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionChangedSFX);
		m_nCursor--;
	}

	// Select an option?
	if(pDI->KeyPressed(DIK_RETURN) || pDI->JoystickButtonPressed(0))
	{
		CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionMadeSFX);
		// Which option is highlighted?
		switch(m_nCursor)
		{
		case NEW:		// play
			{
				// Switch to the Gameplay state
				// (will call this state's Exit method!)
				CGameplayState::GetInstance()->SetLoadingSavedGame(false);
				CGame::GetInstance()->SetGameType(SINGLE_PLAYER);
				CGame::GetInstance()->SetCurrentLevel(CurrentLevel::TUTORIAL);
				pG->PushState(CGameplayState::GetInstance());

				// MUST exit this function immediately!!!
				return true;
			}
			break;
		case CONTINUE:
			CGameplayState::GetInstance()->SetLoadingSavedGame(false);
			CGame::GetInstance()->SetGameType(SINGLE_PLAYER);

			CGame::GetInstance()->SetCurrentLevel(static_cast<CurrentLevel>(CGame::GetInstance()->GetProfile()->GetCampaignProgress()));
			pG->PushState(CGameplayState::GetInstance());
			break;
		case MENU_MULTIPLAYER:
			CGame::GetInstance()->SetGameType(MULTIPLAYER);
			pG->PushState(CMultiPState::GetInstance());
			break;
		case LOAD:
			pG->PushState(CSaveState::GetInstance());
			break;
		case OPTIONS:
			pG->PushState(COptionState::GetInstance());
			break;
		case MENU_TUTORIAL:
			CGameplayState::GetInstance()->SetLoadingSavedGame(false);
			CGame::GetInstance()->SetGameType(SINGLE_PLAYER);
			CGame::GetInstance()->SetCurrentLevel(CurrentLevel::JUST_TUTORIAL);
			pG->PushState(CGameplayState::GetInstance());
			break;
		case HELP:
			pG->PushState(CInstructionState::GetInstance());
			break;
		case CREDITS:
			pG->PushState(CCreditState::GetInstance());
			break;
		case EXIT:		// exit
			return false;
			break;
		}
	}

	return true;	// keep playing
}

//************************************************************
// Update
/*virtual*/ void CMainMenuState::Update( float fElapsedTime )	/*override*/
{
	HINSTANCE hInst = CGame::GetInstance()->GethInstance();

	char testString[100];
	LoadStringA(hInst, IDS_NEWGAME, testString, 100);
	if(_stricmp(newGame, testString) != 0)
	{
		LoadStringA(hInst, IDS_MAINMENU, mainMenu, 20);
		LoadStringA(hInst, IDS_NEWGAME, newGame, 20);
		LoadStringA(hInst, IDS_CONTINUE, continueGame, 20);
		LoadStringA(hInst, IDS_MULTIPLAYER, multiplayer, 20);
		LoadStringA(hInst, IDS_LOADGAME, loadGame, 20);
		LoadStringA(hInst, IDS_INSTRUCTIONS, instructions, 20);
		LoadStringA(hInst, IDS_TUTORIAL, tutorial, 20);
		LoadStringA(hInst, IDS_OPTIONS, options, 20);
		LoadStringA(hInst, IDS_CREDITS, credits, 20);
		LoadStringA(hInst, IDS_EXIT, exit, 20);
	}
}

//************************************************************
// Render
//	- render the main menu
/*virtual*/ void CMainMenuState::Render( void )	/*override*/
{
	// Access the font
	CBMFont* pF = CGame::GetInstance()->GetFont();
	DWORD dwColor = D3DCOLOR_XRGB(12, 104, 0);
	char* currString = new char [100];

	CSGD_TextureManager::GetInstance()->Draw(BG, 0, 0);

	CSGD_TextureManager::GetInstance()->Draw(indicatorIMG, MENU_ITEMS_X - CURSOR_XOFF, MENU_ITEMS_Y + m_nCursor * MENU_ITEMS_YOFF);

	pF->Write(newGame, MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * 0, 1.3f, dwColor);
	pF->Write(loadGame, MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * 1, 1.3f, dwColor);
	pF->Write(continueGame, MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * 2, 1.3f, dwColor);
	pF->Write(multiplayer, MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * 3, 1.3f, dwColor);
	pF->Write(instructions, MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * 4, 1.3f, dwColor);
	pF->Write(tutorial, MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * 5, 1.3f, dwColor);
	pF->Write(options, MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * 6, 1.3f, dwColor);
	pF->Write(credits, MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * 7, 1.3f, dwColor);
	pF->Write(exit, MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * 8, 1.3f, dwColor);

	float fLabelY = CGame::GetInstance()->GetScreenHeight() - pF->GetLineHeight() * 0.75f;
	pF->Write("Press Up/Down to navigate, Enter/A to select, Esc/B to go back", 0, (int)fLabelY, 0.75, D3DCOLOR_XRGB(255, 255, 255));
	delete currString;
}
