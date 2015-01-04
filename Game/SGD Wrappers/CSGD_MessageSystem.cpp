//////////////////////////////////////////////////////////////////////////
//	File	:	"CSGD_MessageSystem.cpp"
//
//	Author	:	David Brown (DB)
//
//	Purpose	:	Handles sending the messages to the specified proc function.
///////////////////////////////////////////////////////////////////////////

#include "CSGD_MessageSystem.h"
using std::queue;


//	Get Instance
CSGD_MessageSystem* CSGD_MessageSystem::GetInstance( void )
{
	static CSGD_MessageSystem s_Instance;
	return &s_Instance;
}

//	Initialize
void CSGD_MessageSystem::Initialize( void (*pfnMsgProc)( IMessage* ) )
{
	//	Store the message callback function.
	m_pfnMsgProc = pfnMsgProc;
}

//	QueueMessage
void CSGD_MessageSystem::QueueMessage( IMessage* pMsg )
{
	//	Error check that the message exists.
	if( pMsg == nullptr )	
		return;

	//	Send the message to the queue for processing later on.
	m_MsgQueue.push( pMsg );
}

//	Process Messages
void CSGD_MessageSystem::ProcessMessages( void )
{
	//	Error check that there is a callback function.
	if( m_pfnMsgProc == nullptr )	
		return;

	//	Go through the entire queue and process the messsages that are waiting.
	while( m_MsgQueue.empty() == false )
	{
		m_pfnMsgProc( m_MsgQueue.front() );		//	Process the first message.
		delete m_MsgQueue.front();				//	Delete the message.
		m_MsgQueue.pop();						//	Go to the next message.
	}
}

//	Clear Messages
void CSGD_MessageSystem::ClearMessages( void )
{
	//	Clear out any messages waititng.
	while( m_MsgQueue.empty() == false )
	{
		delete m_MsgQueue.front();
		m_MsgQueue.pop();
	}
}

//	Terminate
void CSGD_MessageSystem::Terminate( void )
{
	//	Clear out any messages waiting.
	ClearMessages();

	//	Clear the function pointer
	m_pfnMsgProc = NULL;
}