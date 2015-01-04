//////////////////////////////////////////////////////////////////////////
//	File	:	"CSGD_EventSystem.h"
//
//	Author	:	David Brown (DB)
//
//	Purpose	:	Handles broadcasting events to registered listeners.
///////////////////////////////////////////////////////////////////////////
#pragma once

#include <map>
#include <list>

#include "CEvent.h"
#include "IListener.h"

class CSGD_EventSystem
{
private:
	//	Our Database, this will contain clients that can "listen" for events.
	std::multimap< EVENTID, IListener* >	m_ClientDatabase;

	//	Events waiting to be processed.
	std::list< const CEvent >				m_EventQueue;


	//	Utility functions - private because they will only ever be used by this class.
	//
	//	Finds the event in the database and then calls each of the registered client's 
	//	EventHandler functions.
	void DispatchEvent( const CEvent* pEvent );
	bool AlreadyRegistered( const EVENTID& eventID, const IListener* pClient );

	CSGD_EventSystem() {}
	~CSGD_EventSystem() {}

	CSGD_EventSystem( const CSGD_EventSystem& );
	CSGD_EventSystem& operator= ( const CSGD_EventSystem& );


public:

	static CSGD_EventSystem* GetInstance( void );

	//	This adds a client to the database.  This will make new "buckets" if necessary and then
	//	add the client to that given bucket.
	void RegisterClient( const EVENTID& eventID, IListener* pClient);

	//	Unregisters the client for the specified event only
	void UnregisterClient( const EVENTID& eventID, const IListener* pClient);

	//	Removes the client from the database entirely
	void UnregisterClientAll( const IListener* pClient );

	//	Checks if an event is already queued.
	bool HasEventTriggered( const EVENTID& eventID );

	//	Queues an event to be processed later on.
	void QueueEvent( const EVENTID& eventID, void* pData = NULL, const void* pDestination = NULL, void* pSender = NULL );

	//	Queues an event to be processed later on if it has not already been triggered.
	void QueueUniqueEvent( const EVENTID& eventID, void* pData = NULL );

	//	Sends an event to be processed immediately.
	void SendEventNow( const EVENTID& eventID, void* pData = NULL, const void* pDestination = NULL, void* pSender = NULL );

	//	Processes all events in our event list.
	void ProcessEvents( void );

	//	Clears all pending events
	void ClearEvents( void );

	//	Unregisters all objects
	void Terminate( void );
};