////////////////////////////////////////////////////////
//  File			:	"CSGD_Direct3D.h"
//
//  Author			:	Jensen Rivera (JR)
//
//  Date Created	:	5/25/2006
//
//  Purpose			:	Wrapper class for Direct3D.
////////////////////////////////////////////////////////

/* 
	Disclaimer:
	This source code was developed for and is the property of:

	(c) Full Sail University

	Full Sail students may not redistribute or make this code public, 
	but may use it in their own personal projects.
*/

#pragma once

// The include files for Direct3D9
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>

//	For macros
#include "SGD_Util.h" // for SAFE_RELEASE and DXERROR

#if defined( DrawText )
	#undef  DrawText
#endif

class CSGD_Direct3D
{
private:
	// The Direct3D object.
	LPDIRECT3D9					m_lpDirect3DObject;

	// The Direct3D Device.
	LPDIRECT3DDEVICE9			m_lpDirect3DDevice;

	// The Direct3DX Sprite Interface.
	LPD3DXSPRITE				m_lpSprite;

	// The Direct3DX Font Interface.
	LPD3DXFONT					m_lpFont;

	// A Direct3D Texture for Rectangles.
	LPDIRECT3DTEXTURE9			m_lpTexture;

	// The presentation parameters of the device.
	D3DPRESENT_PARAMETERS		m_PresentParams;

	// The handle to the window Direct3D is initialized in.
	HWND						m_hWnd;

	// The starting window style flags.
	LONG						m_lWindowStyle;

	// The single static object of this class.
	static CSGD_Direct3D		m_Instance;

private:
	///////////////////////////////////////////////////////////////////
	//	Function:	"CSGD_Direct3D" (Constructors)
	///////////////////////////////////////////////////////////////////
	CSGD_Direct3D(void);
	CSGD_Direct3D(CSGD_Direct3D &ref);
	CSGD_Direct3D &operator=(CSGD_Direct3D &ref);
	~CSGD_Direct3D(void);

public:	

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetInstance"
	//
	//	Last Modified:		5/25/2006
	//
	//	Input:		void
	//
	//	Return:		An instance to this class.
	//
	//	Purpose:	Returns an instance to this class.
	///////////////////////////////////////////////////////////////////
	static CSGD_Direct3D* GetInstance(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"CSGD_Direct3D(Accessors)"
	//
	//	Last Modified:		5/25/2006
	//
	//	Input:		void
	//
	//	Return:		A pointer to a data member in this class.
	//
	//	Purpose:	Accesses data members in this class.
	///////////////////////////////////////////////////////////////////
	LPDIRECT3D9					GetDirect3DObject(void);
	LPDIRECT3DDEVICE9			GetDirect3DDevice(void);
	LPD3DXSPRITE				GetSprite(void);
	LPD3DXFONT					GetFont(void);
	const D3DPRESENT_PARAMETERS*GetPresentParams(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"Initialize"
	//
	//	Last Modified:		2/26/2013
	//
	//	Input:		hWnd			A handle to the window to initialize
	//								Direct3D in.
	//				nScreenWidth	The width to initialize the device into.
	//				nScreenHeight	The height to initialize the device into.
	//				bIsWindowed		The screen mode to initialize the device into.
	//				bVsync			true to put the device into vsynced display mode.
	//
	//	Return:		true if successful.
	//
	//	Purpose:	Initializes Direct3D9.
	///////////////////////////////////////////////////////////////////
	bool Initialize(HWND hWnd, int nScreenWidth, int nScreenHeight, bool bIsWindowed = true, bool bVsync = true);

	///////////////////////////////////////////////////////////////////
	//	Function:	"Terminate"
	//
	//	Last Modified:		5/25/2006
	//
	//	Input:		void
	//
	//	Return:		void
	//
	//	Purpose:	Shuts down Direct3D9.
	///////////////////////////////////////////////////////////////////
	void Terminate(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"Clear"
	//
	//	Last Modified:		2/26/2013
	//
	//	Input:		dwColor		The color to clear the screen with (use D3DCOLOR_XRGB() macro).
	//
	//	Return:		void
	//
	//	Purpose:	Clears the screen to a given color.
	///////////////////////////////////////////////////////////////////
	void Clear(DWORD dwColor = 0xFF000000);

	///////////////////////////////////////////////////////////////////
	//	Function:	"DeviceBegin"
	//
	//	Last Modified:		5/25/2006
	//
	//	Input:		void
	//
	//	Return:		true if successful.
	//
	//	Purpose:	Begins the device for rendering.
	///////////////////////////////////////////////////////////////////
	bool DeviceBegin(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"SpriteBegin"
	//
	//	Last Modified:		5/25/2006
	//
	//	Input:		void
	//
	//	Return:		true if successful.
	//
	//	Purpose:	Begins the sprite for rendering.
	///////////////////////////////////////////////////////////////////
	bool SpriteBegin(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"DeviceEnd"
	//
	//	Last Modified:		5/25/2006
	//
	//	Input:		void
	//
	//	Return:		true if successful.
	//
	//	Purpose:	Ends the device for rendering.
	///////////////////////////////////////////////////////////////////
	bool DeviceEnd(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"SpriteEnd"
	//
	//	Last Modified:		5/25/2006
	//
	//	Input:		void
	//
	//	Return:		true if successful.
	//
	//	Purpose:	Ends the sprite for rendering.
	///////////////////////////////////////////////////////////////////
	bool SpriteEnd(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"Present"
	//
	//	Last Modified:		5/25/2006
	//
	//	Input:		void
	//
	//	Return:		void
	//
	//	Purpose:	Renders your back buffer to the screen.
	///////////////////////////////////////////////////////////////////
	void Present(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"Resize"
	//
	//	Last Modified:		2/22/2013
	//
	//	Input:		nWidth		The width to change the device to.
	//				nHeight		The height to change the device to.
	//				bWindowed	The mode to put the window in.
	//
	//	Return:		true if successful
	//
	//	Purpose:	Changes the display parameters of the device.
	///////////////////////////////////////////////////////////////////
	bool Resize(int nWidth, int nHeight, bool bWindowed);

	///////////////////////////////////////////////////////////////////
	//	Function:	"DrawRect"
	//
	//	Last Modified:		2/26/2013
	//
	//	Input:		rRt			The region of the screen to fill.
	//				dwColor		The color to fill the rect with (use D3DCOLOR_ARGB() macro).
	//
	//	Return:		void
	//
	//	Purpose:	Draws a rectangle of a given color to the screen.
	///////////////////////////////////////////////////////////////////
	void DrawRect(RECT rRt, DWORD dwColor = 0xFFFFFFFF);

	///////////////////////////////////////////////////////////////////
	//	Function:	"DrawHollowRect"
	//
	//	Last Modified:		3/28/2013
	//
	//	Input:		rRt			The region of the screen to outline.
	//				dwColor		The color of the border (use D3DCOLOR_ARGB() macro).
	//				nWidth		The width of the rect's border.
	//
	//	Return:		void
	//
	//	Purpose:	Draws the border around a rectangle of a given color to the screen.
	///////////////////////////////////////////////////////////////////
	void DrawHollowRect(RECT rRt, DWORD dwColor = 0xFFFFFFFF, int nWidth = 3);

	///////////////////////////////////////////////////////////////////
	//	Function:	"DrawLine"
	//
	//	Last Modified:		3/28/2013
	//
	//	Input:		nX1			The starting x of the line.
	//				nY1			The starting y of the line.
	//				nX2			The ending x of the line.
	//				nY2			The ending y of the line.
	//				dwColor		The color to draw the line with (use D3DCOLOR_ARGB() macro).
	//				nWidth		The width of the line.
	//
	//	Return:		void
	//
	//	Purpose:	Draws a line of a given color to the screen.
	///////////////////////////////////////////////////////////////////
	void DrawLine(int nX1, int nY1, int nX2, int nY2, DWORD dwColor = 0xFFFFFFFF, int nWidth = 3);

	///////////////////////////////////////////////////////////////////
	//	Function:			"DrawText"
	//
	//	Last Modified:		2/26/2013
	//
	//	Input:		lpzText		The text to draw to the screen.
	//				nX			The x position to draw the text at.
	//				nY			The y position to draw the text at.
	//				dwColor		The color to draw the text with (use D3DCOLOR_ARGB() macro).
	//
	//	Return:		void
	//
	//	Purpose:	Draws text to the screen with a given color.
	///////////////////////////////////////////////////////////////////
	void DrawText( const TCHAR *lpzText, int nX, int nY, DWORD dwColor = 0xFFFFFFFF);
};
