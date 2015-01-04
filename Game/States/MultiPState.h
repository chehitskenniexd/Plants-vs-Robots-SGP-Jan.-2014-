#pragma once
#include "igamestate.h"
#include <vector>

class CMultiPState :
	public IGameState
{
private:
	CMultiPState(void);
	~CMultiPState(void);

	int m_nCursor;
	int m_nTimer;
	int m_nSelectedMap;
	int turnOrderOption;

	enum FirstTurn {FLIP, PLANTS, ROBOTS};
	
	int BG;

	bool m_bP1Ready, m_bP2Ready;

	char szTitleString[32];
	char szRegularString[32];
	char szDeathmatchString[32];
	char szDominationString[32];
	char szKotHString[32];
	/*char szToWString[32];*/
	char szMapString[32];
	char szFirstTurnString[32];
	char szTimerString[32];
	char szPlantString[32];
	char szRobotString[32];
	char szCoinFlipString[32];

	char szMapNames[4][32];
	char szExitString[32];
	std::vector<char*> vChoices;

	int selectionChangedSFX;
	int selectionMadeSFX;
	int indicatorIMG;


	int MENU_ITEMS_X;
	int MENU_ITEMS_Y;

	void ScrollHorizField(int& field, const int cursorVertPos, const int min, const int max, const int interval = 1);
public:
	static CMultiPState* GetInstance();

	virtual void Enter ( void ) override;	// load resources
	virtual void Exit (  void ) override;	// unload resources
	virtual bool Input ( void ) override;	// handle user input
	virtual void Update ( float fElapsedTime ) override;	// update game entities / animations
	virtual void Render ( void ) override;	// render game entities / menus
};