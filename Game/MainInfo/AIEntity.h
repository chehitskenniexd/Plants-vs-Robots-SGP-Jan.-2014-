#pragma once
#include "MovingEntity.h"
#include "StaticEntity.h"
#include <vector>

class AIEntity
{
public:
	//********************************************************
	// SINGLETON!
	static AIEntity* GetInstance( void );

	void Update (float fElapsedTime, CEntity* currentEntity);
	void UpdateStates (float fElapsedTime);
	bool InAttackRange (CEntity* target, CMovingEntity* currentUnit, CGrid& map);
	bool InMoveRange (CMovingEntity* currentUnit, CTile* targetTile, CGrid &map);
	void AiTimerUpdate (int numberUnits);
	void AiCreateUnit (CStaticEntity* currentEntity);
	void AiChooseActions ();
	void AiChooseActions (CEntity* currentEntity);
	void NormalAiActions(CMovingEntity* currentEntity);
	void AiBaseCreate (CStaticEntity* currentBase);
	void DefensiveAiActions (CMovingEntity* currentEntity);
	void AggressiveAiActions (CMovingEntity* currentEntity);
	void RetreatAiActions (CMovingEntity* currentEntity);
	CEntity* ClosestLowPlayerUnit (CMovingEntity* currentEntity);


	//********************************************************
	// Accessors and Mutators
	void SetAiState (AIState currentState) {currentAiState = currentState;}
	void SetTimesLooped(int m_nLooped) {this->timesLooped = m_nLooped;}
	std::vector<IEntity*>* SetAiUnits (void) {return &m_vAiUnits;}
	std::vector<IEntity*>* SetAiBases (void) {return &m_vAiBases;}	
	std::vector<IEntity*>* SetPlayerUnits (void) {return &m_vPlayerUnits;}
	std::vector<IEntity*>* SetPlayerBases (void) {return &m_vPlayerBases;}

	std::vector<IEntity*> GetAiUnitsRef (void) {return m_vAiUnits;}
	std::vector<IEntity*> GetAiUnits (void) {return m_vAiUnits;}
	std::vector<IEntity*> GetAiBases (void) {return m_vAiBases;}	
	std::vector<IEntity*> GetPlayerUnits (void) {return m_vPlayerUnits;}
	std::vector<IEntity*> GetPlayerBases (void) {return m_vPlayerBases;}
	AIState GetAiState (void) {return currentAiState;}
	float GetUpdateAiTimer (void) {return updateAiTimer;}

private:
	//********************************************************
	// SINGLETON (not-dynamically allocated)
	AIEntity( void );									// default constructor
	virtual ~AIEntity( void );						// destructor
	AIEntity( const AIEntity& );				// copy constructor
	AIEntity& operator= ( const AIEntity& );	// assignment operator

	std::vector <IEntity*> m_vAiUnits;
	std::vector <IEntity*> m_vAiBases;
	std::vector <IEntity*> m_vPlayerUnits;
	std::vector <IEntity*> m_vPlayerBases;
	AIState currentAiState;
	float updateAiTimer;
	int timesLooped;

	CEntity* targetUnit;
	CEntity* closestUnit; 
	CEntity* closestBase;
};

