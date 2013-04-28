#include <PlayerEntity.hpp>

namespace LD26
{
	PlayerEntity::PlayerEntity( ZED::System::InputManager *p_pInputManager )
	{
		m_pInputManager = p_pInputManager;
		m_pKeyboard = ZED_NULL;
	}

	PlayerEntity::~PlayerEntity( )
	{
	}

	ZED_UINT32 PlayerEntity::Initialise( )
	{
		if( m_pInputManager->Types( ) & ZED_INPUT_DEVICE_KEYBOARD )
		{
			ZED::System::InputDevice *pTmp;
			m_pInputManager->GetDevice( &pTmp,
				ZED_INPUT_DEVICE_KEYBOARD );
			m_pKeyboard = dynamic_cast< ZED::System::Keyboard * >( pTmp );
		}

		return ZED_OK;
	}

	void PlayerEntity::Update( )
	{
		if( m_pKeyboard )
		{
			if( m_pKeyboard->IsKeyDown( 'w' ) )
			{
				zedTrace( "PLAYER MOVING FORWARD\n" );
				// Need to update the world...
			}
		}
	}

	void PlayerEntity::Render( )
	{
	}
}

