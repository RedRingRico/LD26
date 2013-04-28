#ifndef __LD26_GAMEENTITYMANAGER_HPP__
#define __LD26_GAMEENTITYMANAGER_HPP__

#include <vector>
#include <DataTypes.hpp>
#include <GameEntity.hpp>

typedef std::vector< LD26::GameEntity * >EntityList;

namespace LD26
{
	class GameEntityManager
	{
	public:
		GameEntityManager( );
		~GameEntityManager( );

		ZED_UINT32 Add( GameEntity *p_pEntity );

		void Update( );
		void Render( );

	private:
		EntityList m_Entities;
	};
}

#endif

