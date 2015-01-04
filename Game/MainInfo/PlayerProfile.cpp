#include "PlayerProfile.h"


CPlayerProfile::CPlayerProfile(void)
{
	m_fPlantsTime = 0;
	m_fRobotsTime = 0;
	m_fTotalTime = 0;
	m_nTotalRounds = 0;
	m_nCampaignProgress = 0;

	m_nUnitsKilled = 0;
	m_nTanksMade = 0;
	m_nScoutsMade = 0;
	m_nInfantryMade = 0;
	m_vSaveName.clear();
}


CPlayerProfile::~CPlayerProfile(void)
{

}

void CPlayerProfile::SetSaveName (int i, const char* nName)	
{
	m_vSaveName[i] = nName;
}