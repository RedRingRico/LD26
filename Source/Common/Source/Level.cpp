#include <OGL/GLShader.hpp>
#include <OGL/GLModel.hpp>
#include <Level.hpp>

namespace LD26
{
	Level::Level( )
	{
		m_pModel = ZED_NULL;
		m_pShader = ZED_NULL;
	}

	Level::~Level( )
	{
	}

	ZED_UINT32 Level::Initialise( )
	{
		ZED_RENDERER_BACKEND RendererBackEnd = m_pRenderer->BackEnd( );

		switch( RendererBackEnd )
		{
			case ZED_RENDERER_BACKEND_OPENGL:
			{
				m_pShader = new ZED::Renderer::GLShader( );
				m_pModel = new ZED::Renderer::GLModel( m_pRenderer );
				break;
			}
			case ZED_RENDERER_BACKEND_OPENGLES2:
			{
				break;
			}
			case ZED_RENDERER_BACKEND_DIRECT3D:
			{
				break;
			}
			case ZED_RENDERER_BACKEND_DIRECT3D_XBOX:
			{
				break;
			}
			default:
			{
				return ZED_GRAPHICS_ERROR;
			}
		}

		const ZED_CHAR8 *VShaderName =
			"../../Data/Linux/Shaders/VertColour.vsh";
		const ZED_CHAR8 *FShaderName =
			"../../Data/Linux/Shaders/VertColour.fsh";

		m_pShader->Compile( &VShaderName, ZED_VERTEX_SHADER, ZED_TRUE );
		m_pShader->Compile( &FShaderName, ZED_FRAGMENT_SHADER, ZED_TRUE );

		ZED_SHADER_VERTEXATTRIBUTE_GL pAttributes[ 2 ];

		pAttributes[ 0 ].Index = 0;
		pAttributes[ 0 ].pName = "vPosition";
		pAttributes[ 1 ].Index = 1;
		pAttributes[ 1 ].pName = "vColour";
		m_pShader->SetVertexAttributeTypes( pAttributes, 2 );

		ZED_SHADER_CONSTANT_MAP Constant[ 10 ];

		Constant[ 0 ].Index = 0;
		Constant[ 0 ].Type = ZED_MAT4X4;
		Constant[ 0 ].pName = "uWVP";

		Constant[ 1 ].Index = 1;
		Constant[ 1 ].Type = ZED_FLOAT3;
		Constant[ 1 ].pName = "uGlobalAmbient";

		Constant[ 2 ].Index = 2;
		Constant[ 2 ].Type = ZED_FLOAT3;
		Constant[ 2 ].pName = "uLightColour";

		Constant[ 3 ].Index = 3;
		Constant[ 3 ].Type = ZED_FLOAT3;
		Constant[ 3 ].pName = "uLightPos";

		Constant[ 4 ].Index = 4;
		Constant[ 4 ].Type = ZED_FLOAT3;
		Constant[ 4 ].pName = "uEyePos";

		Constant[ 5 ].Index = 5;
		Constant[ 5 ].Type = ZED_FLOAT3;
		Constant[ 5 ].pName = "uEmissive";

		Constant[ 6 ].Index = 6;
		Constant[ 6 ].Type = ZED_FLOAT3;
		Constant[ 6 ].pName = "uAmbient";

		Constant[ 7 ].Index = 7;
		Constant[ 7 ].Type = ZED_FLOAT3;
		Constant[ 7 ].pName = "uDiffuse";

		Constant[ 8 ].Index = 8;
		Constant[ 8 ].Type = ZED_FLOAT3;
		Constant[ 8 ].pName = "uSpecular";

		Constant[ 9 ].Index = 9;
		Constant[ 9 ].Type = ZED_FLOAT1;
		Constant[ 9 ].pName = "uShininess";

		m_pShader->SetConstantTypes( Constant, 10 );

		if( m_pModel->Load( "../../Data/Linux/Models/untitled.zed" ) !=
			ZED_OK )
		{
			return ZED_FAIL;
		}

		return ZED_OK;	

		return ZED_OK;
	}

	void Level::Update( )
	{
		ZED::Arithmetic::Matrix4x4 RotationMatrix;
		ZED::Arithmetic::Matrix4x4 WorldMatrix;
		ZED::Arithmetic::Matrix4x4 PerspProj;
		ZED::Arithmetic::Matrix4x4 WVP;
		ZED::Arithmetic::Vector3 Position( 0.0f, 170.0f, 10.0f );
//		ZED::Arithmetic::Vector3 Look( 0.0f, 100.0f, -100.0f );
//		ZED::Arithmetic::Vector3 Up( 0.0f, 1.0f, 0.0f );
//		m_Position.X( m_XTrans );
//		m_pRenderer->SetViewLookAt( Position, Look, Up );
		m_pRenderer->PerspectiveProjectionMatrix( &PerspProj );
		m_pRenderer->GetWVP( &WorldMatrix );
//		RotationMatrix.RotateY( m_YRotation );
//		m_YRotation += 0.01f;

		ZED_FLOAT32 Matrix[ 16 ];
		ZED::Arithmetic::Matrix4x4 Translation;
		Translation.Translate( m_Position );
		WVP = PerspProj*WorldMatrix;
		WVP.AsFloat( Matrix );
		ZED::Arithmetic::Vector3 GlobalAmbient( 0.1f, 0.1f, 0.1f ),
			LightColour( 1.0f, 1.0f, 1.0f ),
			LightPosition( 12.0f, 12.0f, 0.0f ),
			Emissive( 0.0f, 0.0f, 0.0f ), Ambient( 0.1f, 0.1f, 0.1f ),
			Diffuse( 0.0f, 1.0f, 0.0f ), Specular( 0.0f, 0.0f, 0.0f );
		ZED_FLOAT32 Shininess = 0.0f;

		m_pShader->Activate( );
		m_pShader->SetConstantData( 0, Matrix );
		m_pShader->SetConstantData( 1, ( void * )( &GlobalAmbient ) );
		m_pShader->SetConstantData( 2, ( void * )( &LightColour ) );
		m_pShader->SetConstantData( 3, ( void * )( &LightPosition ) );
		m_pShader->SetConstantData( 4, ( void * )( &Position ) );
		m_pShader->SetConstantData( 5, ( void * )( &Emissive ) );
		m_pShader->SetConstantData( 6, ( void * )( &Ambient ) );
		m_pShader->SetConstantData( 7, ( void * )( &Diffuse ) );
		m_pShader->SetConstantData( 8, ( void * )( &Specular ) );
		m_pShader->SetConstantData( 9, ( void * )( &Shininess ) );
	}

	void Level::Render( )
	{
		m_pModel->Render( );
	}

	ZED_UINT32 Level::LoadMesh( const char *p_pMeshPath )
	{
	}
}

