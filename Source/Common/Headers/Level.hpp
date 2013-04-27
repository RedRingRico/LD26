#ifndef __LD26_LEVEL_HPP__
#define __LD26_LEVEL_HPP__

#include <DataTypes.hpp>

namespace LD26
{
	class Level
	{
	public:
		Level( );
		~Level( );

		ZED_UINT32 LoadMesh( const char *p_pMeshPath );

		void Render( );

	private:
	};
}

#endif

