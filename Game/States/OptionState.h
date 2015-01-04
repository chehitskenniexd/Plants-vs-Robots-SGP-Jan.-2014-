#pragma once
#include "igamestate.h"

class COptionState : public IGameState
{
private:
	enum OptionLanguage {ENGLISH, SPANISH};
	enum SelectedSetting {MUSIC, SOUND, FULLSCREEN, LANGUAGE};

	int m_nCursor;

	float m_fSoundVolume, m_fMusicVolume;
	bool m_bFullscreen;
	OptionLanguage m_nLang;

	int m_nTestSound;

	int BG;

	int MENU_ITEMS_X;
	int MENU_ITEMS_Y;
	int MENU_ITEMS_YOFF; 
	int CURSOR_XOFF;

	int selectionChangedSFX;
	int selectionMadeSFX;
	int selectionBackSFX;
	
public:
	COptionState(void);
	~COptionState(void);

	static COptionState* GetInstance(void);

	int indicatorIMG;

	char musicVolumeString[20];
	char soundVolumeString[20];
	char fullscreenString[20];
	char languageString[20];
	char offString[10];
	char onString[10];

	

	virtual void Enter ( void ) override;	// load resources
	virtual void Exit (  void ) override;	// unload resources
	virtual bool Input ( void ) override;	// handle user input
	virtual void Update ( float fElapsedTime ) override;	// update game entities / animations
	virtual void Render ( void ) override;	// render game entities / menus

	bool SaveOptions(const char * szFilename = "options.xml");
	bool LoadOptions(const char * szFilename = "options.xml");
	void ApplyOptions();
};