#pragma once

#include "../forward/forward.h"

namespace DrawerLib
{

class RightDrawer : public ForwardDrawer
{
public:
	RightDrawer(const DisplayHandle& display);
	RightDrawer(IFrameWindow*& outMainWnd, const WindowHandle& srcWainWindowHandle, boolean fullscrean, boolean stereo);
	~RightDrawer();
	
	void draw(Range<DrawTask*> parallelTasks);


	struct MeshVertexShaderIndex
	{
		union 
		{
			uint16 number;
			struct
			{
				byte format : 4;
				byte lightProbe : 4;
				byte twoSided : 1;
			};
		};

		INLINE boolean operator == (const MeshVertexShaderIndex& b)const { return number == b.number; }
		/*MeshVertexShaderIndex()
		{
			COMPILETIME_ASSERT(sizeof(MeshVertexShaderIndex) == 2);
			number = 0;
		}*/
	};

	struct MeshPixelShaderIndex
	{
		union 
		{
			uint16 number;
			struct
			{
				byte dualLayer : 1;
				byte singleLightProbe : 1;
				byte opacity : 1;
				byte backlight : 1;
				byte anisotropic : 1;
				byte envCubeTex : 1;
				byte params : 1;
				byte colorMatrix : 1;
				byte alphaTest : 1;
			};
		};

		INLINE boolean operator == (const MeshPixelShaderIndex& b)const { return number == b.number; }
		/*MeshPixelShaderIndex()
		{
			COMPILETIME_ASSERT(sizeof(MeshPixelShaderIndex) == 2);
			number = 0;
		}*/
	};

	struct MeshShaderProgramIndex
	{
		union 
		{
			struct
			{
				uint32 number;
			};
			struct
			{
				MeshVertexShaderIndex vs;
				MeshPixelShaderIndex ps;
			};
		};

		INLINE boolean operator == (const MeshShaderProgramIndex& b)const { return number == b.number; }

		MeshShaderProgramIndex()
		{
			static_assert(sizeof(MeshShaderProgramIndex) == 4, "MeshShaderProgramIndex invalid size");
			number = 0;
		}
	};

	struct WaterPlanePixelShaderIndex
	{
		union 
		{
			uint16 number;
			struct
			{
				byte singleLightProbe : 1;
			};
		};

		INLINE boolean operator == (const WaterPlanePixelShaderIndex& b)const { return number == b.number; }
		/*WaterPlanePixelShaderIndex()
		{
			COMPILETIME_ASSERT(sizeof(WaterPlanePixelShaderIndex) == 2);
			number = 0;
		}*/
	};

	struct WaterPlaneShaderProgramIndex
	{
		union 
		{
			uint32 number;
			struct
			{
				MeshVertexShaderIndex vs;
				WaterPlanePixelShaderIndex ps;
			};
		};

		INLINE boolean operator == (const WaterPlaneShaderProgramIndex& b)const { return number == b.number; }

		WaterPlaneShaderProgramIndex()
		{
			static_assert(sizeof(WaterPlaneShaderProgramIndex) == 4, "WaterPlaneShaderProgramIndex invalid size");
			number = 0;
		}
	};
	
protected:
	void draw(DrawTask& task);
	void init();
	void drawBatch(BatchBase& batch, const Matrix44& viewAndProj, const Matrix34& toEyeMatrix, float sensitivity, boolean alphaTest = false);
	
	VertexShader* particlesVS;
	PixelShader* particlesPS;
	ShaderProgram* particlesProg;
	uint particlesVS_nFullTransformM44;
	String getParticlesVertexShaderCode();
	String getParticlesPixelShaderCode();

	struct VertexShaderAndParameters
	{
		int FullTransformMatrix;
		int TexCoordMatrix;
		int ToEyeVectorMatrix;
		int TangentBinormalMatrix;
		int NormalMatrix;
		int LightProbeStar7_coordMAdd;
		int LightProbeStar7_probes;
		int LightProbeOcto_coordMAdd;
		int LightProbeOcto_probes;
		int LightProbeSingle_probe;
		int NoLightBackFacesMultCoef;
		int NoLightBackFacesAddCoef;
		int BoneArray;
		int LightProbeArray;
		int OpacityCoefs;

		int ToLightDirection;
		int LightColor;
		int AmbientColor;

		void init(ShaderProgram* prog);
	};

	struct MeshShaderProgramAndParameters : public VertexShaderAndParameters, public RefCountable
	{
		StrongPtr<ShaderProgram> program;

		//ps
		int EnvColor;
		int NormalMapScale;
		int OpacityScale;
		int GlossinesScale;
		int SpecularMaskScale;
		int GlossinesDeltha;
		int SpecularMaskDeltha;
		int Frensel;
		int Metal;
		int ColorMatrix;
		int NormalMapScale2;
		int OpacityScale2;
		int GlossinesScale2;
		int SpecularMaskScale2;
		int GlossinesDeltha2;
		int SpecularMaskDeltha2;
		int Frensel2;
		int Metal2;
		int ColorMatrix2;
		int ColorMultiplyer;
		int RoughtnessSqrExp2Coef;
		int BacklightMultCoef;
		int BacklightAddCoef;

		TextureNameIndexAndIndex Tex0;
		TextureNameIndexAndIndex Tex1;
		TextureNameIndexAndIndex Tex02;
		TextureNameIndexAndIndex Tex12;
		TextureNameIndexAndIndex EnvTex;

		void init(ShaderProgram* prog);
	};

	void setLightProbeParameters(const VertexShaderAndParameters& prog, const LightProbeBase& lp);

	HashArray< Pair<MeshVertexShaderIndex, StrongPtr<VertexShader> > > meshVertexShaders;
	HashArray< Pair<MeshPixelShaderIndex, StrongPtr<PixelShader> > > meshPixelShaders;
	HashArray< Pair<MeshShaderProgramIndex, StrongPtr<MeshShaderProgramAndParameters> > > meshShaderPrograms;
	
	INLINE boolean01 isSingleLightProbe(ELightProbeType lightProbe, EVertexFormat fmt)
	{
		return lightProbe == ELightProbeType_Single && fmt != EVertexFormat_BuildinLightingMesh;
	}

	const MeshShaderProgramAndParameters& getMeshProgram(MeshShaderProgramIndex params);
	
	Array<String> getVertexShaderAttributesCode(EVertexFormat fmt);

	String getMeshVertexShaderCode(MeshVertexShaderIndex params);
	String getMeshPixelShaderCode(MeshPixelShaderIndex params);
	
	struct WaterPlaneShaderProgramAndParameters : public VertexShaderAndParameters, public RefCountable
	{
		StrongPtr<ShaderProgram> program;

		//ps
		int EnvColor;
		int BlendCoefs;
		int OpacityScale;
		int Albedo;
		int SpecularMask;
		int Frensel;
		int RoughtnessSqr;
		int EnvCoordScale;
		int EnvTexCoordScale;
		int ColorMultiplyer;

		TextureNameIndexAndIndex Tex;
		TextureNameIndexAndIndex EnvTex;

		void init(ShaderProgram* prog);
	};

	String getWaterPlanePixelShaderCode(WaterPlanePixelShaderIndex params);
	HashArray< Pair<WaterPlanePixelShaderIndex, StrongPtr<PixelShader> > > waterPlanePixelShaders;
	HashArray< Pair<WaterPlaneShaderProgramIndex, StrongPtr<WaterPlaneShaderProgramAndParameters> > > waterPlaneShaderPrograms;
	
	const WaterPlaneShaderProgramAndParameters& getWaterPlaneProgram(WaterPlaneShaderProgramIndex params);
};

}

namespace Base
{
	template<>
	class DefaultHashGen<DrawerLib::RightDrawer::MeshVertexShaderIndex>
	{
	public:
		INLINE Hash operator () (const DrawerLib::RightDrawer::MeshVertexShaderIndex& key){ return genHash(key.number); }
	};

	template<>
	class DefaultHashGen<DrawerLib::RightDrawer::MeshPixelShaderIndex>
	{
	public:
		INLINE Hash operator () (const DrawerLib::RightDrawer::MeshPixelShaderIndex& key){ return genHash(key.number); }
	};

	template<>
	class DefaultHashGen<DrawerLib::RightDrawer::MeshShaderProgramIndex>
	{
	public:
		INLINE Hash operator () (const DrawerLib::RightDrawer::MeshShaderProgramIndex& key){ return genHash(key.number); }
	};

	template<>
	class DefaultHashGen<DrawerLib::RightDrawer::WaterPlanePixelShaderIndex>
	{
	public:
		INLINE Hash operator () (const DrawerLib::RightDrawer::WaterPlanePixelShaderIndex& key){ return genHash(key.number); }
	};

	template<>
	class DefaultHashGen<DrawerLib::RightDrawer::WaterPlaneShaderProgramIndex>
	{
	public:
		INLINE Hash operator () (const DrawerLib::RightDrawer::WaterPlaneShaderProgramIndex& key){ return genHash(key.number); }
	};
}
