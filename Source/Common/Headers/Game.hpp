#ifndef __LD26_GAME_HPP__
#define __LD26_GAME_HPP__

#include <DataTypes.hpp>
#include <Renderer.hpp>
#include <VertexCache.hpp>
#include <CanvasDescription.hpp>
#include <cstdio>
#include <Shader.hpp>
#include <InputManager.hpp>
#include <GameEntityManager.hpp>
#include <Level.hpp>
#include <Keyboard.hpp>

namespace LD26
{
	class Game
	{
	public:
		Game( );
		~Game( );

		ZED_UINT32 Initialise( const ZED_BOOL p_FullScreen = ZED_FALSE );
		void Update( const ZED_FLOAT64 p_ElapsedGameTime );
		void Render( );

		ZED_UINT32 Execute( );

		ZED_INLINE ZED_BOOL Running( ) const { return m_Running; }

	private:
#if ( ZED_PLATFORM_PANDORA || ZED_PLATFORM_LINUX )
		FILE	*m_pConfigFile;
#elif ( ZED_PLATFORM_XBOX || ZED_PLATFORM_WINDOWS )
		HANDLE m_ConfigFile;
#endif
#if ( ZED_PLATFORM_WINDOWS )
		HDC m_DeviceContext;

		static LRESULT CALLBACK WindowProc( HWND p_HWND, UINT p_Message,
			WPARAM p_WParam, LPARAM p_LParam );

		ZED_BOOL WindowProc( UINT p_Message, WPARAM p_WParam, LPARAM p_LParam );
#endif
		ZED_BOOL m_FullScreen;

		ZED_BOOL m_Running;

		ZED::Renderer::Renderer				*m_pRenderer;
		ZED::Renderer::CanvasDescription	m_Canvas;
		ZED::Renderer::Window				*m_pWindow;
		ZED::System::InputManager			*m_pInputManager;
		ZED::System::Keyboard				m_Keyboard;
		LD26::GameEntityManager				m_EntityManager;
		LD26::Level							m_Level;
	};
}

#endif

