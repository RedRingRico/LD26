#ifndef __LD26_PLAYERENTITY_HPP__
#define __LD26_PLAYERENTITY_HPP__

#include <GameEntity.hpp>
#include <Model.hpp>
#include <InputManager.hpp>
#include <Keyboard.hpp>

namespace LD26
{
	class PlayerEntity : public GameEntity
	{
	public:
		ZED_EXPLICIT PlayerEntity( ZED::System::InputManager *p_InputManager );
		virtual ~PlayerEntity( );
		
		virtual ZED_UINT32 Initialise( );

		virtual void Update( );
		virtual void Render( );

	private:
		ZED::System::InputManager	*m_pInputManager;
		ZED::System::Keyboard		*m_pKeyboard;
		ZED::Arithmetic::Vector3	m_AvatarHeadOffset;
		ZED::Arithmetic::Matrix4x4	m_RotationMatrix;
		ZED::Arithmetic::Vector3	m_HeadOffset;
		ZED::Arithmetic::Vector3	m_CameraPosition;
		ZED::Arithmetic::Vector3	m_CameraReference;
		ZED::Arithmetic::Vector3	m_TransformedReference;
		ZED::Arithmetic::Vector3	m_CameraLookAt;
		ZED_FLOAT32					m_Yaw;
		ZED_FLOAT32					m_Pitch;
	};
}

#endif

