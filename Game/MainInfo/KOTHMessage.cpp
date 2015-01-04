#include "KOTHMessage.h"


CKOTHMessage::CKOTHMessage(OwnerFaction senderFaction) : CMessage(MSG_KOTH) 
{
	m_nFaction = senderFaction;
}

CKOTHMessage::~CKOTHMessage(void)
{
}