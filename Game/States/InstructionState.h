#pragma once
#include "igamestate.h"

class CSGD_TextureManager;

class CInstructionState :
	public IGameState
{
public:
	CInstructionState(void);
	~CInstructionState(void);

	static CInstructionState* GetInstance(void);

	virtual void Enter ( void ) override;	// load resources
	virtual void Exit (  void ) override;	// unload resources
	virtual bool Input ( void ) override;	// handle user input
	virtual void Update ( float fElapsedTime ) override;	// update game entities / animations
	virtual void Render ( void ) override;	// render game entities / menus

private:
	char instructions[30];
	char units[30];
	char winningConditions[30];
	char winningCondition1[30];
	char winningCondition2[30];
	char tips[30];
	char tip1[30];
	char tip2[30];
	char tileType[30];
	char liquidTiles[30];
	char impassableTiles[30];
	char normalTiles[30];
	char importantInformations[50];
	char importantInformation1[50];
	char importantInformation2[50];
	char Bonuses[100];
	char Penalties[100];
	char waterMechanic[100];
	char impassableMechanic[100];

	int unitID, backgroundID;

	int selectionBackSFX;

	CSGD_TextureManager* TM;
};

