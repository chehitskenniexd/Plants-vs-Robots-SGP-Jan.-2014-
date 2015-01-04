/*************************************************************
|	File:		Entity.cpp
|	Purpose:	To store data for velocity-based entities,
|				& is the base class for all game entities
|
|	Author:		Doug Monroe (dmonroe@fullsail.com)
|	Course:		SGD 1312
*************************************************************/

#include "Entity.h"
#include "../Grid/Tile.h"
#include "../States/GameplayState.h"
#include "../MainInfo/Game.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_MessageSystem.h"
#include "../MainInfo/DestroyEntityMessage.h"
#include "../MainInfo/BMFont.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../Systems/AnimationSystem.h"
#include "../MainInfo/MovingEntity.h"
#include "AIEntity.h"

#include <cassert>

//************************************************************
// CONSTRUCTOR
//	- set ALL the data members
CEntity::CEntity( void )
{
	// Set the entity type
	m_eType		= UNIT_BASE;

	// Set the other members
	m_nImageID	= -1;

	m_nMapX = 0;
	m_nMapY = 0;

	m_nWidth	= 1;
	m_nHeight	= 1;

	m_pCurrentPlayer = nullptr;
	m_pCurrentTile = nullptr;
	m_nBaseHealth = 0;
	m_nCurrentHealth = 0;
	m_nCurrentAction = ACT_NONE;
	m_bUsedAction = false;
	m_bDeathAction = false;
	m_bIsDying = false;
	m_aIdleAnimation.SetOwnerEntity(this);
	m_aDeathAnimation.SetOwnerEntity(this);
	m_aDeathAnimation.SetCurrentAnimation("Explosion");
	m_sCurrentState = NON_AI;

	// Allocating the object is the 'prime' reference
	m_unRefCount	= 1;
	damageTaken = -1;
	damageTakenPos = 0;

	CSGD_EventSystem::GetInstance()->RegisterClient("TRAP", this);
}

//************************************************************
// DESTRUCTOR
CEntity::~CEntity( void )
{
	CSGD_EventSystem::GetInstance()->UnregisterClientAll(this);
}


//************************************************************
// Update
//	- move the entity by its velocity (pixels-per-second)
/*virtual*/ void CEntity::Update( float fElapsedTime )
{
	if (m_bDeathAction == false && this->m_bUsedAction == false)
	{
		m_aIdleAnimation.Update(fElapsedTime);
	}

	if (this->m_nFaction == OWNER_AI
		&& this->m_sCurrentState != NON_AI)
		AIEntity::GetInstance()->Update(fElapsedTime, this);

	//////////////////////////////////////////////////////////
	// BUG FIX: 69|84 
	// BUG FIX: 87|84
	// Fixed By: Kenneth Moy
	//////////////////////////////////////////////////////////
	// Begin Fix
	if (m_bDeathAction == true && m_bIsDying == false)
	{
		AnimationSystem::GetInstance()->AddAnimTimeStamps(m_aDeathAnimation);
		CGameplayState::GetInstance()->GetGameMap()->Unhighlight();
		CGameplayState::GetInstance()->SetSelectedUnit(nullptr);
		CGameplayState::GetInstance()->SetCurrentAction(CGameplayState::CurrentCommand::NONE);
		m_bIsDying = true;
	}
	// End Fix
}

//************************************************************
// Render
//	- draw the entity's image at its position
/*virtual*/ void CEntity::Render( void )
{
	CGameplayState* pGS = CGameplayState::GetInstance();
	// Validate the image
	assert( m_nImageID != -1 && "CEntity::Render - image was not set" );
	DWORD renderColor;



	if(GetCurrTile()->GetPosX() < CGame::GetInstance()->GetScreenWidth() - pGS->OffsetX() && GetCurrTile()->GetPosY() < CGame::GetInstance()->GetScreenHeight() - pGS->OffsetY())
	{
		if (m_bUsedAction)
			renderColor = D3DCOLOR_XRGB(128,128, 128);
		else
			renderColor = D3DCOLOR_XRGB(255, 255, 255);

		int nRenderX = m_pCurrentTile->GetPosX() + pGS->OffsetX();
		int nRenderY = m_pCurrentTile->GetPosY() + pGS->OffsetY();

		// Draw the image
		//CSGD_TextureManager::GetInstance()->Draw(
		//	m_nImageID, nRenderX, nRenderY,
		//	1.0f, 1.0f, nullptr,
		//	(float)(GetWidth()/2), (float)(GetHeight()/2), 0, renderColor);
		if (m_bDeathAction == false)
		{
			if (this->GetUnitType() != UNIT_BASE)
			{
				if (dynamic_cast<CMovingEntity*>(this)->GetConvertingTile() == false)
					AnimationSystem::GetInstance()->Render(this, m_aIdleAnimation);
			}
			else if (this->GetUnitType() == UNIT_BASE)
				AnimationSystem::GetInstance()->Render(this, m_aIdleAnimation);
		}

		if(damageTaken >= 0)
		{
			ostringstream dmg;
			dmg << damageTaken;

			CGame::GetInstance()->GetFont()->Write(dmg.str().c_str(), GetPosX() + pGS->OffsetX() + 32 + int(damageTakenPos), GetPosY() + 15 + pGS->OffsetY() - int(damageTakenPos/2), 1.3f, D3DCOLOR_ARGB(225, 255, 255, 255));
			damageTakenPos += 0.1f;

			if(damageTakenPos > 50)
			{
				damageTaken = -1;
				damageTakenPos = 0;
			}
		}
	}
}


//************************************************************
// GetRect
//	- returns the bounding rectangle
/*virtual*/ RECT CEntity::GetRect( void ) const
{
	RECT rBoundingBox = 
	{
		(LONG)GetPosX(),					// left
		(LONG)GetPosY(),					// top
		(LONG)(GetPosX() + GetWidth()),		// right
		(LONG)(GetPosY() + GetHeight())		// bottom
	};

	return rBoundingBox;
}


//************************************************************
// HandleCollision
//	- generic collision response between objects
/*virtual*/ void CEntity::HandleCollision( const IEntity* pOther )
{
	UNREFERENCED_PARAMETER( pOther );// IGNORED
}

/*virtual*/ void CEntity::HandleEvent( const CEvent* pEvent ) /*override*/
{
	//if (pEvent->GetEventID() == "TRAP" && pEvent->GetDestination() == this)
	//{
	//	CSGD_MessageSystem::GetInstance()->QueueMessage(new CDestroyEntityMessage(this));
	//}
}

//************************************************************
// AddRef
//	- increase the reference count
/*virtual*/ void CEntity::AddRef( void )	/*final*/
{
	// Validate the reference count
	assert( m_unRefCount != UINT_MAX && "CEntity::AddRef - maximum reference count exceeded" );

	++m_unRefCount;
}

//************************************************************
// Release
//	- decrease the reference count
//	- self-destruct when count reaches 0!!!
/*virtual*/ void CEntity::Release( void )	/*final*/
{
	--m_unRefCount;

	if( m_unRefCount == 0 )
		delete this;
}

void CEntity::SetUsedAction(bool hasUsed) 
{
	if (hasUsed)
	{
		//if (CGameplayState::GetInstance()->GetUnitCreating() == false)
		//{
		//	if (this->GetUnitType() == UNIT_BASE 
		//		|| (this->GetUnitType() != UNIT_BASE && dynamic_cast<CMovingEntity*>(this)->GetJustCreated() == false))
		//		CGameplayState::GetInstance()->DecrementActiveUnits();
		//}
		if(this->GetUnitType() == UNIT_BASE
			|| (this->GetUnitType() != UNIT_BASE && dynamic_cast<CMovingEntity*>(this)->GetJustCreated() == false))
			CGameplayState::GetInstance()->DecrementAP();
	}

	m_aIdleAnimation.SetCurrentFrame(0);
	m_bUsedAction = hasUsed;
}