#pragma once

#include "resources.h"

namespace DrawerLib
{

enum EMaterialMethaType
{
	EMaterialMethaType_None = 0,
	EMaterialMethaType_Solid,
	EMaterialMethaType_AlphaTest,
	EMaterialMethaType_Transparent,
	EMaterialMethaType_EnumCount
};

struct SaturationColorCorrection
{
	Vector2_t scale;
	Vector2_t componentSummScale;

	SaturationColorCorrection()
	{
		scale.setOne();
		componentSummScale.setZero();
	}
};

enum ELightProbeType
{
	ELightProbeType_None = 0,
	ELightProbeType_Single,
	ELightProbeType_Star7,
	ELightProbeType_Octo,
};


struct LightProbe
{
	PackedVector4f toLightVectorXIntensivityAndIntensivity;
	PackedVector3f ambientIntensivity;
	float lightIntensivityPacked;
	//lightColorR = lightColorPacked % 1.0;
	//lightColorG = lightColorPacked / 1024;
	//lightColorB = 1.0f - lightColorR - lightColorG;
	//lightColor - normalized color, intensivity stored in lightVector
};


struct LightProbeBase
{
	ELightProbeType type;
	LightProbeBase(ELightProbeType t) : type(t) {}
};

struct LightProbeUnpucked
{
	Vector4 toLightVectorXIntensivityAndIntensivity;
	Vector4 lightIntensivityXEnergyAndEnergy;
	Vector4 ambientIntensivity;
	LightProbeUnpucked()
	{
		toLightVectorXIntensivityAndIntensivity.set(0.0f, 0.0f, 1.0f, 0.0f);
		lightIntensivityXEnergyAndEnergy.setZero();
		ambientIntensivity.setZero();
	}
};

struct LightProbeStar7 : public LightProbeBase
{
	Vector2_t center;
	Vector2_t invScale;

	LightProbeUnpucked c;
	LightProbeUnpucked xn, xp;
	LightProbeUnpucked yn, yp;
	LightProbeUnpucked zn, zp;

	LightProbeStar7() : LightProbeBase(ELightProbeType_Star7)
	{
		center.setZero();
		invScale.setOne();
	}
};

struct LightProbeOcto : public LightProbeBase
{
	Vector2_t zeroPoint;
	Vector2_t invSize;
	LightProbeUnpucked probe[2][2][2];

	LightProbeOcto() : LightProbeBase(ELightProbeType_Octo)
	{
		zeroPoint.setZero();
		invSize.setOne();
	}
};

struct LightProbeSingle : public LightProbeBase
{
	Vector2_t toLightDirection;
	Vector2_t lightIntensivity;
	Vector2_t ambientIntensivity;
	LightProbeSingle() : LightProbeBase(ELightProbeType_Single)
	{
		toLightDirection.set(0.0f, 0.0f, 1.0f);
		lightIntensivity.setOne();
		ambientIntensivity.setZero();
	}
};

struct WaterPlaneParams
{
	Vector4 plane;
	float heightmapLayerParam7Scale;
	float fullBeginDistance;
	float fullEndDistance;
	Matrix34 causticTexCoordMatrix;
	float causticScale;
	ITexture* caustic;

	WaterPlaneParams()
	{
		plane.set(0.0f,0.0f,1.0f,0.0f);
		heightmapLayerParam7Scale = 4.0f;
		fullBeginDistance = 1;
		fullEndDistance = 10.0f;
		causticTexCoordMatrix.setIdentity();
		causticScale = 1;
		caustic = 0;
	}
};

enum EBatchType
{
	EBatchType_None = 0,
	EBatchType_Mesh,
	EBatchType_WaterPlane,
	EBatchType_DepthOnly,
	EBatchType_Transparent,
	EBatchType_Particles,
	EBatchType_Points,
	EBatchType_Lines,
	EBatchType_Triangles,
	EBatchType_InterfaceQuads,
	EBatchType_InterfaceFontGlyphs,
	EBatchType_DrawTask
};

struct BatchBase
{
	EBatchType type;
	float sortDistance;
	BatchBase(EBatchType batchType)
	{
		this->type = batchType;
		sortDistance = 0;
	}
};

class BoneQuatAndPos
{
public:
	Quaternion q;
	Vector2_t p;
	float reserved;
};

class BoneMatrix
{
public:
	Vector4 row[3];
	INLINE BoneMatrix() {}
	INLINE BoneMatrix(const Matrix34& m)
	{
		set(m);
	}
	INLINE void set(const Matrix34& m)
	{
		row[0].set(m.x.x, m.y.x, m.z.x, m.p.x);
		row[1].set(m.x.y, m.y.y, m.z.y, m.p.y);
		row[2].set(m.x.z, m.y.z, m.z.z, m.p.z);
	}
};

struct BatchBuffersDataEssential
{
	IIndexBuffer* ib;
	uint ibFirst;
	uint trisCount;
	boolean ibStrip;

	IVertexBuffer* vb;
	uint vbOffset;
	uint vbSize;
	EVertexFormat vbFormat;

	BatchBuffersDataEssential()
	{
		ib = 0;
		ibFirst = 0;
		trisCount = 0;
		ibStrip = false;

		vb = 0;
		vbOffset = 0;
		vbSize = 0;
		vbFormat = EVertexFormat_None;
	}
};

struct BatchBuffersData : public BatchBuffersDataEssential
{
	ListElement<BatchBuffersData> lel;

	BatchBuffersData() : lel(this)
	{
	}
};

struct MeshBatchBase : public BatchBase
{
	BoneMatrix* localBoneMatrixArray;
	BoneQuatAndPos* localBoneQuatAndPosArray;
	uint localBoneCount;

	uint layerIndex[2];
	float opacity[2];
	Matrix34 matrix;
	Matrix23 texCoordMatrix;

	MeshBatchBase(EBatchType batchType) : BatchBase(batchType)
	{
		localBoneMatrixArray = 0;
		localBoneQuatAndPosArray = 0;
		localBoneCount = 0;

		layerIndex[0] = 0;
		layerIndex[1] = 1;
		opacity[0] = 1.0f;
		opacity[1] = 1.0f;

		matrix.setIdentity();
		texCoordMatrix.setIdentity();
	}
};

struct MeshBatch : public MeshBatchBase
{
	float lightBackFacesTreshold;

	//material
	ITexture* texture[2][3];
	boolean anisotropic;
	boolean backlight;
	boolean twosided;
	boolean metal;
	Matrix33* albedoHueMatrix;
	SaturationColorCorrection* albedoSaturation;
	WaterPlaneParams* waterPlane;
	
	LightProbeBase* lightProbeBase;
	boolean disableSpecular;
	boolean brutalFastMode;

	ITextureCube* enviromentTexture;
	Vector2_t enviromentColor;
	
	ConstRange< LightProbe> lightProbes;

	ListRoot<BatchBuffersData> geometryList;

	MeshBatch() : MeshBatchBase(EBatchType_Mesh)
	{
		lightBackFacesTreshold = 0.1f;

		for(uint i=0; i < 2; ++i)
			for(uint j=0; j < 3; ++j)
				texture[i][j] = 0;
		anisotropic = false;
		backlight = false;
		twosided = false;
		metal = false;
		albedoHueMatrix = 0;
		albedoSaturation = 0;
		waterPlane = 0;

		lightProbeBase = 0;
		disableSpecular = false;
		brutalFastMode = false;

		enviromentTexture = 0;
		enviromentColor.setOne();
	}
};

struct WaterPlaneBatch : public BatchBase
{
	Matrix34 matrix;
	Matrix23 texCoordMatrix;

	ITexture* layer;
	float layerBlendCoef;
	float normalMapScale;
	float opacityScale;
	float glossines;
	float specularMask;
	float frensel;
	float lightBackFacesTreshold;
	float envCoordScale;
	Vector2 envCoordShift;
	float envCoordFromTexCorrdsScale;

	LightProbeBase* lightProbeBase;
	boolean disableSpecular;
	boolean brutalFastMode;

	ITexture* enviromentTexture;
	ITexture* foamTexture;
	float foamTexCoordScale;
	float foamBumpScale;
	Vector2_t enviromentColor;

	ListRoot<BatchBuffersData> geometryList;

	WaterPlaneBatch() : BatchBase(EBatchType_WaterPlane)
	{
		layer = 0;
		layerBlendCoef = 0.0f;
		normalMapScale = 1.0f;
		opacityScale = 1.0f;
		glossines = 0.8f;
		specularMask = 1.0f;
		frensel = 0.2f;
		lightBackFacesTreshold = 0.1f;
		envCoordScale = 0.1f;
		envCoordShift.setZero();
		envCoordFromTexCorrdsScale = 0.1f;

		lightProbeBase = 0;
		disableSpecular = true;
		brutalFastMode = false;

		enviromentTexture = 0;
		enviromentColor.setOne();
		foamTexture = 0;
		foamTexCoordScale = 5.0f;
		foamBumpScale = 0.25f;

		matrix.setIdentity();
		texCoordMatrix.setIdentity();
	}
};

struct DepthOnlyMeshBatch : public MeshBatchBase
{
	ITexture* alphaTestTexture;

	ListRoot<BatchBuffersData> geometryList;

	DepthOnlyMeshBatch() : MeshBatchBase(EBatchType_DepthOnly)
	{
		matrix.setIdentity();
		texCoordMatrix.setIdentity();
		alphaTestTexture = 0;
	}
};

struct ParticlesBatch : public BatchBase
{
	IVertexBuffer* vertxBuffer;
	uint vertxBufferOffset;
	uint quadsCount;
	EVertexFormat vertxBufferFormat;

	ITexture* texture;
	boolean dualMode;

	ParticlesBatch() : BatchBase(EBatchType_Particles)
	{
		vertxBuffer = 0;
		vertxBufferOffset = 0;
		quadsCount = 0;
		vertxBufferFormat = EVertexFormat_Particle;
		texture = 0;
		dualMode = false;
	}
};

struct PointsBatch : public BatchBase
{
	IVertexBuffer* buffer;
	uint bufferOffset;
	uint pointsCount;

	PointsBatch() : BatchBase(EBatchType_Points)
	{
		buffer = 0;
		bufferOffset = 0;
		pointsCount = 0;
	}
};

struct LinesBatch : public BatchBase
{
	IVertexBuffer* buffer;
	uint bufferOffset;
	uint linesCount;

	LinesBatch() : BatchBase(EBatchType_Lines)
	{
		buffer = 0;
		bufferOffset = 0;
		linesCount = 0;
	}
};

struct TrianglesBatch : public BatchBase
{
	IVertexBuffer* buffer;
	uint bufferOffset;
	uint trianglesCount;

	TrianglesBatch() : BatchBase(EBatchType_Triangles)
	{
		buffer = 0;
		bufferOffset = 0;
		trianglesCount = 0;
	}
};


struct InterfaceBatch : public BatchBase
{
	IVertexBuffer* buffer;
	uint bufferOffset;
	uint quadsCount;
	ITexture* texture;
	ITexture* alphaTexture;

	InterfaceBatch(EBatchType type) : BatchBase(type)
	{
		buffer = 0;
		bufferOffset = 0;
		quadsCount = 0;
		texture = 0;
		alphaTexture = 0;
	}
};

struct InterfaceQuadsBatch : public InterfaceBatch
{
	InterfaceQuadsBatch() : InterfaceBatch(EBatchType_InterfaceQuads)
	{
	}
};

enum EFontGlyphsMode
{
	EFontGlyphsMode_SingleChannel = 0,
	EFontGlyphsMode_BitCompressed,
	EFontGlyphsMode_BitCompressedMultisampled,
	EFontGlyphsMode_EnumCount
};
struct InterfaceFontGlyphsBatch : public InterfaceBatch
{
	EFontGlyphsMode mode;
	InterfaceFontGlyphsBatch() : InterfaceBatch(EBatchType_InterfaceFontGlyphs)
	{
		mode = EFontGlyphsMode_SingleChannel;
	}
};

class IDrawTaskCallback
{
public:
	
};

struct ViewParams
{
	Matrix34 place;
	float near;
	float far;
	boolean ortho;
	uint perspectiveDepthPassesCount;
	union
	{
		float perspectiveTanHalfWidth;
		float orthoHalfWidth;
	};
	
	union
	{
		float perspectiveTanHalfHeight;
		float orthoHalfHeight;
	};
	
	ViewParams()
	{
		place.x.set(1.0f, 0.0f, 0.0f);
		place.y.set(0.0f, 0.0f, 1.0f);
		place.z.set(0.0f, -1.0f, 0.0f);
		place.p.set(0.0f, 0.0f, 0.0f);
		near = 0.3f;
		far = 300.0f;
		ortho = false;
		perspectiveDepthPassesCount = 1;
		perspectiveTanHalfWidth = 1.0f;
		perspectiveTanHalfHeight = 1.0f;
	}

	Vector2_t rayFromScreenPoint(Vector2_t& outRayPos, const Vector2& screenPos01)
	{
		Vector2 sp11 = screenPos01 * 2.0f - Vector2(1.0f,1.0f);
		if(ortho)
		{
			outRayPos = place.x * (sp11.x * orthoHalfWidth) + place.y * (sp11.y * orthoHalfHeight) + place.p - place.z * near;
			return - place.z;
		}
		else
		{
			outRayPos = place.p;
			return place.m33() * Vector2_t(sp11.x * perspectiveTanHalfWidth, sp11.y * perspectiveTanHalfHeight, -1.0f);
		}
	}

	void setPerspectiveByVerticalFov(float fov, float aspect)
	{
		ortho = false;
		perspectiveTanHalfHeight = Base::tan(radiansFromDegrees(fov) * 0.5f);
		perspectiveTanHalfWidth = aspect * perspectiveTanHalfHeight;
		Assert(isFinite(perspectiveTanHalfHeight));
		Assert(isFinite(perspectiveTanHalfWidth));
	}

	void setPerspectiveByHorizontalFov(float fov, float aspect)
	{
		ortho = false;
		perspectiveTanHalfWidth = Base::tan(radiansFromDegrees(fov) * 0.5f);
		perspectiveTanHalfHeight = perspectiveTanHalfWidth / aspect;
		Assert(isFinite(perspectiveTanHalfHeight));
		Assert(isFinite(perspectiveTanHalfWidth));
	}

	void setPerspective(float fov, float width, float height)
	{
		if(width < height)
			setPerspectiveByHorizontalFov(fov, width / height);
		else
			setPerspectiveByVerticalFov(fov, width / height);
	}
};

struct DrawTaskParams
{
	//target
	IFrame* renderTarget;

	//viewport
	Vector2i viewportPosition;
	Vector2i viewportSize;
	
	//view
	ViewParams view;
	
	//shading
	boolean trilinerFiltring;
	uint anisotropicFiltring;
	float sensitivity;
	float pixelaspect;
	Vector4 clearColor;
	boolean useClearColor;
	boolean getImageAfterRenderCompleted;
	boolean computeColorStatistic;
	IDrawTaskCallback* callback;
	
	//post effects
	float depthOfFieldFocusRadiys;
	float depthOfFieldSpotPixelSizeOnTwoFocusRadiys; // angle of depth of field spot where point r = 2 * depthOfFieldFocusRadiys or r = depthOfFieldFocusRadiys / 2;
	uint glowSpotPixelSize;
	Matrix34 colorCorrectionMatrix;

	Vector2i interfaceScreenSize;
	
	//debug drawing
	Vector4 debugTriangleColor;
	Vector4 debugInvisibleTriangleColor;
	enum EDebugFillDraw
	{
		EDebugFillDraw_TexCoord = 0,
		EDebugFillDraw_Normal,
		EDebugFillDraw_Tangent,
		EDebugFillDraw_Binormal,
		EDebugFillDraw_TextureLodLevel,
		EDebugFillDraw_EnumCount
	};
	EDebugFillDraw debugFillDraw;

	float time;

	struct DebugInfo
	{
		uint textureChangesCount;
		uint shaderChangesCount;
		uint drawsCount;
		uint trianglesCount;

		DebugInfo()
		{
			clear();
		}

		void clear()
		{
			textureChangesCount = 0;
			shaderChangesCount = 0;
			drawsCount = 0;
			trianglesCount = 0;
		}
	};
	DebugInfo debugInfo;

	DrawTaskParams()
	{
		sensitivity = 1.0f;

		trilinerFiltring = false;
		anisotropicFiltring = 0;

		viewportPosition.setZero();
		viewportSize.setZero();

		time = 0;
	}
};

struct DrawTask : public DrawTaskParams, public BatchBase
{
	struct DepthPass
	{
		float far;
		Array<BatchBase*> solidBatchesSet;
		Array<BatchBase*> solidComplanarBatchesSet;
		Array<BatchBase*> solidAlphatestedBatchesSet;
		Array<BatchBase*> solidComplanarAlphatestedBatchesSet;
		Array<BatchBase*> postSolidDepthOnlyBatchesSet;
		Array<BatchBase*> blendBatchesSet;
		DepthPass()
		{
			far = 150.0f;
		}
	};
	Array<DepthPass, SystemAllocatorWrapper, 4> depthPasses;
	Array<BatchBase*> primitiveBatches;
	Array<BatchBase*> interfaceBatches;

	IAsyncReader* colorGetter;
	
	void clear()
	{
		foreach(it, depthPasses)
		{
			it->solidBatchesSet.clearSaveReserve();
			it->solidComplanarBatchesSet.clearSaveReserve();
			it->solidAlphatestedBatchesSet.clearSaveReserve();
			it->solidComplanarAlphatestedBatchesSet.clearSaveReserve();
			it->postSolidDepthOnlyBatchesSet.clearSaveReserve();
			it->blendBatchesSet.clearSaveReserve();
		}

		primitiveBatches.clearSaveReserve();
		interfaceBatches.clearSaveReserve();
	}

	DrawTask() : BatchBase(EBatchType_DrawTask)
	{
		depthPasses.resize(1);
		colorGetter = 0;
	}
};


static INLINE void cameraRotate(Matrix33& R, float horAngle, float verAngle) //R.x - horizontal
{
	float alpha = atan2(R.x.y, R.x.x) + horAngle;
	float betta = acos(-R.z.z) + verAngle;
	betta = clamp(betta, 0.0f, gcfPi);
	float ca,sa;
	cosSin(ca,sa, alpha);
	float cb,sb;
	cosSin(cb,sb, betta);
	R.x.set(ca, sa, 0.0f);
	R.z.set(sb * sa, -sb * ca, -cb);
	R.y = R.z.cross(R.x);
}


class BatchSortComparator
{
public:
	int compareBoolean(void* a, void* b)const
	{
		bool ba = a != 0;
		bool bb = b != 0;
		return ba - bb;
	}

	int operator () (const BatchBase* batchA, const BatchBase* batchB)const
	{
		int dtype = batchA->type - batchB->type;
		if(dtype)
			return dtype;
		
		if(batchA->type == EBatchType_Mesh)
		{
			const MeshBatch* a = static_cast<const MeshBatch*>(batchA);
			const MeshBatch* b = static_cast<const MeshBatch*>(batchB);

			integer d = a->geometryList.first()->vbFormat - b->geometryList.first()->vbFormat;
			if(d)
				return d;

			d = compareBoolean(a->localBoneMatrixArray, b->localBoneMatrixArray);
			if(d)
				return d;

			d = a->lightProbeBase->type - b->lightProbeBase->type;
			if(d)
				return d;

			for(uint i=0; i < 2; ++i)
				for(uint j=0; j < 3; ++j)
				{
					d = a->texture[i][j] - b->texture[i][j];
					if(d)
						return d;
				}
				
			d = a->enviromentTexture - b->enviromentTexture;
			if(d)
				return d;

			d = a->albedoHueMatrix - b->albedoHueMatrix;
			if(d)
				return d;

			d = a->albedoSaturation - b->albedoSaturation;
			if(d)
				return d;

			//hue saturation analyze
		}
		else if(batchA->type == EBatchType_WaterPlane)
		{
			const WaterPlaneBatch* a = static_cast<const WaterPlaneBatch*>(batchA);
			const WaterPlaneBatch* b = static_cast<const WaterPlaneBatch*>(batchB);

			integer d = a->geometryList.first()->vbFormat - b->geometryList.first()->vbFormat;
			if(d)
				return d;

			d = a->lightProbeBase->type - b->lightProbeBase->type;
			if(d)
				return d;

			d = a->layer - b->layer;
			if(d)
				return d;

			d = a->enviromentTexture - b->enviromentTexture;
			if(d)
				return d;
		}

		return (*(int32*)&batchA->sortDistance) - (*(int32*)&batchB->sortDistance);
	}
};

}
