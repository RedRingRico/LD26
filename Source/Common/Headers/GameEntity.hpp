#ifndef __DAWN_GAMEENTITY_HPP__
#define __DAWN_GAMEENTITY_HPP__

#include <DataTypes.hpp>
#include <Vector3.hpp>
#include <Matrix4x4.hpp>
#include <Renderer.hpp>

namespace LD26
{
	class GameEntity
	{
	public:
		virtual ZED_INLINE ~GameEntity( ){ }
		ZED_INLINE void ID( ZED_UINT32 p_ID ){ m_ID = p_ID; }
		ZED_INLINE ZED_UINT32 ID( ) const { return m_ID; }

		ZED_INLINE void Position( const ZED_FLOAT32 p_X, const ZED_FLOAT32 p_Y,
			const ZED_FLOAT32 p_Z );
		ZED_INLINE void Position( ZED::Arithmetic::Vector3 &p_Vector );

		ZED_INLINE void SetRenderer( ZED::Renderer::Renderer *p_pRenderer )
			{ m_pRenderer = p_pRenderer;}
		ZED_INLINE void SetViewProjection(
			const ZED::Arithmetic::Matrix4x4 &p_ViewProjection )
			{ m_ViewProjection = p_ViewProjection; }

		virtual ZED_UINT32 Initialise( ) = 0;

		virtual void Update( ) = 0;
		virtual void Render( ) = 0;

	protected:
		ZED_UINT32							m_ID;
		ZED::Arithmetic::Vector3			m_Position;

	public:
		static ZED::Arithmetic::Matrix4x4	m_ViewProjection;
		static ZED::Renderer::Renderer		*m_pRenderer;
	};
}

#endif

