/*************************************************************
|	File:		EntityManager.h
|	Purpose:	To store & maintain game entities
|
|	Author:		Doug Monroe (dmonroe@fullsail.com)
|	Course:		SGD 1312
*************************************************************/

#pragma once


//************************************************************
// Uses std::vector for storage (faster than Linked-List)
#include <vector>

// Forward class declaration of the most-parent type
class IEntity;


//************************************************************
// CEntityManager class
//	- store "references" to game entities (keeps them in memory)
//	- updates & renders game entities
class CEntityManager
{
public:
	//********************************************************
	// Constructor & Destructor
	CEntityManager( void );
	~CEntityManager( void );


	//********************************************************
	// Storage
	void	AddEntity	( IEntity* pEntity, unsigned int unBucket );
	void	RemoveEntity( IEntity* pEntity, unsigned int unBucket );
	void	RemoveEntity( IEntity* pEntity );
	void	RemoveAll	( unsigned int unBucket );
	void	RemoveAll	( void );


	//********************************************************
	// Upkeep
	void	UpdateAll	( float fElapsedTime );
	void	RenderAll	( void );

	void	CheckAllCollisions( unsigned int unBucket1, unsigned int unBucket2 );

	std::vector<IEntity*>	GetBucket (int pBucket);


private:
	//********************************************************
	// Cannot copy the object
	CEntityManager( const CEntityManager& );				// copy constructor
	CEntityManager& operator= ( const CEntityManager& );	// assignment operator


	//********************************************************
	// Typedefs to simplify the templates
	typedef std::vector< IEntity* >		EntityVector;
	typedef std::vector< EntityVector >	EntityTable;


	EntityTable		m_tEntities;	// vector-of-vector-of-IEntity* (2D table)
	bool			m_bIterating;	// read/write lock
	float			m_fAiTimer;
	bool			m_bAiUpdated;
	unsigned int	m_nCurrAiUnit;

};

