#pragma once

#include "../forward/forward.h"

namespace DrawerLib
{

class ClassicDrawer : public ForwardDrawer
{
public:
	ClassicDrawer(const DisplayHandle& display);
	ClassicDrawer(IFrameWindow*& outMainWnd, const WindowHandle& srcWainWindowHandle, boolean fullscrean, boolean stereo);
	~ClassicDrawer();
	
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
				byte reflectVector : 1;
				byte waterReflection : 1;
				byte specular : 1;
				byte brutalFast : 1;
				byte caustic : 1;
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
				byte envCubeTex : 1;
				byte alphaTest : 1;
				byte specular : 1;
				byte hueMatrix : 1;
				byte saturation : 1;
				byte metal : 1;
				byte caustic : 1;
				byte tripleTexuresMode : 1;
				byte brutalFast : 1;
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
			uint32 number;
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
			static_assert(sizeof(MeshVertexShaderIndex) == 2, "MeshVertexShaderIndex invalid size");
			static_assert(sizeof(MeshPixelShaderIndex) == 2, "MeshPixelShaderIndex invalid size");
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
				bool specular : 1;
				bool foam : 1;
				byte brutalFast : 1;
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

	struct DepthOnlyMeshVertexShaderIndex
	{
		union 
		{
			uint16 number;
			struct
			{
				byte format : 4;
			};
		};

		INLINE boolean operator == (const DepthOnlyMeshVertexShaderIndex& b)const { return number == b.number; }
		/*MeshVertexShaderIndex()
		{
			COMPILETIME_ASSERT(sizeof(MeshVertexShaderIndex) == 2);
			number = 0;
		}*/
	};

	struct DepthOnlyMeshPixelShaderIndex
	{
		union 
		{
			uint16 number;
			struct
			{
				byte alphaTest : 1;
			};
		};

		INLINE boolean operator == (const DepthOnlyMeshPixelShaderIndex& b)const { return number == b.number; }
		/*MeshPixelShaderIndex()
		{
			COMPILETIME_ASSERT(sizeof(MeshPixelShaderIndex) == 2);
			number = 0;
		}*/
	};

	struct DepthOnlyMeshShaderProgramIndex
	{
		union 
		{
			uint32 number;
			struct
			{
				DepthOnlyMeshVertexShaderIndex vs;
				DepthOnlyMeshPixelShaderIndex ps;
			};
		};

		INLINE boolean operator == (const DepthOnlyMeshShaderProgramIndex& b)const { return number == b.number; }

		DepthOnlyMeshShaderProgramIndex()
		{
			static_assert(sizeof(DepthOnlyMeshShaderProgramIndex) == 4, "DepthOnlyMeshShaderProgramIndex invalid size");
			static_assert(sizeof(DepthOnlyMeshVertexShaderIndex) == 2, "DepthOnlyMeshVertexShaderIndex invalid size");
			static_assert(sizeof(DepthOnlyMeshPixelShaderIndex) == 2, "DepthOnlyMeshPixelShaderIndex invalid size");
			number = 0;
		}
	};
	
protected:
	void draw(DrawTask& task, boolean interfaceSubViewport = false);
	void init();
	void drawBatch(BatchBase& batch, const Matrix44& viewAndProj, const Matrix34& toEyeMatrix, float sensitivity, boolean alphaTest = false);
	
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
		int Opacity0Coef;
		int	Opacity1Coef;
		int CausticTexCoordMatrix;
		int CausticPlane;
		int CausticLayer7Scale;
		int CausticFadeBack;

		int ToLightDirection;
		int LightIntensivity;
		int AmbientIntensivity;
		int IntensivityMultiplyer;
		int ReflectionPalneCoordOffsetAndScale;

		void init(ShaderProgram* prog);
	};

	struct MeshShaderProgramAndParameters : public VertexShaderAndParameters, public RefCountable
	{
		StrongPtr<ShaderProgram> program;

		//ps
		int EnvColor;
		int HueMatrix;
		int SaturationCoefs;
		int CausticScale;

		TextureNameIndexAndIndex Tex0;
		TextureNameIndexAndIndex Tex1;
		TextureNameIndexAndIndex Tex2;
		TextureNameIndexAndIndex Tex02;
		TextureNameIndexAndIndex Tex12;
		TextureNameIndexAndIndex Tex22;
		TextureNameIndexAndIndex EnvTex;
		TextureNameIndexAndIndex CausticTex;
		TextureNameIndexAndIndex SpecularFuncTex;

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

	String getMeshVertexShaderCode(MeshVertexShaderIndex params);
	String getMeshPixelShaderCode(MeshPixelShaderIndex params);
	
	struct WaterPlaneShaderProgramAndParameters : public VertexShaderAndParameters, public RefCountable
	{
		StrongPtr<ShaderProgram> program;

		//ps
		int NormalScale;
		int SpecularIntensivity;
		int FrenselBase;
		int EnvIntensivity;
		int RoughtnessSqr;
		int Glossines;
		int FoamTexScale;
		int FoamBumpScale;

		TextureNameIndexAndIndex Tex;
		TextureNameIndexAndIndex EnvTex;
		TextureNameIndexAndIndex FoamTex;
		TextureNameIndexAndIndex SpecularFuncTex;

		void init(ShaderProgram* prog);
	};

	String getWaterPlanePixelShaderCode(WaterPlanePixelShaderIndex params);
	HashArray< Pair<WaterPlanePixelShaderIndex, StrongPtr<PixelShader> > > waterPlanePixelShaders;
	HashArray< Pair<WaterPlaneShaderProgramIndex, StrongPtr<WaterPlaneShaderProgramAndParameters> > > waterPlaneShaderPrograms;
	
	const WaterPlaneShaderProgramAndParameters& getWaterPlaneProgram(WaterPlaneShaderProgramIndex params);


	StrongPtr<VertexShader> particlesVS1;
	StrongPtr<PixelShader> particlesPS1;
	StrongPtr<ShaderProgram> particlesProg1;
	uint particlesProg1_nFullTransformMatrix_M44;
	uint particlesProg1_ColorScale;
	TextureNameIndexAndIndex particlesProg1_Tex;

	StrongPtr<VertexShader> particlesVS2;
	StrongPtr<PixelShader> particlesPS2;
	StrongPtr<ShaderProgram> particlesProg2;
	uint particlesProg2_nFullTransformMatrix_M44;
	uint particlesProg2_ColorScale;
	TextureNameIndexAndIndex particlesProg2_Tex;

	String getParticlesVertexShaderCode();
	String getParticlesPixelShaderCode(boolean bDualTex);

	struct DepthOnlyMeshShaderProgramAndParameters : public RefCountable
	{
		StrongPtr<ShaderProgram> program;

		//vs
		int FullTransformMatrix;
		int TexCoordMatrix;
		int BoneArray;

		//ps
		TextureNameIndexAndIndex Tex;
		
		void init(ShaderProgram* prog);
	};

	HashArray< Pair<DepthOnlyMeshVertexShaderIndex, StrongPtr<VertexShader> > > depthOnlyMeshVertexShaders;
	HashArray< Pair<DepthOnlyMeshPixelShaderIndex, StrongPtr<PixelShader> > > depthOnlyMeshPixelShaders;
	HashArray< Pair<DepthOnlyMeshShaderProgramIndex, StrongPtr<DepthOnlyMeshShaderProgramAndParameters> > > depthOnlyMeshShaderPrograms;
	const DepthOnlyMeshShaderProgramAndParameters& getDepthOnlyMeshProgram(DepthOnlyMeshShaderProgramIndex params);
	String getDepthOnlyMeshVertexShaderCode(DepthOnlyMeshVertexShaderIndex params);
	String getDepthOnlyMeshPixelShaderCode(DepthOnlyMeshPixelShaderIndex params);

	StrongPtr<ITexture> specularTexture;
	TextureBase* getSpecularTexture();
};
	
}


namespace Base
{
	template<>
	class DefaultHashGen<DrawerLib::ClassicDrawer::MeshVertexShaderIndex>
	{
	public:
		INLINE Hash operator () (const DrawerLib::ClassicDrawer::MeshVertexShaderIndex& key){ return genHash(key.number); }
	};

	template<>
	class DefaultHashGen<DrawerLib::ClassicDrawer::MeshPixelShaderIndex>
	{
	public:
		INLINE Hash operator () (const DrawerLib::ClassicDrawer::MeshPixelShaderIndex& key){ return genHash(key.number); }
	};

	template<>
	class DefaultHashGen<DrawerLib::ClassicDrawer::MeshShaderProgramIndex>
	{
	public:
		INLINE Hash operator () (const DrawerLib::ClassicDrawer::MeshShaderProgramIndex& key){ return genHash(key.number); }
	};

	template<>
	class DefaultHashGen<DrawerLib::ClassicDrawer::WaterPlanePixelShaderIndex>
	{
	public:
		INLINE Hash operator () (const DrawerLib::ClassicDrawer::WaterPlanePixelShaderIndex& key){ return genHash(key.number); }
	};

	template<>
	class DefaultHashGen<DrawerLib::ClassicDrawer::WaterPlaneShaderProgramIndex>
	{
	public:
		INLINE Hash operator () (const DrawerLib::ClassicDrawer::WaterPlaneShaderProgramIndex& key){ return genHash(key.number); }
	};


	template<>
	class DefaultHashGen<DrawerLib::ClassicDrawer::DepthOnlyMeshVertexShaderIndex>
	{
	public:
		INLINE Hash operator () (const DrawerLib::ClassicDrawer::DepthOnlyMeshVertexShaderIndex& key){ return genHash(key.number); }
	};

	template<>
	class DefaultHashGen<DrawerLib::ClassicDrawer::DepthOnlyMeshPixelShaderIndex>
	{
	public:
		INLINE Hash operator () (const DrawerLib::ClassicDrawer::DepthOnlyMeshPixelShaderIndex& key){ return genHash(key.number); }
	};

	template<>
	class DefaultHashGen<DrawerLib::ClassicDrawer::DepthOnlyMeshShaderProgramIndex>
	{
	public:
		INLINE Hash operator () (const DrawerLib::ClassicDrawer::DepthOnlyMeshShaderProgramIndex& key){ return genHash(key.number); }
	};
}
