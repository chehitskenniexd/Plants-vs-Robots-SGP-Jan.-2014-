#pragma once
#include "IGameState.h"
#include "../MainInfo/AnimTimeStamps.h"
#include <vector>

enum OptionStates {CONTINUE_CAMPAIGN, REPLAY_LEVEL, MAIN_MENU, EXIT_GAME};


class CGameOverState :
	public IGameState
{
public:
	//********************************************************
	// SINGLETON!
	static CGameOverState* GetInstance( void );

	virtual void Enter ( void ) override;	// load resources
	virtual void Exit (  void ) override;	// unload resources
	virtual bool Input ( void ) override;	// handle user input
	virtual void Update ( float fElapsedTime ) override;	// update game entities / animations
	virtual void Render ( void ) override;	// render game entities / menus

private:
	//********************************************************
	// SINGLETON (not-dynamically allocated)
	CGameOverState( void );									// default constructor
	virtual ~CGameOverState( void );						// destructor
	CGameOverState( const CGameOverState& );				// copy constructor
	CGameOverState& operator= ( const CGameOverState& );	// assignment operator

	int m_nCursorPos;

	// character arrays for the text
	char plantsWin[20];
	char robotsWin[20];
	char continueOption [20];
	char replayOption [20];
	char exitGameOption [20];
	char mainMenuOption [20];
	char campaign1[50];
	char campaign2[50];
	char campaign3[50];
	char timeElapsed[20];
	char roundsElapsed[20];

	int selectionChangedSFX;
	int selectionMadeSFX;
	int victorySFX;
	int loseSFX;
	int plantsWinImg;
	int robotWinImg;

	//********************************************************
	// Screen Attributes
	int						m_nScreenWidth;
	int						m_nScreenHeight;

	//********************************************************
	// Option Chosen
	OptionStates currOption;

	//********************************************************
	// Stats Related
	int m_nHours, m_nMinutes, m_nSeconds;
};

