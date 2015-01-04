/*************************************************************
|	File:		IEntity.h
|	Purpose:	To declare the virtual interface for entities
|				& is the abstract base class for all game entities
|
|	Author:		Doug Monroe (dmonroe@fullsail.com)
|	Course:		SGD 1312
*************************************************************/

#pragma once


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>	// uses RECT


//************************************************************
// IEntity class
//	- abstract base class for all game entities
//	- Interface class
//		- contains the virtual methods for children classes to override
//		- does not store any data members
class IEntity
{
public:
	//********************************************************
	// Virtual Destructor
	//	- allows upcasted pointers to trigger child's destructor
	virtual ~IEntity( void )		// = default;
	{
	}

	
	//********************************************************
	// IEntity Interface:
	//	- MUST be overridden in child class
	virtual void Update( float fElapsedTime )	= 0;
	virtual void Render( void )					= 0;

	virtual int		GetType	( void ) const			= 0;
	virtual RECT	GetRect	( void ) const			= 0;
	virtual void	HandleCollision( const IEntity* pOther )	= 0;

	
	//********************************************************
	// Reference Counting:
	//	- keeps a shared object in memory while there are pointers to it
	virtual void AddRef		( void )	= 0;
	virtual void Release	( void )	= 0;
};

