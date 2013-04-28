#include <Game.hpp>
#include <Time.hpp>
#include <LinuxRendererOGL3.hpp>
#include <LinuxInputManager.hpp>
#include <LinuxWindow.hpp>
#include <unistd.h>

namespace LD26
{
	Game::Game( )
	{
		m_pRenderer = ZED_NULL;
		m_pConfigFile = ZED_NULL;
		m_FullScreen = ZED_FALSE;
		m_pInputManager = ZED_NULL;
		m_pWindow = ZED_NULL;
	}

	Game::~Game( )
	{
		if( m_pInputManager )
		{
			delete m_pInputManager;
			m_pInputManager = ZED_NULL;
		}

		if( m_pRenderer )
		{
			delete m_pRenderer;
			m_pRenderer = ZED_NULL;
		}

		if( m_pWindow )
		{
			delete m_pWindow;
			m_pWindow = ZED_NULL;
		}
	}

	ZED_UINT32 Game::Initialise( const ZED_BOOL p_FullScreen )
	{
		m_FullScreen = p_FullScreen;

		ZED_UINT32 X = 0, Y = 0, Width = 640, Height = 480;
		ZED::Renderer::ZED_SCREENSIZE NativeSize;
		ZED::Renderer::ZED_SCREENSIZE *pScreenSizes = ZED_NULL;
		ZED_MEMSIZE ScreenSizeCount = 0;
		// TODO
		// Attempt to create an OpenGL 3 renderer, then fall back to 2,
		// finally try to get an OpenGL 1 renderer
		m_pRenderer = new ZED::Renderer::LinuxRendererOGL3( );
		m_pWindow = new ZED::Renderer::LinuxWindow( );

		ZED::Renderer::EnumerateScreenSizes( &pScreenSizes, &ScreenSizeCount,
			ZED::Renderer::GetCurrentScreenNumber( ) );

		if( ZED::Renderer::GetNativeScreenSize(
			ZED::Renderer::GetCurrentScreenNumber( ), NativeSize ) != ZED_OK )
		{
			zedTrace( "[LD26::Game::Initialise] <ERROR> "
				"Could not get native screen size\n" );

			return ZED_FAIL;
		}

		if( p_FullScreen )
		{
			X = 0;
			Y = 0;
			Width = NativeSize.Width;
			Height = NativeSize.Height;
		}
		else
		{
			ZED_MEMSIZE ScreenNum;
			ZED_UINT32 LowestWidth = pScreenSizes[ 0 ].Width;
			for( ZED_MEMSIZE i = 1; i < ScreenSizeCount; ++i )
			{
				if( pScreenSizes[ i ].Width < LowestWidth )
				{
					ScreenNum = i;
					LowestWidth = pScreenSizes[ i ].Width;
				}
			}

			if( NativeSize.Width < NativeSize.Height )
			{
				Width = pScreenSizes[ ScreenNum ].Height;
				Height = pScreenSizes[ ScreenNum ].Width;
			}
			else
			{
				Width = pScreenSizes[ ScreenNum ].Width;
				Height = pScreenSizes[ ScreenNum ].Height;
			}

			X = ( NativeSize.Width / 2 ) - ( Width / 2 );
			Y = ( NativeSize.Height / 2 ) - ( Height / 2 );
		}

//		Width = 800;
//		Height = 600;

		m_pWindow->Create( X, Y, Width, Height );

		m_Canvas.Width( Width );
		m_Canvas.Height( Height );

		// TODO
		// Make sure the following Canvas items are valid
		m_Canvas.BackBufferCount( 1 );
		m_Canvas.DepthStencilFormat( ZED_FORMAT_D24S8 );
		m_Canvas.ColourFormat( ZED_FORMAT_ARGB8 );

		m_pRenderer->Create( m_Canvas, ( *m_pWindow ) );

		m_pRenderer->ClearColour( 0.15f, 0.0f, 0.15f );

		m_pRenderer->SetRenderState( ZED_RENDERSTATE_CULLMODE,
			ZED_CULLMODE_CCW );
		m_pRenderer->SetRenderState( ZED_RENDERSTATE_DEPTH, ZED_ENABLE );
		m_pRenderer->SetClippingPlanes( 1.0f, 100000.0f );
		m_pRenderer->PerspectiveProjectionMatrix( 45.0f,
			static_cast< ZED_FLOAT32 >( Width )/
			static_cast< ZED_FLOAT32 >( Height ) );

		ZED::Renderer::ZED_WINDOWDATA WinData = m_pWindow->WindowData( );
		m_pInputManager =
			new ZED::System::LinuxInputManager( WinData.pX11Display );
		m_pInputManager->AddDevice( &m_Keyboard );

		if( pScreenSizes )
		{
			delete [ ] pScreenSizes;
			pScreenSizes = ZED_NULL;
		}

		LD26::GameEntity::m_pRenderer =  m_pRenderer;
		m_pHero = new LD26::PlayerEntity( m_pInputManager );

		m_EntityManager.Add( &m_Level );
		m_EntityManager.Add( m_pHero );

		return ZED_OK;
	}


	ZED_UINT32 Game::Execute( )
	{
		XEvent m_Events;
		KeySym Key;
		m_Running = ZED_TRUE;
		ZED::Renderer::ZED_WINDOWDATA WinData = m_pWindow->WindowData( );
		m_pRenderer->ForceClear( ZED_TRUE, ZED_TRUE, ZED_TRUE );

		ZED_UINT64 ElapsedTime = 0ULL;
		ZED_UINT64 TimeStep = 16667ULL;
		ZED_UINT64 OldTime = ZED::System::GetTimeMiS( );
		ZED_UINT64 FrameTime = ZED::System::GetTimeMiS( );
		ZED_MEMSIZE FrameRate = 0;
		ZED_UINT64 Accumulator = 0ULL;

		while( m_Running == ZED_TRUE )
		{
			m_pInputManager->Update( );

			if( m_Keyboard.IsKeyDown( K_ESCAPE ) )
			{
				m_Running = ZED_FALSE;
			}
			while( XPending( WinData.pX11Display ) > 0 )
			{
				XNextEvent( WinData.pX11Display, &m_Events );
				switch( m_Events.type )
				{
					case EnterNotify:
					{
						XGrabKeyboard( WinData.pX11Display, WinData.X11Window,
							True, GrabModeAsync, GrabModeAsync, CurrentTime );
						XGrabPointer( WinData.pX11Display, WinData.X11Window,
							True, EnterWindowMask | LeaveWindowMask |
							PointerMotionMask, GrabModeAsync, GrabModeAsync,
							None, None, CurrentTime );
						m_pWindow->HideCursor( );

						break;
					}
					case LeaveNotify:
					{
						XUngrabKeyboard( WinData.pX11Display, CurrentTime );
						XUngrabPointer( WinData.pX11Display, CurrentTime );
						m_pWindow->ShowCursor( );

						break;
					}
					default:
					{
						break;
					}
				}
			}

			const ZED_UINT64 NewTime = ZED::System::GetTimeMiS( );
			ZED_UINT64 DeltaTime = NewTime-OldTime;

			if( DeltaTime > 250000ULL )
			{
				DeltaTime = 250000ULL;
			}

			OldTime = NewTime;

			Accumulator += DeltaTime;

			while( Accumulator >= TimeStep )
			{
				this->Update( 0.0f );
				ElapsedTime += TimeStep;
				Accumulator -= TimeStep;
			}

			this->Render( );
			++FrameRate;

			if( ( NewTime - FrameTime ) >= 1000000ULL )
			{
				zedTrace( "FPS: %d\n", FrameRate );
				FrameTime = ZED::System::GetTimeMiS( );
				FrameRate = 0;
			}
		}
		return ZED_OK;
	}

	void Game::Update( const ZED_FLOAT64 p_ElapsedGameTime )
	{
		m_EntityManager.Update( );
	}

	void Game::Render( )
	{
		m_pRenderer->BeginScene( ZED_TRUE, ZED_TRUE, ZED_TRUE );
		m_EntityManager.Render( );
		m_pRenderer->EndScene( );
	}
}

