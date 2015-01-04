#include "OptionState.h"
#include "../MainInfo/Game.h"
#include "GameplayState.h"

#include "../MainInfo/BMFont.h"
#include "../SGD Wrappers/tinyxml.h"

#include "../SGD Wrappers/SGD_Math.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../MainInfo/resource.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"

#include <string>
#include <ostream>
#include <ShlObj.h>
using namespace std;

#define MENU_ITEMS 4

COptionState::COptionState(void)
{
	m_nCursor = 0;

	m_fSoundVolume = 1.0f;
	m_fMusicVolume = 1.0f;
	m_bFullscreen = false;
	m_nLang = ENGLISH;

	m_nTestSound = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/coin.wav"));
}

COptionState::~COptionState(void)
{

}

/*static*/ COptionState* COptionState::GetInstance(void)
{
	static COptionState s_Instance;
	return &s_Instance;
}


/*virtual*/ void COptionState::Enter ( void ) /*override*/
{
	LoadOptions();
	ApplyOptions();

	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_MUSIC_VOLUME, musicVolumeString, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_SOUND_VOLUME, soundVolumeString, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_FULLSCREEN, fullscreenString, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_ON, onString, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_OFF, offString, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_LANGUAGE, languageString, 20);

	selectionChangedSFX = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/SelectionChange.wav"));
	selectionMadeSFX = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/SelectionMade.wav"));
	selectionBackSFX = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/SelectionBack.wav"));

	MENU_ITEMS_X = int(CGame::GetInstance()->GetScreenWidth() * .29f);
	MENU_ITEMS_Y = int( CGame::GetInstance()->GetScreenWidth() * 0.15f);
	MENU_ITEMS_YOFF = 42; 
	CURSOR_XOFF = 50;

	indicatorIMG = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/PlantAP.png"));

	BG = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/EmptyMenu.png"));

}

/*virtual*/ void COptionState::Exit	( void ) /*override*/
{				 
	SaveOptions();
	ApplyOptions();
}				 

/*virtual*/ bool COptionState::Input ( void ) /*override*/
{				 
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
	CSGD_XAudio2* pXA = CSGD_XAudio2::GetInstance();
	if (pDI->KeyPressed(DIK_ESCAPE) || pDI->JoystickButtonPressed(1) || pDI->KeyPressed(DIK_NUMPAD6))
	{
		CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionBackSFX);
		CGame::GetInstance()->PopState();
	}

	if ((pDI->KeyPressed(DIK_DOWN) || pDI->JoystickDPadPressed(DIR_DOWN)) && m_nCursor < MENU_ITEMS - 1)
	{
		CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionChangedSFX);
		m_nCursor++;
	}
	else if ((pDI->KeyPressed(DIK_UP) || pDI->JoystickDPadPressed(DIR_UP)) && m_nCursor > 0)
	{
		CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionChangedSFX);
		m_nCursor--;
	}

	switch (m_nCursor)
	{
	case MUSIC:
		if (pDI->KeyPressed(DIK_RIGHT) || pDI->JoystickDPadPressed(DIR_RIGHT))
		{
			m_fMusicVolume += 0.1f;

			if (m_fMusicVolume > 1.0f)
				m_fMusicVolume = 1.0f;

			pXA->MusicSetMasterVolume(m_fMusicVolume);
			ApplyOptions();
		}
		else if (pDI->KeyPressed(DIK_LEFT) || pDI->JoystickDPadPressed(DIR_LEFT))
		{
			m_fMusicVolume -= 0.1f;

			if (m_fMusicVolume < 0)
				m_fMusicVolume = 0;

			pXA->MusicSetMasterVolume(m_fMusicVolume);
			ApplyOptions();

		}
		break;
	case SOUND:
		if (pDI->KeyPressed(DIK_RIGHT) || pDI->JoystickDPadPressed(DIR_RIGHT))
		{
			m_fSoundVolume += 0.1f;

			if (m_fSoundVolume > 1.0f)
				m_fSoundVolume = 1.0f;

			pXA->SFXSetSoundVolume(m_nTestSound, m_fSoundVolume);
			pXA->SFXPlaySound(m_nTestSound);
			ApplyOptions();

		}
		else if (pDI->KeyPressed(DIK_LEFT) || pDI->JoystickDPadPressed(DIR_LEFT))
		{
			m_fSoundVolume -= 0.1f;

			if (m_fSoundVolume < 0)
				m_fSoundVolume = 0;

			pXA->SFXSetSoundVolume(m_nTestSound, m_fSoundVolume);
			pXA->SFXPlaySound(m_nTestSound);
			ApplyOptions();

		}
		break;
	case FULLSCREEN:
		if (pDI->KeyPressed(DIK_RIGHT) || pDI->JoystickDPadPressed(DIR_RIGHT) || pDI->KeyPressed(DIK_LEFT) || pDI->JoystickDPadPressed(DIR_LEFT))
		{
			m_bFullscreen = !m_bFullscreen;
			CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionChangedSFX);
			ApplyOptions();
		}
		break;
	case LANGUAGE:
		if(pDI->KeyPressed(DIK_RIGHT) || pDI->JoystickDPadPressed(DIR_RIGHT) || pDI->KeyPressed(DIK_LEFT) || pDI->JoystickDPadPressed(DIR_LEFT))
		{
			if(m_nLang == ENGLISH)
				m_nLang = SPANISH;
			else
				m_nLang = ENGLISH;

			CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionChangedSFX);
			ApplyOptions();
		}
		break;
	}

	return true;
}				 

/*virtual*/ void COptionState::Update ( float fElapsedTime ) /*override*/
{				 

}				 

/*virtual*/ void COptionState::Render ( void ) /*override*/
{
	DWORD dwColor = D3DCOLOR_XRGB(12, 104, 0);
	CBMFont* pF = CGame::GetInstance()->GetFont();

	CSGD_TextureManager::GetInstance()->Draw(BG, 0, 0);

	RECT musRect = {MENU_ITEMS_X, MENU_ITEMS_Y +  MENU_ITEMS_YOFF * 1, static_cast<LONG>(Lerp((float)MENU_ITEMS_X, (float)MENU_ITEMS_X + 250, m_fMusicVolume)), MENU_ITEMS_Y + MENU_ITEMS_YOFF * 2};
	RECT sfxRect = {MENU_ITEMS_X, MENU_ITEMS_Y +  MENU_ITEMS_YOFF * 3, static_cast<LONG>(Lerp((float)MENU_ITEMS_X, (float)MENU_ITEMS_X + 250, m_fSoundVolume)), MENU_ITEMS_Y + MENU_ITEMS_YOFF * 4};

	CSGD_TextureManager::GetInstance()->Draw(indicatorIMG, MENU_ITEMS_X - CURSOR_XOFF, MENU_ITEMS_Y + m_nCursor * MENU_ITEMS_YOFF * 2);

	pF->Write(musicVolumeString, MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * 0, 1.3f, dwColor); //Labels
	pF->Write(soundVolumeString, MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * 2, 1.3f, dwColor);
	pF->Write(fullscreenString, MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * 4, 1.3f, dwColor);
	pF->Write(languageString, MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * 6, 1.3f, dwColor);

	CSGD_Direct3D::GetInstance()->DrawRect(musRect, D3DCOLOR_XRGB(0, 255, 0));
	CSGD_Direct3D::GetInstance()->DrawRect(sfxRect, D3DCOLOR_XRGB(0, 255, 0));

	sfxRect.right = musRect.right = MENU_ITEMS_X + 250;

	CSGD_Direct3D::GetInstance()->DrawHollowRect(musRect, D3DCOLOR_XRGB(0, 255, 0), 1);
	CSGD_Direct3D::GetInstance()->DrawHollowRect(sfxRect, D3DCOLOR_XRGB(0, 255, 0), 1);

	pF->Write((m_bFullscreen ? onString : offString), MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * 5, 1, dwColor);
	pF->Write((m_nLang == 0 ? "     English" : "     Espanol"), MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * 7, 1, dwColor);
	float nLabelY = CGame::GetInstance()->GetScreenHeight() - pF->GetLineHeight() * 0.75f;
}

bool COptionState::SaveOptions(const char* szFilename)
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

	std::string pathtowrite = stringstream.str() += "\\AllInGames\\PvR\\";
	pathtowrite += szFilename;
	TiXmlDocument optionDoc;

	optionDoc.LinkEndChild(new TiXmlDeclaration("1.0", "utf-8", ""));

	TiXmlElement* pConfig = new TiXmlElement("config");
	pConfig->SetDoubleAttribute("sfx_volume", m_fSoundVolume);
	pConfig->SetDoubleAttribute("mus_volume", m_fMusicVolume);
	pConfig->SetAttribute("fullscreen", m_bFullscreen);
	pConfig->SetAttribute("lang", m_nLang);

	optionDoc.LinkEndChild(pConfig);

	if (!optionDoc.SaveFile(pathtowrite.c_str()))
		return false;

	return true;
}

bool COptionState::LoadOptions(const char* szFilename)
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

	std::string pathtowrite = stringstream.str() += "\\AllInGames\\PvR\\";
	pathtowrite += szFilename;


	double dTempSFX, dTempMus;
	int nFullscreen, nLang;

	TiXmlDocument optionDoc;

	if (!optionDoc.LoadFile(pathtowrite.c_str()))
		return false;

	TiXmlElement* pConfig = optionDoc.RootElement();

	pConfig->Attribute("fullscreen", &nFullscreen);
	pConfig->Attribute("sfx_volume", &dTempSFX);
	pConfig->Attribute("mus_volume", &dTempMus);
	pConfig->Attribute("lang", &nLang);

	m_bFullscreen = (nFullscreen ? true : false); //Awkward, I know

	m_fMusicVolume = static_cast<float>(dTempMus);
	m_fSoundVolume = static_cast<float>(dTempSFX);
	m_nLang = static_cast<OptionLanguage>(nLang);
	return true;
}

void COptionState::ApplyOptions()
{
	CSGD_XAudio2* pXA = CSGD_XAudio2::GetInstance();

	pXA->SFXSetMasterVolume(m_fSoundVolume);
	pXA->MusicSetMasterVolume(m_fMusicVolume);

	CSGD_Direct3D::GetInstance()->Resize(CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight(), !m_bFullscreen);

	if (m_nLang == ENGLISH)
	{
		SetThreadPreferredUILanguages(MUI_RESET_FILTERS, NULL, NULL);
		SetThreadUILanguage(LANG_ENGLISH);
	}
	else if (m_nLang == SPANISH)
	{
		SetThreadPreferredUILanguages(MUI_RESET_FILTERS, NULL, NULL);
		SetThreadUILanguage(LANG_SPANISH);
	}

	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_MUSIC_VOLUME, musicVolumeString, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_SOUND_VOLUME, soundVolumeString, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_FULLSCREEN, fullscreenString, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_ON, onString, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_OFF, offString, 20);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_LANGUAGE, languageString, 20);

}