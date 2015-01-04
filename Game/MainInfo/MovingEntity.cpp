#include "MovingEntity.h"
#include "../Grid/Grid.h"
#include "../MainInfo/Game.h"
#include "../MainInfo/AIEntity.h"
#include "../States/GameplayState.h"
#include "../Systems/AnimationSystem.h"
#include "DestroyEntityMessage.h"

#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_MessageSystem.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"


CMovingEntity::CMovingEntity(void)
{
	this->SetHasMoved(false);
	m_bActiveIdle = false;
	m_bActiveAttack = false;
	m_bMoving = false;
	m_bJustCreated = true;
	m_nPathCurrentTile = 1;
	pXA = CSGD_XAudio2::GetInstance();
	m_bConvertingTile = false;
	this->unitConvertStamp.SetOwnerEntity(dynamic_cast<CEntity*>(this));
	this->unitAttackStamp.SetOwnerEntity(dynamic_cast<CEntity*>(this));
	this->unitMoveStamp.SetOwnerEntity(dynamic_cast<CEntity*>(this));
}

CMovingEntity::~CMovingEntity(void)
{
	ClearPath();
}

void CMovingEntity::Initialize(void)
{

}

void CMovingEntity::PlayInjured(void)
{
	CSGD_XAudio2::GetInstance()->SFXPlaySound(injuredSFX);
}

bool CMovingEntity::AttackUnit(CEntity* pTarget, CTile* pTile, CGrid& pMap)
{
	if(abs(pTile->GetPosX()/pMap.GetTileWidth() - GetPosX()/pMap.GetTileWidth()) + abs(pTile->GetPosY()/pMap.GetTileHeight() - GetPosY()/pMap.GetTileHeight()) <= m_nAttackRange)
	{
		pTarget->SetBaseHealth(pTarget->GetBaseHealth() - GetCurrDamage());
		pTarget->SetCurrHealth(pTarget->GetCurrHealth() - GetCurrDamage());
		pTarget->SetDamageTaken(GetCurrDamage());

		if(dynamic_cast<CMovingEntity*>(pTarget))
			dynamic_cast<CMovingEntity*>(pTarget)->PlayInjured();

		pXA->SFXPlaySound(attackSFX);

		m_bActiveAttack = true;
		AnimationSystem::GetInstance()->AddAnimTimeStamps(this->unitAttackStamp);
		this->pTarget = pTarget;

		if (0 >= pTarget->GetCurrHealth())
		{
			pTarget->SetCurrentAnimStamp(pTarget->GetDeathAnimations());
			pTarget->SetDeathAction(true);
			AnimationSystem::GetInstance()->AddAnimTimeStamps(*pTarget->GetDeathAnimation());
		}

		SetUsedAction(true);
		if (this->GetFaction() == OWNER_AI)
		{
			AIEntity::GetInstance()->SetTimesLooped(0);
		}
		return true;
	}

	return false;
}

bool CMovingEntity::MoveToTile(CTile* pTile, CGrid& pMap)
{
	if (pTile->GetOccupyingUnit() || pTile->GetTileType() == MOUNTAIN || pTile->GetTileType() == FOREST || pTile->GetTileType() == VOLCANO)
		return false;

	CGrid& currMap = CGameplayState::GetInstance()->GetCurrentMap();
	int startX, startY, goalX, goalY;

	startX = GetCurrTile()->GetPosX() / currMap.GetTileWidth();
	startY = GetCurrTile()->GetPosY() / currMap.GetTileHeight();

	goalX = pTile->GetPosX() / currMap.GetTileWidth();
	goalY = pTile->GetPosY() / currMap.GetTileHeight();

	ClearPath();
	vPath = currMap.FindPath(startX, startY, goalX, goalY, GetMoveRange(), *this, m_nFaction);

	if(!vPath.empty())
	{
		targetTile = pTile;
		m_nPathCurrentTile = 1;
		this->SetMoving (true);
		GetCurrTile()->SetOccupyingUnit(nullptr);
		nextTile = CGameplayState::GetInstance()->GetGameMap()->GetTileCpy(
			vPath[m_nPathCurrentTile]->GetPosX() / CGameplayState::GetInstance()->GetGameMap()->GetTileWidth(),
			vPath[m_nPathCurrentTile]->GetPosY() / CGameplayState::GetInstance()->GetGameMap()->GetTileHeight());
		nextTile.SetPosX(vPath[m_nPathCurrentTile]->GetPosX() * 64);
		nextTile.SetPosY(vPath[m_nPathCurrentTile]->GetPosY() * 64);
		pXA->SFXPlaySound(moveSFX);

		return true;
	}
	return false;
}

bool CMovingEntity::ConvertTile(CTile* pTile, CGrid& pMap)
{
	CEntity* pUnit = pTile->GetOccupyingUnit();
	if(abs(pTile->GetPosX()/pMap.GetTileWidth() - GetPosX()/pMap.GetTileWidth()) + 
		abs(pTile->GetPosY()/pMap.GetTileHeight() - GetPosY()/pMap.GetTileHeight()) <= m_nConvertRange 
		&& pUnit == nullptr
		|| (pUnit->GetFaction() == GetFaction()) 
		|| (pUnit->GetType() == UNIT_BASE && pUnit->GetFaction() == OWNER_NEUTRAL))
	{
		if(pTile->Convert(this->GetFaction()) == true)
		{	
			if (this->GetFaction() == OWNER_AI)
			{
				AIEntity::GetInstance()->SetTimesLooped(0);
			}
			SetUsedAction(true);
		}

		CGameplayState::GetInstance()->GetGameMap()->Unhighlight();
		return true;
	}

	//////////////////////////////////////////////////////////
	// BUG FIX: 65|84
	// Fixed By: Kenneth Moy
	//////////////////////////////////////////////////////////
	// Begin Fix
	CGameplayState::GetInstance()->GetGameMap()->Unhighlight();
	// End Fix
	return false;
}

void CMovingEntity::ShowAttackRange(CGrid &pMap)
{
	for(int y = 0; y < pMap.GetMapHeight(); y++)
	{
		for(int x = 0; x < pMap.GetMapWidth(); x++)
		{	
			if(abs(x - GetPosX()/pMap.GetTileWidth()) + abs(y - GetPosY()/pMap.GetTileHeight()) <= m_nAttackRange /* && pMap.GetTile(x,y)->GetOccupyingUnit() && pMap.GetTile(x,y)->GetOccupyingUnit()->GetFaction() != GetFaction()*/)
				pMap.GetTile(x,y)->Highlight(D3DCOLOR_ARGB(128, 0, 0, 255));
		}
	}
}

void CMovingEntity::ShowConvertRange(CGrid &pMap)
{
	for(int y = 0; y < pMap.GetMapHeight(); y++)
	{
		for(int x = 0; x < pMap.GetMapWidth(); x++)
		{	
			if(abs(x - GetPosX()/pMap.GetTileWidth()) + abs(y - GetPosY()/pMap.GetTileHeight()) <= m_nConvertRange)
				pMap.GetTile(x,y)->Highlight(D3DCOLOR_ARGB(128, 255, 255, 0));
		}
	}
}

void CMovingEntity::ShowMoveRange(CGrid &pMap)
{
	for(int y = 0; y < pMap.GetMapHeight(); y++)
	{
		for(int x = 0; x < pMap.GetMapWidth(); x++)
		{	
			if(pMap.GetTile(x, y)->GetTileType() == MOUNTAIN)
				continue;

			//ClearPath();

			vector<CNode*> tempPath;

			tempPath = pMap.FindPath(GetPosX()/pMap.GetTileWidth(), GetPosY()/pMap.GetTileHeight(), pMap.GetTile(x, y)->GetPosX()/pMap.GetTileWidth(), pMap.GetTile(x, y)->GetPosY()/pMap.GetTileHeight(), GetMoveRange(), *this, m_nFaction);

			if(!tempPath.empty())
			{
				if (pMap.GetTile(x,y)->GetFactionType() == OWNER_NEUTRAL || pMap.GetTile(x,y)->GetFactionType() == this->GetFaction())
					pMap.GetTile(x,y)->Highlight(D3DCOLOR_ARGB(128, 0, 255, 0));
				else if (pMap.GetTile(x,y)->GetFactionType() != this->GetFaction() && this->GetCurrHealth() - pMap.GetTile(x,y)->GetHpModifier() <= 0)
					pMap.GetTile(x,y)->Highlight(D3DCOLOR_ARGB(128, 255, 0, 0));
				else
					pMap.GetTile(x,y)->Highlight(D3DCOLOR_ARGB(128, 0, 255, 0));
			}

			for (unsigned int i = 0; i < tempPath.size(); i++)
				delete tempPath[i];

			tempPath.clear();
		}
	}
}

void CMovingEntity::UpdateStats()
{
	CGameplayState* pGS =  CGameplayState::GetInstance();
	int plantsBonus, robotsBonus;

	if (pGS->GetTotalTiles())
	{
		plantsBonus = static_cast<int>((float)pGS->GetPlantTiles() / (float)pGS->GetTotalTiles() / 0.2f);
		robotsBonus = static_cast<int>((float)pGS->GetRobotTiles() / (float)pGS->GetTotalTiles() / 0.2f);
	}

	//////////////////////////////////////////////////////////
	// BUG FIX: ??|84
	// Fixed By: Kenneth Moy
	//////////////////////////////////////////////////////////
	// Begin Fix
	if (this->GetMoving() == false)
	{
		if (GetCurrTile()->GetFactionType() == OwnerFaction::OWNER_PLANTS)
		{
			if (GetFaction() == OwnerFaction::OWNER_PLANTS)
			{
				SetCurrHealth(this->GetBaseHealth() + plantsBonus + 2);
				SetCurrDamage(this->GetBaseDamage() + plantsBonus + 2);
			}
			else if (this->GetFaction() == OwnerFaction::OWNER_ROBOTS || this->GetFaction() == OWNER_AI)
			{
				SetCurrHealth(this->GetBaseHealth()+ robotsBonus - 2);
				SetCurrDamage(this->GetBaseDamage()+ robotsBonus - 2);
				if (this->GetCurrDamage() <= 0)
					this->SetCurrDamage(1);
			}
		}
		else if (GetCurrTile()->GetFactionType() == OwnerFaction::OWNER_ROBOTS)
		{
			if (GetFaction() == OwnerFaction::OWNER_ROBOTS || this->GetFaction() == OWNER_AI)
			{
				SetCurrHealth(GetBaseHealth() + 2);
				SetCurrDamage(GetBaseDamage() + 2);
			}
			else if (GetFaction() == OwnerFaction::OWNER_PLANTS)
			{
				SetCurrHealth(GetBaseHealth() - 2);
				SetCurrDamage(GetBaseDamage() - 2);
				if (GetCurrDamage() <= 0)
					SetCurrDamage(1);
			}
		}
		//////////////////////////////////////////////////////////
		// BUG FIX: 2|84
		// Fixed By: Kenneth Moy
		//////////////////////////////////////////////////////////
		// Begin Fix
		else if (GetCurrTile()->GetFactionType() == OWNER_NEUTRAL)
		{
			SetCurrHealth(GetBaseHealth());
			SetCurrDamage(GetBaseDamage());
		}
		// End Fix
	}
	// End Fix

	if (0 >= this->GetCurrHealth())
	{
		this->SetCurrentAnimStamp(this->GetDeathAnimations());
		this->SetDeathAction(true);
	}

}

void CMovingEntity::SetHasMoved(bool moved)
{
	if (moved)
	{
		//Subtract an AP here
		if (this->GetFaction() == CGameplayState::GetInstance()->GetCurrentFaction()
			&& CGameplayState::GetInstance()->GetUnitCreating() == false)
			CGameplayState::GetInstance()->DecrementAP();
		ClearPath();
	}

	m_bHasMoved = moved;
}

/*virtual*/ void CMovingEntity::Update(float dT) /*override*/
{
	if (this->m_bConvertingTile == true)
		unitConvertStamp.Update(dT);

	if (this->m_bMoving == true && vPath.empty() == false)
		//unitMoveStamp.Update(dT);
	{
		if (this->GetCurrTile() != targetTile)
		{
			if (this->GetPosX() == nextTile.GetPosX() && this->GetPosY() == nextTile.GetPosY())
			{
				this->SetTile(CGameplayState::GetInstance()->GetGameMap()->GetTile(
					GetPosX()/CGameplayState::GetInstance()->GetGameMap()->GetTileWidth(), 
					GetPosY()/CGameplayState::GetInstance()->GetGameMap()->GetTileHeight()));

				if (m_nPathCurrentTile != vPath.size())
				{
					++m_nPathCurrentTile;
					if (m_nPathCurrentTile != vPath.size())
					{
						nextTile = CGameplayState::GetInstance()->GetGameMap()->GetTileCpy(
							vPath[m_nPathCurrentTile]->GetPosX() / CGameplayState::GetInstance()->GetGameMap()->GetTileWidth(),
							vPath[m_nPathCurrentTile]->GetPosY() / CGameplayState::GetInstance()->GetGameMap()->GetTileHeight());
						nextTile.SetPosX(vPath[m_nPathCurrentTile]->GetPosX() * 64);
						nextTile.SetPosY(vPath[m_nPathCurrentTile]->GetPosY() * 64);
					}
				}
			}
			else if (this->GetPosX() == nextTile.GetPosX())
			{
				if (this->GetPosY() > nextTile.GetPosY())
					this->SetY(int(this->GetPosY() - 300 * dT));
				else if (this->GetPosY() < nextTile.GetPosY())
					this->SetY(int(this->GetPosY() + 300 * dT));
			}
			else if (this->GetPosY() == nextTile.GetPosY())
			{
				if (this->GetPosX() > nextTile.GetPosX())
					this->SetX(int(this->GetPosX() - 300 * dT));
				else if (this->GetPosX() < nextTile.GetPosX())
					this->SetX(int(this->GetPosX() + 300 * dT));
			}
		}
		else if (this->GetCurrTile() == targetTile)
		{
			vPath.clear();
			this->SetMoving(false);
			this->SetHasMoved(true);
			m_nPathCurrentTile = 1;
			targetTile->SetOccupyingUnit(this);
			CGameplayState::GetInstance()->Deselect();
			if (this->GetFaction() == OWNER_AI)
			{
				AIEntity::GetInstance()->SetTimesLooped(0);
			}
		}
	}

	this->UpdateStats();

	CEntity::Update(dT);
}

/*virtual*/ void CMovingEntity::Render() /*override*/
{
	if (m_bConvertingTile == true)
		AnimationSystem::GetInstance()->Render(this, unitConvertStamp);


	CEntity::Render();
}

//If a stored path exists, clear it and return true. Otherwise, simply return false
bool CMovingEntity::ClearPath(void)
{
	if(!vPath.empty())
	{
		for (unsigned int i = 0; i < vPath.size(); i++)
			delete vPath[i];

		vPath.clear();
		return true;
	}

	return false;
}

bool CMovingEntity::UndoMove(void)
{
	GetCurrTile()->SetOccupyingUnit(nullptr);
	GetPreviousTile()->SetOccupyingUnit(this);

	SetTile(GetPreviousTile());
	SetX(GetPreviousTile()->GetPosX());
	SetY(GetPreviousTile()->GetPosY());

	SetHasMoved(false);
	ClearPath();
	return true;
}

void CMovingEntity::SetUsedAction (bool hasUsed)
{
	ClearPath();
	CEntity::SetUsedAction(hasUsed);
}
