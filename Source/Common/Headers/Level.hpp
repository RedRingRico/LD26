#ifndef __LD26_LEVEL_HPP__
#define __LD26_LEVEL_HPP__

#include <DataTypes.hpp>
#include <GameEntity.hpp>
#include <Model.hpp>
#include <Shader.hpp>

namespace LD26
{
	class Level : public GameEntity
	{
	public:
		Level( );
		virtual ~Level( );

		virtual ZED_UINT32 Initialise( );

		virtual void Update( );
		virtual void Render( );

		ZED_UINT32 LoadMesh( const char *p_pMeshPath );

	private:
		ZED::Renderer::Shader	*m_pShader;
		ZED::Renderer::Model	*m_pModel;
	};
}

#endif

