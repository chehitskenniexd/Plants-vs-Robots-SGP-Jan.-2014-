#pragma once
#include "igamestate.h"
class CCreditState :
	public IGameState
{
public:
	CCreditState(void);
	~CCreditState(void);

	static CCreditState* GetInstance(void);

	virtual void Enter ( void ) override;	// load resources
	virtual void Exit (  void ) override;	// unload resources
	virtual bool Input ( void ) override;	// handle user input
	virtual void Update ( float fElapsedTime ) override;	// update game entities / animations
	virtual void Render ( void ) override;	// render game entities / menus

private:
	char szCredits[32];

	int BG;

	float creditsTimer;
	int creditsAlpha;

	int MENU_ITEMS_X;
	int MENU_ITEMS_Y;

	char executive[32];
	char associate[32];
	char programmer[32];
	char artist[32];
	char sound[32];

	int selectionBackSFX;
};

