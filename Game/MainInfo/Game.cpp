/*************************************************************
|	File:		Game.cpp
|	Purpose:	To run the game state machine
|				& initialize the core SGD wrappers
|
|	Author:		Doug Monroe (dmonroe@fullsail.com)
|	Course:		SGD 1312
*************************************************************/

#include "Game.h"

#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../MainInfo/resource.h"

//#include "BitmapFont.h"
#include "BMFont.h"

//States
#include "../States/MainMenuState.h"
#include "../States/GameplayState.h"
#include "../States/ProfileState.h"
#include "../States/OptionState.h"

#include <ctime>
#include <cassert>


//************************************************************
// SINGLETON!
//	- instantiation of static member
/*static*/ CGame* CGame::s_pInstance = nullptr;


// GetInstance
//	- allocate only ONE instance of the class
//	- return the instance
/*static*/ CGame* CGame::GetInstance( void )
{
	if( s_pInstance == nullptr )
		s_pInstance = new CGame();

	return s_pInstance;
}

// DeleteInstance
//	- deallocate the instance
/*static*/ void CGame::DeleteInstance( void )
{
	delete s_pInstance;
	s_pInstance = nullptr;
}


//************************************************************
// CONSTRUCTOR
//	- set ALL data members to 'clean' values
CGame::CGame( void )
{
	srand(static_cast<unsigned int>(time(0)));

	m_pD3D	= nullptr;
	m_pDI	= nullptr;
	m_pTM	= nullptr;
	m_pXA	= nullptr;
	paused = false;

	m_nScreenWidth	= 1;
	m_nScreenHeight	= 1;
	m_bWindowed		= true;

	m_dwCurrTime	= 0;

	m_pFont	= nullptr;

	m_nBGM = -1;

	campaignLevelCompleted = false;
	campaignCompleted = false;
	multiplayerCompleted = false;
	this->SetCurrentLevel(NO_LEVEL);
}

//************************************************************
// DESTRUCTOR
CGame::~CGame( void )
{
	//if (m_nBGM != -1)
	//{
	//	CSGD_XAudio2::GetInstance()->MusicUnloadSong(m_nBGM);
	//	m_nBGM = -1;
	//}
}

HINSTANCE CGame::GethInstance(void)
{
	return hInstance;	
}

CEntity* CGame::GetCurrentUnit (void) 
{return this->currentUnit;}

void CGame::SetCurrentUnit (CEntity* entity)
{this->currentUnit = entity;}

//************************************************************
// Initialize
//	- initialize the SGD wrappers
//	- load the starting game state
void CGame::Initialize( HWND hWnd, HINSTANCE hInstance,
					   int nScreenWidth, int nScreenHeight,
					   bool bWindowed )
{
	this->hInstance = hInstance;

	// Store the SGD singletons
	m_pD3D	= CSGD_Direct3D::GetInstance();
	m_pDI	= CSGD_DirectInput::GetInstance();
	m_pTM	= CSGD_TextureManager::GetInstance();
	m_pXA	= CSGD_XAudio2::GetInstance();

	// Store the parameters
	m_nScreenWidth	= nScreenWidth;
	m_nScreenHeight	= nScreenHeight;
	m_bWindowed		= bWindowed;

	// Initialize the SGD Wrappers
	m_pD3D->Initialize( hWnd, m_nScreenWidth, m_nScreenHeight, m_bWindowed, false );
	m_pDI->Initialize( hWnd, hInstance, DI_KEYBOARD /*| DI_MOUSE */| DI_JOYSTICKS);
	m_pTM->Initialize( m_pD3D->GetDirect3DDevice(), m_pD3D->GetSprite() );
	m_pXA->Initialize();

	// Initialize the font
	m_pFont = new CBMFont();
	m_pFont->Initialize();

	//Load in config options
	COptionState::GetInstance()->LoadOptions();
	COptionState::GetInstance()->ApplyOptions();

	// Start in the MainMenu state
	PushState(CMainMenuState::GetInstance());
	PushState(CProfileState::GetInstance());

	// Store the starting time
	m_dwCurrTime	= GetTickCount();

	m_nBGM = m_pXA->MusicLoadSong(_T("Resources/Sounds/BGM.xwm"));
	m_pXA->MusicPlaySong(m_nBGM, true);
}

//************************************************************
// Main
//	- run one frame of the game
bool CGame::Main( void )
{
		// Check input
		if( Input() == false )
			return false;

		// Update & Render
		Update();
		Render();

	return true;
}

//************************************************************
// Terminate
//	- unload the current game state
//	- terminate the SGD wrappers
void CGame::Terminate( void )
{
	// Exit the current state
	PopState(); 

	// Deallocate the font
	if( m_pFont != nullptr )
	{
		m_pFont->Terminate();
		delete m_pFont;
		m_pFont = nullptr;
	}

	// Terminate the wrappers in REVERSE order!
	m_pXA->Terminate();
	m_pXA = nullptr;

	m_pTM->Terminate();
	m_pTM = nullptr;

	m_pDI->Terminate();
	m_pDI = nullptr;

	m_pD3D->Terminate();
	m_pD3D = nullptr;
}

//************************************************************
// Input
//	- handle application-wide user input
//	- allow the current game state to handle other input
bool CGame::Input( void )
{
	// Refresh the input buffers
	m_pDI->ReadDevices();			// ONLY HERE IN CGAME!!!

	// Allow current state to handle input
	return m_sStates.top()->Input();
}

//************************************************************
// Update
//	- update the current game state
void CGame::Update( void )
{
	// Refresh the audio buffers
	m_pXA->Update();			// ONLY HERE IN CGAME!

	// Calculate the elapsed time between frames
	DWORD dwNow = GetTickCount();
	float fElapsedTime = (dwNow - m_dwCurrTime) / 1000.0f;
	m_dwCurrTime = dwNow;

	// Cap the elapsed time at 1/8th of a second
	if( fElapsedTime > 0.125f )
		fElapsedTime = 0.125f;

	// Allow the current state to update
	m_sStates.top()->Update( fElapsedTime );
}

//************************************************************
// Render
//	- render the current game state
void CGame::Render( void )
{
	// Clear the buffer
	m_pD3D->Clear( D3DCOLOR_XRGB( 0, 0, 0 ) );

	// Start rendering
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();
	{
		// Allow the current state to render
		m_sStates.top()->Render();
	}
	// End rendering
	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();


	// Present the buffer to the screen
	m_pD3D->Present();
}

////************************************************************
//// ChangeState
////	- switch from the current state to the new state
//void CGame::ChangeState( IGameState* pNewState )
//{
//	// Exit the current state (if it exists)
//	if( m_pCurrState != nullptr )
//		m_pCurrState->Exit();
//
//	// Store the new state
//	m_pCurrState = pNewState;
//
//	// Enter the new state (if it exists)
//	if( m_pCurrState != nullptr )
//		m_pCurrState->Enter();
//}
void CGame::PushState(IGameState* pState) //Push new state onto stack and enter it.
{
	//Leave current state
	//if (!m_sStates.empty())
		//m_sStates.top()->Exit();

	m_sStates.push(pState);
	m_sStates.top()->Enter();
}
void CGame::PopState(void) //Exit curr state, pop and reenter new state;
{
	if (m_sStates.empty())
		return;

	m_sStates.top()->Exit();
	m_sStates.pop();

	/*if (!m_sStates.empty())
		m_sStates.top()->Enter();*/
}

//Pop curr state, push new one, and enter it.
void CGame::ChangeState( IGameState* pNewState )
{
	//Stop current state if it exists
	if (!m_sStates.empty())
		m_sStates.top()->Exit();

	//Swap the new state directly in
	m_sStates.top() = pNewState;
	pNewState->Enter();
}
