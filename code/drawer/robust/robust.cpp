#include "robust.h"

namespace DrawerLib
{
	
RobustDrawer::RobustDrawer(const DisplayHandle& display) : ForwardDrawer(display)
{
	if(isValid())
		init();
}

RobustDrawer::RobustDrawer(IFrameWindow*& outMainWnd, const WindowHandle& srcWainWindowHandle, boolean fullscrean, boolean stereo)
: ForwardDrawer(outMainWnd, srcWainWindowHandle, fullscrean, stereo)
{
	if(isValid())
		init();
}

void RobustDrawer::init()
{
	particlesVS1 = new VertexShader(*this);
	particlesVS1->init(getParticlesVertexShaderCode());
	particlesPS1 = new PixelShader(*this);
	particlesPS1->init(getParticlesPixelShaderCode(false));
	particlesProg1 = new ShaderProgram(*this);
	particlesProg1->init(*particlesVS1, *particlesPS1, _CS("vertPosition"), _CS("vertDepth"), _CS("vertColor"), _CS("vertTexCoords"), _CS("vertHue"), _CS("vertLayersAndBlendAndSat"));
	particlesProg1_nFullTransformMatrix_M44 = particlesProg1->getParameterIndex(_CS("FullTransformMatrix"));
	particlesProg1_ColorScale = particlesProg1->getParameterIndex(_CS("ColorScale"));
	particlesProg1_Tex = particlesProg1->getTextureIndex(_CS("Tex"));

	particlesVS2 = new VertexShader(*this);
	particlesVS2->init(getParticlesVertexShaderCode());
	particlesPS2 = new PixelShader(*this);
	particlesPS2->init(getParticlesPixelShaderCode(true));
	particlesProg2 = new ShaderProgram(*this);
	particlesProg2->init(*particlesVS2, *particlesPS2, _CS("vertPosition"), _CS("vertDepth"), _CS("vertColor"), _CS("vertTexCoords"), _CS("vertHue"), _CS("vertLayersAndBlendAndSat"));
	particlesProg2_nFullTransformMatrix_M44 = particlesProg2->getParameterIndex(_CS("FullTransformMatrix"));
	particlesProg2_ColorScale = particlesProg2->getParameterIndex(_CS("ColorScale"));
	particlesProg2_Tex = particlesProg2->getTextureIndex(_CS("Tex"));

	/*
	MeshShaderProgramIndex params;
	params.vs.format = EVertexFormat_Mesh;
	params.vs.lightProbe = ELightProbeType_Single;
	params.vs.twoSided = false;

	params.ps.dualLayer = false;
	params.ps.envCubeTex = false;
	
	for(uint metal=0; metal < 2; metal++)
		for(uint alphaTest=0; alphaTest < 2; alphaTest++)
	for(uint specular=0; specular < 2; specular++)
		for(uint hueMatrix=0; hueMatrix < 2; hueMatrix++)
			for(uint saturation=0; saturation < 2; saturation++)
			{
				params.ps.metal = metal;
				params.ps.alphaTest = alphaTest;
				params.ps.specular = specular;
				params.ps.hueMatrix = hueMatrix;
				params.ps.saturation = saturation;
				getMeshProgram(params);
			}
			*/

	
}

TextureBase* RobustDrawer::getSpecularTexture()
{
	if(specularTexture)
		return toBaseTex(specularTexture);

	specularTexture = createTexture();
	const uint N = 128;
	StrongPtr<Image<ColorL> > img = new Image<ColorL>(N,N);
	for(uint j=0; j < N; ++j)
		for(uint i=0; i < N; ++i)
		{
			float NdotH = float(i) / float(N-1);
			float gloss = float(j) / float(N-1);

			float cosAlpha2 = NdotH + 0.001f;
			cosAlpha2 *= cosAlpha2;
			float rXc = cosAlpha2 * exp2(-9.0f * gloss);
			float spec = 0.25f * exp( (cosAlpha2 - 1.0f) / rXc ) / (rXc * cosAlpha2);

			img->at(i,j) = clampt<int>(int(sqrt(spec) * 255.0f / 2.0f + 0.5f), 0, 255);
		}
	specularTexture->init(*img);
	return toBaseTex(specularTexture);
}

RobustDrawer::~RobustDrawer()
{
	meshShaderPrograms.clear();
	waterPlaneShaderPrograms.clear();
	meshPixelShaders.clear();
	meshVertexShaders.clear();
	waterPlanePixelShaders.clear();
}

void RobustDrawer::draw(Range<DrawTask*> parallelTasks)
{
	foreach(it, parallelTasks)
		if(*it)
			draw(**it);
}

const RobustDrawer::MeshShaderProgramAndParameters& RobustDrawer::getMeshProgram(MeshShaderProgramIndex params)
{
	boolean bNew;
	auto it = meshShaderPrograms.insert(bNew, params);
	if(!bNew)
		return *it->val;

	it->val = new MeshShaderProgramAndParameters();

	boolean bNewPS;
	auto psIt = meshPixelShaders.insert(bNewPS, params.ps);
	PixelShader* ps;
	if(bNewPS){
		ps = new PixelShader(*this);
		String code = getMeshPixelShaderCode(params.ps);
		ps->init(code);
		psIt->val = ps;
	}
	else
		ps = psIt->val;

	boolean bNewVS;
	auto vsIt = meshVertexShaders.insert(bNewVS, params.vs);
	VertexShader* vs;
	if(bNewVS){
		vs = new VertexShader(*this);
		String code = getMeshVertexShaderCode(params.vs);
		vs->init(code);
		vsIt->val = vs;
	}
	else
		vs = vsIt->val;

	ShaderProgram* prog = new ShaderProgram(*this);
	Array<String> args = getVertexShaderAttributesCode((EVertexFormat)params.vs.format);
	Array<ConstString> arr;
	foreach(it, args)
		arr.push(*it);
	prog->init(*vs, *ps, arr);
	it->val->init(prog);
	return *it->val;
}

const RobustDrawer::WaterPlaneShaderProgramAndParameters& RobustDrawer::getWaterPlaneProgram(WaterPlaneShaderProgramIndex params)
{
	boolean bNew;
	auto it = waterPlaneShaderPrograms.insert(bNew, params);
	if(!bNew)
		return *it->val;

	it->val = new WaterPlaneShaderProgramAndParameters();

	boolean bNewPS;
	auto psIt = waterPlanePixelShaders.insert(bNewPS, params.ps);
	PixelShader* ps;
	if(bNewPS){
		ps = new PixelShader(*this);
		String code = getWaterPlanePixelShaderCode(params.ps);
		ps->init(code);
		psIt->val = ps;
	}
	else
		ps = psIt->val;

	boolean bNewVS;
	auto vsIt = meshVertexShaders.insert(bNewVS, params.vs);
	VertexShader* vs;
	if(bNewVS){
		vs = new VertexShader(*this);
		String code = getMeshVertexShaderCode(params.vs);
		vs->init(code);
		vsIt->val = vs;
	}
	else
		vs = vsIt->val;

	ShaderProgram* prog = new ShaderProgram(*this);
	Array<String> args = getVertexShaderAttributesCode((EVertexFormat)params.vs.format);
	Array<ConstString> arr;
	foreach(it, args)
		arr.push(*it);
	prog->init(*vs, *ps, arr);
	it->val->init(prog);
	return *it->val;
}

const RobustDrawer::DepthOnlyMeshShaderProgramAndParameters& RobustDrawer::getDepthOnlyMeshProgram(DepthOnlyMeshShaderProgramIndex params)
{
	boolean bNew;
	auto it = depthOnlyMeshShaderPrograms.insert(bNew, params);
	if(!bNew)
		return *it->val;

	it->val = new DepthOnlyMeshShaderProgramAndParameters();

	boolean bNewPS;
	auto psIt = depthOnlyMeshPixelShaders.insert(bNewPS, params.ps);
	PixelShader* ps;
	if(bNewPS){
		ps = new PixelShader(*this);
		String code = getDepthOnlyMeshPixelShaderCode(params.ps);
		ps->init(code);
		psIt->val = ps;
	}
	else
		ps = psIt->val;

	boolean bNewVS;
	auto vsIt = depthOnlyMeshVertexShaders.insert(bNewVS, params.vs);
	VertexShader* vs;
	if(bNewVS){
		vs = new VertexShader(*this);
		String code = getDepthOnlyMeshVertexShaderCode(params.vs);
		vs->init(code);
		vsIt->val = vs;
	}
	else
		vs = vsIt->val;

	ShaderProgram* prog = new ShaderProgram(*this);
	Array<String> args = getVertexShaderAttributesCode((EVertexFormat)params.vs.format);
	Array<ConstString> arr;
	foreach(it, args)
		arr.push(*it);
	prog->init(*vs, *ps, arr);
	it->val->init(prog);
	return *it->val;
}


void RobustDrawer::VertexShaderAndParameters::init(ShaderProgram* prog)
{
	GET_SHADER_PARAMETER_INDEX(prog, FullTransformMatrix);
	GET_SHADER_PARAMETER_INDEX(prog, TexCoordMatrix);
	GET_SHADER_PARAMETER_INDEX(prog, ToEyeVectorMatrix);
	GET_SHADER_PARAMETER_INDEX(prog, TangentBinormalMatrix);
	GET_SHADER_PARAMETER_INDEX(prog, NormalMatrix);
	GET_SHADER_PARAMETER_INDEX(prog, LightProbeStar7_coordMAdd);
	GET_SHADER_PARAMETER_INDEX(prog, LightProbeStar7_probes);
	GET_SHADER_PARAMETER_INDEX(prog, LightProbeOcto_coordMAdd);
	GET_SHADER_PARAMETER_INDEX(prog, LightProbeOcto_probes);
	GET_SHADER_PARAMETER_INDEX(prog, LightProbeSingle_probe);
	GET_SHADER_PARAMETER_INDEX(prog, NoLightBackFacesMultCoef);
	GET_SHADER_PARAMETER_INDEX(prog, NoLightBackFacesAddCoef);
	GET_SHADER_PARAMETER_INDEX(prog, BoneArray);
	GET_SHADER_PARAMETER_INDEX(prog, LightProbeArray);
	GET_SHADER_PARAMETER_INDEX(prog, OpacityCoefs);
	GET_SHADER_PARAMETER_INDEX(prog, Opacity0Coef);
	GET_SHADER_PARAMETER_INDEX(prog, Opacity1Coef);
	GET_SHADER_PARAMETER_INDEX(prog, CausticTexCoordMatrix);
	GET_SHADER_PARAMETER_INDEX(prog, CausticPlane);
	GET_SHADER_PARAMETER_INDEX(prog, CausticLayer7Scale);
	GET_SHADER_PARAMETER_INDEX(prog, CausticFadeBack);

	GET_SHADER_PARAMETER_INDEX(prog, ToLightDirection);
	GET_SHADER_PARAMETER_INDEX(prog, LightIntensivity);
	GET_SHADER_PARAMETER_INDEX(prog, AmbientIntensivity);
	GET_SHADER_PARAMETER_INDEX(prog, IntensivityMultiplyer);
	GET_SHADER_PARAMETER_INDEX(prog, ReflectionPalneCoordOffsetAndScale);
}

void RobustDrawer::MeshShaderProgramAndParameters::init(ShaderProgram* prog)
{
	program = prog;
	VertexShaderAndParameters::init(prog);

	GET_SHADER_PARAMETER_INDEX(prog, EnvColor);
	GET_SHADER_PARAMETER_INDEX(prog, HueMatrix);
	GET_SHADER_PARAMETER_INDEX(prog, SaturationCoefs);
	GET_SHADER_PARAMETER_INDEX(prog, CausticScale);

	Tex0 = prog->getTextureIndex(_CS("Tex0"));
	Tex1 = prog->getTextureIndex(_CS("Tex1"));
	Tex2 = prog->getTextureIndex(_CS("Tex2"));
	Tex02 = prog->getTextureIndex(_CS("Tex02"));
	Tex12 = prog->getTextureIndex(_CS("Tex12"));
	Tex22 = prog->getTextureIndex(_CS("Tex22"));
	EnvTex = prog->getTextureIndex(_CS("EnvTex"));
	CausticTex = prog->getTextureIndex(_CS("CausticTex"));
	SpecularFuncTex = prog->getTextureIndex(_CS("SpecularFuncTex"));
}

void RobustDrawer::WaterPlaneShaderProgramAndParameters::init(ShaderProgram* prog)
{
	program = prog;
	VertexShaderAndParameters::init(prog);

	GET_SHADER_PARAMETER_INDEX(prog, Opacity0Coef);
	GET_SHADER_PARAMETER_INDEX(prog, Opacity1Coef);
	GET_SHADER_PARAMETER_INDEX(prog, NormalScale);
	GET_SHADER_PARAMETER_INDEX(prog, SpecularIntensivity);
	GET_SHADER_PARAMETER_INDEX(prog, EnvIntensivity);
	GET_SHADER_PARAMETER_INDEX(prog, FrenselBase);
	GET_SHADER_PARAMETER_INDEX(prog, RoughtnessSqr);
	GET_SHADER_PARAMETER_INDEX(prog, Glossines);
	GET_SHADER_PARAMETER_INDEX(prog, FoamTexScale);
	GET_SHADER_PARAMETER_INDEX(prog, FoamBumpScale);

	Tex = prog->getTextureIndex(_CS("Tex"));
	EnvTex = prog->getTextureIndex(_CS("EnvTex"));
	FoamTex = prog->getTextureIndex(_CS("FoamTex"));
	SpecularFuncTex = prog->getTextureIndex(_CS("SpecularFuncTex"));
}

void RobustDrawer::DepthOnlyMeshShaderProgramAndParameters::init(ShaderProgram* prog)
{
	program = prog;
	GET_SHADER_PARAMETER_INDEX(prog, FullTransformMatrix);
	GET_SHADER_PARAMETER_INDEX(prog, TexCoordMatrix);
	GET_SHADER_PARAMETER_INDEX(prog, BoneArray);
	Tex = prog->getTextureIndex(_CS("Tex"));
}

}
