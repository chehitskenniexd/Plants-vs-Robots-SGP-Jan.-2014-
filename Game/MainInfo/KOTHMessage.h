#pragma once
#include "message.h"
#include "Entity.h"
class CKOTHMessage :
	public CMessage
{
public:
	CKOTHMessage(OwnerFaction senderFaction);
	~CKOTHMessage(void);
	OwnerFaction GetFaction (void) const {return m_nFaction;}
private:
	OwnerFaction m_nFaction;
};

