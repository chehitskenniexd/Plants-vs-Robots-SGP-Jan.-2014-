//////////////////////////////////////////////////////////////////////////
//	File	:	"CSGD_EventSystem.cpp"
//
//	Author	:	David Brown (DB)
//
//	Purpose	:	Handles broadcasting events to registered listeners.
///////////////////////////////////////////////////////////////////////////

#include "CSGD_EventSystem.h"

using std::pair;
using std::multimap;
using std::list;


//	Get Instance
/*static*/ CSGD_EventSystem* CSGD_EventSystem::GetInstance( void )
{
	static CSGD_EventSystem s_Instance;
	return &s_Instance;
}

//	Register Client
void CSGD_EventSystem::RegisterClient( const EVENTID& eventID, IListener* pClient )
{
	//	Error check to make sure the client exists and hasn't been registered for this event already.
	if( pClient == nullptr || AlreadyRegistered( eventID, pClient ) == true )
		return;

	//	Add the client to the database.
	m_ClientDatabase.insert( make_pair(eventID, pClient) );
}

//	Unregister Client
void CSGD_EventSystem::UnregisterClient( const EVENTID& eventID, const IListener* pClient )
{
	//	Make an iterator that will iterate all of our clients that
	//	should be receiveing this event.
	pair< multimap< EVENTID, IListener* >::iterator,
		  multimap< EVENTID, IListener* >::iterator > range;

	//	Find all of the clients that are able to receive this event.
	range = m_ClientDatabase.equal_range( eventID );

	//	Go through the list of clients that are able to receive this event.
	for( multimap< EVENTID, IListener* >::iterator mmIter = range.first; mmIter != range.second; ++mmIter )
	{
		//	Check if the iterator is the client to find.
		if( mmIter->second == pClient )
		{
			//	Remove the client from the database.
			mmIter = m_ClientDatabase.erase( mmIter );
			break;
		}
	}
}

//	Unregister Client All
void CSGD_EventSystem::UnregisterClientAll( const IListener* pClient )
{
	//	Make an iterator that will iterate all of our clients
	//	in the database.
	multimap< EVENTID, IListener* >::iterator mmIter = m_ClientDatabase.begin();

	//	Go through the list of clients.
	while( mmIter != m_ClientDatabase.end() )
	{
		// Check if the iterator is the client to find.
		if( mmIter->second == pClient )
		{
			//	Remove the client from the database.
			mmIter = m_ClientDatabase.erase(mmIter);
		}
		else
			++mmIter;
	}
}

//	Dispatch Event
void CSGD_EventSystem::DispatchEvent( const CEvent* pEvent )
{
	//	Make an iterator that will iterate all of our clients that
	//	should be receiveing this event
	pair< multimap< EVENTID, IListener* >::iterator,
		  multimap< EVENTID, IListener* >::iterator > range;

	//	Find all of the clients that are able to receive this event.
	range = m_ClientDatabase.equal_range( pEvent->GetEventID() );

	//	Go through the linked list of clients that are able to receive this event.
	for( multimap< EVENTID, IListener* >::iterator mmIter = range.first; mmIter != range.second; ++mmIter )
	{
		//	Pass this event to the client
		mmIter->second->HandleEvent( pEvent );
	}
}

//	Already Registered
bool CSGD_EventSystem::AlreadyRegistered( const EVENTID& eventID, const IListener* pClient )
{
	bool bIsAlreadyRegistered = false;

	//	Make an iterator that will iterate all of our clients that
	//	should be receiveing this event
	pair< multimap< EVENTID, IListener* >::iterator,
		  multimap< EVENTID, IListener* >::iterator > range;

	//	Find all of the clients that are able to receive this event.
	range = m_ClientDatabase.equal_range(eventID);

	//	Go through the list of clients that are able to receive this event.
	for( multimap< EVENTID, IListener* >::iterator mmIter = range.first; mmIter != range.second; ++mmIter )
	{
		//	Check if the pointer is equal to the client.
		if( mmIter->second == pClient )
		{
			bIsAlreadyRegistered = true;
			break;
		}
	}

	return bIsAlreadyRegistered;
}

//	Has Event Triggered
bool CSGD_EventSystem::HasEventTriggered( const EVENTID& eventID )
{
	// Check if the event is waiting to be processed.
	for( list< const CEvent >::iterator iter = m_EventQueue.begin(); iter != m_EventQueue.end(); ++iter )
		if( iter->GetEventID() == eventID )
			return true;


	// Not in the queue
	return false;
}

//	Queue Event
void CSGD_EventSystem::QueueEvent( const EVENTID& eventID, void* pData, const void* pDestination, void* pSender )
{
	//	Push the event into the list.
  	m_EventQueue.push_back( CEvent(eventID, pData, pDestination, pSender) );
}

//	Queue Unique Event
void CSGD_EventSystem::QueueUniqueEvent( const EVENTID& eventID, void* pData )
{
	//	Ignore if the event is already in the queue
	if( HasEventTriggered( eventID ) == true )
		return;

	//	Queue the event
	QueueEvent( eventID, pData );
}

//	Send Event Now
void CSGD_EventSystem::SendEventNow( const EVENTID& eventID, void* pData, const void* pDestination, void* pSender )
{
	//	Create and process the event immediately
	CEvent newEvent(eventID, pData, pDestination, pSender);

	DispatchEvent( &newEvent );
}

//	Process Events
void CSGD_EventSystem::ProcessEvents( void )
{
	//	Go through the list of events that are waiting to be processed.
	while( m_EventQueue.empty() == false )
	{
		//	Send the event to every registered listener.
		DispatchEvent( &m_EventQueue.front() );

		//	Remove the event from the queue.
		m_EventQueue.pop_front();
	}
}

//	Clear Events
void CSGD_EventSystem::ClearEvents( void )
{
	m_EventQueue.clear();
}

//	Shutdown Event System
void CSGD_EventSystem::Terminate( void )
{
	m_EventQueue.clear();
	m_ClientDatabase.clear();
}
