#pragma once
#include "Message.h"
#include "../MainInfo/Entity.h"
#include"../MainInfo/Game.h"
#include "../States/GameplayState.h"
#include "../States/GameOverState.h"

class CLevelCompletedMessage: public CMessage
{
private:


public:
	CLevelCompletedMessage(IGameState sCurrentState):CMessage(MSG_LEVEL_COMPLETED)
	{

	}

	~CLevelCompletedMessage(void)
	{

	}



};