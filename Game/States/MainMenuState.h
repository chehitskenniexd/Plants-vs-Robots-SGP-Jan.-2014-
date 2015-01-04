/*************************************************************
|	File:		MainMenuState.h
|	Purpose:	To handle the main menu
|
|	Author:		Doug Monroe (dmonroe@fullsail.com)
|	Course:		SGD 1312
*************************************************************/

#pragma once

#include "IGameState.h"




//************************************************************
// CMainMenuState class
//	- runs the main menu
//	- SINGLETON! (Static allocation, not dynamic)
class CMainMenuState : public IGameState
{
public:
	//********************************************************
	// SINGLETON
	static CMainMenuState* GetInstance( void );


	//********************************************************
	// IGameState Interface:
	virtual void	Enter	( void )	override;	// load resources
	virtual void	Exit	( void )	override;	// unload resources

	virtual bool	Input	( void )	override;	// handle user input
	virtual void	Update	( float fElapsedTime )	override;	// update game entities / animations
	virtual void	Render	( void )	override;	// render game entities / menus


private:
	//********************************************************
	// SINGLETON (not-dynamically allocated)
	CMainMenuState( void );									// default constructor
	virtual ~CMainMenuState( void );						// destructor

	CMainMenuState( const CMainMenuState& );				// copy constructor
	CMainMenuState& operator= ( const CMainMenuState& );	// assignment operator

	// *************************
	// Images
	int BG;

	//**********************
	// Sounds
	int selectionChangedSFX;
	int selectionMadeSFX;

	//********************************************************
	// cursor position 
	int	m_nCursor;
	char mainMenu[20];
	char newGame[20];
	char continueGame[20];
	char multiplayer[20];
	char loadGame[20];
	char instructions[20];
	char tutorial[20];
	char options[20];
	char credits[20];
	char exit[20];

	int MENU_ITEMS_X;
	int MENU_ITEMS_Y;
	int MENU_ITEMS_YOFF; 
	int CURSOR_XOFF;

	int indicatorIMG;
};

