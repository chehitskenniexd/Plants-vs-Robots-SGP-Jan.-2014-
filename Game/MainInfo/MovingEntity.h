#pragma once
#include "entity.h"
#include "AnimTimeStamps.h"
#include <vector>
#include "../Grid/Tile.h"
#include <vector>

class CGrid;
class CSGD_XAudio2;

class CMovingEntity : public CEntity
{
private:
	int m_nBaseDamage;
	int m_nCurrDamage;
	bool m_bHasMoved;
	bool m_bStatChangesApplied;
	bool m_bActiveAttack, m_bActiveIdle, m_bConvertingTile, m_bMoving, m_bJustCreated;
	int m_nAttackRange;
	int m_nMoveRange;
	int m_nConvertRange;
	AnimTimeStamps unitConvertStamp, unitAttackStamp, unitMoveStamp;
	CEntity* pTarget;
	CSGD_XAudio2* pXA;
	CTile* targetTile;
	CTile* prevTile;
	CTile nextTile;

	int attackSFX;
	int injuredSFX;
	int moveSFX;
	int deathSFX;
	int selectedSFX;
	int m_nPathCurrentTile;
	std::vector<CNode*>vPath;

	bool ClearPath(void);
public:
	CMovingEntity(void);
	virtual ~CMovingEntity(void);

	void Initialize(void);
	bool AttackUnit(CEntity* pTarget, CTile* pTile, CGrid& pMap);
	bool MoveToTile(CTile* pTile, CGrid & pMap);
	bool ConvertTile(CTile* pTile, CGrid& pMap);

	void ShowAttackRange(CGrid &pMap);
	void ShowConvertRange(CGrid &pMap);
	void ShowMoveRange(CGrid &pMap);
	void UpdateStats();
	bool UndoMove (void);
	void PlayInjured();

	virtual void Update(float dT) override;
	virtual void Render() override;
	

#pragma region GettersAndSetters
	//Accessors and mutators
	int GetBaseDamage(void) const {return m_nBaseDamage;}
	int GetCurrDamage(void) const {return m_nCurrDamage;}
	bool GetMoved(void) const {return m_bHasMoved;}
	bool GetStatsChanged(void) const {return m_bStatChangesApplied;}
	int GetAttackRange(void) const {return m_nAttackRange;}
	int GetMoveRange(void) const {return m_nMoveRange;}
	int GetConvertRange(void)const {return m_nConvertRange;}
	AnimTimeStamps* GetUnitAttackStamp(void) {return &unitAttackStamp;}
	AnimTimeStamps GetUnitConvertStamps(void) {return unitConvertStamp;}
	AnimTimeStamps* GetUnitConvertStamp(void) {return &unitConvertStamp;}
	AnimTimeStamps* GetUnitMoveStamp(void) {return &unitMoveStamp;}
	CTile* GetPreviousTile (void) {return prevTile;}
	CTile* GetTargetTile (void) {return targetTile;}
	CEntity* GetTargetUnit (void) {return pTarget;}
	bool GetConvertingTile (void) {return this->m_bConvertingTile;}
	bool GetMoving(void) {return this->m_bMoving;}
	bool GetJustCreated (void) {return this->m_bJustCreated;}

	void SetAttackSFX(int sfxID) {attackSFX = sfxID;}
	void SetInjuredSFX(int sfxID) {injuredSFX = sfxID;}
	void SetMoveSFX(int sfxID) {moveSFX = sfxID;}
	void SetDeathSFX(int sfxID) {deathSFX = sfxID;}
	void SetSelectedSFX(int sfxID) {selectedSFX = sfxID;}

	void SetJustCreated(bool pCreated) {m_bJustCreated = pCreated;}
	void SetBaseDamage(int damage) {m_nBaseDamage = damage;}
	void SetCurrDamage(int damage) {m_nCurrDamage = damage;}
	void SetConvertRange(int range) {m_nConvertRange = range;}
	void SetAttackRange(int range) {m_nAttackRange = range;}
	void SetMoveRange(int range) {m_nMoveRange = range;}
	void SetHasMoved(bool moved);
	void SetStatsChanged(bool changed) {m_bStatChangesApplied = changed;}
	void SetUnitConvertStamp (AnimTimeStamps stamp) {this->unitConvertStamp = stamp;}
	void SetUnitMoveStamp (AnimTimeStamps stamp) {this->unitMoveStamp = stamp;}
	void SetUnitAttackStamp (AnimTimeStamps stamp) {this->unitAttackStamp = stamp;}
	void SetPreviousTile (CTile* pTile) {this->prevTile = pTile;}
	void SetTargetTile (CTile* pTile) {this->targetTile = pTile;}
	void SetTargetUnit (CEntity* pTarget) {this->pTarget = pTarget;}
	void SetConvertingTile (bool pConvert) {this->m_bConvertingTile = pConvert;}
	void SetMoving(bool bMove) {this->m_bMoving = bMove;}
	void SetNextTile (CTile pNext) {this->nextTile = pNext;}
	virtual void SetUsedAction (bool hasUsed) override;

	bool PathExists(void) const {return !vPath.empty();}
#pragma endregion
};
