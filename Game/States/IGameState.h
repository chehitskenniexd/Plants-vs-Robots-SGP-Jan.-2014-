/*************************************************************
|	File:		IGameState.h
|	Purpose:	Interface base class for game state
|
|	Author:		Doug Monroe (dmonroe@fullsail.com)
|	Course:		SGD 1312
*************************************************************/

#pragma once


//************************************************************
// IGameState class
//	- interface base class for game states
//	- pure virtual methods for children to override
class IGameState
{
public:
	//********************************************************
	// Abstract Base Class
	virtual ~IGameState( void )	= 0
	{
	}
	
	//********************************************************
	// IGameState Interface:
	virtual void	Enter	( void )	= 0;	// load resources
	virtual void	Exit	( void )	= 0;	// unload resources

	virtual bool	Input	( void )	= 0;	// handle user input
	virtual void	Update	( float fElapsedTime )	= 0;	// update game entities / animations
	virtual void	Render	( void )	= 0;	// render game entities / menus
};

