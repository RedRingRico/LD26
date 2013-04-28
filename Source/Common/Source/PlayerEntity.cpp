#include <PlayerEntity.hpp>

namespace LD26
{
	PlayerEntity::PlayerEntity( ZED::System::InputManager *p_pInputManager )
	{
		m_pInputManager = p_pInputManager;
		m_pKeyboard = ZED_NULL;
		m_Position.Set( 0.0f, 0.0f, 0.0f );
		m_AvatarHeadOffset.Set( 0.0f, 170.0f, 0.0f );
		m_CameraReference.Set( 0.0f, 0.0f, 10.0f );
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
			// W
			if( m_pKeyboard->IsKeyDown( 119 ) )
			{
				ZED::Arithmetic::Matrix4x4 ForwardMovement;
				ForwardMovement.RotateY( m_Yaw );
				ZED::Arithmetic::Vector3 Velocity( 0.0f, 0.0f,
					PlayerVelocity );
				Velocity = ForwardMovement.Transform( Velocity );
				m_Position[ 2 ] += Velocity[ 2 ];
				m_Position[ 0 ] += Velocity[ 0 ];
			}
			// A
			if( m_pKeyboard->IsKeyDown( 115 ) )
			{
				ZED::Arithmetic::Matrix4x4 StrafeMovement;
				StrafeMovement.RotateY( m_Yaw );
				ZED::Arithmetic::Vector3 Velocity( -PlayerVelocity, 0.0f,
					0.0f );
				Velocity = StrafeMovement.Transform( Velocity );
				m_Position[ 2 ] += Velocity[ 2 ];
				m_Position[ 0 ] += Velocity[ 0 ];
			}
			// S
			if( m_pKeyboard->IsKeyDown( 100 ) )
			{
				ZED::Arithmetic::Matrix4x4 ForwardMovement;
				ForwardMovement.RotateY( m_Yaw );
				ZED::Arithmetic::Vector3 Velocity( 0.0f, 0.0f,
					-PlayerVelocity );
				Velocity = ForwardMovement.Transform( Velocity );
				m_Position[ 2 ] += Velocity[ 2 ];
				m_Position[ 0 ] += Velocity[ 0 ];
			}
			// D
			if( m_pKeyboard->IsKeyDown( 102 ) )
			{
				ZED::Arithmetic::Matrix4x4 StrafeMovement;
				StrafeMovement.RotateY( m_Yaw );
				ZED::Arithmetic::Vector3 Velocity( PlayerVelocity, 0.0f,
					0.0f );
				Velocity = StrafeMovement.Transform( Velocity );
				m_Position[ 2 ] += Velocity[ 2 ];
				m_Position[ 0 ] += Velocity[ 0 ];
			}
			// I
			if( m_pKeyboard->IsKeyDown( 111 ) )
			{
				m_Pitch -= LookVelocity;
			}
			// J
			if( m_pKeyboard->IsKeyDown( 107 ) )
			{
				m_Yaw -= LookVelocity;
			}
			// K
			if( m_pKeyboard->IsKeyDown( 108 ) )
			{
				m_Pitch += LookVelocity;
			}
			// L
			if( m_pKeyboard->IsKeyDown( 59 ) )
			{
				m_Yaw += LookVelocity;
			}
		}

		m_RotationMatrix.RotateY( m_Yaw );
//		m_RotationMatrix.Rotate( 0.0f, m_Pitch, m_Yaw ); <--NEED TO FIX!!
		ZED::Arithmetic::Matrix4x4 RotX;
		ZED::Arithmetic::Matrix4x4 CameraRot;
		RotX.RotateX( m_Pitch );
		m_RotationMatrix *= RotX;
		m_HeadOffset = m_RotationMatrix.Transform( m_AvatarHeadOffset );
		m_CameraPosition = m_Position + m_HeadOffset;

		CameraRot = m_RotationMatrix*RotX;

		m_TransformedReference =
			CameraRot.Transform( m_CameraReference );
		m_CameraLookAt = m_TransformedReference + m_CameraPosition;
		ZED::Arithmetic::Vector3 Up( 0.0f, 1.0f, 0.0f );

		m_pRenderer->SetViewLookAt( m_CameraPosition, m_CameraLookAt, Up );
	}

	void PlayerEntity::Render( )
	{
	}
}

