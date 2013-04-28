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
	};
}

#endif

