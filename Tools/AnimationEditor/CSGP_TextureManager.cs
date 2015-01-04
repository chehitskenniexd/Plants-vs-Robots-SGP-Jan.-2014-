////////////////////////////////////////////////////////////////////////////////////////
//  File			:	"ManagedTextureManager.cs"
//
//  Author			:	David Brown
//                      Ron Powell
//                      Rob Martinez
//                      John OLeske
//						Doug Monroe
//
//  Purpose			:	Wrapper class for Managed DirectX Textures.
//
//  NOTE:   In order to use these wrapper classes you will need
//          to add References to the following assemblies to your Solution:
//
//          -Microsoft.DirectX              version: 1.0.2902.0 runtime: v1.1.4322
//          -Microsoft.DirectX.Direct3D     version: 1.0.2902.0 runtime: v1.1.4322
//          -Microsoft.DirectX.Direct3DX    version: 1.0.2902.0 runtime: v1.1.4322
////////////////////////////////////////////////////////////////////////////////////////
using System;
using System.Drawing;				// for Rectangle & Color
using System.Windows.Forms;         // for MessageBox
using System.Collections.Generic;	// for List

using System.Diagnostics;           // for Debug.Assert

using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

namespace SGP
{
	public sealed class CSGP_TextureManager
	{
		// Texture info structure
		struct TextureInfo
		{
			public string fileName;		// full file path & name
			public int refCount;		// reference count
			public Texture texture;		// Direct3D texture
			public int width;			// texture width
			public int height;			// texture height
		}

		// List of texture info
		List<TextureInfo> m_Textures = null;

		// Direct3D Device
		Device m_Device = null;

		// Sprite Library for Textures
		Sprite m_Sprite = null;


		// SINGLETON! (Thread-safe instance)
		static readonly CSGP_TextureManager instance = new CSGP_TextureManager( );

		// Explicit static constructor to tell C# compiler
		// not to mark type as beforefieldinit
		static CSGP_TextureManager( )
		{
		}

		CSGP_TextureManager( )
		{
		}

		/// <summary>
		/// Gets the instance of the singleton.
		/// </summary>
		public static CSGP_TextureManager GetInstance( )
		{
			return instance;
		}

		/// <summary>
		/// Initializes the Texture Manager.
		/// </summary>
		/// <param name="device">Direct3D device.</param>
		/// <param name="sprite">Sprite device.</param>
		/// <returns>true if successful, false otherwise.</returns>
		public bool Initialize( Device device, Sprite sprite )
		{
			// Verify the parameters
			if( device == null )
			{
				MessageBox.Show( "Invalid Device", "ManagedTextureManager::Error", MessageBoxButtons.OK, MessageBoxIcon.Error );
				return false;
			}
			if( sprite == null )
			{
				MessageBox.Show( "Invalid Sprite", "ManagedTextureManager::Error", MessageBoxButtons.OK, MessageBoxIcon.Error );
				return false;
			}

			// Store the parameters
			m_Device = device;
			m_Sprite = sprite;
			m_Textures = new List<TextureInfo>( );

			// success.
			return true;
		}

		/// <summary>
		/// Shuts down the Texture Manager, releasing all the memory used by the textures.
		/// </summary>
		public void Terminate( )
		{
			// Dispose of the textures
			for( int i = 0; i < m_Textures.Count; i++ )
			{
				if( m_Textures[ i ].texture != null )
					m_Textures[ i ].texture.Dispose( );

			}

			// Clear the list of all loaded textures.
			m_Textures.Clear( );

			// Erase the data members
			m_Device = null;
			m_Sprite = null;
			m_Textures = null;
		}
		


		/// <summary>
		/// Loads & stores a texture.
		/// </summary>
		/// <param name="fileName">File name of the texture to load (relative to the Current Working Directory).</param>
		/// <returns>The id for the loaded texture, -1 on failure.</returns>
		public int LoadTexture( string fileName )
		{
			return LoadTexture( fileName, Color.FromArgb( 0 ) );
		}

		/// <summary>
		/// Loads & stores a texture using a color key for transparent pixels.
		/// </summary>
		/// <param name="fileName">File name of the texture to load (relative to the Current Working Directory).</param>
		/// <param name="colorkey">Transparent color (use Color.FromArgb() function )</param>
		/// <returns>The id for the loaded texture, -1 on failure.</returns>
		public int LoadTexture( string fileName, Color colorkey )
		{
			// Verify the filename
			if( string.IsNullOrWhiteSpace( fileName ) )
				return -1;


			// Check if this file has already been loaded (with any color key)
			for( int i = 0; i < m_Textures.Count; i++ )
			{
				TextureInfo info = m_Textures[ i ];

				// Compare filenames, ignoring case
				if( info.fileName.Equals( fileName, StringComparison.OrdinalIgnoreCase ) )
				{
					info.refCount++;		// increase the reference count
					m_Textures[ i ] = info;	// store the new value back into the list
					return i;				// return the index of the existing texture
				}
			}
			


			// Create the new TextureInfo to use
			TextureInfo tex = new TextureInfo();

			// Copy the filename of the loaded texture.
			tex.fileName = fileName;


			// Attempt to load the texture from file
			try
			{
				tex.texture = TextureLoader.FromFile( m_Device, fileName, 0, 0, 1, Usage.None, Format.Unknown, Pool.Managed, Filter.None, Filter.None, colorkey.ToArgb( ) );
			}
			catch( Exception )
			{
				MessageBox.Show( "Failed to load texture: " + fileName, "ManagedTextureManager::Error", MessageBoxButtons.OK, MessageBoxIcon.Error );
				return -1;
			}


			// AddRef.
			tex.refCount = 1;

			// Get Width & Height of the texture.
			SurfaceDescription d3dSurfDesc = tex.texture.GetSurfaceLevel( 0 ).Description;
			tex.width = d3dSurfDesc.Width;
			tex.height = d3dSurfDesc.Height;
			


			// Look for an open spot
			int id = -1;
			for( int i = 0; i < m_Textures.Count; i++ )
			{
				if( m_Textures[ i ].refCount == 0 )
				{
					id = i;
					break;
				}
			}

			// if we didn't find an open spot, load it into a new one
			if( id == -1 )
			{
				m_Textures.Add( tex );
				id = m_Textures.Count - 1;
			}
			else
			{
				// Make sure the old texture has been disposed
				if( m_Textures[ id ].texture != null )
					m_Textures[ id ].texture.Dispose( );

				// Overwrite with the new texture
				m_Textures[ id ] = tex;
			}


			// Check if the surface size matches the original image's size
			ImageInformation img = TextureLoader.ImageInformationFromFile( fileName );
			if( tex.width != img.Width || tex.height != img.Height )
			{
				MessageBox.Show( "Image (" + fileName + ") is stretched from " 
									+ img.Width.ToString() + "x"+ img.Height.ToString() + " to "
									+ tex.width.ToString() + "x" + tex.height.ToString(),
									"ManagedTextureManager::Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning );

			}

			// Return the id of the texture.
			return id;
		}



		/// <summary>
		/// Releases the reference to the texture. When the reference is 0,
		/// the memory for the Direct3D texture will be disposed.
		/// </summary>
		/// <param name="id">Id of the texture to release.</param>
		public void UnloadTexture( int id )
		{
			// Make sure the id is in range.
			Debug.Assert( (id > -1 && id < m_Textures.Count), "id is out of range" );

			// Release a reference.
			TextureInfo t = m_Textures[ id ];
			t.refCount--;

			// Release the texture if it is not being used
			if( t.refCount <= 0 )
			{
				// Do a lazy delete and leave this spot empty
				if( t.texture != null )
					t.texture.Dispose( );
				t.texture = null;

				t.fileName = "";
				t.refCount = 0;
				t.width = -1;
				t.height = -1;
			}

			// Put the changed info back into the list.
			m_Textures[ id ] = t;
		}



		/// <summary>
		/// Gets the width of a specific texture.
		/// </summary>
		/// <param name="id">Id of the texture to get the width of.</param>
		/// <returns>The width of the specific texture.</returns>
		public int GetTextureWidth( int id )
		{
			// Make sure the id is in range.
			Debug.Assert( (id > -1 && id < m_Textures.Count), "id is out of range" );

			return m_Textures[ id ].width;
		}

		/// <summary>
		/// Gets the height of a specific texture.
		/// </summary>
		/// <param name="id">Id of the texture to get the height of.</param>
		/// <returns>The height of the specific texture.</returns>
		public int GetTextureHeight( int id )
		{
			// Make sure the id is in range.
			Debug.Assert( (id > -1 && id < m_Textures.Count), "id is out of range" );

			return m_Textures[ id ].height;
		}



		/// <summary>
		/// Draws a full texture to the screen (with default scale, source rect, rotation, and color).
		/// </summary>
		/// <param name="id">Id of the texture to draw.</param>
		/// <param name="x">X position to draw at.</param>
		/// <param name="y">Y position to draw at.</param>
		/// <returns>true if successful, false otherwise.</returns>
		public bool Draw( int id, int x, int y )
		{
			return Draw( id, x, y, 1.0f, 1.0f, Rectangle.Empty, 0.0f, 0.0f, 0.0f, Color.White );
		}

		/// <summary>
		/// Draws a full texture to the screen (with default source rect, rotation, and color.
		/// </summary>
		/// <param name="id">Id of the texture to draw.</param>
		/// <param name="x">X position to draw at.</param>
		/// <param name="y">Y position to draw at.</param>
		/// <param name="scaleX">X scale.</param>
		/// <param name="scaleY">Y scale.</param>
		/// <returns>true if successful, false otherwise.</returns>
		public bool Draw( int id, int x, int y, float scaleX, float scaleY )
		{
			return Draw( id, x, y, scaleX, scaleY, Rectangle.Empty, 0.0f, 0.0f, 0.0f, Color.White );
		}

		/// <summary>
		/// Draws a texture to the screen (with default rotation and color).
		/// </summary>
		/// <param name="id">Id of the texture to draw.</param>
		/// <param name="x">X position to draw at.</param>
		/// <param name="y">Y position to draw at.</param>
		/// <param name="scaleX">X scale.</param>
		/// <param name="scaleY">Y scale.</param>
		/// <param name="section">Section of the image to draw.</param>
		/// <returns>true if successful, false otherwise.</returns>
		public bool Draw( int id, int x, int y, float scaleX, float scaleY, Rectangle section )
		{
			return Draw( id, x, y, scaleX, scaleY, section, 0.0f, 0.0f, 0.0f, Color.White );
		}

		/// <summary>
		/// Draws a texture to the screen (with default color).
		/// </summary>
		/// <param name="id">Id of the texture to draw.</param>
		/// <param name="x">X position to draw at.</param>
		/// <param name="y">Y position to draw at.</param>
		/// <param name="scaleX">X scale.</param>
		/// <param name="scaleY">Y scale.</param>
		/// <param name="section">Section of the image to draw (Use Rectangle.Empty for the whole surface).</param>
		/// <param name="rotCenterX">X position on the texture to rotate around.</param>
		/// <param name="rotCenterY">Y position on the texture to rotate around.</param>
		/// <param name="rotation">Amount to rotate by (in radians).</param>
		/// <returns>true if successful, false otherwise.</returns>
		public bool Draw( int id, int x, int y,
						 float scaleX, float scaleY, Rectangle section,
						 float rotCenterX, float rotCenterY, float rotation )
		{
			return Draw( id, x, y, scaleX, scaleY, section, rotCenterX, rotCenterY, rotation, Color.White );
		}

		/// <summary>
		/// Draws a texture to the screen.
		/// </summary>
		/// <param name="id">Id of the texture to draw.</param>
		/// <param name="x">X position to draw at.</param>
		/// <param name="y">Y position to draw at.</param>
		/// <param name="scaleX">X scale.</param>
		/// <param name="scaleY">Y scale.</param>
		/// <param name="section">Section of the image to draw (Use Rectangle.Empty for the whole surface).</param>
		/// <param name="rotCenterX">X position on the texture to rotate around.</param>
		/// <param name="rotCenterY">Y position on the texture to rotate around.</param>
		/// <param name="rotation">Amount to rotate by (in radians).</param>
		/// <param name="color">System Color to modulate the image with</param>
		/// <returns>true if successful, false otherwise.</returns>
		public bool Draw( int id, int x, int y,
						 float scaleX, float scaleY, Rectangle section,
						 float rotCenterX, float rotCenterY, float rotation, Color color )
		{
			// Make sure the id is in range.
			Debug.Assert( (id > -1 && id < m_Textures.Count), "id is out of range" );

			// Make sure the texture is valid
			Debug.Assert( m_Textures[ id ].texture != null, "Attempting to draw released texture id" );

			// Make sure the sprite was created.
			if( m_Sprite == null )
				return false;

			// Initialize to Identity.
			Matrix result = Matrix.Identity;

			// Scale the sprite.
			Matrix scale = Matrix.Scaling( scaleX, scaleY, 1.0f );
			result *= scale;

			// Rotate the sprite.
			Matrix translate = Matrix.Translation( -rotCenterX, -rotCenterY, 0.0f );
			result *= translate;

			Matrix rotate = Matrix.RotationZ( rotation );
			result *= rotate;

			translate = Matrix.Translation( rotCenterX, rotCenterY, 0.0f );
			result *= translate;

			// Translate the sprite.
			translate = Matrix.Translation( x, y, 0.0f );
			result *= translate;

			// Apply the transform.
			m_Sprite.Transform = result;

			// Draw the sprite.
			m_Sprite.Draw( m_Textures[ id ].texture, section, Vector3.Empty, Vector3.Empty, color );


			// Move the world back to identity.
			m_Sprite.Transform = Matrix.Identity;

			// success.
			return true;
		}
	} // end ManagedTextureManager
}
