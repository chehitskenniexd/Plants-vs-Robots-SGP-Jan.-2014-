#pragma once
#include "Message.h"

//Forward decs
class CEntity;

class CDestroyEntityMessage :
	public CMessage
{
private:
	CEntity* m_pSender;
	CEntity* m_pOwner;
public:

	CDestroyEntityMessage(CEntity* pSender) : CMessage(MSG_DESTROY_ENTITY)
	{
		m_pSender = pSender;
	}

	CDestroyEntityMessage(CEntity* pOwner, CEntity* pSender) : CMessage(MSG_DESTROY_ENTITY)
	{
		m_pOwner = pOwner;
		m_pSender = pSender;
	}

	~CDestroyEntityMessage(void)
	{
	}

	CEntity* GetSender(void)
	{
		return m_pSender;
	}

	CEntity* GetOwner(void)
	{
		return m_pOwner;
	}
};