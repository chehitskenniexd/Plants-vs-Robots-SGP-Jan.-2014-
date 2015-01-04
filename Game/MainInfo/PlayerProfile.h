#pragma once

#include <string>
#include <vector>
class CPlayerProfile
{
public:
	CPlayerProfile(void);
	~CPlayerProfile(void);

	void SetCampaignProgress	(int nProgress)				{m_nCampaignProgress = nProgress;}
	void AppendSaveName			(std::string szSaveName)	{m_vSaveName.push_back(szSaveName);}
	void SetProfName			(std::string szProfName)	{m_szProfName = szProfName;}
	void SetSaveName			(int i, const char* nName);
	void AppendSaveTime			(time_t nTime)				{m_vSaveTime.push_back(nTime);}
	void AppendProfileTime (std::string nTime)				{m_vProfileTime.push_back(nTime);}
	void SetRounds				(int nRounds)				{m_nTotalRounds = nRounds;}
	void SetTime				(float fTime)				{m_fTotalTime = fTime;}
	void SetKills				(int nKills)				{m_nUnitsKilled = nKills;}
	void SetInfantry			(int nInfantry)				{m_nInfantryMade = nInfantry;}
	void SetTanks				(int nTanks)				{m_nTanksMade = nTanks;}
	void SetScouts				(int nScouts)				{m_nScoutsMade = nScouts;}

	const char * GetProfName	(void) const	{return m_szProfName.c_str();}
	const char * GetSaveName	(int i) const	{return m_vSaveName[i].c_str();}
	unsigned int GetSaveTime	(int i) const	{return static_cast<unsigned int>(m_vSaveTime[i]);}
	std::string GetProfileTime (unsigned int i) const {return m_vProfileTime[i];}
	int GetCampaignProgress		(void) const	{return m_nCampaignProgress;}
	int GetRounds				(void) const	{return m_nTotalRounds;}
	float GetTime				(void) const	{return m_fTotalTime;}
	int GetKills				(void) const	{return m_nUnitsKilled;}
	int GetTanks				(void) const	{return m_nTanksMade;}
	int GetInfantry				(void) const	{return m_nInfantryMade;}
	int GetScouts				(void) const	{return m_nScoutsMade;}
	unsigned int GetNumSaves	(void) const	{return m_vSaveName.size();}
private:
	std::string m_szProfName;

	std::vector<std::string> m_vSaveName;
	std::vector<time_t> m_vSaveTime;
	std::vector<std::string>m_vProfileTime;

	float m_fTotalTime, m_fPlantsTime, m_fRobotsTime;
	int m_nCampaignProgress, m_nTotalRounds;

	int m_nUnitsKilled;
	int m_nInfantryMade, m_nScoutsMade, m_nTanksMade;
};