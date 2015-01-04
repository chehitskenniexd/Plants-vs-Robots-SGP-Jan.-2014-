#pragma once
#include "igamestate.h"
class CSinglePSetup :
	public IGameState
{
public:
	CSinglePSetup(void);
	~CSinglePSetup(void);

	virtual void Enter ( void ) override;	// load resources
	virtual void Exit (  void ) override;	// unload resources
	virtual bool Input ( void ) override;	// handle user input
	virtual void Update ( float fElapsedTime ) override;	// update game entities / animations
	virtual void Render ( void ) override;	// render game entities / menus
};

