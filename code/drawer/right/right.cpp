#include "right.h"

namespace DrawerLib
{
	
RightDrawer::RightDrawer(const DisplayHandle& display) : ForwardDrawer(display)
{
	if(isValid())
		init();
}

RightDrawer::RightDrawer(IFrameWindow*& outMainWnd, const WindowHandle& srcWainWindowHandle, boolean fullscrean, boolean stereo)
: ForwardDrawer(outMainWnd, srcWainWindowHandle, fullscrean, stereo)
{
	if(isValid())
		init();
}

void RightDrawer::init()
{
}

RightDrawer::~RightDrawer()
{
	meshShaderPrograms.clear();
	waterPlaneShaderPrograms.clear();
	meshPixelShaders.clear();
	meshVertexShaders.clear();
	waterPlanePixelShaders.clear();
}

void RightDrawer::draw(Range<DrawTask*> parallelTasks)
{
	foreach(it, parallelTasks)
		if(*it)
			draw(**it);
}

const RightDrawer::MeshShaderProgramAndParameters& RightDrawer::getMeshProgram(MeshShaderProgramIndex params)
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
		arr.push(it->toConst());
	prog->init(*vs, *ps, arr.constRange());
	it->val->init(prog);
	return *it->val;
}

const RightDrawer::WaterPlaneShaderProgramAndParameters& RightDrawer::getWaterPlaneProgram(WaterPlaneShaderProgramIndex params)
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
		arr.push(it->toConst());
	prog->init(*vs, *ps, arr.constRange());
	it->val->init(prog);
	return *it->val;
}


void RightDrawer::VertexShaderAndParameters::init(ShaderProgram* prog)
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

	GET_SHADER_PARAMETER_INDEX(prog, ToLightDirection);
	GET_SHADER_PARAMETER_INDEX(prog, LightColor);
	GET_SHADER_PARAMETER_INDEX(prog, AmbientColor);
}

void RightDrawer::MeshShaderProgramAndParameters::init(ShaderProgram* prog)
{
	program = prog;
	VertexShaderAndParameters::init(prog);

	GET_SHADER_PARAMETER_INDEX(prog, EnvColor);
	GET_SHADER_PARAMETER_INDEX(prog, NormalMapScale);
	GET_SHADER_PARAMETER_INDEX(prog, OpacityScale);
	GET_SHADER_PARAMETER_INDEX(prog, GlossinesScale);
	GET_SHADER_PARAMETER_INDEX(prog, SpecularMaskScale);
	GET_SHADER_PARAMETER_INDEX(prog, GlossinesDeltha);
	GET_SHADER_PARAMETER_INDEX(prog, SpecularMaskDeltha);
	GET_SHADER_PARAMETER_INDEX(prog, Frensel);
	GET_SHADER_PARAMETER_INDEX(prog, Metal);
	GET_SHADER_PARAMETER_INDEX(prog, ColorMatrix);
	GET_SHADER_PARAMETER_INDEX(prog, NormalMapScale2);
	GET_SHADER_PARAMETER_INDEX(prog, OpacityScale2);
	GET_SHADER_PARAMETER_INDEX(prog, GlossinesScale2);
	GET_SHADER_PARAMETER_INDEX(prog, SpecularMaskScale2);
	GET_SHADER_PARAMETER_INDEX(prog, GlossinesDeltha2);
	GET_SHADER_PARAMETER_INDEX(prog, SpecularMaskDeltha2);
	GET_SHADER_PARAMETER_INDEX(prog, Frensel2);
	GET_SHADER_PARAMETER_INDEX(prog, Metal2);
	GET_SHADER_PARAMETER_INDEX(prog, ColorMatrix2);
	GET_SHADER_PARAMETER_INDEX(prog, ColorMultiplyer);
	GET_SHADER_PARAMETER_INDEX(prog, RoughtnessSqrExp2Coef);
	GET_SHADER_PARAMETER_INDEX(prog, BacklightMultCoef);
	GET_SHADER_PARAMETER_INDEX(prog, BacklightAddCoef);

	Tex0 = prog->getTextureIndex(_CS("Tex0"));
	Tex1 = prog->getTextureIndex(_CS("Tex1"));
	Tex02 = prog->getTextureIndex(_CS("Tex02"));
	Tex12 = prog->getTextureIndex(_CS("Tex12"));
	EnvTex = prog->getTextureIndex(_CS("EnvTex"));
}

void RightDrawer::WaterPlaneShaderProgramAndParameters::init(ShaderProgram* prog)
{
	program = prog;
	VertexShaderAndParameters::init(prog);

	GET_SHADER_PARAMETER_INDEX(prog, EnvColor);
	GET_SHADER_PARAMETER_INDEX(prog, BlendCoefs);
	GET_SHADER_PARAMETER_INDEX(prog, OpacityScale);
	GET_SHADER_PARAMETER_INDEX(prog, Albedo);
	GET_SHADER_PARAMETER_INDEX(prog, SpecularMask);
	GET_SHADER_PARAMETER_INDEX(prog, Frensel);
	GET_SHADER_PARAMETER_INDEX(prog, RoughtnessSqr);
	GET_SHADER_PARAMETER_INDEX(prog, EnvCoordScale);
	GET_SHADER_PARAMETER_INDEX(prog, EnvTexCoordScale);
	GET_SHADER_PARAMETER_INDEX(prog, ColorMultiplyer);

	Tex = prog->getTextureIndex(_CS("Tex"));
	EnvTex = prog->getTextureIndex(_CS("EnvTex"));
}

}
