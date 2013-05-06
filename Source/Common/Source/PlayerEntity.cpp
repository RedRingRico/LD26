#include <PlayerEntity.hpp>

namespace LD26
{
	PlayerEntity::PlayerEntity( ZED::System::InputManager *p_pInputManager )
	{
		m_pInputManager = p_pInputManager;
		m_pKeyboard = ZED_NULL;
		m_Position.Set( 0.0f, 0.0f, 0.0f );
		m_AvatarHeadOffset.Set( 0.0f, 170.0f, 0.0f );
		m_CameraReference.Set( 0.0f, 0.0f, -1.0f );
		m_UpReference.Set( 0.0f, 1.0f, 0.0f );
		m_Yaw = m_Pitch = 0.0f;
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
			ZED_FLOAT32 PlayerVelocity = 62.0f;
			ZED_FLOAT32 LookVelocity = 0.05f;
			ZED::Arithmetic::Matrix3x3 MoveDirection;
			MoveDirection.Identity( );
			MoveDirection.RotateY( m_Yaw );
			// W
			if( m_pKeyboard->IsKeyDown( 119 ) )
			{
				ZED::Arithmetic::Vector3 Velocity( 0.0f, 0.0f,
					-PlayerVelocity );
				Velocity = Velocity*MoveDirection;
				m_Position[ 2 ] += Velocity[ 2 ];
				m_Position[ 0 ] += Velocity[ 0 ];

			}
			// A
			if( m_pKeyboard->IsKeyDown( 115 ) )
			{
				ZED::Arithmetic::Vector3 Velocity( -PlayerVelocity, 0.0f,
					0.0f );
				Velocity = Velocity*MoveDirection;
				m_Position[ 2 ] += Velocity[ 2 ];
				m_Position[ 0 ] += Velocity[ 0 ];

			}
			// S
			if( m_pKeyboard->IsKeyDown( 100 ) )
			{
				ZED::Arithmetic::Vector3 Velocity( 0.0f, 0.0f,
					PlayerVelocity );
				Velocity = Velocity*MoveDirection;
				m_Position[ 2 ] += Velocity[ 2 ];
				m_Position[ 0 ] += Velocity[ 0 ];
			}
			// D
			if( m_pKeyboard->IsKeyDown( 102 ) )
			{
				ZED::Arithmetic::Vector3 Velocity( PlayerVelocity, 0.0f,
					0.0f );
				Velocity = Velocity*MoveDirection;
				m_Position[ 2 ] += Velocity[ 2 ];
				m_Position[ 0 ] += Velocity[ 0 ];
			}
			// I
			if( m_pKeyboard->IsKeyDown( 111 ) )
			{
				m_Pitch += LookVelocity;
			}
			// J
			if( m_pKeyboard->IsKeyDown( 107 ) )
			{
				m_Yaw += LookVelocity;
			}
			// K
			if( m_pKeyboard->IsKeyDown( 108 ) )
			{
				m_Pitch -= LookVelocity;
			}
			// L
			if( m_pKeyboard->IsKeyDown( 59 ) )
			{
				m_Yaw -= LookVelocity;
			}
		}


		if( m_Pitch >= ( ( ZED_Pi / 2.0f ) - 0.01f ) )
		{
			m_Pitch =( ( ZED_Pi / 2.0f ) - 0.01f );
		}
		if( m_Pitch <= -( ( ZED_Pi / 2.0f ) - 0.01f ) )
		{
			m_Pitch = -( ( ZED_Pi / 2.0f ) - 0.01f );
		}

		if( m_Position[ 0 ] < -418.0f )
		{
			m_Position[ 0 ] = -418.0f;
		}
		if( m_Position[ 0 ] > 424.0f )
		{
			m_Position[ 0 ] = 424.0f;
		}

		if( m_Position[ 2 ] < -118.0f )
		{
			m_Position[ 2 ] = -118.0f;
		}
		if( m_Position[ 2 ] > 50.0f )
		{
			m_Position[ 2 ] = 50.0f;
		}

		ZED::Arithmetic::Matrix3x3 UpRot;
		ZED::Arithmetic::Matrix3x3 DownRot;
		ZED::Arithmetic::Vector3 LookPoint;
		ZED::Arithmetic::Vector3 WorldUp;
		ZED::Arithmetic::Matrix3x3 CameraRot;
		ZED::Arithmetic::Vector3 CameraPosition;

		CameraRot = ( UpRot.RotateX( m_Pitch )*DownRot.RotateY( m_Yaw ) );
		m_HeadOffset = m_AvatarHeadOffset*CameraRot;
		m_CameraPosition = m_Position + m_HeadOffset;
		ZED::Arithmetic::Vector3 RotTarg = m_CameraReference*CameraRot;
		m_CameraLookAt = m_CameraPosition + RotTarg;
		WorldUp = m_UpReference*CameraRot;


		ZED::Arithmetic::Vector3 MinOffset( -40.0f, 0.0f, -40.0f );
		ZED::Arithmetic::Vector3 MaxOffset( 40.0f, 170.0f, 40.0f );
		// Do collisions with the environment
		m_BoundingBox.Min( m_Position + MinOffset );
		m_BoundingBox.Max( m_Position + MaxOffset );

		m_pRenderer->SetViewLookAt( m_CameraPosition, m_CameraLookAt, WorldUp );
	}

	void PlayerEntity::Render( )
	{
	}
}

