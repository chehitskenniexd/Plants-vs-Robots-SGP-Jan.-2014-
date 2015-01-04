/*************************************************************
|	File:		Entity.h
|	Purpose:	To store data for velocity-based entities,
|				& is the base class for all game entities
|
|	Author:		Doug Monroe (dmonroe@fullsail.com)
|	Course:		SGD 1312
*************************************************************/

#pragma once

#include "IEntity.h"
#include "AnimTimeStamps.h"
#include "../SGD Wrappers/IListener.h"

enum	UnitAction			{ACT_NONE, ACT_MOVE, ACT_ATTACK, ACT_CONVERT, ACT_EXIT, ACT_CREATESCOUT, ACT_CREATEINFANTRY, ACT_CREATETANK};
enum	UnitType				{UNIT_BASE, UNIT_INFANTRY, UNIT_SCOUT, UNIT_TANK};
enum	OwnerFaction	{OWNER_PLANTS, OWNER_ROBOTS, OWNER_AI, OWNER_NEUTRAL};
enum	AIState					{NON_AI, NORMAL_AI, DEFENSE_AI, AGGRESSIVE_AI, RETREAT_AI};


class BasePlayer;
class CTile;

//************************************************************
// CEntity class
//	- base class for game entities
//	- velocity-based movement
class CEntity : public IEntity, public IListener
{
protected:
	//********************************************************
	// entity type
	UnitType m_eType;
	OwnerFaction m_nFaction;
public:
	//********************************************************
	// Constructor & Destructor
	CEntity( void );
	virtual ~CEntity( void );


	//********************************************************
	// IEntity Interface:
	//	- can be overridden by grandchild class
	virtual void	Update	( float fElapsedTime )	override;
	virtual void	Render	( void )				override;

	virtual int		GetType	( void ) const			override	{	return m_eType;		}
	virtual RECT	GetRect	( void ) const			override;
	virtual void	HandleCollision( const IEntity* pOther )	override;
	virtual void	HandleEvent( const CEvent* pEvent )	override;

	// CANNOT override 'final' methods in the grandchild class
	virtual void AddRef		( void )	final;
	virtual void Release	( void )	final;

	//********************************************************
	// Accessors:
	int	GetImageID( void ) const {return m_nImageID;	}
	int	GetWidth( void ) const {return	m_nWidth;}
	int	GetHeight( void ) const {return	m_nHeight;}
	int	GetPosX	( void ) const { return	m_nMapX;}
	int	GetPosY	( void ) const { return	m_nMapY;}	
	int GetBaseHealth(void) const {return m_nBaseHealth;}
	int GetCurrHealth(void) const {return m_nCurrentHealth;}
	int GetUnitType(void) const {return m_eType;}
	int GetCurrAction(void) const {return m_nCurrentAction;}
	CTile* GetCurrTile(void) const {return m_pCurrentTile;}
	BasePlayer* GetPlayer(void) const {return m_pCurrentPlayer;}
	OwnerFaction GetFaction(void) const {return m_nFaction;}
	AnimTimeStamps* GetCurrentAnimStamp(void) {return &currentAnimStamp;}
	AnimTimeStamps* GetDeathAnimation(void) {return &m_aDeathAnimation;}
	AnimTimeStamps GetDeathAnimations(void) {return m_aDeathAnimation;}
	AnimTimeStamps* GetIdleAnimationStamp(void) {return &m_aIdleAnimation;}
	AnimTimeStamps GetIdleAnimationStamps(void) {return m_aIdleAnimation;}
	bool GetUsedAction(void) const {return m_bUsedAction;}
	bool GetDeathAction(void) const {return m_bDeathAction;}
	AIState GetCurrentAiState(void) {return m_sCurrentState;}


	// Mutators:
	void SetImageID(int id) {m_nImageID	= id;}
	void SetWidth(int w){m_nWidth = w;}
	void SetHeight(int h){m_nHeight = h;}
	void SetX(int x) {m_nMapX = x;}
	void SetY(int y) {m_nMapY = y;}
	void SetBaseHealth(int health) {m_nBaseHealth = health;}
	void SetCurrHealth(int health) {m_nCurrentHealth = health;}
	void SetUnitType(UnitType type) {m_eType = type;}
	void SetCurrAction(UnitAction action) {m_nCurrentAction = action;}
	void SetTile(CTile* tile) {m_pCurrentTile = tile;}
	void SetPlayer(BasePlayer* player) {m_pCurrentPlayer = player;}
	void SetFaction(OwnerFaction faction) {m_nFaction = faction;}
	void SetCurrentAnimStamp (AnimTimeStamps pStamp) {currentAnimStamp = pStamp;}
	void SetDeathAnimation (AnimTimeStamps pStamp) {m_aDeathAnimation = pStamp;}
	void SetIdleAnimationStamp (AnimTimeStamps pStamp) {m_aIdleAnimation = pStamp;}
	virtual void SetUsedAction(bool hasUsed);
	void SetDeathAction(bool hasDied) {this->m_bDeathAction = hasDied;}
	void SetDamageTaken(int dmg) { damageTaken = dmg;}
	void SetCurrentAiState(AIState sState) {this->m_sCurrentState = sState;}

private:
	//********************************************************
	// image
	int	m_nImageID;

	//Position on map
	int	m_nMapX, m_nMapY;

	// size
	int	m_nWidth;
	int	m_nHeight;

	//Gameplay stats
	int m_nBaseHealth;
	int m_nCurrentHealth;

	//UnitType m_nCurrentType;
	UnitAction m_nCurrentAction;

	CTile* m_pCurrentTile;
	BasePlayer* m_pCurrentPlayer;
	AnimTimeStamps m_aIdleAnimation, m_aDeathAnimation, currentAnimStamp;
	AIState m_sCurrentState;

	bool m_bUsedAction;
	bool m_bDeathAction;
	bool m_bIsDying;

	int damageTaken;
	float damageTakenPos;

	// reference count
	unsigned int	m_unRefCount;
};

