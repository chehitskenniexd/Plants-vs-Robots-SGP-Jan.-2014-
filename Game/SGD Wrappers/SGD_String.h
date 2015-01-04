//////////////////////////////////////////////////////////////////////////
//	File	:	"SGD_String.h"
//
//	Author	:	Doug Monroe (DM)
//
//	Purpose	:	To wrap TCHAR functionality
///////////////////////////////////////////////////////////////////////////


#pragma once


#include <tchar.h>
#include <string>
#include <sstream>

// Unicode-friendly std strings:
#if defined UNICODE || defined _UNICODE 
	typedef std::wstring			TSTRING;
	typedef std::wostringstream		TOSTRINGSTREAM;
#else
	typedef std::string				TSTRING;
	typedef std::ostringstream		TOSTRINGSTREAM;
#endif


// TCHAR-friendly str functions
#define TSTRCPY_S		_tcscpy_s
#define TSTRCMP			_tcscmp
#define TSTRICMP		_tcsicmp
#define TSTRLEN			_tcslen
#define TSPRINTF_S		_stprintf_s

#define TCTOUPPER		_totupper
#define TCTOLOWER		_totlower


// Conversion functions

/*
errno_t wcstombs_s( size_t *pNumCharsConverted, char *szDest, size_t unDestSizeInBytes, const wchar_t *wszSource, size_t unMaxCountInBytes );
*/

#if defined UNICODE || defined _UNICODE 

	#define TSTR_TO_CSTR	WSTR_TO_CSTR		//	( char *szDest,		size_t unNumberOfElements,	const wchar_t *wszSource	);
	#define CSTR_TO_TSTR	CSTR_TO_WSTR		//	( wchar_t *wszDest,	size_t unNumberOfElements,	const char *szSource		);

	#define TSTR_TO_WSTR	WSTR_TO_WSTR		//	( wchar_t *wszDest,	size_t unNumberOfElements,	const wchar_t *wszSource	);
	#define WSTR_TO_TSTR	WSTR_TO_WSTR		//	( wchar_t *wszDest,	size_t unNumberOfElements,	const wchar_t *wszSource	);

#else
	
	#define TSTR_TO_CSTR	CSTR_TO_CSTR		//	( char *szDest,		size_t unNumberOfElements,	const char *szSource		);
	#define CSTR_TO_TSTR	CSTR_TO_CSTR		//	( char *szDest,		size_t unNumberOfElements,	const char *szSource		);

	#define TSTR_TO_WSTR	CSTR_TO_WSTR		//	( wchar_t *wszDest,	size_t unNumberOfElements,	const char *szSource		);
	#define WSTR_TO_TSTR	WSTR_TO_CSTR		//	( char *szDest,		size_t unNumberOfElements,	const wchar_t *wszSource	);

#endif


#define CSTR_TO_WSTR( /* wchar_t* */wszDest,	/* size_t */unNumberOfElements,	/* const char* */szSource ) \
	mbstowcs_s(nullptr, wszDest, unNumberOfElements, szSource, _TRUNCATE );

#define WSTR_TO_CSTR( /* char* */szDest,		/* size_t */unNumberOfElements,	/* const wchar_t* */wszSource ) \
	wcstombs_s(nullptr, szDest, unNumberOfElements, wszSource, _TRUNCATE );
	
#define WSTR_TO_WSTR( /* wchar_t* */wszDest,	/* size_t */unNumberOfElements,	/* const wchar_t* */wszSource ) \
	wcsncpy_s(wszDest, unNumberOfElements, wszSource, _TRUNCATE );
	
#define CSTR_TO_CSTR( /* char* */szDest,		/* size_t */unNumberOfElements,	/* const char* */szSource ) \
	strncpy_s(szDest, unNumberOfElements, szSource, _TRUNCATE );
