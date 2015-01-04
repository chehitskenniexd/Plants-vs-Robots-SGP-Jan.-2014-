#include "ProfileState.h"

#include "../MainInfo/Game.h"
#include "../MainInfo/BMFont.h"
#include "../SGD Wrappers/tinyxml.h"
#include "../MainInfo/PlayerProfile.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"

#include <ctime>
#include <ShlObj.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

CProfileState::CProfileState(void)
{
	m_nCursor = 0;
	m_bEnteringText = false;
	m_bReadyForKey = false;
}

CProfileState::~CProfileState(void)
{
	for (unsigned int i = 0; i < m_vProfiles.size(); i++)
		delete m_vProfiles[i];

	m_vProfiles.clear();
}

/*static*/ CProfileState* CProfileState::GetInstance()
{
	static CProfileState s_Instance;
	return &s_Instance;
}

/*virtual*/ void CProfileState::Enter ( void ) /*override*/
{
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_SELECTPROF, sSelect, 64);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CAMPAIGNPROGRESS, szProg, 32);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TEXTENTRY, sText, 32);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_ENTER_NAME, enterName, 32);

	if(CSGD_DirectInput::GetInstance()->JoystickIsUnplugged())
		LoadStringA(CGame::GetInstance()->GethInstance(), IDS_PROFILE_INSTRUCT_KEYBOARD, instruct, 100);
	else
		LoadStringA(CGame::GetInstance()->GethInstance(), IDS_PROFILE_INSTRUCT, instruct, 100);

	LoadProfiles();

	MENU_ITEMS_X = int(CGame::GetInstance()->GetScreenWidth() * .29f);
	MENU_ITEMS_Y = int( CGame::GetInstance()->GetScreenWidth() * 0.15f);
	MENU_ITEMS_YOFF = 42; 
	CURSOR_XOFF = 50;

	indicatorIMG = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/PlantAP.png"));
	BG = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/EmptyMenu.png"));

	selectionChangedSFX = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/SelectionChange.wav"));
	selectionMadeSFX = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/SelectionMade.wav"));
	selectionBackSFX = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/SelectionBack.wav"));
}

/*virtual*/ void CProfileState::Exit	( void ) /*override*/
{				 

}				 

/*virtual*/ bool CProfileState::Input ( void ) /*override*/
{				 
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	if (m_bEnteringText)
	{
		unsigned char tempChar;
		//m_bReadyForKey = true;
		if (pDI->KeyPressed(DIK_ESCAPE))
		{
			SaveProfiles();
			CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionMadeSFX);
			CGame::GetInstance()->SetProfile(m_vProfiles[m_nCursor]);
			CGame::GetInstance()->PopState();
			//Clear temp string
			tempStr.clear();
			m_bEnteringText = false;
		}
		else if (pDI->KeyPressed(DIK_RETURN))
		{
			//Rename and clear temp string
			m_vProfiles[m_nCursor]->SetProfName(tempStr);
			tempStr.clear();
			m_bEnteringText = false;
		}
		else if (m_bReadyForKey && pDI->KeyPressed(DIK_BACKSPACE) && !tempStr.empty())
		{
			tempStr.pop_back();
			m_bReadyForKey = false;
		}
		else
		{
			tempChar = pDI->CheckKeys();

			if (tempChar != '\0' && m_bReadyForKey)
			{
				tempStr += tempChar;
				m_bReadyForKey = false;
			}
			else if (!tempChar)
				m_bReadyForKey = true;
		}
	}
	else
	{
		if (m_vProfiles.size())
		{
			if (pDI->KeyPressed(DIK_RETURN) || pDI->JoystickButtonPressed(0) || pDI->KeyPressed(DIK_NUMPAD2))
			{
				SaveProfiles();
				CGame::GetInstance()->SetProfile(m_vProfiles[m_nCursor]);
				CGame::GetInstance()->PopState();
				return true;
			}
			else if (pDI->KeyPressed(DIK_LSHIFT) || pDI->JoystickButtonPressed(2))
				m_bEnteringText = true;
		}
		else
		{
			AddNewProfile("New Default Profile");
			m_bEnteringText = true;
		}
		if ((pDI->KeyPressed(DIK_DOWN) || pDI->JoystickDPadPressed(DIR_DOWN)) && m_nCursor < static_cast<signed int>(m_vProfiles.size()) - 1)
		{
			CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionChangedSFX);
			m_nCursor++;
		}
		else if ((pDI->KeyPressed(DIK_UP) || pDI->JoystickDPadPressed(DIR_UP)) && m_nCursor > 0)
		{
			CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionChangedSFX);
			m_nCursor--;
		}
		if (m_nCursor < 0)
			m_nCursor = 0;
	}

	if (pDI->KeyPressed(DIK_DELETE) || pDI->JoystickButtonPressed(1))
	{
		CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionBackSFX);
		if (!m_vProfiles.empty() && m_vProfiles[m_nCursor])
		{
			delete m_vProfiles[m_nCursor];
			m_vProfiles.erase(m_vProfiles.begin() + m_nCursor--);
		}
	}
	else if (pDI->KeyPressed(DIK_SPACE) || pDI->JoystickButtonPressed(3))
	{
		CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionMadeSFX);
		AddNewProfile("New Profile");
		m_nCursor++;
		m_bEnteringText = true;
	}
	return true;
}

/*virtual*/ void CProfileState::Update ( float fElapsedTime ) /*override*/
{				 

}				 

/*virtual*/ void CProfileState::Render ( void ) /*override*/
{
	DWORD dwColor = D3DCOLOR_XRGB(12, 104, 0);
	CBMFont* pF = CGame::GetInstance()->GetFont();

	CSGD_TextureManager::GetInstance()->Draw(BG, 0, 0);
	if (!m_bEnteringText)
	{
		CSGD_TextureManager::GetInstance()->Draw(indicatorIMG, MENU_ITEMS_X - CURSOR_XOFF, int(MENU_ITEMS_Y + m_nCursor * MENU_ITEMS_YOFF));


		for (unsigned int i = 0; i < m_vProfiles.size(); i++)
		{
			std::ostringstream oss;

			oss << m_vProfiles[i]->GetProfName() << '\t' << szProg << " " << m_vProfiles[i]->GetCampaignProgress() - 3;
			pF->Write(oss.str().c_str(), MENU_ITEMS_X, MENU_ITEMS_Y + i * MENU_ITEMS_YOFF, 1.0f, dwColor);

		pF->Write(instruct, MENU_ITEMS_X, MENU_ITEMS_Y + 10 * MENU_ITEMS_YOFF, 1.0f, dwColor);

		}
	}
	else
	{
		pF->Write(enterName, MENU_ITEMS_X, MENU_ITEMS_Y + 0 * MENU_ITEMS_YOFF, 1.0f, dwColor);
		pF->Write(tempStr.c_str(), MENU_ITEMS_X + MENU_ITEMS_YOFF, MENU_ITEMS_Y + 3 * MENU_ITEMS_YOFF, 1, dwColor);
	}
}

bool CProfileState::LoadProfiles(const char * szFilename)
{
	std::ostringstream stringstream;
	char path[MAX_PATH];
	LPWSTR wszPath = NULL;
	size_t   size;

	HRESULT hr = SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, 0, &wszPath);

	wcstombs_s(&size,path,MAX_PATH,wszPath,MAX_PATH);

	// Convert char types
	if(hr == S_OK)
		stringstream << path;

	std::string pathtowrite = stringstream.str();

	// Add the company and game information
	pathtowrite += "\\AllInGames\\PvR\\";

	//SHCreateDirectoryExA(NULL, pathtowrite.c_str(), 0);

	pathtowrite += szFilename;

	TiXmlDocument indexDoc;
	std::string tempName, tempSave;
	int tempRounds, tempProgress;
	int tempKills, tempTanks, tempInf, tempScouts;
	double tempTime;

	if (!indexDoc.LoadFile(pathtowrite.c_str()))
		return false;

	TiXmlElement* pProf = indexDoc.FirstChildElement("profile");

	if (!pProf)
		return false;

	while (pProf)
	{
		tempName = pProf->Attribute("name");
		//tempSave = pProf->Attribute("file");

		pProf->Attribute("rounds", &tempRounds);
		pProf->Attribute("time", &tempTime);
		pProf->Attribute("progress", &tempProgress);

		pProf->Attribute("kills", &tempKills);
		pProf->Attribute("tanks", &tempTanks);
		pProf->Attribute("scouts", &tempScouts);
		pProf->Attribute("infantry", &tempInf);

		CPlayerProfile* pProfile = new CPlayerProfile;
		pProfile->SetProfName(tempName.c_str());

		TiXmlElement* pSave = pProf;

		pSave = pSave->FirstChildElement("save");
		int tempSaveTime;

		while (pSave)
		{
			tempSave = pSave->Attribute("file");
			pSave->Attribute("time", &tempSaveTime);

			pProfile->AppendSaveName(tempSave);
			pProfile->AppendSaveTime(static_cast<time_t>(tempSaveTime));

			pSave = pSave->NextSiblingElement("save");
		}

		pProfile->SetCampaignProgress(tempProgress);
		pProfile->SetRounds(tempRounds);
		pProfile->SetTime(static_cast<float>(tempTime));

		pProfile->SetKills(tempKills);
		pProfile->SetTanks(tempTanks);
		pProfile->SetScouts(tempScouts);
		pProfile->SetInfantry(tempInf);

		m_vProfiles.push_back(pProfile);
		pProf = pProf->NextSiblingElement("profile");
	}

	return true;
}

bool CProfileState::SaveProfiles(const char* szFilename)
{

	std::ostringstream stringstream;
	char path[MAX_PATH];
	LPWSTR wszPath = NULL;
	size_t   size;

	HRESULT hr = SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, 0, &wszPath);

	wcstombs_s(&size,path,MAX_PATH,wszPath,MAX_PATH);

	// Convert char types
	if(hr == S_OK)
		stringstream << path;

	std::string pathtowrite = stringstream.str();

	// Add the company and game information
	pathtowrite += "\\AllInGames\\PvR\\";

	SHCreateDirectoryExA(NULL, pathtowrite.c_str(), 0);

	pathtowrite += szFilename;

	TiXmlDocument doc;
	doc.LinkEndChild(new TiXmlDeclaration("1.0", "utf-8", ""));

	TiXmlElement* pProf = nullptr;

	for (unsigned int i = 0; i < m_vProfiles.size(); i++)
	{
		pProf = new TiXmlElement("profile");
		pProf->SetAttribute("name", m_vProfiles[i]->GetProfName());
		pProf->SetAttribute("rounds", m_vProfiles[i]->GetRounds());
		pProf->SetDoubleAttribute("time", static_cast<double>(m_vProfiles[i]->GetTime()));
		pProf->SetAttribute("progress", m_vProfiles[i]->GetCampaignProgress());
		pProf->SetAttribute("kills", m_vProfiles[i]->GetKills());
		pProf->SetAttribute("tanks", m_vProfiles[i]->GetTanks());
		pProf->SetAttribute("scouts", m_vProfiles[i]->GetScouts());
		pProf->SetAttribute("infantry", m_vProfiles[i]->GetInfantry());

		TiXmlElement* pSave = nullptr;
		for (unsigned int j = 0; j < m_vProfiles[i]->GetNumSaves(); j++)
		{
			pSave = new TiXmlElement("save");
			pSave->SetAttribute("file", m_vProfiles[i]->GetSaveName(j));
			pSave->SetAttribute("time", static_cast<int>(time(0)));

			pProf->LinkEndChild(pSave);
		}

		doc.LinkEndChild(pProf);
	}
	return doc.SaveFile(pathtowrite.c_str());
}

void CProfileState::AddNewProfile(const char * szFileName, const char * szSaveName)
{
	CPlayerProfile* pProf = new CPlayerProfile;
	pProf->SetProfName(szFileName);
	pProf->SetCampaignProgress(4);

	m_vProfiles.push_back(pProf);
}