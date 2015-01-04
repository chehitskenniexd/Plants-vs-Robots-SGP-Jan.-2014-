#pragma once
#include "igamestate.h"
#include <vector>

class CSaveState : public IGameState
{
private:
	CSaveState(void);
	~CSaveState(void);

	int MENU_ITEMS_X;
	int MENU_ITEMS_Y;

	int BG;

	bool m_bReadyForKey;
	bool m_bEnteringText;
	std::string tempStr;

	char szTitleString[32];
	char szTextEntry[32];
	char szHelpString[256];
	unsigned int m_nCursor;
	std::vector<const char*> vChoices;

	std::string ComposePath(const char* szFilename);

	void GetProfs();
public:
	static CSaveState* GetInstance();

	virtual void Enter ( void ) override;	// load resources
	virtual void Exit (  void ) override;	// unload resources
	virtual bool Input ( void ) override;	// handle user input
	virtual void Update ( float fElapsedTime ) override;	// update game entities / animations
	virtual void Render ( void ) override;	// render game entities / menus
};

