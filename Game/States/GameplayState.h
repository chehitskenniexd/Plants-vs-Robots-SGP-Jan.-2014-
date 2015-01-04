/*************************************************************
| File:  GameplayState.h
| Purpose: To initialize & run the gameplay
|
| Author:  Doug Monroe (dmonroe@fullsail.com)
| Course:  SGD 1312
*************************************************************/

#pragma once

#include "IGameState.h"
#include "../MainInfo/Entity.h"
#include "../MainInfo/HUD.h"
#include "../Grid/Grid.h"
#include "../MainInfo/PlayerProfile.h"
#include "../MainInfo/AnimTimeStamps.h"

#define KOTHWINAMOUNT 25

//************************************************************
// Forward Class Declarations:
class CSGD_EventSystem;
class CSGD_MessageSystem;
class CSGD_XAudio2;
class IMessage;
class CCreateEntityMessage;
class CEntityManager;
class CEntity;

enum GameMode {STANDARD, DEATHMATCH, DOMINATION, KOTH, TOW};

//************************************************************
// CGameplayState class
// - runs gameplay
// - SINGLETON! (Static allocation, not dynamic)
class CGameplayState : public IGameState
{
public:
	enum GameplayStatus {COINFLIP, PLAYING, END_TURN, ENDROUND, PAUSED, OVER};
	
	//********************************************************
	// SINGLETON!
	static CGameplayState* GetInstance( void );

	//********************************************************
	// IGameState Interface:
	virtual void Enter( void )     override; // load resources
	virtual void Exit ( void )     override; // unload resources

	virtual bool Input ( void )     override; // handle user input
	virtual void Update( float fElapsedTime ) override; // update game entities / menus
	virtual void Render( void )     override; // render game entities / menus

	int OffsetX(void);
	int OffsetY(void);

	//int GetNumberRobotUnits (void) {return m_nRobotUnits;}

	void SetLoadingSavedGame(bool bLoading) {m_bLoadingSavedGame = bLoading;}
	void SetFirstTurn (OwnerFaction nFaction);
	void SetSelectedUnit (CEntity* currentUnit) {m_pSelectedUnit = currentUnit;}
	enum CurrentCommand {NONE, MOVE, ATTACK, CONVERT, CREATE, CREATE_TANK, CREATE_INFANTRY, CREATE_SCOUT};
	void SetCurrentAction (CurrentCommand pCommand) {m_nSelectedAction = pCommand;}
	enum SavingStatus {UNDECIDED, YES, NO};

	CGrid* GetGameMap() { return &gameMap;}
	CGrid GetCurrentMap() {return gameMap;}
	float GetTimer() { return m_fTurnTimer;}
	int GetAP (void) const {return m_nAP;}
	CEntity* GetSelectedUnit() { return m_pSelectedUnit;}
	CEntity* GetCurrUnit() { return pCurUnit;}
	CurrentCommand GetSelectedAction() { return m_nSelectedAction;}
	OwnerFaction GetWinningFaction (void) {return m_nWinner;}
	OwnerFaction GetCurrentFaction (void) {return m_nCurrFaction;}
	int GetRoundsStat(void) {return m_nRoundsStat;}
	float GetTotalGameTime (void) {return m_fTotalTime;}
	bool GetUnitCreating (void) { return this->m_bBaseCreate;}
	void SetUnitCreating (bool pCreating) { this->m_bBaseCreate = pCreating;}

	bool GetBeingPlayed(void) const {return m_bBeingPlayed;}
	int GetPlantTiles (void) const {return m_nPlantTiles;}
	int GetRobotTiles (void) const {return m_nRobotTiles;}
	int GetTotalTiles (void) const {return m_nPlantTiles + m_nRobotTiles + m_nNeutralTiles;}
	int GetPlantKotH (void) const {return m_nKotHPlants;}
	int GetRobotKotH (void) const {return m_nKotHRobots;}
	GameplayStatus GetStatus (void) const {return m_nStatus;}
	void DecrementActiveUnits() {m_nActiveUnits--;}
	GameMode GetMode (void) const {return m_nMode;}


	void SetMode (GameMode nMode) {m_nMode = nMode;}
	void SetTurnTimer (float fSecs) {m_fSecsPerTurn = fSecs;}
	void SetAPDef (bool bAP) {m_bAPDec = bAP;}

	void Deselect(); //A helper function for Input()

	int tileSetImgID;
	bool drawTestRect;

	bool SaveXml(const char* szFilename);
	bool LoadXml(const char* szFilename);

	void DecrementAP(void);
	void IncreaseAP(void);

	bool TutorialInput();

	void EndTurn(void);
	/*CPlayerProfile* GetPlayer(void) const {return m_pPlayer;}
	void SetPlayer(CPlayerProfile* pPlayer) {m_pPlayer = pPlayer;}*/

private:
	//********************************************************
	// SINGLETON (not-dynamically allocated)
	CGameplayState( void );         // default constructor
	virtual ~CGameplayState( void );      // destructor
	CGameplayState( const CGameplayState& );    // copy constructor
	CGameplayState& operator= ( const CGameplayState& ); // assignment operator

	//********************************************************
	// FPS
	unsigned int   m_unFPS;
	unsigned int   m_unFrames;
	float     m_fFPSTimer;

	bool m_bBeingPlayed;
	int coinPos;
	//Pause status

	enum CoinStatus {UNFLIPPED, FLIPPING, HEADS, TAILS, FLIPPED};
	CoinStatus m_nCoin;

	AnimTimeStamps m_aCoinFlip;
	AnimTimeStamps m_aAPExplosion;

	bool m_bEndTurn;

	bool m_bCoinFlipped, m_bCoinFlipping, m_bWinnerChosen;
	float m_fCoinFlipTimer;
	GameplayStatus m_nStatus;
	GameMode m_nMode;

	bool m_bLoadingSavedGame;
	bool m_bAPDec;
	//bool m_bIsPaused;

	float m_fSecsPerTurn;

	//Camera position info
	float m_fCamX, m_fCamY;

	int m_nCamStartX, m_nCamStartY;
	int m_nCamDestX, m_nCamDestY;

	float m_fCamTimer;
	bool m_bCamLocked;
	bool m_bBaseCreate;

	// Turn Timer
	float m_fTurnTimer;
	float m_fEndGameTimer;
	float m_fCoinAiTimer;

	//Cursor position in tiles, NOT pixels
	int m_nCursorX, m_nCursorY;


	int turnChangePosX;

	CurrentCommand m_nSelectedAction;
	OwnerFaction m_nFirstTurn;
	OwnerFaction m_nCurrFaction;
	OwnerFaction m_nWinner;

	CEntity* m_pSelectedUnit;

	int m_nAP;
	int m_nPlantUnits, m_nRobotUnits;
	int m_nPlantTiles, m_nNeutralTiles, m_nRobotTiles;
	int m_nActiveUnits; //Number of units with available actions
	int m_nKotHPlants, m_nKotHRobots;

	//Statistics for this game
	int m_nRoundsStat;
	float m_fPlantsTime, m_fRobotsTime;
	float m_fTotalTime;
	float m_fAiUpdateTimer;
	vector<CTile*> convertedTiles;

	//********************************************************
	// Game Assets
	int testImageID;
	int tutorialBox;
	int turnChange;
	int storyBoard;

	//********************************************************
	// Game Entities
	CEntityManager*   m_pEM;

	//********************
	// Map
	CGrid gameMap;

	//*************************
	// HUD
	CHUD hud;

	void PanTo(float fX, float fY);

	//********************************************************
	// Factory Methods
	CEntity* CreateMovingObject(CCreateEntityMessage* pMsg);
	CEntity* CreateStaticObject(CCreateEntityMessage* pMsg);

	//********************************************************
	// Event & Message Systems
	CSGD_EventSystem*  m_pES;
	CSGD_MessageSystem*  m_pMS;
	CSGD_XAudio2*  m_pXA;

	CTile* pCurTile;
	CEntity* pCurUnit;

	short tutorialStep;
	RECT tutorialHighlight;
	char tutorialInstruction[200];
	char storyTimeString[250];
	char turnChangeString[20];
	char coinString[32];
	char continueConfirm[32];
	int m_nCursor;

	bool P1Turn, P2Turn, inTutorial, storyTime, step1, step2, step3;

	static void MessageProc( IMessage* pMsg );

	//*******************************************************************
	// Sounds
	int selectionChangedSFX;
	int selectionMadeSFX;
	int selectionBackSFX;
	int convertNormalSFX;
	int convertSpecialSFX;
	int convertWaterSFX;
	int plantDeselectSFX;
	int plantSelectSFX;
	int robotDeselectSFX;
	int robotSelectSFX;
	int turnEndSFX;
	int roundEndSFX;
	int timerLowSFX;

	bool hasBeenWarned;
	
	int BG;
	enum PauseMenuChoice {RESUME = 1, SAVE, OPTIONS, QUITTOMENU, QUITGAME};
};


