#include "CreditState.h"
#include "../MainInfo/Game.h"

#include "../MainInfo/BMFont.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"


//Height of each line
#define MENU_ITEMS_YOFF 55 


CCreditState::CCreditState(void)
{
}


CCreditState::~CCreditState(void)
{
}

/*static*/ CCreditState* CCreditState::GetInstance(void)
{
	static CCreditState s_Instance;
	return &s_Instance;
}


/*virtual*/ void CCreditState::Enter ( void ) /*override*/
{
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_CREDITS, szCredits, 32);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_EXECUTIVE_PRODUCER, executive, 32);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_ASSOCIATE_PRODUCER, associate, 32);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_ARTISTS, artist, 32);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_PROGRAMMERS, programmer, 32);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_SOUND, sound, 32);

	selectionBackSFX = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/SelectionBack.wav"));
	BG = CSGD_TextureManager::GetInstance()->LoadTexture(_T("Resources/Images/EmptyMenu.png"));

	MENU_ITEMS_X = int(CGame::GetInstance()->GetScreenWidth() * 0.40f);
	MENU_ITEMS_Y = int(CGame::GetInstance()->GetScreenHeight() * 0.40f);

	creditsAlpha = 0;
	creditsTimer = 0.0f;
}

/*virtual*/ void CCreditState::Exit	( void ) /*override*/
{				 

}				 

/*virtual*/ bool CCreditState::Input ( void ) /*override*/
{				 
	if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed(1) || CSGD_DirectInput::GetInstance()->KeyPressed(DIK_NUMPAD2))
	{
		CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionBackSFX);
		CGame::GetInstance()->PopState();
	}

	return true;	
}				 

/*virtual*/ void CCreditState::Update ( float fElapsedTime ) /*override*/
{				 
	creditsTimer += fElapsedTime;

	if(creditsTimer >= 25.0)
	{
		creditsTimer = 0.0f;
		creditsAlpha = 0;
	}

	if(creditsTimer - int(creditsTimer / 5)*5 <= 1.0f)
	{
		creditsAlpha += int(255 * fElapsedTime);
	}
	else if(creditsTimer - int(creditsTimer / 5)*5 <= 3.0f)
	{
		creditsAlpha = 255;
	}
	else if(creditsTimer - int(creditsTimer / 5)*5 <= 4.0f)
	{
		creditsAlpha -= int(255 * fElapsedTime);

		if(creditsAlpha <= 0)
			creditsAlpha = 0;
	}
}				 

/*virtual*/ void CCreditState::Render ( void ) /*override*/
{
	CBMFont* pF = CGame::GetInstance()->GetFont();

	CSGD_TextureManager::GetInstance()->Draw(BG, 0, 0);

	if(creditsTimer <= 5.0f)
	{
		pF->Write(executive, MENU_ITEMS_X, MENU_ITEMS_Y, 1.5f, D3DCOLOR_ARGB(creditsAlpha, 0, 255, 0));
		pF->Write("John O'Leske", MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF, 1.3f, D3DCOLOR_ARGB(creditsAlpha, 0, 255, 0));	
	}
	else if(creditsTimer <= 10.0f)
	{
		pF->Write(associate, MENU_ITEMS_X, MENU_ITEMS_Y, 1.5f, D3DCOLOR_ARGB(creditsAlpha, 255, 0, 255));
		pF->Write("Rob Martinez", MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF, 1.3f, D3DCOLOR_ARGB(creditsAlpha, 255, 0, 255));
	}
	else if(creditsTimer <= 15.0f)
	{
		pF->Write(programmer, MENU_ITEMS_X, MENU_ITEMS_Y, 1.5f, D3DCOLOR_ARGB(creditsAlpha, 0, 255, 0));
		pF->Write("William Snelson", MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF, 1.3f, D3DCOLOR_ARGB(creditsAlpha, 0, 255, 0));
		pF->Write("Kenneth Moy", MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * 2, 1.3f, D3DCOLOR_ARGB(creditsAlpha, 0, 255, 0));
		pF->Write("Joshua Cohen", MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * 3, 1.3f, D3DCOLOR_ARGB(creditsAlpha, 0, 255, 0));
	}
	else if(creditsTimer <= 20.0f)
	{
		pF->Write(artist, MENU_ITEMS_X, MENU_ITEMS_Y, 1.5f, D3DCOLOR_ARGB(creditsAlpha, 255, 0, 255));
		pF->Write("Gregory Bey", MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF, 1.3f, D3DCOLOR_ARGB(creditsAlpha, 255, 0, 255));
		pF->Write("Caris Frazier", MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF * 2, 1.3f, D3DCOLOR_ARGB(creditsAlpha, 255, 0, 255));
	}
	else if (creditsTimer <= 25.0f)
	{
		pF->Write(sound, MENU_ITEMS_X, MENU_ITEMS_Y, 1.5f, D3DCOLOR_ARGB(creditsAlpha, 255, 0, 255));
		pF->Write("Casey Pollard", MENU_ITEMS_X, MENU_ITEMS_Y + MENU_ITEMS_YOFF, 1.3f, D3DCOLOR_ARGB(creditsAlpha, 255, 0, 255));
	}
}