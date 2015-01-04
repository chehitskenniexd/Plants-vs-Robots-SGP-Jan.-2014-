#include "SaveState.h"
#include "../MainInfo/Game.h"
#include "../MainInfo/PlayerProfile.h"

#include "GameplayState.h"
#include "ProfileState.h"
#include "../MainInfo/BMFont.h"

#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"	// for color macro
#include "../SGD Wrappers/CSGD_DirectInput.h"

#include <ShlObj.h>
#include <sstream>
#include <ctime>

//Height of each line
#define MENU_ITEMS_YOFF 25 

//Back the cursor off from the X by these many pixels
#define CURSOR_XOFF 50 

CSaveState::CSaveState(void)
{
	m_bEnteringText = false;
	m_bReadyForKey = false;
}


CSaveState::~CSaveState(void)
{
}
/*static*/ CSaveState* CSaveState::GetInstance()
{
	static CSaveState s_Instance;
	return &s_Instance;
}

/*virtual*/ void CSaveState::Enter ( void ) /*override*/
{
	CGame* pG = CGame::GetInstance();
	HINSTANCE hInst = pG->GethInstance();

	MENU_ITEMS_X = int(pG->GetScreenWidth() * .29f);
	MENU_ITEMS_Y = int(pG->GetScreenWidth() * 0.15f);

	LoadStringA(hInst, IDS_SAVELOAD, szTitleString, 32);
	LoadStringA(hInst, IDS_SAVELOADINSTRUCTIONS, szHelpString, 128);
	LoadStringA(hInst, IDS_TEXTENTRY, szTextEntry, 32);

	BG = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/EmptyMenu.png"));

	CPlayerProfile* currProf = pG->GetProfile();
	for (unsigned int i = 0; i < currProf->GetNumSaves(); i++)
	{
		std::ostringstream tempStream;

		vChoices.push_back(currProf->GetSaveName(i));
	}
}

/*virtual*/ void CSaveState::Exit	( void ) /*override*/
{				 
	vChoices.clear();
}				 

/*virtual*/ bool CSaveState::Input ( void ) /*override*/
{				 
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
	if (m_bEnteringText)
	{
		unsigned char tempChar;
		//m_bReadyForKey = true;
		if (pDI->KeyPressed(DIK_ESCAPE))
		{
			//Clear temp string
			tempStr.clear();
			m_bEnteringText = false;
		}
		else if (pDI->KeyPressed(DIK_RETURN))
		{
			//Rename and clear temp string
			string oldPath = ComposePath(CGame::GetInstance()->GetProfile()->GetSaveName(m_nCursor));
			string newPath = ComposePath(tempStr.c_str()) + ".sav";

			rename(oldPath.c_str(), newPath.c_str());
			CGame::GetInstance()->GetProfile()->SetSaveName(m_nCursor, tempStr.c_str());

			CProfileState::GetInstance()->SaveProfiles();
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
		if (pDI->KeyPressed(DIK_ESCAPE) || pDI->JoystickButtonPressed(7))
			CGame::GetInstance()->PopState();

		if ((pDI->KeyPressed(DIK_DOWN) || pDI->JoystickDPadPressed(DIR_DOWN)) && m_nCursor < vChoices.size())
			m_nCursor++;
		else if ((pDI->KeyPressed(DIK_UP) || pDI->JoystickDPadPressed(DIR_UP)) && m_nCursor > 0)
			m_nCursor--;
		else if (pDI->KeyPressed(DIK_SPACE) && m_nCursor < vChoices.size())
		{
			tempStr.clear();
			m_bEnteringText = true;
		}

		if (pDI->KeyPressed(DIK_NUMPAD4) || pDI->JoystickButtonPressed(2)) //Save
		{
			if (CGameplayState::GetInstance()->GetBeingPlayed())
			{
				char tempFileName[16];

				if (m_nCursor < vChoices.size())
					CGameplayState::GetInstance()->SaveXml(vChoices[m_nCursor]);
				else
				{
					_itoa_s(static_cast<int>(time(0)), tempFileName, 10);
					strcat_s(tempFileName, 16, ".sav");

					CGameplayState::GetInstance()->SaveXml(tempFileName);

					CGame::GetInstance()->GetProfile()->AppendSaveName(tempFileName);
					CGame::GetInstance()->GetProfile()->AppendSaveTime(time(0));

					CProfileState::GetInstance()->SaveProfiles();
				}

				GetProfs();
				if (m_nCursor >= vChoices.size())
				{
				tempStr.clear();
				m_bEnteringText = true;
				}
			}
		}
		if (pDI->KeyPressed(DIK_NUMPAD2) || pDI->JoystickButtonPressed(0)) //Load
		{
			if (m_nCursor < vChoices.size())
			{
				tempStr = ComposePath(vChoices[m_nCursor]);

				CGame::GetInstance()->ChangeState(CGameplayState::GetInstance());

				CGameplayState::GetInstance()->SetLoadingSavedGame(true);
				CGameplayState::GetInstance()->LoadXml(tempStr.c_str());
				return true;
			}
		}
	}
	return true;
}				 

/*virtual*/ void CSaveState::Update ( float fElapsedTime ) /*override*/
{				 

}				 

/*virtual*/ void CSaveState::Render ( void ) /*override*/
{
	DWORD dwColor = D3DCOLOR_XRGB(12, 104, 0);
	CBMFont* pF = CGame::GetInstance()->GetFont();

	CSGD_TextureManager::GetInstance()->Draw(BG, 0, 0);
	pF->Write(szTitleString, 0, 0, 1.0, D3DCOLOR_XRGB(255, 255, 255));

	float fLabelY = CGame::GetInstance()->GetScreenHeight() - pF->GetLineHeight() * 0.75f;
	pF->Write(szHelpString, 0, (int)fLabelY, 0.75, D3DCOLOR_XRGB(255, 255, 255));

	if (m_bEnteringText)
	{
		pF->Write(szTextEntry, MENU_ITEMS_X, MENU_ITEMS_Y + 0 * MENU_ITEMS_YOFF, 1.0f, dwColor);
		pF->Write(tempStr.c_str(), CGame::GetInstance()->GetScreenWidth() / 2, CGame::GetInstance()->GetScreenHeight() / 2, 1, dwColor);
	}
	else
	{
		for (unsigned int i = 0; i < vChoices.size(); i++)
		{
			CPlayerProfile* currProf = CGame::GetInstance()->GetProfile();

			ostringstream oss;
			time_t tempTime = currProf->GetSaveTime(i);
			char tempTimeStr[32];
			ctime_s(tempTimeStr, 32, &tempTime);

			oss << vChoices[i] << '\t' << tempTimeStr;

			pF->Write(oss.str().c_str(), MENU_ITEMS_X, MENU_ITEMS_Y + i * MENU_ITEMS_YOFF, 1.0, dwColor);
		}

		pF->Write("-->", MENU_ITEMS_X - CURSOR_XOFF, MENU_ITEMS_Y + m_nCursor * MENU_ITEMS_YOFF, 1, dwColor);
	}
}

std::string CSaveState::ComposePath(const char* szFilename)
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

	return pathtowrite.c_str();
}

void CSaveState::GetProfs(void)
{
	vChoices.clear();
	CPlayerProfile* currProf = CGame::GetInstance()->GetProfile();
	for (unsigned int i = 0; i < currProf->GetNumSaves(); i++)
	{
		std::ostringstream tempStream;

		vChoices.push_back(currProf->GetSaveName(i));
	}
}