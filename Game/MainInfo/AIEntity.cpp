#include "AIEntity.h"
#include "Entity.h"
#include "../States/GameplayState.h"
#include "../SGD Wrappers/CSGD_MessageSystem.h"

//************************************************************
// GetInstance
//	- allocate static global instance
//	- return THE instance
/*static*/ AIEntity* AIEntity::GetInstance( void )
{
	static AIEntity s_Instance;
	return &s_Instance;
}

AIEntity::AIEntity(void)
{
	targetUnit = nullptr;
	closestUnit = nullptr;
	closestBase = nullptr;
	timesLooped = 0;
}

AIEntity::~AIEntity(void)
{
	m_vAiUnits.clear();
	m_vAiBases.clear();
	m_vPlayerUnits.clear();
	m_vPlayerBases.clear();
}

void AIEntity::Update (float fElapsedTime, CEntity* currentEntity)
{
#pragma region Set closest unit / closest base for current entity
	targetUnit = nullptr;
	closestUnit = nullptr;
	closestBase = nullptr;

	if  (m_vPlayerUnits.empty() == false)
	{
		for (unsigned int i = 0; i < m_vPlayerUnits.size(); ++i)
		{
			if (closestUnit == nullptr)
				closestUnit = dynamic_cast<CEntity*>(m_vPlayerUnits[i]);
			else if (closestUnit != nullptr)
			{
				if (abs(dynamic_cast<CEntity*>(m_vPlayerUnits[i])->GetPosX() - currentEntity->GetPosX()) +
					abs(dynamic_cast<CEntity*>(m_vPlayerUnits[i])->GetPosY() - currentEntity->GetPosY()) <
					abs(closestUnit->GetPosX() - currentEntity->GetPosX()) + abs(closestUnit->GetPosY() - currentEntity->GetPosY()))
					closestUnit = dynamic_cast<CEntity*>(m_vPlayerUnits[i]);
			}
		}
	}
	else if (m_vPlayerUnits.empty() == true)
		closestUnit = nullptr;

	if (m_vPlayerBases.empty() == false)
	{
		for (unsigned int i = 0; i < m_vPlayerBases.size(); ++i)
		{
			if (closestBase == nullptr)
				closestBase = dynamic_cast<CEntity*>(m_vPlayerBases[i]);
			else if (closestBase != nullptr)
			{

				if (abs(dynamic_cast<CEntity*>(m_vPlayerBases[i])->GetPosX() - currentEntity->GetPosX()) +
					abs(dynamic_cast<CEntity*>(m_vPlayerBases[i])->GetPosY() - currentEntity->GetPosY()) <
					abs(closestBase->GetPosX() - currentEntity->GetPosX()) + abs(closestBase->GetPosY() - currentEntity->GetPosY())
					//////////////////////////////////////////////////////////
					// BUG FIX: 55|84
					// Fixed By: Kenneth Moy
					//////////////////////////////////////////////////////////
					// Begin Fix
					&& dynamic_cast<CEntity*>(m_vPlayerBases[i])->GetFaction() != OWNER_PLANTS)
					// End Fix
					closestUnit = dynamic_cast<CEntity*>(m_vPlayerBases[i]);
			}
		}
	}
	else if (m_vPlayerBases.empty() == true)
		closestBase = nullptr;
#pragma endregion
}

void AIEntity::UpdateStates (float fElapsedTime)
{
	// Reset all AI to Normal
	if (m_vAiBases.empty() == false)
	{
		for( unsigned int i = 0; i < m_vAiBases.size(); ++i)
		{
			dynamic_cast<CEntity*>(m_vAiBases[i])->SetCurrentAiState(NORMAL_AI);
		}
	}
	if (m_vAiUnits.empty() == false)
	{
		for( unsigned int i = 0; i < m_vAiUnits.size(); ++i)
		{
			dynamic_cast<CEntity*>(m_vAiUnits[i])->SetCurrentAiState(NORMAL_AI);
		}
	}
	// Set a defensive AI State
	if (m_vAiBases.empty () != false
		&& m_vAiUnits.empty() != false)
	{
#pragma region Defensive AI State for Bases and Units
		if (m_vAiBases.empty() == false)
		{
			for (unsigned int i = 0; i < m_vAiBases.size(); ++i)
			{
				CStaticEntity* currentBase = dynamic_cast<CStaticEntity*>(m_vAiBases[i]);
				if (currentBase->GetCurrHealth() < 4)
				{
					currentBase->SetCurrentAiState(DEFENSE_AI);
					if (m_vAiUnits.empty() == false)
					{
						for (unsigned int i = 0; i <m_vAiUnits.size(); ++i)
						{
							dynamic_cast<CEntity*>(m_vAiUnits[i])->SetCurrentAiState(DEFENSE_AI);
						}
					}
					break;
				}
				else if (currentBase->GetCurrHealth() > 4)
				{
					currentBase->SetCurrentAiState(NORMAL_AI);
					for (unsigned int i = 0; i < m_vPlayerUnits.size(); ++i)
					{
						CMovingEntity* currentUnit = dynamic_cast<CMovingEntity*>(m_vPlayerUnits[i]);
						if (abs(currentUnit->GetPosX()/CGameplayState::GetInstance()->GetGameMap()->GetTileWidth() -
							currentBase->GetPosX()/CGameplayState::GetInstance()->GetGameMap()->GetTileWidth()) +
							abs(currentUnit->GetPosY()/CGameplayState::GetInstance()->GetGameMap()->GetTileHeight() - 
							currentBase->GetPosY())/CGameplayState::GetInstance()->GetGameMap()->GetTileHeight() <= 6)
						{
							currentBase->SetCurrentAiState(DEFENSE_AI);
							if (m_vAiUnits.empty() == false)
							{
								for (unsigned int i = 0; i <m_vAiUnits.size(); ++i)
								{
									dynamic_cast<CEntity*>(m_vAiUnits[i])->SetCurrentAiState(DEFENSE_AI);
								}
							}
							break;
						}
					}
				}
			}
		}

#pragma endregion
#pragma region Aggressive State for AI for Units
		if (m_vPlayerUnits.empty() == false)
		{
			for (unsigned int i = 0; i < m_vPlayerUnits.size(); ++i)
			{
				if(dynamic_cast<CEntity*>(m_vPlayerUnits[i])->GetCurrHealth() <= 4)
				{
					for (unsigned int i = 0; i < m_vAiUnits.size(); ++i)
					{
						//dynamic_cast<CMovingEntity*>(m_vAiUnits[i])->SetTargetUnit(dynamic_cast<CEntity*>(m_vPlayerUnits[i]));
						dynamic_cast<CMovingEntity*>(m_vAiUnits[i])->SetCurrentAiState(AGGRESSIVE_AI);
					}
				}
			}
		}
#pragma endregion
#pragma region Retreat AI State for Units
		if (m_vPlayerBases.empty() == false)
		{
			for (unsigned int i = 0; i < m_vAiUnits.size(); ++i)
			{
				if (dynamic_cast<CEntity*>(m_vAiUnits[i])->GetCurrHealth() <= 2)
					dynamic_cast<CEntity*>(m_vAiUnits[i])->SetCurrentAiState(RETREAT_AI);
			}
		}
#pragma endregion
	}
}

void AIEntity::AiCreateUnit(CStaticEntity* currentEntity)
{
	int randomUnit = rand()%3;
	int randomPlace = rand()%4;
	CTile* targetTile = nullptr;

	switch (randomPlace)
	{
	case 0: // North
		{
			targetTile = CGameplayState::GetInstance()->GetGameMap()->GetTile(
				(currentEntity->GetCurrTile()->GetPosX() / CGameplayState::GetInstance()->GetGameMap()->GetTileWidth()),
				(currentEntity->GetCurrTile()->GetPosY() / CGameplayState::GetInstance()->GetGameMap()->GetTileHeight()) - 1);
		}
		break;
	case 1: // West
		{
			targetTile = CGameplayState::GetInstance()->GetGameMap()->GetTile(
				(currentEntity->GetCurrTile()->GetPosX() / CGameplayState::GetInstance()->GetGameMap()->GetTileWidth()) - 1,
				(currentEntity->GetCurrTile()->GetPosY() / CGameplayState::GetInstance()->GetGameMap()->GetTileHeight()));
		}
		break;
	case 2: // East
		{
			targetTile = CGameplayState::GetInstance()->GetGameMap()->GetTile(
				(currentEntity->GetCurrTile()->GetPosX() / CGameplayState::GetInstance()->GetGameMap()->GetTileWidth()) + 1,
				(currentEntity->GetCurrTile()->GetPosY() / CGameplayState::GetInstance()->GetGameMap()->GetTileHeight()));
		}
		break;
	case 3: //South
		{
			targetTile = CGameplayState::GetInstance()->GetGameMap()->GetTile(
				(currentEntity->GetCurrTile()->GetPosX() / CGameplayState::GetInstance()->GetGameMap()->GetTileWidth()),
				(currentEntity->GetCurrTile()->GetPosY() / CGameplayState::GetInstance()->GetGameMap()->GetTileHeight()) + 1);
		}
		break;
	default:
		break;
	}

	if (targetTile != nullptr
		&& currentEntity->CheckCreateRange(targetTile, CGameplayState::GetInstance()->GetCurrentMap()) == true)
	{
		switch(randomUnit)
		{
		case 0: // Create an Infantry
			{
				currentEntity->CreateUnit(UnitType::UNIT_INFANTRY, currentEntity->GetFaction(), targetTile);
			}
			break;
		case 1: // Create a Scout
			{
				currentEntity->CreateUnit(UnitType::UNIT_SCOUT, currentEntity->GetFaction(), targetTile);
			}
			break;
		case 2: // Create a Tank
			{
				currentEntity->CreateUnit(UnitType::UNIT_TANK, currentEntity->GetFaction(), targetTile);
			}
			break;
		default:
			break;
		}
	}
}

bool AIEntity::InAttackRange(CEntity* target, CMovingEntity* currentUnit, CGrid& map)
{
	if (abs(target->GetPosX()/map.GetTileWidth() - currentUnit->GetPosX()/map.GetTileWidth())
		+ abs(target->GetPosY()/map.GetTileHeight() - currentUnit->GetPosY()/map.GetTileHeight())
		<= currentUnit->GetAttackRange())
		return true;

	return false;
}

bool AIEntity::InMoveRange (CMovingEntity* currentUnit, CTile* targetTile, CGrid &map)
{
	//////////////////////////////////////////////////////////
	// BUG FIX: 75|84
	// Fixed By: Kenneth Moy
	//////////////////////////////////////////////////////////
	// Begin Fix
	if (abs(currentUnit->GetPosX()/map.GetTileWidth() - targetTile->GetPosX()/map.GetTileWidth()) + 
		abs(currentUnit->GetPosY()/map.GetTileHeight() - targetTile->GetPosY()/map.GetTileHeight()) <
		currentUnit->GetMoveRange()/2
		&& targetTile->GetOccupyingUnit() == nullptr)
		return true;
	// End Fix
	return false;
}

void AIEntity::AiTimerUpdate (int numberUnits)
{
	if (numberUnits <= 10 && numberUnits > 0)
	{
		updateAiTimer = 7.5f/numberUnits;
	}
	else if (numberUnits <= 20 && numberUnits > 10)
	{
		updateAiTimer = 15.0f/numberUnits;
	}
	else if (numberUnits <= 30 && numberUnits > 20)
	{
		updateAiTimer = 22.5f/numberUnits;
	}
}

void AIEntity::AiChooseActions ()
{
	if (timesLooped >= 10)
	{
		timesLooped = 0;
		CGameplayState::GetInstance()->EndTurn();
		return;
	}
	int baseAction = rand()%10;

	// If baseAction == 0, create a unit with a base
	if (baseAction == 0 && m_vAiBases.empty() == false)
	{
		int base = rand()% m_vAiBases.size();
		CStaticEntity* currentBase = dynamic_cast<CStaticEntity*>(m_vAiBases[base]);
		if (currentBase->GetUsedAction() == true)
		{
			++timesLooped;
			return;
		}

		switch (currentBase->GetCurrentAiState())
		{
		case NORMAL_AI:
		case DEFENSE_AI:
		case AGGRESSIVE_AI:
		case RETREAT_AI:
			AiBaseCreate(currentBase);
			break;
		default:
			break;
		}
	}
	else if (baseAction != 0 && m_vAiUnits.empty() == false
		|| baseAction == 0 && m_vAiBases.empty() == true)
	{
		int unit = rand()% m_vAiUnits.size();
		CMovingEntity* currentEntity = dynamic_cast<CMovingEntity*>(m_vAiUnits[unit]);

		// Check to make sure the unit hasn't used an action & isn't nullptr
		if (currentEntity != nullptr)
		{
			if (currentEntity->GetUsedAction() == true)
			{
				++timesLooped;
				return;
			}

			switch (currentEntity->GetCurrentAiState())
			{
			case NORMAL_AI:
				NormalAiActions(currentEntity);
				break;
			case DEFENSE_AI:
				DefensiveAiActions(currentEntity);
				break;
			case AGGRESSIVE_AI:
				AggressiveAiActions(currentEntity);
				break;
			case RETREAT_AI:
				RetreatAiActions(currentEntity);
				break;
			default:
				break;
			}
		}
	}
	++timesLooped;
}

void AIEntity::AiChooseActions (CEntity* currentEntity)
{
	if (timesLooped >= 10)
	{
		timesLooped = 0;
		CGameplayState::GetInstance()->EndTurn();
		return;
	}
	int baseAction = rand()%10;

	// If baseAction == 0, create a unit with a base
	if (baseAction == 0 && m_vAiBases.empty() == false)
	{
		int base = rand()% m_vAiBases.size();
		CStaticEntity* currentBase = dynamic_cast<CStaticEntity*>(m_vAiBases[base]);
		if (currentBase->GetUsedAction() == true)
		{
			++timesLooped;
			return;
		}

		switch (currentBase->GetCurrentAiState())
		{
		case NORMAL_AI:
		case DEFENSE_AI:
		case AGGRESSIVE_AI:
		case RETREAT_AI:
			AiBaseCreate(currentBase);
			break;
		default:
			break;
		}
	}
	else if (baseAction != 0 && m_vAiUnits.empty() == false
		|| baseAction == 0 && m_vAiBases.empty() == true)
	{
		// Check to make sure the unit hasn't used an action & isn't nullptr
		if (currentEntity != nullptr)
		{
			if (currentEntity->GetUsedAction() == true)
			{
				++timesLooped;
				return;
			}

			switch (currentEntity->GetCurrentAiState())
			{
			case NORMAL_AI:
				NormalAiActions(dynamic_cast<CMovingEntity*>(currentEntity));
				break;
			case DEFENSE_AI:
				DefensiveAiActions(dynamic_cast<CMovingEntity*>(currentEntity));
				break;
			case AGGRESSIVE_AI:
				AggressiveAiActions(dynamic_cast<CMovingEntity*>(currentEntity));
				break;
			case RETREAT_AI:
				RetreatAiActions(dynamic_cast<CMovingEntity*>(currentEntity));
				break;
			default:
				break;
			}
		}
	}
	++timesLooped;
}

void AIEntity::AiBaseCreate (CStaticEntity* currentBase)
{
	int tileDirection;
	int randDir = rand()%2;
	if (randDir == 0)
		tileDirection = -1;
	else if (randDir == 1)
		tileDirection = 1;

	CTile* targetTile = nullptr;
	switch(randDir)
	{
	case 0: //Top and Bottom
		targetTile = CGameplayState::GetInstance()->GetGameMap()->GetTile(
			(currentBase->GetCurrTile()->GetPosX() / CGameplayState::GetInstance()->GetGameMap()->GetTileWidth()),
			currentBase->GetCurrTile()->GetPosY() / CGameplayState::GetInstance()->GetGameMap()->GetTileHeight()  + tileDirection);
		break;
	case 1: //Right and Left
		targetTile = CGameplayState::GetInstance()->GetGameMap()->GetTile(
			(currentBase->GetCurrTile()->GetPosX() / CGameplayState::GetInstance()->GetGameMap()->GetTileWidth()) + tileDirection,
			currentBase->GetCurrTile()->GetPosY() / CGameplayState::GetInstance()->GetGameMap()->GetTileHeight());
		break;
	default:
		break;
	}

	int randomUnit = rand()%3;
	if (targetTile != nullptr)
	{
		switch(randomUnit)
		{
		case 0: // Infantry 
			currentBase->CreateUnit(UNIT_INFANTRY, OWNER_AI, targetTile);
			break;
		case 1: // Scout
			currentBase->CreateUnit(UNIT_SCOUT, OWNER_AI, targetTile);
			break;
		case 2: // Tank
			currentBase->CreateUnit(UNIT_TANK, OWNER_AI, targetTile);
			break;
		default:
			break;
		}
	}
}

CEntity* AIEntity::ClosestLowPlayerUnit (CMovingEntity* currentEntity)
{
	CEntity* lowHpUnit = nullptr;
	if (currentEntity->GetCurrentAiState() == AGGRESSIVE_AI)
	{
		// closestUnit must have less than or equal to 4 HP for targeting
		for (unsigned int i = 0; i < m_vPlayerUnits.size(); ++i)
		{
			if (lowHpUnit == nullptr
				&& dynamic_cast<CEntity*>(m_vPlayerUnits[i])->GetCurrHealth() <= 4)
			{
				lowHpUnit = dynamic_cast<CEntity*>(m_vPlayerUnits[i]);
			}
			else if (lowHpUnit != nullptr)
			{
				if (abs(dynamic_cast<CEntity*>(m_vPlayerUnits[i])->GetPosX() - currentEntity->GetPosX()) +
					abs(dynamic_cast<CEntity*>(m_vPlayerUnits[i])->GetPosY() - currentEntity->GetPosY()) <
					abs(lowHpUnit->GetPosX() - currentEntity->GetPosX()) + abs(lowHpUnit->GetPosY() - currentEntity->GetPosY())
					&& dynamic_cast<CEntity*>(m_vPlayerUnits[i])->GetCurrHealth() <= 4)
				{
					lowHpUnit = dynamic_cast<CEntity*>(m_vPlayerUnits[i]);
				}
			}
		}
	}
	return lowHpUnit;
}

void AIEntity::NormalAiActions(CMovingEntity* currentEntity)
{
	//////////////////////////////////////////////////////////
	// BUG FIX: 35|84
	// Fixed By: Kenneth Moy
	//////////////////////////////////////////////////////////
	// Begin Fix
	if (currentEntity->GetUsedAction() == true)
		return;

	if (closestBase != nullptr
		&& InAttackRange(closestBase, currentEntity, CGameplayState::GetInstance()->GetCurrentMap()))
		currentEntity->AttackUnit(closestBase, closestBase->GetCurrTile(), CGameplayState::GetInstance()->GetCurrentMap());

	if (closestUnit != nullptr
		&& InAttackRange(closestUnit, currentEntity, CGameplayState::GetInstance()->GetCurrentMap()))
		currentEntity->AttackUnit(closestUnit, closestUnit->GetCurrTile(), CGameplayState::GetInstance()->GetCurrentMap());

	if (currentEntity->GetMoved() == false)
	{
		std::vector<CTile*>CanMoveTo;
		CGrid* currentMap = CGameplayState::GetInstance()->GetGameMap();
		CTile* closestTile = nullptr;

		int TileX = currentEntity->GetPosX()/CGameplayState::GetInstance()->GetGameMap()->GetTileWidth();
		int TileY = currentEntity->GetPosY()/CGameplayState::GetInstance()->GetGameMap()->GetTileHeight();
		int tileXMin = 0;
		int tileXMax = 0;
		int tileYMin = 0;
		int tileYMax = 0;

		if (closestUnit != nullptr)
		{
			if(closestUnit->GetPosX() < currentEntity->GetPosX())
			{
#pragma region upper left
				if (closestUnit->GetPosY() < currentEntity->GetPosY())
				{
					tileXMin = TileX - (currentEntity->GetMoveRange()/2);
					if (tileXMin < 0)
						tileXMin = 0;
					tileXMax = TileX;
					tileYMin = TileY - (currentEntity->GetMoveRange()/2);
					if (tileYMin < 0)
						tileYMin = 0;
					tileYMax = TileY;
				}
#pragma endregion
#pragma region lower left
				else if (closestUnit->GetPosY() > currentEntity->GetPosY())
				{
					tileXMin = TileX - (currentEntity->GetMoveRange()/2);
					if (tileXMin < 0)
						tileXMin = 0;
					tileXMax = TileX;
					tileYMin = TileY;
					tileYMax = TileY + (currentEntity->GetMoveRange()/2);
					if (tileYMax >= CGameplayState::GetInstance()->GetGameMap()->GetMapHeight())
						tileYMax = CGameplayState::GetInstance()->GetGameMap()->GetMapHeight();
				}
#pragma endregion
			}
			if(closestUnit->GetPosX() > currentEntity->GetPosX())
			{
#pragma region upper right
				if (closestUnit->GetPosY() < currentEntity->GetPosY())
				{
					tileXMin = TileX;
					tileXMax = TileX  + (currentEntity->GetMoveRange()/2);
					if (tileXMax >= CGameplayState::GetInstance()->GetGameMap()->GetMapWidth())
						tileXMax = CGameplayState::GetInstance()->GetGameMap()->GetMapWidth();
					tileYMin = TileY - (currentEntity->GetMoveRange()/2);
					if (tileYMin < 0)
						tileYMin = 0;
					tileYMax = TileY;
				}
#pragma endregion
#pragma region lower right
				else if (closestUnit->GetPosY() > currentEntity->GetPosY())
				{
					tileXMin = TileX;
					tileXMax = TileX  + (currentEntity->GetMoveRange()/2);
					if (tileXMax >= CGameplayState::GetInstance()->GetGameMap()->GetMapWidth())
						tileXMax = CGameplayState::GetInstance()->GetGameMap()->GetMapWidth();
					tileYMin = TileY;
					tileYMax = TileY + (currentEntity->GetMoveRange()/2);
					if (tileYMax >= CGameplayState::GetInstance()->GetGameMap()->GetMapHeight())
						tileYMax = CGameplayState::GetInstance()->GetGameMap()->GetMapHeight();
				}
#pragma endregion
			}
		}
		else if (closestUnit == nullptr)
		{
#pragma region entire map
			tileXMin = TileX - (currentEntity->GetMoveRange()/2);
			if (tileXMin < 0)
				tileXMin = 0;
			tileXMax = TileX  + (currentEntity->GetMoveRange()/2);
			if (tileXMax >= CGameplayState::GetInstance()->GetGameMap()->GetMapWidth())
				tileXMax = CGameplayState::GetInstance()->GetGameMap()->GetMapWidth();
			tileYMin = TileY - (currentEntity->GetMoveRange()/2);
			if (tileYMin < 0)
				tileYMin = 0;
			tileYMax = TileY + (currentEntity->GetMoveRange()/2);
			if (tileYMax >= CGameplayState::GetInstance()->GetGameMap()->GetMapHeight())
				tileYMax = CGameplayState::GetInstance()->GetGameMap()->GetMapHeight();
#pragma endregion
		}

		// Find the distance to the shortest tile that is not occupied
		// Check the entire X range (left to right)
		for (int i = tileXMin; i < tileXMax; ++i)
		{
			// Check the entire Y range (top to bottom)
			for (int j = tileYMin; j < tileYMax; ++j)
			{
				CTile* pTile = CGameplayState::GetInstance()->GetGameMap()->GetTile(i,j);
				if (InMoveRange(currentEntity, pTile, CGameplayState::GetInstance()->GetCurrentMap()) == true)
				{
					CanMoveTo.push_back(pTile);
				}
			}
		}

		if (CanMoveTo.size() != 0)
		{
			int max = (int)CanMoveTo.size();
			int decision = rand()%max;
			currentEntity->MoveToTile(CanMoveTo[decision], CGameplayState::GetInstance()->GetCurrentMap());
			currentEntity->SetMoving(true);
		}
		CanMoveTo.clear();
	}
	else if (currentEntity->GetMoved() == true)
	{
		if (closestBase == nullptr)
			targetUnit = closestUnit;
		else if (closestUnit == nullptr)
			targetUnit = closestBase;
		else if (closestBase != nullptr && closestUnit != nullptr 
			&& abs(closestBase->GetPosX() - currentEntity->GetPosX()) + abs(closestBase->GetPosY() - currentEntity->GetPosY()) 
			> abs(closestUnit->GetPosX() - currentEntity->GetPosX()) + abs(closestUnit->GetPosY() - currentEntity->GetPosY()))
			targetUnit = closestUnit;
		else if (closestBase != nullptr && closestUnit != nullptr 
			&& abs(closestBase->GetPosX() - currentEntity->GetPosX()) + abs(closestBase->GetPosY() - currentEntity->GetPosY()) 
			< abs(closestUnit->GetPosX() - currentEntity->GetPosX()) + abs(closestUnit->GetPosY() - currentEntity->GetPosY()))
			targetUnit = closestBase;

		if (targetUnit != nullptr && InAttackRange(targetUnit, currentEntity, CGameplayState::GetInstance()->GetCurrentMap()))
		{
			currentEntity->AttackUnit(targetUnit, targetUnit->GetCurrTile(), CGameplayState::GetInstance()->GetCurrentMap());
		}
		else if (targetUnit != nullptr 
			&& InAttackRange(targetUnit, currentEntity, CGameplayState::GetInstance()->GetCurrentMap()) == false)
		{
			CTile* targetTile = CGameplayState::GetInstance()->GetGameMap()->GetTile(
				currentEntity->GetCurrTile()->GetPosX() / CGameplayState::GetInstance()->GetGameMap()->GetTileWidth(),
				(currentEntity->GetCurrTile()->GetPosY() / CGameplayState::GetInstance()->GetGameMap()->GetTileHeight()) - 1
				);

			//////////////////////////////////////////////////////////
			// BUG FIX: 36|84
			// Fixed By: Kenneth Moy
			//////////////////////////////////////////////////////////
			// Begin Fix
			if( targetTile->GetFactionType() != OWNER_AI || targetTile->GetFactionType() != OWNER_ROBOTS)
				currentEntity->ConvertTile(targetTile, CGameplayState::GetInstance()->GetCurrentMap());
			// End Fix
		}
	}
	// End Fix
}

void AIEntity::DefensiveAiActions (CMovingEntity* currentEntity)
{
	CStaticEntity* defendBase = nullptr;
	if (m_vAiBases.empty() == false)
	{
		for (unsigned int i = 0; i < m_vAiBases.size(); ++i)
		{
			if (dynamic_cast<CEntity*>(m_vAiBases[i])->GetCurrentAiState() == DEFENSE_AI)
			{
				defendBase = dynamic_cast<CStaticEntity*>(m_vAiBases[i]);
				break;
			}
		}
	}

	int distance = (abs(currentEntity->GetPosX()/CGameplayState::GetInstance()->GetGameMap()->GetTileWidth()- 
		defendBase->GetPosX()/CGameplayState::GetInstance()->GetGameMap()->GetTileWidth()) + 
		abs(currentEntity->GetPosY()/CGameplayState::GetInstance()->GetGameMap()->GetTileHeight() - 
		defendBase->GetPosY()/CGameplayState::GetInstance()->GetGameMap()->GetTileHeight()));
	CTile* shortestTile = nullptr;

	if (distance > 2)
	{
		int TileX = currentEntity->GetPosX()/CGameplayState::GetInstance()->GetGameMap()->GetTileWidth();
		int TileY = currentEntity->GetPosY()/CGameplayState::GetInstance()->GetGameMap()->GetTileHeight();
		int tileXMin, tileXMax;
		int tileYMin, tileYMax;

		tileXMin = TileX - (currentEntity->GetMoveRange()/2);
		if (tileXMin < 0)
			tileXMin = 0;
		tileXMax = TileX +(currentEntity->GetMoveRange()/2);
		if (tileXMax >= CGameplayState::GetInstance()->GetGameMap()->GetMapWidth())
			tileXMax = CGameplayState::GetInstance()->GetGameMap()->GetMapWidth();
		tileYMin = TileY - (currentEntity->GetMoveRange()/2);
		if (tileYMin < 0)
			tileYMin = 0;
		tileYMax = TileY + (currentEntity->GetMoveRange()/2);
		if (tileYMax >= CGameplayState::GetInstance()->GetGameMap()->GetMapHeight())
			tileYMax = CGameplayState::GetInstance()->GetGameMap()->GetMapHeight();

		// Find the distance to the shortest tile that is not occupied
		// Check the entire X range (left to right)
		for (int i = tileXMin; i < tileXMax; ++i)
		{
			// Check the entire Y range (top to bottom)
			for (int j = tileYMin; j < tileYMax; ++j)
			{
				if (shortestTile == nullptr 
					&& CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j)->GetOccupyingUnit() == nullptr)
					shortestTile = CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j);

				else if (shortestTile != nullptr &&
					abs(shortestTile->GetPosX() - defendBase->GetPosX()) +
					abs(shortestTile->GetPosY() - defendBase->GetPosY()) >
					abs(CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j)->GetPosX() - defendBase->GetPosX()) +
					abs(CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j)->GetPosY() - defendBase->GetPosY())
					&& CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j)->GetOccupyingUnit() == nullptr)
					shortestTile = CGameplayState::GetInstance()->GetGameMap()->GetTile(i,j);
			}
		}
		// If in move range, move here to defend
		if(InMoveRange(currentEntity, shortestTile, CGameplayState::GetInstance()->GetCurrentMap()) == true)
		{
			if (currentEntity->GetMoved() == false)
			{
				currentEntity->SetMoving(true);
				currentEntity->MoveToTile(shortestTile, CGameplayState::GetInstance()->GetCurrentMap());
			}
		}

		// If outside range, move closer to
		else if (abs(currentEntity->GetPosX() - shortestTile->GetPosX()) + abs(currentEntity->GetPosY() - shortestTile->GetPosY()) >
			currentEntity->GetMoveRange())
		{
			CGrid* currentMap = CGameplayState::GetInstance()->GetGameMap();
			CTile* distanceCheck = nullptr;

			for (int i = tileXMin; i < tileXMax; ++i)
			{
				// Check the entire Y range (top to bottom)
				for (int j = tileYMin; j < tileYMax; ++j)
				{
					if (shortestTile == nullptr 
						&& CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j)->GetOccupyingUnit() == nullptr)
						shortestTile = CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j);

					else if (shortestTile != nullptr &&
						abs(shortestTile->GetPosX() - defendBase->GetPosX()) +
						abs(shortestTile->GetPosY() - defendBase->GetPosY()) >
						abs(CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j)->GetPosX() - defendBase->GetPosX()) +
						abs(CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j)->GetPosY() - defendBase->GetPosY())
						&& CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j)->GetOccupyingUnit() == nullptr
						&& InMoveRange(currentEntity, 
						CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j),
						CGameplayState::GetInstance()->GetCurrentMap()) == true)
						shortestTile = CGameplayState::GetInstance()->GetGameMap()->GetTile(i,j);
				}
			}
			if (currentEntity->GetMoved() == false)
			{
				currentEntity->SetMoving(true);
				currentEntity->MoveToTile(shortestTile, CGameplayState::GetInstance()->GetCurrentMap());
			}
		}
	}

	targetUnit = closestUnit;
	if (InAttackRange(targetUnit, currentEntity, CGameplayState::GetInstance()->GetCurrentMap()) == true)
	{
		currentEntity->AttackUnit(targetUnit, targetUnit->GetCurrTile(), CGameplayState::GetInstance()->GetCurrentMap());
	}
}

void AIEntity::AggressiveAiActions (CMovingEntity* currentEntity)
{
	targetUnit = ClosestLowPlayerUnit(currentEntity);
	if (targetUnit != nullptr)
	{
		if (currentEntity->GetMoved() == false)
		{
			CGrid* currentMap = CGameplayState::GetInstance()->GetGameMap();
			int distance = (abs(currentEntity->GetPosX()/currentMap->GetTileWidth() - targetUnit->GetPosX()/currentMap->GetTileWidth()) +
				abs(currentEntity->GetPosY()/currentMap->GetTileHeight() - targetUnit->GetPosY()/currentMap->GetTileHeight()));
			CTile* closestTile = nullptr;

			// Move currentEntity to the unit
			int TileX = currentEntity->GetPosX()/CGameplayState::GetInstance()->GetGameMap()->GetTileWidth();
			int TileY = currentEntity->GetPosY()/CGameplayState::GetInstance()->GetGameMap()->GetTileHeight();
			int tileXMin, tileXMax;
			int tileYMin, tileYMax;

			tileXMin = TileX - (currentEntity->GetMoveRange()/2);
			if (tileXMin < 0)
				tileXMin = 0;
			tileXMax = TileX + (currentEntity->GetMoveRange()/2);
			if (tileXMax >= CGameplayState::GetInstance()->GetGameMap()->GetMapWidth())
				tileXMax = CGameplayState::GetInstance()->GetGameMap()->GetMapWidth();
			tileYMin = TileY - (currentEntity->GetMoveRange()/2);
			if (tileYMin < 0)
				tileYMin = 0;
			tileYMax = TileY + (currentEntity->GetMoveRange()/2);
			if (tileYMax >= CGameplayState::GetInstance()->GetGameMap()->GetMapHeight())
				tileYMax = CGameplayState::GetInstance()->GetGameMap()->GetMapHeight();

			// Find the distance to the shortest tile that is not occupied
			// Check the entire X range (left to right)
			for (int i = tileXMin; i < tileXMax; ++i)
			{
				// Check the entire Y range (top to bottom)
				for (int j = tileYMin; j < tileYMax; ++j)
				{
					if (closestTile == nullptr 
						&& CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j)->GetOccupyingUnit() == nullptr)
						closestTile = CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j);

					else if (closestTile != nullptr &&
						abs(closestTile->GetPosX() - targetUnit->GetPosX()) +
						abs(closestTile->GetPosY() - targetUnit->GetPosY()) >
						abs(CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j)->GetPosX() - targetUnit->GetPosX()) +
						abs(CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j)->GetPosY() - targetUnit->GetPosY())
						&& InMoveRange(currentEntity, CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j),
						CGameplayState::GetInstance()->GetCurrentMap()) == true
						&& CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j)->GetOccupyingUnit() == nullptr)
						closestTile = CGameplayState::GetInstance()->GetGameMap()->GetTile(i,j);
				}
			}

			if (closestTile != nullptr)
			{
				if (InMoveRange(currentEntity, closestTile, CGameplayState::GetInstance()->GetCurrentMap()) == true)
				{
					if (currentEntity->GetMoved() == false)
					{
						currentEntity->SetMoving(true);
						currentEntity->MoveToTile(closestTile, CGameplayState::GetInstance()->GetCurrentMap());
					}
				}
			}
		}
		else if (currentEntity->GetMoved() == true)
		{
			if (InAttackRange(targetUnit, currentEntity, CGameplayState::GetInstance()->GetCurrentMap()) == true)
			{
				currentEntity->AttackUnit(targetUnit, targetUnit->GetCurrTile(), CGameplayState::GetInstance()->GetCurrentMap());
			}
		}
	}
	else if (targetUnit == nullptr)
		return;
}

void AIEntity::RetreatAiActions (CMovingEntity* currentEntity)
{
	CGrid* currentMap = CGameplayState::GetInstance()->GetGameMap();

	if (closestUnit == nullptr)
		return;

	int distance = (abs(currentEntity->GetPosX()/currentMap->GetTileWidth() - closestUnit->GetPosX()/currentMap->GetTileWidth()) +
		abs(currentEntity->GetPosY()/currentMap->GetTileHeight() - closestUnit->GetPosY()/currentMap->GetTileHeight()));
	CTile* farthestTile = nullptr;

	// Move currentEntity to the unit
	int TileX = currentEntity->GetPosX()/CGameplayState::GetInstance()->GetGameMap()->GetTileWidth();
	int TileY = currentEntity->GetPosY()/CGameplayState::GetInstance()->GetGameMap()->GetTileHeight();
	int tileXMin, tileXMax;
	int tileYMin, tileYMax;

	tileXMin = TileX - (currentEntity->GetMoveRange()/2);
	if (tileXMin < 0)
		tileXMin = 0;
	tileXMax = TileX + (currentEntity->GetMoveRange()/2);
	if (tileXMax >= CGameplayState::GetInstance()->GetGameMap()->GetMapWidth())
		tileXMax = CGameplayState::GetInstance()->GetGameMap()->GetMapWidth();
	tileYMin = TileY - (currentEntity->GetMoveRange()/2);
	if (tileYMin < 0)
		tileYMin = 0;
	tileYMax = TileY + (currentEntity->GetMoveRange()/2);
	if (tileYMax >= CGameplayState::GetInstance()->GetGameMap()->GetMapHeight())
		tileYMax = CGameplayState::GetInstance()->GetGameMap()->GetMapHeight();

	// Find the distance to the shortest tile that is not occupied
	// Check the entire X range (left to right)
	for (int i = tileXMin; i < tileXMax; ++i)
	{
		// Check the entire Y range (top to bottom)
		for (int j = tileYMin; j < tileYMax; ++j)
		{
			if (farthestTile == nullptr 
				&& CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j)->GetOccupyingUnit() == nullptr)
				farthestTile = CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j);

			else if (farthestTile != nullptr &&
				abs(farthestTile->GetPosX() - closestUnit->GetPosX()) +
				abs(farthestTile->GetPosY() - closestUnit->GetPosY()) <
				abs(CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j)->GetPosX() - closestUnit->GetPosX()) +
				abs(CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j)->GetPosY() - closestUnit->GetPosY())
				&& InMoveRange(currentEntity, CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j),
				CGameplayState::GetInstance()->GetCurrentMap()) == true
				&& CGameplayState::GetInstance()->GetGameMap()->GetTile(i, j)->GetOccupyingUnit() == nullptr)
				farthestTile = CGameplayState::GetInstance()->GetGameMap()->GetTile(i,j);
		}
	}
	if (farthestTile != nullptr)
	{
		if (InMoveRange(currentEntity, farthestTile, CGameplayState::GetInstance()->GetCurrentMap()) == true)
		{
			if (currentEntity->GetMoved() == false)
			{
				currentEntity->SetMoving(true);
				currentEntity->MoveToTile(farthestTile, CGameplayState::GetInstance()->GetCurrentMap());
			}
		}
	}
}

