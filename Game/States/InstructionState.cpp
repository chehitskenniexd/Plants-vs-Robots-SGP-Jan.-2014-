#include "InstructionState.h"
#include "../MainInfo/Game.h"

#include "../MainInfo/BMFont.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"

CInstructionState::CInstructionState(void)
{
}


CInstructionState::~CInstructionState(void)
{
}

/*static*/ CInstructionState* CInstructionState::GetInstance(void)
{
	static CInstructionState s_Instance;
	return &s_Instance;
}


/*virtual*/ void CInstructionState::Enter ( void ) /*override*/
{
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_INSTRUCTIONS, instructions, 30);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_UNITS, units, 30);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_WINCONDITIONS, winningConditions, 30);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_WINCONDITION1, winningCondition1, 30);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_WINCONDITION2, winningCondition2, 30);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TIPS, tips, 30);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TIP1, tip1, 30);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TIP2, tip2, 30);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_TILETYPES, tileType, 30);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_LIQUIDTILES, liquidTiles, 30);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_IMPASSABLETILES, impassableTiles, 30);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_NORMALTILES, normalTiles, 30);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_IMPORTANTINFOS, importantInformations, 50);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_IMPORTANTINFO1, importantInformation1, 50);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_IMPORTANTINFO2, importantInformation2, 50);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_BONUS, Bonuses, 100);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_PENALTY, Penalties, 100);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_LIQUIDMECH, waterMechanic, 100);
	LoadStringA(CGame::GetInstance()->GethInstance(), IDS_IMPASSABLEMECH, impassableMechanic, 100);

	selectionBackSFX = CSGD_XAudio2::GetInstance()->SFXLoadSound(_T("Resources/Sounds/SelectionBack.wav"));

	TM = CSGD_TextureManager::GetInstance();

	unitID = TM->LoadTexture(_T("../Resources/Images/Sprite Sheet.png"));
	backgroundID = TM->LoadTexture(_T("../Resources/Images/BG.png"));
}

/*virtual*/ void CInstructionState::Exit	( void ) /*override*/
{				 

}				 

/*virtual*/ bool CInstructionState::Input ( void ) /*override*/
{				 
	if (CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed(1) || CSGD_DirectInput::GetInstance()->KeyPressed(DIK_NUMPAD6))
	{
		CSGD_XAudio2::GetInstance()->SFXPlaySound(selectionBackSFX);
		CGame::GetInstance()->PopState();
	}

	return true;
}				 

/*virtual*/ void CInstructionState::Update ( float fElapsedTime ) /*override*/
{				 

}				 

/*virtual*/ void CInstructionState::Render ( void ) /*override*/
{
	CBMFont* pF = CGame::GetInstance()->GetFont();

	//////////////////////////////////////////////////////////
	// BUG FIX: 76|84
	// Fixed By: Kenneth Moy
	//////////////////////////////////////////////////////////
	// Begin Fix
	TM->Draw(backgroundID, 0, 0);

	pF->Write(units, 200, 80, 1.3f, D3DCOLOR_XRGB(255, 100, 100));
	//pF->Write(plants, 50, 130, 1f, D3DCOLOR_XRGB(255, 255, 255));
	//pF->Write(robots, 50, 180, 1f, D3DCOLOR_XRGB(255, 255, 255));
	RECT plantBase = {164, 74, 228, 138};
	TM->Draw(unitID, 50, 150, 1.0f, 1.0f, &plantBase);

	RECT plantInfantry = {178, 204, 242, 268};
	TM->Draw(unitID, 120, 150, 1.0f, 1.0f, &plantInfantry);

	RECT plantScout = {276, 308, 340, 372};
	TM->Draw(unitID, 190, 150, 1.0f, 1.0f, &plantScout);

	RECT plantTank = {190, 581, 254, 645};
	TM->Draw(unitID, 260, 150, 1.0f, 1.0f, &plantTank);

	RECT robotBase = {868, 589, 932, 653};
	TM->Draw(unitID, 50, 220, 1.0f, 1.0f, &robotBase);

	RECT robotInfantry = {890, 127, 954, 191};
	TM->Draw(unitID, 120, 220, 1.0f, 1.0f, &robotInfantry);

	RECT robotScout = {892, 260, 956, 324};
	TM->Draw(unitID, 190, 220, 1.0f, 1.0f, &robotScout);

	RECT robotTank = {718, 462, 782, 526};
	TM->Draw(unitID, 260, 220, 1.0f, 1.0f, &robotTank);
	// End Fix

	pF->Write(tileType, int(CGame::GetInstance()->GetScreenWidth()*0.61f), int(CGame::GetInstance()->GetScreenHeight()*0.1f), 1.3f, D3DCOLOR_XRGB(255, 100, 100));
	pF->Write(normalTiles, int(CGame::GetInstance()->GetScreenWidth()*0.49f), int(CGame::GetInstance()->GetScreenHeight()*0.17f), 1.0f, D3DCOLOR_XRGB(255, 255, 255));
	pF->Write(Bonuses, int(CGame::GetInstance()->GetScreenWidth()*0.69f), int(CGame::GetInstance()->GetScreenHeight()*0.175f), 0.8f, D3DCOLOR_XRGB(200, 200, 200));
	pF->Write(Penalties, int(CGame::GetInstance()->GetScreenWidth()*0.69f), int(CGame::GetInstance()->GetScreenHeight()*0.235f), 0.8f, D3DCOLOR_XRGB(200, 200, 200));
	pF->Write(liquidTiles, int(CGame::GetInstance()->GetScreenWidth()*0.49f), int(CGame::GetInstance()->GetScreenHeight()*0.300f), 1.0f, D3DCOLOR_XRGB(255, 255, 255));
	pF->Write(waterMechanic, int(CGame::GetInstance()->GetScreenWidth()*0.69f), int(CGame::GetInstance()->GetScreenHeight()*0.305f), 0.8f, D3DCOLOR_XRGB(200, 200, 200));
	pF->Write(impassableTiles, int(CGame::GetInstance()->GetScreenWidth()*0.49f), int(CGame::GetInstance()->GetScreenHeight()*0.371f), 1.0f, D3DCOLOR_XRGB(255, 255, 255));
	pF->Write(impassableMechanic, int(CGame::GetInstance()->GetScreenWidth()*0.69f), int(CGame::GetInstance()->GetScreenHeight()*0.376f), 0.8f, D3DCOLOR_XRGB(200, 200, 200));

	pF->Write(winningConditions, 100, 350, 1.3f, D3DCOLOR_XRGB(255, 100, 100));
	pF->Write(winningCondition1, 50, 410, 0.8f, D3DCOLOR_XRGB(200, 200, 200));
	pF->Write(winningCondition2, 50, 450, 0.8f, D3DCOLOR_XRGB(200, 200, 200));

	pF->Write(importantInformations, 90, 550, 1.3f, D3DCOLOR_XRGB(255, 100, 100));
	pF->Write(importantInformation1, 50, 610, 0.8f, D3DCOLOR_XRGB(200, 200, 200));
	pF->Write(importantInformation2, 50, 650, 0.8f, D3DCOLOR_XRGB(200, 200, 200));

	pF->Write(tips, 625, 550, 1.3f, D3DCOLOR_XRGB(255, 100, 100));
	pF->Write(tip1, 500, 610, 0.8f, D3DCOLOR_XRGB(200, 200, 200));
	pF->Write(tip2, 500, 650, 0.8f, D3DCOLOR_XRGB(200, 200, 200));

}