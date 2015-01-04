#include "Tile.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../States/GameplayState.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../Systems/AnimationSystem.h"
#include "../MainInfo/StaticEntity.h"
#include "Grid.h"

CTile::CTile(void)
{
	tileType = TileType::GRASS;
	tileID = CGameplayState::GetInstance()->tileSetImgID;
	tileEvent = "NONE";
	impassable = false;
	occupyingUnit = nullptr;
	hpModifier = 2;
	dmgModifier = 2;
	posX = 0;
	posY = 0;
	toUpdate = false;
	robotConvertChance = 0;
	plantConvertChance = 0;
	highlight = false;
	waterFlow = WaterFlow::NONE;
	isConverting = false;
	convertRender.SetCurrentAnimation("Explosion");
}

void CTile::ResetUpdate()
{
	toUpdate = false;
	robotConvertChance = 0;
	plantConvertChance = 0;
	highlight = false;
}

void CTile::GetPos(int &x, int &y)
{
	x = posX; 
	y = posY;
}

bool CTile::Convert(OwnerFaction newFaction)
{
	if(highlight == true || CGameplayState::GetInstance()->GetCurrentFaction() == OWNER_AI)
	{
		if(occupyingUnit != nullptr)
		{
			if(occupyingUnit->GetFaction() != OWNER_NEUTRAL && newFaction != occupyingUnit->GetFaction())
			{
				return false;
			}
		}

		if(newFaction == faction)
			return false;

		CSGD_XAudio2* pXA = CSGD_XAudio2::GetInstance();

		switch (tileType)
		{
		case TileType::GRASS:
		case TileType::NEUTRAL:
		case TileType::NANO:
			tileType = newFaction == OwnerFaction::OWNER_ROBOTS || newFaction == OwnerFaction::OWNER_AI ? TileType::NANO : TileType::GRASS;
			faction = newFaction;
			CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/ConvertNormal.wav"));
			break;
		case TileType::FOREST:
			tileType = newFaction == OwnerFaction::OWNER_ROBOTS || newFaction == OwnerFaction::OWNER_AI ? TileType::MOUNTAIN : TileType::FOREST;
			faction = newFaction == OwnerFaction::OWNER_ROBOTS || newFaction ==  OwnerFaction::OWNER_AI ? OwnerFaction::OWNER_NEUTRAL : OwnerFaction::OWNER_PLANTS;
			pXA->SFXLoadSound(_T("Resources/Sounds/ConvertSpecial.wav"));
			break;
		case TileType::MOUNTAIN:
			tileType = newFaction == OwnerFaction::OWNER_ROBOTS || newFaction ==  OwnerFaction::OWNER_AI ? TileType::VOLCANO : TileType::FOREST;
			faction = newFaction;
			pXA->SFXLoadSound(_T("Resources/Sounds/ConvertSpecial.wav"));
			break;
		case TileType::VOLCANO:
			tileType = newFaction == OwnerFaction::OWNER_ROBOTS || newFaction == OwnerFaction::OWNER_AI ? TileType::VOLCANO : TileType::MOUNTAIN;
			faction = newFaction == OwnerFaction::OWNER_ROBOTS || newFaction == OwnerFaction::OWNER_AI ? OwnerFaction::OWNER_ROBOTS : OwnerFaction::OWNER_NEUTRAL;
			pXA->SFXLoadSound(_T("Resources/Sounds/ConvertSpecial.wav"));
			break;
		case TileType::SLUDGE:
		case TileType::WATER:
		case TileType::SWAMP:
			if(toUpdate == true)
			{
				if(newFaction == OwnerFaction::OWNER_NEUTRAL)
					tileType = TileType::WATER;
				if(newFaction == OwnerFaction::OWNER_PLANTS)
					tileType = TileType::SWAMP;
				if(newFaction == OwnerFaction::OWNER_ROBOTS || newFaction == OwnerFaction::OWNER_AI)
					tileType = TileType::SLUDGE;

				faction = newFaction;
			}
			pXA->SFXLoadSound(_T("Resources/Sounds/ConvertWater.wav"));
			break;
		default:
			return false;
			break;
		}

		//////////////////////////////////////////////////////////
		// BUG FIX: 31|84
		// Fixed By: Kenneth Moy
		//////////////////////////////////////////////////////////
		// Begin Fix
		if (occupyingUnit && UNIT_BASE == occupyingUnit->GetType() && OWNER_NEUTRAL == occupyingUnit->GetFaction())
		{
			GetOccupyingUnit()->SetFaction(newFaction);
			if (newFaction == OWNER_PLANTS)
			{
				this->GetOccupyingUnit()->GetIdleAnimationStamp()->SetCurrentAnimation("P Base Idle");
				dynamic_cast<CStaticEntity*>(this->GetOccupyingUnit())->SetCreateSFX(pXA->SFXLoadSound(_T("Resources/Sounds/PlantCreate.wav")));
			}
			else
			{
				this->GetOccupyingUnit()->GetIdleAnimationStamp()->SetCurrentAnimation("R Base Idle");
				dynamic_cast<CStaticEntity*>(this->GetOccupyingUnit())->SetCreateSFX(pXA->SFXLoadSound(_T("Resources/Sounds/RobotCreate.wav")));
			}
		}
		// End Fix

		if (faction == OWNER_AI)
			faction = OWNER_ROBOTS;

		srcRect = CGameplayState::GetInstance()->GetGameMap()->GetTileSet()[tileType];

		//pXA->SFXPlaySound(convertSFX, false);
		return true;
	}
	return false;
}

void CTile::SetToUpdate(bool isGuaranteed, OwnerFaction owner)
{
	toUpdate = true;

	if((tileType == TileType::GRASS || tileType == TileType::NEUTRAL || tileType == TileType::NANO) && isGuaranteed == false)
	{
		if(owner == OWNER_ROBOTS)
			robotConvertChance += 10;
		else if(owner == OWNER_PLANTS)
			plantConvertChance += 10;
	}
	else if(tileType == TileType::SLUDGE || tileType == TileType::WATER || tileType == TileType::SWAMP)
	{
		if(owner == OWNER_ROBOTS)
			robotConvertChance += 100;
		else if(owner == OWNER_PLANTS)
			plantConvertChance += 100;
	}
	else if((tileType == TileType::GRASS || tileType == TileType::NEUTRAL || tileType == TileType::NANO) && isGuaranteed == true)
	{
		if(owner == OWNER_ROBOTS && plantConvertChance == 0)
			robotConvertChance += 100;
		else if(owner == OWNER_PLANTS && robotConvertChance == 0)
			plantConvertChance += 100;
		else
			robotConvertChance = plantConvertChance = 50;
	}


	highlight = true;
}

CTile::~CTile(void)
{
}

void CTile::Highlight(D3DCOLOR color)
{
	highlightColor = color;
	highlight = true;
}

void CTile::Render(int tileW, int tileH)
{
	CSGD_TextureManager::GetInstance()->Draw(tileID, posX + CGameplayState::GetInstance()->OffsetX(), posY + CGameplayState::GetInstance()->OffsetY(), 1.0f, 1.0f, &srcRect);

	if (isConverting == true)
		AnimationSystem::GetInstance()->Render(posX + CGameplayState::GetInstance()->OffsetX() - 32,
		posY + CGameplayState::GetInstance()->OffsetY() - 32, convertRender);

	if(highlight == true)
	{
		RECT rect;
		rect.top = posY + CGameplayState::GetInstance()->OffsetY();
		rect.left = posX + CGameplayState::GetInstance()->OffsetX();
		rect.bottom = rect.top + tileH;
		rect.right = rect.left + tileW;
		CSGD_Direct3D::GetInstance()->DrawRect(rect, highlightColor);
	}
}

void CTile::UnHighlight()
{
	highlight = false;
}

void CTile::SetOccupyingUnit(CEntity* unit)
{
	occupyingUnit = unit;

	if(tileType == TileType::SLUDGE || tileType == TileType::WATER || tileType == TileType::SWAMP)
	{
		if(unit != nullptr)
		{
			tileType = unit->GetFaction() == OwnerFaction::OWNER_ROBOTS ? TileType::SLUDGE : TileType::SWAMP;
			faction = unit->GetFaction();
			srcRect = CGameplayState::GetInstance()->GetGameMap()->GetTileSet()[tileType];
		}
	}

	if(tileEvent != "NONE")
		CSGD_EventSystem::GetInstance()->QueueEvent(tileEvent, NULL, unit, this);
}