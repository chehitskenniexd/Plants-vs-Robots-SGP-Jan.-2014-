//////////////////////////////////////////////////////////////////////////
//	File	:	"IListener.h"
//
//	Author	:	David Brown (DB)
//
//	Purpose	:	Base class for objects registered with CSGD_EventSystem to
//				handle event broadcasts.
///////////////////////////////////////////////////////////////////////////
#pragma once

// Forward declaration does not require header includes.
class CEvent;

// ABSTRACT BASE CLASS
class IListener
{
public:
	IListener( void ) {}
	virtual ~IListener( void ) {}

	//	Pure virtual method - MUST be defined in the derived class.
	//	Callback function triggers whenever this listener receives an event.
	virtual void HandleEvent( const CEvent* pEvent ) = 0;
};
