//////////////////////////////////////////////////////////////////////////
//	File	:	"CSGD_MessageSystem.h"
//
//	Author	:	David Brown (DB)
//
//	Purpose	:	Handles sending the messages to the specified proc function.
///////////////////////////////////////////////////////////////////////////
#pragma once

#include <queue>

#include "IMessage.h"

class CSGD_MessageSystem
{
private:
	std::queue< IMessage* >		m_MsgQueue;						//	Stores messages.
	void						(*m_pfnMsgProc)( IMessage* );	//	Points to user defined function.

	CSGD_MessageSystem() { m_pfnMsgProc = NULL;	}
	~CSGD_MessageSystem() {}

	CSGD_MessageSystem( const CSGD_MessageSystem& );
	CSGD_MessageSystem& operator= ( const CSGD_MessageSystem& );


public:
	
	static CSGD_MessageSystem* GetInstance( void );

	//	How many messages waiting to be processed.
	inline int GetNumMessages( void ) { return (int)m_MsgQueue.size(); }

	//	Setup the function pointer for our messages.
	void Initialize( void (*pfnMsgProc)( IMessage* ) );

	//	Sends the message into the queue and awaits processing later on through
	//	The ProcessMessages() function.
	void QueueMessage( IMessage* pMsg );

	//	Processes all the messages that are waiting inside of the queue.
	//	Normally you only call this function once per frame.
	void ProcessMessages( void );

	//	Clears any messages that may be remaining
	void ClearMessages( void );

	//	Clears any messages that may be remaining and sets the function pointer to NULL.
	void Terminate( void );
};