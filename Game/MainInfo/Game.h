/*************************************************************
|	File:		Game.h
|	Purpose:	To run the game state machine
|				& initialize the core SGD wrappers
|
|	Author:		Doug Monroe (dmonroe@fullsail.com)
|	Course:		SGD 1312
*************************************************************/

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stack>
#include <map>
#include "resource.h"
#include "Entity.h"

#ifdef _DEBUG
#define _MESSAGE_EVENT_TEST 1
#endif

//************************************************************
// Forward declaration:
//	- tells the compiler that the type exists
//	- used for pointers or references
class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_XAudio2;

class CBMFont;
class IGameState;
class CPlayerProfile;

// Enum for Game Levels
enum					CurrentLevel {NO_LEVEL, TESTING, JUST_TUTORIAL, TUTORIAL, CAMPAIGN_LEVEL1, CAMPAIGN_LEVEL2, CAMPAIGN_LEVEL3, MPLEVEL1, MPLEVEL2, MPLEVEL3, MPLEVEL4};
enum					GameType {SINGLE_PLAYER, MULTIPLAYER};


//************************************************************
// CGame class
//	- runs the game state machine
//	- manages the SGD Wrappers
//	- SINGLETON
//		- only ONE instance of the class created
//		- global access to the instance
class CGame
{
public:
	//********************************************************
	// SINGLETON!
	static CGame*	GetInstance( void );		// Accessor / allocator
	static void		DeleteInstance( void );		// Deallocator


	//********************************************************
	// Setup:
	void Initialize( HWND hWnd, HINSTANCE hInstance,
		int nScreenWidth, int nScreenHeight,
		bool bWindowed );

	// Play:
	bool Main( void );

	// Cleanup:
	void Terminate( void );

	CEntity* GetCurrentUnit (void) ;
	void SetCurrentUnit (CEntity* entity);

	//********************************************************
	// Screen Accessors:
	int		GetScreenWidth	( void ) const	{	return m_nScreenWidth;	}
	int		GetScreenHeight	( void ) const	{	return m_nScreenHeight;	}
	bool	IsWindowed		( void ) const	{	return m_bWindowed;		}

	// Font Accessor:
	CBMFont*	GetFont	( void ) const	{	return m_pFont;			}

	// State Control:

	void ChangeState( IGameState* pNewState );
	void PushState(IGameState* pState); //Put current state on stack, enter new state
	void PopState(void); //Exit curr state, pop and reenter new state;
	HINSTANCE GethInstance(void);

	void SetProfile(CPlayerProfile* pProfile) {m_pProfile = pProfile;}
	CPlayerProfile* GetProfile(void) const {return m_pProfile;}

	// Misc. Accessors/Mutators for Win/Loss
	void	SetCampaignLevCompleted (bool bLevelCompleted ) {campaignLevelCompleted = bLevelCompleted;}
	void	SetCampaignCompleted (bool bCampaignCompleted ) {campaignCompleted = bCampaignCompleted;}
	void	SetMultiplayerCompleted (bool bMultiCompleted ) {multiplayerCompleted = bMultiCompleted;}
	void	SetCurrentLevel (CurrentLevel cCurrentLevel) {currentLevel = cCurrentLevel;}
	void	SetGameType (GameType cGameType) { gameType = cGameType;}

	bool	GetCampainLevelCompleted (void) {return campaignLevelCompleted;}
	bool	GetCampainCompleted (void) {return campaignCompleted;}
	bool	GetMultiplayerCompleted (void) {return multiplayerCompleted;}
	CurrentLevel	GetCurrentLevel (void) {return currentLevel;}
	GameType	GetGameType (void) {return gameType;}

	void Pause() { paused = true;} 
	void Unpause()  { paused = false;} 

	bool IsPaused() { return paused;}

	std::stack<IGameState*> GetCurrentStateStack (void) {return m_sStates;}

private:
	//********************************************************
	// Singleton object
	static CGame*	s_pInstance;

	// "Hidden" / "protected" constructor & destructor
	CGame( void );
	~CGame( void );

	CGame( const CGame& );				//= delete;
	CGame& operator= ( const CGame& );	//= delete;

	int m_nBGM;
	CEntity* currentUnit;

	//********************************************************
	// Helper Methods:
	bool	Input	( void );
	void	Update	( void );
	void	Render	( void );


	//********************************************************
	// SGD Wrappers
	CSGD_Direct3D*			m_pD3D;
	CSGD_DirectInput*		m_pDI;
	CSGD_TextureManager*	m_pTM;
	CSGD_XAudio2*			m_pXA;


	//********************************************************
	// Screen Attributes
	int						m_nScreenWidth;
	int						m_nScreenHeight;
	bool					m_bWindowed;

	//********************************************************
	// String Table Related
	HINSTANCE hInstance;

	//********************************************************
	// Current Time
	DWORD					m_dwCurrTime;


	//********************************************************
	// Font
	CBMFont*			m_pFont;

	//********************************************************
	// Winning Screen Info
	bool						campaignLevelCompleted;
	bool						campaignCompleted;
	bool						multiplayerCompleted;	
	CurrentLevel		currentLevel;
	GameType			gameType;


	bool paused;
	//********************************************************
	// Game state stack:
	std::stack<IGameState*>	m_sStates;
	CPlayerProfile* m_pProfile;
};
