////////////////////////////////////////////////////////////////////////////////////////
//  File			:	"ManagedDirect3D.cs"
//
//  Authors			:	David Brown
//                      Ron Powel
//                      Rob Martinez
//                      John OLeske
//						Doug Monroe
//
//  Purpose			:	Wrapper class for Managed Direct3D.
//
//  NOTE:   In order to use these wrapper classes you will need
//          to add References to the following assemblies to your Solution:
//
//          -Microsoft.DirectX              version: 1.0.2902.0 runtime: v1.1.4322
//          -Microsoft.DirectX.Direct3D     version: 1.0.2902.0 runtime: v1.1.4322
//          -Microsoft.DirectX.Direct3DX    version: 1.0.2902.0 runtime: v1.1.4322
////////////////////////////////////////////////////////////////////////////////////////

using System;						// for Int32
using System.Drawing;				// for Rectangle & Color
using System.Windows.Forms;			// for Control & MessageBox
using System.Collections.Generic;	// for List

using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

namespace SGP
{
	public sealed class CSGP_Direct3D
	{
		// Direct3D Device
		Device m_Device = null;
		PresentParameters m_PresentParams = null;

		// Sprite Library for Textures & Geometry
		Sprite m_Sprite = null;
		Texture m_Texture = null;

		// Font
		Microsoft.DirectX.Direct3D.Font m_Font = null;
		FontDescription m_FontDescription;

		// Render Targets
		List<RenderTarget> m_RenderTargets = new List<RenderTarget>( );
		int m_ActiveTarget = 0;


		// SINGLETON! (Thread-safe instance)
		static readonly CSGP_Direct3D instance = new CSGP_Direct3D( );

		// Explicit static constructor to tell C# compiler
		// not to mark type as beforefieldinit
		static CSGP_Direct3D( )
		{
		}

		CSGP_Direct3D( )
		{
		}

		/// <summary>
		/// Gets the instance of the singleton.
		/// </summary>
		public static CSGP_Direct3D GetInstance()
		{
			return instance;
		}

		/// <summary>
		/// Gets the Direct3D device.
		/// </summary>
		public Microsoft.DirectX.Direct3D.Device Device
		{
			get
			{
				return m_Device;
			}
		}

		/// <summary>
		/// Gets the Sprite object.
		/// </summary>
		public Microsoft.DirectX.Direct3D.Sprite Sprite
		{
			get
			{
				return m_Sprite;
			}
		}



		/// <summary>
		/// Initializes Direct3D with a render target.
		/// </summary>
		/// <param name="renderWindow">Window to render to.</param>
		/// <param name="vsync">Should the renderer wait for a vsync before drawing?</param>
		/// <returns>true if successful, false otherwise.</returns>
		public bool Initialize( Control renderWindow, bool vsync )
		{
			// Initialize D3D with the render target's size
			return Initialize( renderWindow, renderWindow.Width, renderWindow.Height, vsync );
		}

		/// <summary>
		/// Initializes Direct3D with a render target and backbuffer size.
		/// </summary>
		/// <param name="renderWindow">Window to render to.</param>
		/// <param name="screenWidth">Width of the screen in pixels.</param>
		/// <param name="screenHeight">Height of the screen in pixels.</param>
		/// <param name="vsync">Should the renderer wait for a vsync before drawing?</param>
		/// <returns>true if successful, false otherwise.</returns>
		public bool Initialize( Control renderWindow, int screenWidth, int screenHeight, bool vsync )
		{
			// Verify that the device has not been initialized
			if( m_Device != null )
				return false;


			// Set up the Direct3D device
			try
			{
				// Set the present parameters
				m_PresentParams = new PresentParameters( );
				m_PresentParams.BackBufferWidth = screenWidth;
				m_PresentParams.BackBufferHeight = screenHeight;
				m_PresentParams.BackBufferFormat = Format.Unknown;
				m_PresentParams.BackBufferCount = 1;
				m_PresentParams.MultiSample = MultiSampleType.None;
				m_PresentParams.MultiSampleQuality = 0;
				m_PresentParams.SwapEffect = SwapEffect.Copy;
				m_PresentParams.DeviceWindow = renderWindow;
				m_PresentParams.Windowed = true;
				m_PresentParams.EnableAutoDepthStencil = false;
				m_PresentParams.FullScreenRefreshRateInHz = 0;
				m_PresentParams.PresentationInterval = (vsync) ? PresentInterval.Default : PresentInterval.Immediate;


				// Create the device
				m_Device = new Device( 0, DeviceType.Hardware, renderWindow,
							CreateFlags.HardwareVertexProcessing, m_PresentParams );

				// Add the window to the render target list
				m_RenderTargets.Add( new RenderTarget( m_Device, m_PresentParams ) );
			}
			catch( Exception ex)
			{
				DialogResult r = MessageBox.Show( "Failed to Create D3D Device\n\n" + ex.Message, "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation );

				return false;
			}


			// Set up the Sprite object
			try
			{
				m_Sprite = new Sprite( m_Device );
			}
			catch( Exception ex)
			{
				DialogResult r = MessageBox.Show( "Failed to Create the Sprite object\n\n" + ex.Message, "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation );

				return false;
			}


			// Set up the Texture for rectangles & lines
			try
			{
				// Allocate the texture
				m_Texture = new Texture( m_Device, 1, 1, 1, Usage.None, Format.A8R8G8B8, Pool.Managed );

				// Set the pixel color to White
				GraphicsStream stream = m_Texture.LockRectangle( 0, LockFlags.Discard );
				
				byte[] color = new byte[4];
				for( int i = 0; i < 4; i++ )
					color[ i ] = 0xFF;
				stream.Write( color, 0, 4 );
				
				m_Texture.UnlockRectangle( 0 );
			}
			catch( Exception ex )
			{
				DialogResult r = MessageBox.Show( "Failed to Create the Texture Object\n\n" + ex.Message, "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation );

				return false;
			}


			// Set up the Font object
			try
			{
				m_FontDescription.FaceName = "Arial";
				m_FontDescription.Quality = FontQuality.Default;
				m_FontDescription.Weight = FontWeight.SemiBold;

				m_Font = new Microsoft.DirectX.Direct3D.Font( m_Device, m_FontDescription );
			}
			catch( Exception ex )
			{
				DialogResult r = MessageBox.Show( "Failed to Create the font Object\n\n" + ex.Message, "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation );

				return false;
			}

			return true;
		}


		/// <summary>
		/// Shuts down Direct3D, releasing all the memory used by the render targets
		/// </summary>
		/// <returns>true if successful, false otherwise.</returns>
		public bool Terminate( )
		{
			for( int i = 0; i < m_RenderTargets.Count; i++ )
			{
				m_RenderTargets[ i ].Release( );
			}
			return true;
		}



		/// <summary>
		/// Adds the window to the render target list, and sets it as the Active target.
		/// </summary>
		/// <param name="renderWindow">Window to render to.</param>
		/// <returns>true if successful, false otherwise.</returns>
		public bool AddRenderTarget( Control renderWindow )
		{
			// Add the target with the window's size
			return AddRenderTarget( renderWindow, renderWindow.Width, renderWindow.Height );
		}

		/// <summary>
		/// Adds the window to the render target list, and sets it as the Active target.
		/// </summary>
		/// <param name="renderWindow">Window to render to.</param>
		/// <param name="width">Width of the window in pixels.</param>
		/// <param name="height">Height of the window in pixels.</param>
		/// <returns>true if successful, false otherwise.</returns>
		public bool AddRenderTarget( Control renderWindow, int width, int height )
		{
			// Verify the device has been initialized
			if( m_Device == null )
				return false;


			// Set up the render target
			try
			{
				// Copy the present parameters from the initial window
				PresentParameters presentParams = new PresentParameters( m_PresentParams );

				// Set the backbuffer to this window
				presentParams.BackBufferWidth = width;
				presentParams.BackBufferHeight = height;
				presentParams.DeviceWindow = renderWindow;

				// Add the window to the render target list
				m_RenderTargets.Add( new RenderTarget( m_Device, presentParams ) );
			}
			catch( Exception ex )
			{
				DialogResult r = MessageBox.Show( "Failed to Create Render Target\n\n" + ex.Message, "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation );

				return false;
			}


			// success.
			return true;
		}

		/// <summary>
		/// Finds the render window in the list and sets it as the Active render target
		/// </summary>
		/// <param name="renderWindow">Window to render to.</param>
		/// <returns>true if successful, false otherwise.</returns>
		private bool SetActiveRenderTarget( Control renderWindow )
		{
			// Find the swap chain
			m_ActiveTarget = -1;
			for( int i = 0; i < m_RenderTargets.Count; i++ )
			{
				if( m_RenderTargets[ i ].Chain.Disposed )
				{
					m_RenderTargets[ i ].Reset( m_Device );
					//return true;
				}
				if( m_RenderTargets[ i ].Chain.PresentParameters.DeviceWindow == renderWindow )
				{
					m_ActiveTarget = i;
					break;
				}
			}
			if( m_ActiveTarget < 0 )
			{
				DialogResult r = MessageBox.Show( "Not a valid render window", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation );
				return false;
			}
			return true;
		}



		/// <summary>
		/// Clears the Active render target to a specific color.
		/// </summary>
		/// <param name="color">What color to clear the background to.</param>
		/// <returns>true if successful, false otherwise.</returns>
		public bool Clear( Color color )
		{
			return Clear( m_RenderTargets[ m_ActiveTarget ].RenderWindow, color );
		}

		/// <summary>
		/// Clears the render window to a specific color, and sets it to the Active render target.
		/// </summary>
		/// <param name="renderWindow">The window we want to clear.</param>
		/// <param name="color">What color to clear the background to.</param>
		/// <returns>true if successful, false otherwise.</returns>
		public bool Clear( Control renderWindow, Color color )
		{
			if( m_Device == null )
				return false;

			// Check for alt+tab
			try
			{
				m_Device.TestCooperativeLevel( );
			}
			catch( DeviceLostException )
			{
			}
			catch( DeviceNotResetException )
			{
				Reset( );
			}

			// Find the swap chain
			SetActiveRenderTarget( renderWindow );

			Surface pBackBuffer = m_RenderTargets[ m_ActiveTarget ].Chain.GetBackBuffer( 0, BackBufferType.Mono );

			// set it as our current back buffer
			m_Device.SetRenderTarget( 0, pBackBuffer );

			//Clear the backbuffer to a color 
			m_Device.Clear( ClearFlags.Target, color, 1.0f, 0 );


			pBackBuffer.Dispose( );

			return true;
		}



		/// <summary>
		/// Begins the Device rendering.
		/// </summary>
		/// <returns>true if successful, false otherwise.</returns>
		public bool DeviceBegin( )
		{
			if( m_Device == null )
				return false;

			try
			{
				m_Device.BeginScene( );
			}
			catch( InvalidCallException )
			{
				DialogResult r = MessageBox.Show( "Failed to begin device scene.", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation );
				return false;
			}

			return true;
		}

		/// <summary>
		/// Begins the Sprite rendering (MUST be called between DeviceBegin and DeviceEnd!).
		/// </summary>
		/// <returns>true if successful, false otherwise.</returns>
		public bool SpriteBegin( )
		{
			if( m_Sprite == null )
				return false;

			try
			{
				m_Sprite.Begin( SpriteFlags.AlphaBlend );
			}
			catch( Exception )
			{
				DialogResult r = MessageBox.Show( "Failed to begin sprite scene.", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation );
				return false;
			}

			return true;
		}

		/// <summary>
		/// Ends the Device rendering.
		/// </summary>
		/// <returns>true if successful, false otherwise.</returns>
		public bool DeviceEnd( )
		{
			if( m_Device == null )
				return false;

			try
			{
				m_Device.EndScene( );
			}
			catch( Exception )
			{
				DialogResult r = MessageBox.Show( "Failed to end device scene.", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation );
				return false;
			}

			return true;
		}

		/// <summary>
		/// Ends the Sprite rendering (MUST be called between DeviceBegin and DeviceEnd!).
		/// </summary>
		/// <returns>true if successful, false otherwise.</returns>
		public bool SpriteEnd( )
		{
			if( m_Sprite == null )
				return false;

			try
			{
				m_Sprite.End( );
			}
			catch( Exception )
			{
				DialogResult r = MessageBox.Show( "Failed to end sprite scene.", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation );
				return false;
			}

			return true;
		}



		/// <summary>
		/// Presents the Active render target's backbuffer.
		/// </summary>
		/// <returns>true if successful, false otherwise.</returns>
		public bool Present( )
		{
			if( m_Device == null )
				return false;

			try
			{
				m_RenderTargets[ m_ActiveTarget ].Chain.Present( );
			}
			catch( Exception )
			{
				DialogResult r;
				r = MessageBox.Show( "Failed to present.", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation );
				return false;
			}

			return true;
		}



		/// <summary>
		/// Resets the display parameters of the Managed Direct3D Wrapper for the render window
		/// </summary>
		/// <param name="renderWindow">Window to render to.</param>
		/// <param name="vsync">Should the renderer wait for a vsync before drawing?</param>
		/// <returns>true if successful, false otherwise.</returns>
		public bool Resize( Control renderWindow, bool vsync )
		{
			return Resize( renderWindow, renderWindow.Width, renderWindow.Height, vsync );
		}

		/// <summary>
		/// Changes the display parameters of the Managed Direct3D Wrapper for the render window
		/// </summary>
		/// <param name="renderWindow">Window to render to.</param>
		/// <param name="screenWidth">Width of the screen in pixels.</param>
		/// <param name="screenHeight">Height of the screen in pixels.</param>
		/// <param name="vsync">Should the renderer wait for a vsync before drawing?</param>
		/// <returns>true if successful, false otherwise.</returns>
		public bool Resize( Control renderWindow, int screenWidth, int screenHeight, bool vsync )
		{
			try
			{
				// Now  setup our D3D present parameters
				m_PresentParams = new PresentParameters( );
				m_PresentParams.BackBufferWidth = screenWidth;
				m_PresentParams.BackBufferHeight = screenHeight;
				// taking out option for fullscreen mode
				//presentParams.BackBufferFormat = (isWindowed) ? Format.Unknown : Format.R5G6B5;
				m_PresentParams.BackBufferFormat = Format.Unknown;
				m_PresentParams.BackBufferCount = 1;
				m_PresentParams.MultiSample = MultiSampleType.None;
				m_PresentParams.MultiSampleQuality = 0;
				m_PresentParams.SwapEffect = SwapEffect.Copy;// Discard;
				m_PresentParams.DeviceWindow = renderWindow;
				// taking out option for fullscreen mode
				//presentParams.Windowed = isWindowed;
				m_PresentParams.Windowed = true;
				m_PresentParams.EnableAutoDepthStencil = false;
				m_PresentParams.FullScreenRefreshRateInHz = 0;
				m_PresentParams.PresentationInterval = (vsync) ? PresentInterval.Default : PresentInterval.Immediate;

				// Find the swap chain
				SetActiveRenderTarget( renderWindow );

				m_Font.OnLostDevice( );
				m_Sprite.OnLostDevice( );
				m_Device.Reset( m_PresentParams );
				m_Sprite.OnResetDevice( );
				m_Font.OnResetDevice( );

				m_RenderTargets[ m_ActiveTarget ].Reset( m_Device, m_PresentParams );
			}
			catch( Exception )
			{
				DialogResult r = MessageBox.Show( "Failed to change the render window", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation );

				return false;
			}

			return true;
		}

		/// <summary>
		/// Handles resetting the device (Private utility function).
		/// </summary>
		private void Reset( )
		{
			m_Font.OnLostDevice( );
			m_Sprite.OnLostDevice( );

			m_Device.Reset( m_PresentParams );

			m_Font.OnResetDevice( );
			m_Sprite.OnResetDevice( );

			for( int i = 0; i < m_RenderTargets.Count; i++ )
			{
				m_RenderTargets[ i ].Reset( m_Device );
			}
		}



		/// <summary>
		/// Draws a colored rectangle to the Active render target
		/// </summary>
		/// <param name="rect">The rectangle holding the area to fill.</param>
		/// <param name="color">The color of the rect we are going to draw.</param>
		/// <returns>true if successful, false otherwise.</returns>
		public bool DrawRect( Rectangle rect, Color color )
		{
			// Verify the device has been initialized
			if( m_Device == null )
				return false;


			// OLD WAY: clear out a target region of the backbuffer
			//System.Drawing.Rectangle[ ] regions = new System.Drawing.Rectangle[ 1 ];
			//regions[ 0 ] = rect;

			//m_Device.Clear( ClearFlags.Target, color, 1.0f, 0, regions );


			// NEW WAY: use a 1x1 texture

			// Create the transform matrix with scaling & translation applied
			Matrix transform = Matrix.Identity;

			transform.M11 = (float)(rect.Width);
			transform.M22 = (float)(rect.Height);
			transform.M41 = (float)(rect.Left);
			transform.M42 = (float)(rect.Top);

			// Apply the transform
			m_Sprite.Transform = transform;

			// Draw the texture
			m_Sprite.Draw( m_Texture, System.Drawing.Rectangle.Empty, Vector3.Empty, Vector3.Empty, color );

			
			// Move the world back to identity
			m_Sprite.Transform = Matrix.Identity;

			// success.
			return true;
		}

		/// <summary>
		/// Draws the border around a rectangle of a given color to the screen.
		/// </summary>
		/// <param name="rect">The rectangle holding the area to draw.</param>
		/// <param name="color">The color of the rect we are going to draw.</param>
		/// <param name="width">The width of the rect's border.</param>
		/// <returns>true if successful, false otherwise.</returns>
		public bool DrawHollowRect( Rectangle rect, Color color, int width )
		{
			// OLD WAY: draw four lines
			//if( m_Line == null )
			//	return false;

			//Vector2[ ] verts = new Vector2[ 2 ];

			//LineBegin( );
			////Top
			//verts[ 0 ].X = rect.Left;
			//verts[ 0 ].Y = rect.Top;

			//verts[ 1 ].X = rect.Right;
			//verts[ 1 ].Y = rect.Top;

			//m_Line.Draw( verts, color );

			////Bottom
			//verts[ 0 ].X = rect.Left;
			//verts[ 0 ].Y = rect.Bottom;

			//verts[ 1 ].X = rect.Right;
			//verts[ 1 ].Y = rect.Bottom;

			//m_Line.Draw( verts, color );

			////Left
			//verts[ 0 ].X = rect.Left;
			//verts[ 0 ].Y = rect.Top;

			//verts[ 1 ].X = rect.Left;
			//verts[ 1 ].Y = rect.Bottom;

			//m_Line.Draw( verts, color );

			////Right
			//verts[ 0 ].X = rect.Right;
			//verts[ 0 ].Y = rect.Top;

			//verts[ 1 ].X = rect.Right;
			//verts[ 1 ].Y = rect.Bottom;

			//m_Line.Draw( verts, color );
			//LineEnd( );



			// NEW WAY: use a 1x1 texture to draw 4 lines

			// Verify the device has been initialized
			if( m_Device == null )
				return false;


			// Enlarge the rect 1 pixel to outline the bottom & right edges
			rect.Width += 1;
			rect.Height += 1;

			// Store the line width
			float lineWidth = (float)width;
			float halfWidth = lineWidth * 0.5f;


			// Create the transform matrix with scaling & translation applied
			Matrix transform = Matrix.Identity;

			// Top line
			transform.M11 = (float)(rect.Width);
			transform.M22 = (float)(lineWidth);
			transform.M41 = (float)(rect.Left - halfWidth);
			transform.M42 = (float)(rect.Top - halfWidth);

			// Apply the transform
			m_Sprite.Transform = transform;

			// Draw the texture
			m_Sprite.Draw( m_Texture, System.Drawing.Rectangle.Empty, Vector3.Empty, Vector3.Empty, color );


			// Left line
			transform.M11 = (float)(lineWidth);
			transform.M22 = (float)(rect.Height);
			transform.M41 = (float)(rect.Left - halfWidth);
			transform.M42 = (float)(rect.Top + halfWidth);

			// Apply the transform
			m_Sprite.Transform = transform;

			// Draw the texture
			m_Sprite.Draw( m_Texture, System.Drawing.Rectangle.Empty, Vector3.Empty, Vector3.Empty, color );


			// Bottom line
			transform.M11 = (float)(rect.Width);
			transform.M22 = (float)(lineWidth);
			transform.M41 = (float)(rect.Left + halfWidth);
			transform.M42 = (float)(rect.Bottom - halfWidth);

			// Apply the transform
			m_Sprite.Transform = transform;

			// Draw the texture
			m_Sprite.Draw( m_Texture, System.Drawing.Rectangle.Empty, Vector3.Empty, Vector3.Empty, color );


			// Right line
			transform.M11 = (float)(lineWidth);
			transform.M22 = (float)(rect.Height);
			transform.M41 = (float)(rect.Right - halfWidth);
			transform.M42 = (float)(rect.Top - halfWidth);

			// Apply the transform
			m_Sprite.Transform = transform;

			// Draw the texture
			m_Sprite.Draw( m_Texture, System.Drawing.Rectangle.Empty, Vector3.Empty, Vector3.Empty, color );



			// Move the world back to identity
			m_Sprite.Transform = Matrix.Identity;
			return true;
		}


		/// <summary>
		/// Draws a colored line to the screen.
		/// Does not support alpha
		/// </summary>
		/// <param name="x1">Starting X position.</param>
		/// <param name="y1">Starting Y position.</param>
		/// <param name="x2">Ending X position.</param>
		/// <param name="y2">Ending Y position.</param>
		/// <param name="color">The color of the rect we are going to draw.</param>
		/// <param name="width">The width of the line.</param>
		/// <returns>true if successful, false otherwise.</returns>
		public bool DrawLine( int x1, int y1, int x2, int y2, Color color, int width )
		{
			// OLD WAY: draw line

			//if( m_Line == null )
			//	return false;

			//Vector2[ ] verts = new Vector2[ 2 ];

			////Top
			//verts[ 0 ].X = x1;
			//verts[ 0 ].Y = y1;

			//verts[ 1 ].X = x2;
			//verts[ 1 ].Y = y2;

			//m_Line.Draw( verts, color );


			// NEW WAY: use a 1x1 texture to draw the line

			// Verify the device has been initialized
			if( m_Device == null )
				return false;


			// Calculate difference in positions
			int deltaX = x2 - x1;
			int deltaY = y2 - y1;

			// Line must have a difference in at least one direction
			if( deltaX == 0 && deltaY == 0 || width == 0 )
				return false;


			// Calculate the length and direction
			float length = (float)Math.Sqrt( (deltaX * deltaX) + (deltaY * deltaY) );
			float angle = (float)Math.Acos( deltaX / length );	// abbreviated DotProduct against <1, 0>


			// Store the line width
			float lineWidth = (float)width;
			float halfWidth = width * 0.5f;


			// Adjust the angle
			if( float.IsNaN( angle ) )
				angle = 0.0f;
			else if( deltaY < 0 )
				angle = -angle;


			// Store sin & cos
			float sin = (float)Math.Sin( angle );
			float cos = (float)Math.Cos( angle );


			// Create the transform matrix with scaling & translation applied
			Matrix transform = Matrix.Identity;

			transform.M11 = (float)(length * cos);	// scale & rotate
			transform.M12 = (float)(length * sin);
			transform.M21 = (float)(width * -sin);
			transform.M22 = (float)(width * cos);
			transform.M41 = (float)(x1 + halfWidth * sin);
			transform.M42 = (float)(y1 + halfWidth * -cos);

			// Apply the transform
			m_Sprite.Transform = transform;

			// Draw the texture
			m_Sprite.Draw( m_Texture, System.Drawing.Rectangle.Empty, Vector3.Empty, Vector3.Empty, color );


			// Move the world back to identity
			m_Sprite.Transform = Matrix.Identity;

			// success.
			return true;
		}

		/// <summary>
		/// Draws colored text to the screen.
		/// </summary>
		/// <param name="text">Text to print to the screen.</param>
		/// <param name="x">X position of text.</param>
		/// <param name="y">Y position of text.</param>
		/// <param name="color">The color of the rect we are going to draw.</param>
		/// <returns>true if successful, false otherwise.</returns>
		public bool DrawText( string text, int x, int y, Color color )
		{
			if( m_Font == null )
				return false;

			m_Font.DrawText( m_Sprite, text, new System.Drawing.Point( x, y ), color );

			return true;
		}

	} // end ManagedDirect3D


	public class RenderTarget
	{
		PresentParameters presentParams = null;
		Control renderWindow = null;
		SwapChain chain = null;

		public Control RenderWindow
		{
			get
			{
				return renderWindow;
			}
			set
			{
				renderWindow = value;
			}
		}

		public SwapChain Chain
		{
			get
			{
				return chain;
			}
		}

		public RenderTarget( Device device, PresentParameters param )
		{
			presentParams = param;
			renderWindow = presentParams.DeviceWindow;
			chain = new SwapChain( device, presentParams );
		}
		public void Reset( Device device )
		{
			chain.Dispose( );
			chain = new SwapChain( device, presentParams );
		}
		public void Reset( Device device, PresentParameters param )
		{
			presentParams = param;
			renderWindow = presentParams.DeviceWindow;
			chain.Dispose( );
			chain = new SwapChain( device, param );
		}
		public void Release( )
		{
			chain.Dispose( );
			chain = null;
			presentParams = null;
		}
	}
}
