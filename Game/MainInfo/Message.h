/*************************************************************
|	File:		Message.h
|	Purpose:	To identify the type of message sent through
|				the Message System to be received by the 
|				game's message proc function.
|
|	Author:		Doug Monroe (dmonroe@fullsail.com)
|	Course:		SGD 1312
*************************************************************/

#pragma once

#include "../SGD Wrappers/IMessage.h"


//************************************************************
// Enumeration of message types
enum MsgType { UNIT_MOVE, UNIT_CONVERT, MSG_DESTROY_ENTITY, MSG_CREATE_ENTITY, MSG_LEVEL_COMPLETED, MSG_KOTH };


//************************************************************
// CMessage class
//	- concrete base class for messages sent to the Message System
//	- message ID must be one of the MsgType enumerators
//	- child classes store additional data when necessary
class CMessage : public IMessage
{
public:
	//********************************************************
	// Constructor REQUIRES a message type enumerator
	CMessage( MsgType id )						{	m_eMessageID = id;		}
	virtual ~CMessage( void )					{	/* do nothing */		}

	
	//********************************************************
	// IMessage Interface:
	virtual MSGID GetMessageID( void ) final	{	return m_eMessageID;	}

private:
	//********************************************************
	// message id
	MsgType		m_eMessageID;
};

