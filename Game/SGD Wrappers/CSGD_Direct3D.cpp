////////////////////////////////////////////////////////
//  File			:	"CSGD_Direct3D.cpp"
//
//  Author			:	Jensen Rivera (JR)
//						Doug Monroe
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

#include "CSGD_Direct3D.h"

// The library files for Direct3D9
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")



// Instantiate the static object.
CSGD_Direct3D CSGD_Direct3D::m_Instance;


///////////////////////////////////////////////////////////////////
//	Function:	"CSGD_Direct3D" (Constructor)
///////////////////////////////////////////////////////////////////
CSGD_Direct3D::CSGD_Direct3D(void)
{
	m_lpDirect3DObject	= NULL;
	m_lpDirect3DDevice	= NULL;
	m_lpSprite			= NULL;
	m_lpFont			= NULL;
	m_lpTexture			= NULL;
	m_hWnd				= NULL;
	m_lWindowStyle		= 0L;
	memset(&m_PresentParams, 0, sizeof(D3DPRESENT_PARAMETERS));
}

///////////////////////////////////////////////////////////////////
//	Function:	"~CSGD_Direct3D" (Destructor)
///////////////////////////////////////////////////////////////////		
CSGD_Direct3D::~CSGD_Direct3D(void)
{
}

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
CSGD_Direct3D* CSGD_Direct3D::GetInstance(void)
{
	return &m_Instance;
}

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
LPDIRECT3D9					CSGD_Direct3D::GetDirect3DObject(void)	{ return m_lpDirect3DObject; }
LPDIRECT3DDEVICE9			CSGD_Direct3D::GetDirect3DDevice(void)	{ return m_lpDirect3DDevice; }
LPD3DXSPRITE				CSGD_Direct3D::GetSprite(void)			{ return m_lpSprite;		 }
LPD3DXFONT					CSGD_Direct3D::GetFont(void)			{ return m_lpFont;			 }
const D3DPRESENT_PARAMETERS*CSGD_Direct3D::GetPresentParams(void)	{ return &m_PresentParams;	 }

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
bool CSGD_Direct3D::Initialize(HWND hWnd, int nScreenWidth, int nScreenHeight, bool bIsWindowed, bool bVsync)
{
	// Make sure the hWnd is valid.
	if (!hWnd) return false;

	// Set the handle to the window & store window style.
	m_hWnd = hWnd;
	m_lWindowStyle = GetWindowLong( m_hWnd, GWL_STYLE );

	// Create the Direct3D9 Object.
	m_lpDirect3DObject = Direct3DCreate9(D3D_SDK_VERSION);

	// Make sure the object is valid.
	if (!m_lpDirect3DObject) DXERROR(_T("Failed to Create D3D Object"))

	// Setup the parameters for using Direct3D.
	m_PresentParams.BackBufferWidth				= nScreenWidth;
	m_PresentParams.BackBufferHeight			= nScreenHeight;
	m_PresentParams.BackBufferFormat			= D3DFMT_UNKNOWN;
	m_PresentParams.BackBufferCount				= 1;
	m_PresentParams.MultiSampleType				= D3DMULTISAMPLE_NONE;
	m_PresentParams.MultiSampleQuality			= 0;
	m_PresentParams.SwapEffect					= D3DSWAPEFFECT_COPY;
	m_PresentParams.hDeviceWindow				= hWnd;
	m_PresentParams.Windowed					= true;
	m_PresentParams.EnableAutoDepthStencil		= false;
	m_PresentParams.Flags						= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	m_PresentParams.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
	m_PresentParams.PresentationInterval		= (bVsync) ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;

	// Create the Direct3D Device.
	if (FAILED(m_lpDirect3DObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_PresentParams, &m_lpDirect3DDevice)))
		DXERROR(_T("Failed to Create D3D Device"))

	// Create Sprite Object.
	if (FAILED(D3DXCreateSprite(m_lpDirect3DDevice, &m_lpSprite)))
		DXERROR(_T("Failed to Create the Sprite object"))

	// Create Font Object.
	if (FAILED(
		D3DXCreateFont( m_lpDirect3DDevice,		// D3D device
                         23,					// Height
                         0,                     // Width
						 FW_SEMIBOLD,           // Weight
                         1,                     // MipLevels, 0 = autogen mipmaps
                         FALSE,                 // Italic
                         DEFAULT_CHARSET,       // CharSet
                         OUT_DEFAULT_PRECIS,    // OutputPrecision
                         DEFAULT_QUALITY,       // Quality
                         DEFAULT_PITCH | FF_DONTCARE, // PitchAndFamily
                         _T("Arial"),           // pFaceName
                         &m_lpFont )            // ppFont
		))
		DXERROR(_T("Failed to Create the Font Object"))


	// Create the Rect Texture.
	if (FAILED(D3DXCreateTexture(m_lpDirect3DDevice, 1, 1, 1, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, &m_lpTexture)))
		DXERROR(_T("Failed to Create the Rect Texture"))


	// Fill the Rect Texture.
    D3DLOCKED_RECT area;
    m_lpTexture->LockRect(0,&area,0,D3DLOCK_DISCARD);
    ((DWORD*)area.pBits)[0] = 0xFFFFFFFF;
    m_lpTexture->UnlockRect(0);
	

	// Preload font characters for speed.
	m_lpFont->PreloadCharacters( '0', '9' );
	m_lpFont->PreloadCharacters( 'A', 'Z' );
	m_lpFont->PreloadCharacters( 'a', 'z' );
	

	// Switch to fullscreen?
	if( !bIsWindowed )
		return Resize( nScreenWidth, nScreenHeight, bIsWindowed );

	// Return success.
	return true;
}

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
void CSGD_Direct3D::Terminate(void)
{
	SAFE_RELEASE(m_lpTexture);
	SAFE_RELEASE(m_lpFont);
	SAFE_RELEASE(m_lpSprite);
	SAFE_RELEASE(m_lpDirect3DDevice);
	SAFE_RELEASE(m_lpDirect3DObject);
}

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
void CSGD_Direct3D::Clear(DWORD dwColor)
{
	// Clear the device buffer with the given color (alpha is ignored)
	m_lpDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, dwColor, 1.0f, 0);

	// Check for lost device (could happen from an ALT+TAB or ALT+ENTER).
	if (m_lpDirect3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		m_lpFont->OnLostDevice();
		m_lpSprite->OnLostDevice();
		m_lpDirect3DDevice->Reset(&m_PresentParams);
		m_lpSprite->OnResetDevice();
		m_lpFont->OnResetDevice();
	}
}

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
bool CSGD_Direct3D::DeviceBegin(void)
{
	if (FAILED(m_lpDirect3DDevice->BeginScene()))
		DXERROR(_T("Failed to begin device scene."))

	return true;
}

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
bool CSGD_Direct3D::SpriteBegin(void)
{
	if (FAILED(m_lpSprite->Begin(D3DXSPRITE_ALPHABLEND)))
		DXERROR(_T("Failed to begin sprite scene."))

	return true;
}

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
bool CSGD_Direct3D::DeviceEnd(void)
{
	if (FAILED(m_lpDirect3DDevice->EndScene()))
		DXERROR(_T("Failed to end device scene."))

	return true;
}

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
bool CSGD_Direct3D::SpriteEnd(void)
{
	if (FAILED(m_lpSprite->End()))
		DXERROR(_T("Failed to end sprite scene."))

	return true;
}

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
void CSGD_Direct3D::Present(void)
{
	m_lpDirect3DDevice->Present(NULL, NULL, NULL, NULL);
}

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
bool CSGD_Direct3D::Resize(int nWidth, int nHeight, bool bWindowed)
{
	// Supported format
	D3DFORMAT format = D3DFMT_UNKNOWN;

	// Verify the resolution for full screen
	if( bWindowed == false )
	{
		D3DFORMAT larger = D3DFMT_UNKNOWN;
		int nLargerWidth  = INT_MAX;
		int nLargerHeight = INT_MAX;


		// Search the device types for a supported color format
		D3DFORMAT formats[ ] = { D3DFMT_X8R8G8B8, D3DFMT_R5G6B5, D3DFMT_X1R5G5B5 };
		for( int i = 0; i < _countof(formats) && format == D3DFMT_UNKNOWN; i++ )
		{			
			if( FAILED(m_lpDirect3DObject->CheckDeviceType( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, formats[ i ], formats[ i ], FALSE )) )
				continue;

			// Search the adapter modes for a matching resolution
			int size = m_lpDirect3DObject->GetAdapterModeCount( D3DADAPTER_DEFAULT, formats[ i ] );
			for( int j = 0; j < size; j++ )
			{
				D3DDISPLAYMODE mode = { };
				m_lpDirect3DObject->EnumAdapterModes( D3DADAPTER_DEFAULT, formats[ i ], j, &mode );

				// Exact resolution?
				if( nWidth == (int)mode.Width && nHeight == (int)mode.Height )
				{
					format = formats[ i ];
					break;
				}
				// Larger?
				else if( nWidth <= (int)mode.Width && nHeight <= (int)mode.Height )
				{
					// Smaller than the existing larger-size?
					if( (int)mode.Width <= nLargerWidth && (int)mode.Height <= nLargerHeight )
					{
						// But not the exact same size as the existing larger-size?
						if( (int)mode.Width != nLargerWidth || (int)mode.Height != nLargerHeight )
						{
							larger = formats[ i ];
							nLargerWidth  = (int)mode.Width;
							nLargerHeight = (int)mode.Height;
						}
					}
				}
			}
		}

		// Use a larger resolution?
		if( format == D3DFMT_UNKNOWN )
		{
			TCHAR error[ 72 ] = {};
			_stprintf_s( error, 72, _T("SGD DIRECT3D - Cannot go fullscreen with resolution %ix%i"), nWidth, nHeight );
			OutputDebugString( error );
			OutputDebugString( _T("\n") );

			if( larger != D3DFMT_UNKNOWN )
			{
				format  = larger;
				nWidth  = nLargerWidth;
				nHeight = nLargerHeight;
				
				_stprintf_s( error, 72, _T("SGD DIRECT3D - Going fullscreen with resolution %ix%i"), nWidth, nHeight );
				OutputDebugString( error );
				OutputDebugString( _T("\n") );
			}
			else
			{
				// FAIL!
				OutputDebugString( _T("SGD DIRECT3D - Failed to go fullscreen") );
				OutputDebugString( _T("\n") );
				return false;
			}
		}
		
	}


	// Set the new Presentation Parameters.
	m_PresentParams.BackBufferFormat	= format;
	m_PresentParams.Windowed			= bWindowed;
	m_PresentParams.BackBufferWidth		= nWidth;
	m_PresentParams.BackBufferHeight	= nHeight;

	// Reset the device.
	m_lpFont->OnLostDevice();
	m_lpSprite->OnLostDevice();
	m_lpDirect3DDevice->Reset(&m_PresentParams);
	m_lpSprite->OnResetDevice();
	m_lpFont->OnResetDevice();

	
	// Reset the window.
	if (bWindowed)
	{
		// Setup window style flags
		DWORD dwWindowStyleFlags = m_lWindowStyle; //WS_VISIBLE | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		SetWindowLong(m_hWnd, GWL_STYLE, dwWindowStyleFlags);
		//ShowCursor(TRUE); // show the mouse cursor

		// Center the window on the screen

		// Setup the desired client area size
		RECT rWindow;
		rWindow.left	= 0;
		rWindow.top		= 0;
		rWindow.right	= nWidth;
		rWindow.bottom	= nHeight;

		// Get the dimensions of a window that will have a client rect that
		// will really be the resolution we're looking for.
		AdjustWindowRectEx(&rWindow, 
							dwWindowStyleFlags,
							FALSE, 
							WS_EX_APPWINDOW);
		
		// Calculate the width/height of that window's dimensions
		int windowWidth		= rWindow.right - rWindow.left;
		int windowHeight	= rWindow.bottom - rWindow.top;

		SetWindowPos(m_hWnd, HWND_NOTOPMOST,	
						(GetSystemMetrics(SM_CXSCREEN) - windowWidth)/2,
						(GetSystemMetrics(SM_CYSCREEN) - windowHeight)/2,
						windowWidth, windowHeight, SWP_SHOWWINDOW);
	}
	else
	{
		// Setup window style flags
		DWORD dwWindowStyleFlags = WS_VISIBLE | WS_POPUP;
		SetWindowLong(m_hWnd, GWL_STYLE, dwWindowStyleFlags);
		//ShowCursor(FALSE); // hide the mouse cursor
		
		// Let windows know the window has changed.
		SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	}

	return true;
}


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
void CSGD_Direct3D::DrawRect(RECT rRt, DWORD dwColor)
{
	// OLD:	Clear a rectangle from the backbuffer
	//		(ignores alpha and draw order)

	//D3DRECT d3dRect;
	//d3dRect.x1 = rRt.left;
	//d3dRect.y1 = rRt.top;
	//d3dRect.x2 = rRt.right;
	//d3dRect.y2 = rRt.bottom;
	
	//m_lpDirect3DDevice->Clear(1, &d3dRect, D3DCLEAR_TARGET, D3DCOLOR_ARGB( 0, ucRed, ucGreen, ucBlue), 1.0f, 0);


	// NEW:	Use a 1x1 texture scaled and translated
	//		supports alpha & draw order
	//		no significant FPS change
	D3DXMATRIX transform;
	D3DXMatrixIdentity(&transform);

	transform.m[0][0] = (FLOAT)(rRt.right - rRt.left);
	transform.m[1][1] = (FLOAT)(rRt.bottom - rRt.top);
	transform.m[3][0] = (FLOAT)(rRt.left);
	transform.m[3][1] = (FLOAT)(rRt.top);

	// Apply the transform.
	m_lpSprite->SetTransform(&transform);

	// Draw the rectangle.
	m_lpSprite->Draw( m_lpTexture, NULL, NULL, NULL, dwColor );
	
	// Move the world back to identity.
	D3DXMatrixIdentity(&transform);
	m_lpSprite->SetTransform(&transform);
}

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
void CSGD_Direct3D::DrawHollowRect(RECT rRt, DWORD dwColor, int nWidth)
{
	// Use a 1x1 texture scaled and translated
	//		supports alpha & draw order
	//		no significant FPS change
	D3DXMATRIX transform;
	D3DXMatrixIdentity(&transform);

	// Enlarge the rect by 1 pixel to outline a rect
	rRt.right  += 1;
	rRt.bottom += 1;

	// Calculate size
	float width		= (float)(rRt.right - rRt.left);
	float height	= (float)(rRt.bottom - rRt.top);
	float lineWidth = (float)nWidth;
	float halfWidth = lineWidth * 0.5f;


	// Top line
	transform.m[0][0] = (FLOAT)width;
	transform.m[1][1] = (FLOAT)lineWidth;
	transform.m[3][0] = (FLOAT)(rRt.left - halfWidth);
	transform.m[3][1] = (FLOAT)(rRt.top	 - halfWidth);

	// Apply the transform.
	m_lpSprite->SetTransform(&transform);

	// Draw the rectangle.
	m_lpSprite->Draw( m_lpTexture, NULL, NULL, NULL, dwColor );
	

	// Left line
	transform.m[0][0] = (FLOAT)lineWidth;
	transform.m[1][1] = (FLOAT)height;
	transform.m[3][0] = (FLOAT)(rRt.left - halfWidth);
	transform.m[3][1] = (FLOAT)(rRt.top	 + halfWidth);

	// Apply the transform.
	m_lpSprite->SetTransform(&transform);

	// Draw the rectangle.
	m_lpSprite->Draw( m_lpTexture, NULL, NULL, NULL, dwColor );



	// Bottom line
	transform.m[0][0] = (FLOAT)width;
	transform.m[1][1] = (FLOAT)lineWidth;
	transform.m[3][0] = (FLOAT)(rRt.left   + halfWidth);
	transform.m[3][1] = (FLOAT)(rRt.bottom - halfWidth);

	// Apply the transform.
	m_lpSprite->SetTransform(&transform);

	// Draw the rectangle.
	m_lpSprite->Draw( m_lpTexture, NULL, NULL, NULL, dwColor );



	// Right line
	transform.m[0][0] = (FLOAT)lineWidth;
	transform.m[1][1] = (FLOAT)height;
	transform.m[3][0] = (FLOAT)(rRt.right - halfWidth);
	transform.m[3][1] = (FLOAT)(rRt.top	  - halfWidth);

	// Apply the transform.
	m_lpSprite->SetTransform(&transform);

	// Draw the rectangle.
	m_lpSprite->Draw( m_lpTexture, NULL, NULL, NULL, dwColor );


	// Move the world back to identity.
	D3DXMatrixIdentity(&transform);
	m_lpSprite->SetTransform(&transform);
}

///////////////////////////////////////////////////////////////////
//	Function:	"DrawLine"
//
//	Last Modified:		4/22/2013
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
void CSGD_Direct3D::DrawLine(int nX1, int nY1, int nX2, int nY2, DWORD dwColor, int nWidth)
{
	// OLD: Use ID3DXLine interface to render line
	//		does not support draw order

	//D3DXVECTOR2 verts[2];

	//verts[0].x = (float)nX1;
	//verts[0].y = (float)nY1;
	//verts[1].x = (float)nX2;
	//verts[1].y = (float)nY2;

	//m_lpLine->Draw(verts, 2, dwColor);

	
	// NEW:	Use a 1x1 texture scaled and translated
	//		supports alpha & draw order
	//		no significant FPS change
	int deltaX = nX2 - nX1;
	int deltaY = nY2 - nY1;

	// Line must have a difference in at least one direction
	if( deltaX == 0 && deltaY == 0 || nWidth <= 0 )
		return;

	// Calculate the length and direction
	float length	= sqrtf( float(deltaX * deltaX + deltaY * deltaY) );
	float angle		= acos ( deltaX / length );	// abbreviated DotProduct against <1,0>
	
	float width		= (float)nWidth;
	float halfWidth = width * 0.5f;
	//if( length < width )
	//	length = width;

	// Indefinite is 0 ... I suppose
	if( _isnan( angle ) )
		angle = 0.0f;
	else if( deltaY < 0 )
		angle = -angle;
	
	float cos = cosf( angle );
	float sin = sinf( angle );


	D3DXMATRIX transform;
	D3DXMatrixIdentity(&transform);

	transform.m[0][0] = (FLOAT)(length *  cos);		// scale & rotate
	transform.m[0][1] = (FLOAT)(length *  sin);
	transform.m[1][0] = (FLOAT)(width  * -sin);
	transform.m[1][1] = (FLOAT)(width  *  cos);

	transform.m[3][0] = (FLOAT)(nX1 + halfWidth * (+sin));			// translate to (x, y) rotated around (0, halfW)
	transform.m[3][1] = (FLOAT)(nY1 + halfWidth * (- cos));
	//transform.m[3][0] = (FLOAT)(nX1);
	//transform.m[3][1] = (FLOAT)(nY1);

	// Apply the transform.
	m_lpSprite->SetTransform(&transform);

	// Draw the rectangle.
	m_lpSprite->Draw( m_lpTexture, NULL, NULL, NULL, dwColor );
	
	// Move the world back to identity.
	D3DXMatrixIdentity(&transform);
	m_lpSprite->SetTransform(&transform);
}

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
void CSGD_Direct3D::DrawText(const TCHAR *lpzText, int nX, int nY, DWORD dwColor)
{
	// OLD: Use Win32 calls to render text to backbuffer
	//		(ignores alpha and draw order)

	//// Pointer to the Back Buffer.
	//LPDIRECT3DSURFACE9 d3dBackBuffer = 0;

	//// Get the Back Buffer from the Device.
	//m_lpDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &d3dBackBuffer);

	//// Get a Device Context.
	//HDC hdc = 0;
	//d3dBackBuffer->GetDC(&hdc);

	//// Print the string out to the screen.
	//SetTextColor(hdc, RGB(ucRed, ucGreen, ucBlue));
	//SetBkMode(hdc, TRANSPARENT);
	//TextOut(hdc, nX, nY, lpzText, (int)_tcslen(lpzText));

	//// Release the Device Context.
	//d3dBackBuffer->ReleaseDC(hdc);
	//SAFE_RELEASE(d3dBackBuffer);


	// NEW: Use D3DXFont object to render text
	//		supports alpha and draw order
	RECT rDrawRegion = { nX, nY };	// right & bottom are default

#if defined( _UNICODE ) || defined( UNICODE )
	m_lpFont->DrawTextW( m_lpSprite, lpzText, -1, &rDrawRegion, DT_NOCLIP, dwColor );
#else
	m_lpFont->DrawTextA( m_lpSprite, lpzText, -1, &rDrawRegion, DT_NOCLIP, dwColor );
#endif
}
