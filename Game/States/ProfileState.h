#pragma once
#include "igamestate.h"
#include <vector>
#include <time.h>
class CPlayerProfile;

class CProfileState :
	public IGameState
{
public:

	static CProfileState* GetInstance();

	virtual void Enter ( void ) override;	// load resources
	virtual void Exit (  void ) override;	// unload resources
	virtual bool Input ( void ) override;	// handle user input
	virtual void Update ( float fElapsedTime ) override;	// update game entities / animations
	virtual void Render ( void ) override;	// render game entities / menus
	tm GetProfileTime (void) {return this->profileTime;}

	bool LoadProfiles(const char* szFilename = "profiles.xml");
	bool SaveProfiles(const char* szFilename = "profiles.xml");
private:
	CProfileState(void);
	~CProfileState(void);

	void AddNewProfile(const char * szProfileName, const char * szSaveName = "");

	bool m_bReadyForKey;

	bool m_bEnteringText;
	
	std::string tempStr;
	std::vector<CPlayerProfile*> m_vProfiles;
	int m_nCursor;
	int BG;

	int selectionChangedSFX;
	int selectionMadeSFX;
	int selectionBackSFX;

	char sSelect[64];
	char sText[32];
	char timebuf[26];
	char instruct[100];
	char enterName[20];
	char szProg[32];
	tm profileTime;

	int indicatorIMG;

	int MENU_ITEMS_X;
	int MENU_ITEMS_Y;
	int MENU_ITEMS_YOFF; 
	int CURSOR_XOFF;
};