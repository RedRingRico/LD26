#include <GameEntityManager.hpp>

namespace LD26
{
	GameEntityManager::GameEntityManager( )
	{
	}

	GameEntityManager::~GameEntityManager( )
	{
	}

	ZED_UINT32 GameEntityManager::Add( GameEntity *p_pEntity )
	{
		p_pEntity->Initialise( );
		m_Entities.push_back( p_pEntity );
	}

	void GameEntityManager::Update( )
	{
		EntityList::iterator Itr = m_Entities.begin( );

		for( ; Itr != m_Entities.end( ); ++Itr )
		{
			( *Itr )->Update( );
		}
	}

	void GameEntityManager::Render( )
	{
		EntityList::iterator Itr = m_Entities.begin( );

		for( ; Itr != m_Entities.end( ); ++Itr )
		{
			( *Itr )->Render( );
		}
	}
}

