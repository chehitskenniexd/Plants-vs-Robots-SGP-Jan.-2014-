/*************************************************************
|	File:		EntityManager.cpp
|	Purpose:	To store & maintain game entities
|
|	Author:		Doug Monroe (dmonroe@fullsail.com)
|	Course:		SGD 1312
*************************************************************/

#include "EntityManager.h"

#include "IEntity.h"
#include "../States/GameplayState.h"
#include "AIEntity.h"

#include <cassert>


//************************************************************
// CONSTRUCTOR
CEntityManager::CEntityManager( void )
{
	m_bIterating = false;
	m_fAiTimer = 50.0f;
	m_bAiUpdated = false;
	m_nCurrAiUnit = 0;
}

//************************************************************
// DESTRUCTOR
CEntityManager::~CEntityManager( void )
{
	// Validate the iteration state
	assert( m_bIterating == false && "~CEntityManager - cannot be destroyed while iterating!" );
}


//************************************************************
// AddEntity
//	- store the entity into the desired bucket (row)
//	- holding a reference to the entity (to keep it in memory)
void CEntityManager::AddEntity( IEntity* pEntity, unsigned int unBucket )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::AddEntity - cannot add while iterating" );

	// Validate the parameter
	assert( pEntity != nullptr && "CEntityManager::AddEntity - cannot add a null pointer!" );


	// Resize the vector if necessary
	if( unBucket >= m_tEntities.size() )
		m_tEntities.resize( unBucket + 1 );


	// Append the new entity
	m_tEntities[ unBucket ].push_back( pEntity );
	pEntity->AddRef();
}


//************************************************************
// RemoveEntity
//	- remove the entity from the specified bucket
//	- release the reference to the entity
void CEntityManager::RemoveEntity( IEntity* pEntity, unsigned int unBucket )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RemoveEntity - cannot remove while iterating" );

	// Validate the parameters
	assert( pEntity != nullptr && "CEntityManager::RemoveEntity - cannot remove NULL" );
	assert( unBucket < m_tEntities.size() && "CEntityManager::RemoveEntity - invalid bucket" );


	// Try to find the entity
	EntityVector& vec = m_tEntities[ unBucket ];
	for( unsigned int i = 0; i < vec.size(); i++ )
	{
		if( vec[ i ] == pEntity )
		{
			// Remove the entity
			vec.erase( vec.begin() + i );
			pEntity->Release();
			break;
		}
	}
}


//************************************************************
// RemoveEntity
//	- remove & release the entity from any bucket
void CEntityManager::RemoveEntity( IEntity* pEntity )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RemoveEntity - cannot remove while iterating" );

	// Validate the parameters
	assert( pEntity != nullptr && "CEntityManager::RemoveEntity - pointer cannot be null" );


	// Try to find the entity in any buckect
	for( unsigned int bucket = 0; bucket < m_tEntities.size(); bucket++ )
	{
		EntityVector& vec = m_tEntities[ bucket ];
		for( unsigned int i = 0; i < vec.size(); i++ )
		{
			if( vec[ i ] == pEntity )
			{
				// Remove the entity
				vec.erase( vec.begin() + i );
				pEntity->Release();
				return;
			}
		}
	}
}


//************************************************************
// RemoveAll
//	- remove all entities from a specific bucket
void CEntityManager::RemoveAll( unsigned int unBucket )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RemoveAll - cannot remove while iterating" );

	// Validate the parameter
	assert( unBucket < m_tEntities.size() && "CEntityManager::RemoveAll - invalid bucket" );


	// Lock the iterator
	m_bIterating = true;
	{
		// Release the reference to EVERY entity
		EntityVector& vec = m_tEntities[ unBucket ];
		for( unsigned int i = 0; i < vec.size(); i++ )
		{
			vec[ i ]->Release();
			vec[ i ] = nullptr;
		}

		vec.clear();
	}
	// Unlock the iterator
	m_bIterating = false;
}


//************************************************************
// RemoveAll
//	- RELEASE the reference to every entity in the table
void CEntityManager::RemoveAll( void )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RemoveAll - cannot remove while iterating" );


	// Lock the iterator
	m_bIterating = true;
	{
		// Iterate through the table
		for( unsigned int i = 0; i < m_tEntities.size(); i++ )
		{
			// Reference the current bucket
			EntityVector& vec = m_tEntities[ i ];


			// Iterate through the bucket
			for( unsigned int j = 0; j < vec.size(); j++ )
			{
				vec[ j ]->Release();
				vec[ j ] = nullptr;
			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;


	m_tEntities.clear();
}


//************************************************************
// UpdateAll
//	- update every entity in every bucket
void CEntityManager::UpdateAll( float fElapsedTime )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::UpdateAll - cannot update while iterating" );


	// Lock the iterator
	m_bIterating = true;
	{
		// Iterate through the table
		for( unsigned int i = 0; i < m_tEntities.size(); i++ )
		{
			// Reference the current bucket
			EntityVector& vec = m_tEntities[ i ];

#pragma region AIEntity Vectors
			AIEntity::GetInstance()->SetAiBases()->clear();
			AIEntity::GetInstance()->SetAiUnits()->clear();
			AIEntity::GetInstance()->SetPlayerBases()->clear();
			AIEntity::GetInstance()->SetPlayerUnits()->clear();

			for (unsigned int j = 0; j < vec.size(); ++j)
			{
				if (dynamic_cast<CEntity*>(vec[j])->GetFaction() == OWNER_AI)
				{
					if (vec[j]->GetType() != UNIT_BASE)
					{
						AIEntity::GetInstance()->SetAiUnits()->push_back(vec[j]);
					}

					else if (vec[j]->GetType() == UNIT_BASE)
					{
						if (dynamic_cast<CEntity*>(vec[j])->GetFaction() != OWNER_NEUTRAL)
							AIEntity::GetInstance()->SetAiBases()->push_back(vec[j]);
					}
				}
				else if (dynamic_cast<CEntity*>(vec[j])->GetFaction() != OWNER_AI)
				{
					if (vec[j]->GetType() != UNIT_BASE)
					{
						AIEntity::GetInstance()->SetPlayerUnits()->push_back(vec[j]);
					}

					else if (vec[j]->GetType() == UNIT_BASE)
					{
						if (dynamic_cast<CEntity*>(vec[j])->GetFaction() != OWNER_NEUTRAL)
							AIEntity::GetInstance()->SetPlayerBases()->push_back(vec[j]);
					}
				}
			}
			if (m_nCurrAiUnit >= AIEntity::GetInstance()->GetAiUnits().size())
				m_nCurrAiUnit = 0;
#pragma endregion

			// Iterate through the bucket
			for( unsigned int j = 0; j < vec.size(); j++ )
			{
				vec[ j ]->Update( fElapsedTime );
			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}

//************************************************************
// RenderAll
//	- render every entity in every bucket
void CEntityManager::RenderAll( void )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RenderAll - cannot render while iterating" );


	// Lock the iterator
	m_bIterating = true;
	{
		// Iterate through the table
		for( unsigned int i = 0; i < m_tEntities.size(); i++ )
		{
			// Reference the current bucket
			EntityVector& vec = m_tEntities[ i ];


			// Iterate through the bucket
			for( unsigned int j = 0; j < vec.size(); j++ )
			{
				vec[ j ]->Render();
			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}


//************************************************************
// CheckAllCollisions
//	- check collision between the entities within the two buckets
void CEntityManager::CheckAllCollisions( unsigned int unBucket1, unsigned int unBucket2 )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::CheckAllCollisions - cannot collide while iterating" );

	// Quietly validate the parameters
	if( unBucket1 >= m_tEntities.size() 
		|| unBucket2 >= m_tEntities.size()
		|| m_tEntities[ unBucket1 ].size() == 0 
		|| m_tEntities[ unBucket2 ].size() == 0 )
		return;


	// Lock the iterator
	m_bIterating = true;
	{
		// Are they different buckets?
		if( unBucket1 != unBucket2 )
		{
			// Which bucket is smaller?
			//	should be the outer loop for less checks (n0)*(n1+1) + 1
			EntityVector* pVec1 = &m_tEntities[ unBucket1 ];
			EntityVector* pVec2 = &m_tEntities[ unBucket2 ];

			if( pVec2->size() < pVec1->size() )
			{
				EntityVector* pTemp = pVec1;
				pVec1 = pVec2;
				pVec2 = pTemp;
			}

			// Iterate through the smaller bucket
			for( int i = (int)(pVec1->size()-1); i >= 0; i-- )
			{
				// Iterate through the larger bucket
				for( int j = (int)(pVec2->size()-1); j >= 0; j-- )
				{
					// Helper pointers
					IEntity* pEntity1 = (*pVec1)[ i ];
					IEntity* pEntity2 = (*pVec2)[ j ];

					// Ignore self-collision
					if( pEntity1 == pEntity2 )
						continue;


					// Local variables help with debugging
					RECT rOverlap = { };
					RECT rEntity1 = pEntity1->GetRect();
					RECT rEntity2 = pEntity2->GetRect();

					// Check for collision between the entities
					if( IntersectRect( &rOverlap, &rEntity1, &rEntity2 ) == TRUE )
					{
						pEntity1->HandleCollision( pEntity2 );
						pEntity2->HandleCollision( pEntity1 );
					}
				}
			}
		}
		else // unBucket1 == unBucket2
		{
			EntityVector& vec = m_tEntities[ unBucket1 ];

			// Optimized loop to ensure objects do not collide with
			// each other twice
			for( int i = (int)(vec.size()-1); i >= 1; i-- )
			{
				for( int j = i-1; j >= 0; j-- )
				{
					// Helper pointers
					IEntity* pEntity1 = vec[ i ];
					IEntity* pEntity2 = vec[ j ];

					// Ignore self-collision
					if( pEntity1 == pEntity2 )
						continue;


					// Local variables help with debugging
					RECT rOverlap = { };
					RECT rEntity1 = pEntity1->GetRect();
					RECT rEntity2 = pEntity2->GetRect();

					// Check for collision between the entities
					if( IntersectRect( &rOverlap, &rEntity1, &rEntity2 ) == TRUE )
					{
						pEntity1->HandleCollision( pEntity2 );
						pEntity2->HandleCollision( pEntity1 );
					}
				}
			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}

std::vector<IEntity*>	CEntityManager::GetBucket (int pBucket)
{
	return m_tEntities[pBucket];
}
