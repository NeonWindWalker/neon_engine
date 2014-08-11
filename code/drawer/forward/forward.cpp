#include "forward.h"

namespace DrawerLib
{
	
ForwardDrawer::ForwardDrawer(const DisplayHandle& display) : HALDevice(display)
{
	if(isValid())
		init();
}

ForwardDrawer::ForwardDrawer(IFrameWindow*& outMainWnd, const WindowHandle& srcWainWindowHandle, boolean fullscrean, boolean stereo)
: HALDevice(outMainWnd, srcWainWindowHandle, fullscrean, stereo)
{
	if(isValid())
		init();
}

void ForwardDrawer::init()
{
	logInfo << "Compiling Shaders";

	context->blendXAlpha();

	simplePrimitivesVS = new VertexShader(*this);
	simplePrimitivesVS->init(getSimplePrimitivesVertexShaderCode());
	simplePrimitivesPS = new PixelShader(*this);
	simplePrimitivesPS->init(getSimplePrimitivesPixelShaderCode());
	simplePrimitivesProg = new ShaderProgram(*this);
	simplePrimitivesProg->init(*simplePrimitivesVS, *simplePrimitivesPS, _CS("vertPosition"), _CS("vertColor"));
#ifdef __ANDROID__
	simplePrimitivesProg->init(*simplePrimitivesVS, *simplePrimitivesPS, _CS("vertPosition"), _CS("vertColor")); //facking driver
#endif
	simplePrimitivesProg_nFullTransformMatrix_M44 = simplePrimitivesProg->getParameterIndex(_CS("FullTransformMatrix"));


	interfaceQuadsVS = new VertexShader(*this);
	interfaceQuadsVS->init(getInterfaceQuadsVertexShaderCode());
	interfaceQuadsPS = new PixelShader(*this);
	interfaceQuadsPS->init(getInterfaceQuadsPixelShaderCode(false));
	interfaceQuadsProg = new ShaderProgram(*this);
	interfaceQuadsProg->init(*interfaceQuadsVS, *interfaceQuadsPS, _CS("vertPosition"), _CS("vertTexCoord"), _CS("vertColor"));
	interfaceQuadsProg_nFullTransformMatrix_M23 = interfaceQuadsProg->getParameterIndex(_CS("FullTransformMatrix"));
	interfaceQuadsProg_Tex = interfaceQuadsProg->getTextureIndex(_CS("Tex"));
	interfaceQuadsPSA = new PixelShader(*this);
	interfaceQuadsPSA->init(getInterfaceQuadsPixelShaderCode(true));
	interfaceQuadsProgA = new ShaderProgram(*this);
	interfaceQuadsProgA->init(*interfaceQuadsVS, *interfaceQuadsPSA, _CS("vertPosition"), _CS("vertTexCoord"), _CS("vertColor"));
	interfaceQuadsProgA_nFullTransformMatrix_M23 = interfaceQuadsProgA->getParameterIndex(_CS("FullTransformMatrix"));
	interfaceQuadsProgA_Tex = interfaceQuadsProgA->getTextureIndex(_CS("Tex"));
	interfaceQuadsProgA_AlphaTex = interfaceQuadsProgA->getTextureIndex(_CS("AlphaTex"));

	for(uint i=0; i < EFontGlyphsMode_EnumCount; ++i)
	{
		interfaceFontVS[i] = new VertexShader(*this);
		interfaceFontVS[i]->init(getInterfaceFontVertexShaderCode((EFontGlyphsMode)i));
		interfaceFontPS[i] = new PixelShader(*this);
		interfaceFontPS[i]->init(getInterfaceFontPixelShaderCode((EFontGlyphsMode)i));
		interfaceFontProg[i] = new ShaderProgram(*this);
		interfaceFontProg[i]->init(*interfaceFontVS[i], *interfaceFontPS[i], _CS("vertPosition"), _CS("vertTexCoord"), _CS("vertColor"), _CS("vertByteBitLen"));
		interfaceFontProg_nFullTransformMatrix_M23[i] = interfaceFontProg[i]->getParameterIndex(_CS("FullTransformMatrix"));
		interfaceFontProg_Tex[i] = interfaceFontProg[i]->getTextureIndex(_CS("Tex"));
	}

	interfaceFontProg_DTexCrd = interfaceFontProg[EFontGlyphsMode_BitCompressedMultisampled]->getParameterIndex(_CS("DTexCrd"));
	

	logInfo << "Compiling Shaders Complite";
}

ForwardDrawer::~ForwardDrawer()
{
	delete interfaceQuadsProg;
	delete interfaceQuadsPS;
	delete interfaceQuadsVS;

	delete simplePrimitivesProg;
	delete simplePrimitivesPS;
	delete simplePrimitivesVS;
}


String ForwardDrawer::getCommonVertexShadersCode()
{
#ifdef DRAWER_API_OPENGL
	const char* str =
		"#define highp \n"
		"#define mediump \n"
		"#define lowp \n";
	return constString(str);
#else
	const char* str = "";
	return constString(str);
#endif
}

String ForwardDrawer::getCommonPixelShadersCode()
{
#ifdef DRAWER_API_OPENGL
	const char* str =
		"#define highp \n"
		"#define mediump \n"
		"#define lowp \n";
	return constString(str);
#else
	const char* str = "";
	return constString(str);
#endif
}


String ForwardDrawer::getCommonFunctionsCode()
{
	const char* str = 
	""
	"mediump vec3 unpuckNormal(mediump vec2 src)\n"
	"{\n"
	"	mediump vec3 n;"
	"	n.x = abs(src.x) * 2.0 - 1.0;\n"
	"	n.y = src.y;\n"
	"	n.z = sign(src.x) * sqrt(1.0 -  dot(n.xy,n.xy));\n"
	"	return n;\n"
	"}\n"
	""
	"mediump vec3 unpackPackedNormalizedColorF32(highp float src)\n"
	"{\n"
	"	const highp float scale = 0.0009765625;\n"
	"	highp float r = 2.0 * fract(src);\n"
	"	highp float g = floor(src) * scale;\n"
	"	return vec3(r,g,1.0 - r - g);\n"
	"}\n"
	""
	"mediump vec3 unpackPackedNormalizedColorS16(highp float src)\n"
	"{\n"
	"	const highp float scaleR = 0.00787401574803;\n"
	"	const highp float scaleG1 = 127.99609375;\n"
	"	const highp float scaleG2 = 0.00787401574803;\n"
	"	highp float r = 2.0 * fract(src * scaleR);\n"
	"	highp float g = floor(src * scaleG1) * scaleG2;\n"
	"	return vec3(r,g,1.0 - r - g);\n"
	"}\n"
	"\n"
	"mediump float beckmanSpecularFunction(mediump float HdotNPositive, mediump float roughtnessSqr)\n"
	"{\n"
	"	const mediump float constant_epsilon = 0.001;\n"
	"	const mediump float constant_X = 0.25;\n"
	"	mediump float cosAlpha2 = HdotNPositive + constant_epsilon;\n"
	"	cosAlpha2 *= cosAlpha2;\n"
	"	mediump float rXc = cosAlpha2 * roughtnessSqr;\n"
	"	return constant_X * exp( (cosAlpha2 - 1.0) / rXc ) / (rXc * cosAlpha2);\n"
	"}\n"
	"\n"
	"mediump float roughtnessSqrFromGlossines(mediump float glossines)\n"
	"{\n"
	"	const mediump float constant_X = -9.0;\n"
	"	return exp2(constant_X * glossines);\n"
	"}\n"
	"\n"
	"mediump vec3 hueConversion(mediump vec4 HueMatrix[3], mediump vec3 albedo)\n"
	"{\n"
	"	mediump float albedoMin = min(min(albedo.r, albedo.g), albedo.b);\n"
	"	mediump vec3 albedoSat = albedo - albedoMin;\n"
	"	mediump vec3 hued = HueMatrix[0].rgb * albedoSat.r + HueMatrix[1].rgb * albedoSat.g + HueMatrix[2].rgb * albedoSat.b;\n"
	"	albedoSat = hued - min(min(hued.r, hued.g), hued.b);\n"
	"	return albedoSat + albedoMin;\n"
	"}\n"
	"mediump vec3 sqrtApproximationForColor(mediump vec3 S)\n"
	"{\n"
	"	return vec3(sqrt(S.x), sqrt(S.y), sqrt(S.z));\n"
	"}\n"
	"";

	
	/*

	"	mediump vec3 g025 = vec3(0.25,0.25,0.25);\n"
	"	mediump vec3 g333 = vec3(3.0,3.0,3.0);\n"
	"	mediump vec3 a = S;\n"
	"	mediump vec3 c = S - 1.0;\n"
	"	a -= a * c * 0.5;\n"
	"	c = c * c * (c - g333) * g025;\n"
	"	a -= a * c * 0.5;\n"
	"	c = c * c * (c - g333) * g025;\n"
	"	a -= a * c * 0.5;\n"
	"	c = c * c * (c - g333) * g025;\n"
	"	a -= a * c * 0.5;\n"
	"	c = c * c * (c - g333) * g025;\n"
	"	a -= a * c * 0.5;\n"
	"	c = c * c * (c - g333) * g025;\n"
	"	return abs(a - vec3(sqrt(S.x), sqrt(S.y), sqrt(S.z))) * 1.0f;\n"

	double dAngle = 0.001f;

	for(double rrr=0; rrr <= 1.0001; rrr += 0.1f)
	{
		float intens = 0;
		uint count = 0;
		float geom = 0;

		double roughtnessSqr = exp2(-10.0 * rrr);
		for(double tetha = dAngle; tetha < gcPi; tetha += dAngle)
		{
			double dAlphaAngle = dAngle * gcPi / tetha;
			for(double fi = 0; fi < gcTwoPi; fi += dAlphaAngle)
			{
				count ++;
				double V[3] = {cos(fi) * sin(tetha), sin(fi) * sin(tetha), cos(tetha)};
				double H[3] = {cos(fi) * sin(tetha * 0.5), sin(fi) * sin(tetha * 0.5), cos(tetha * 0.5)};

				double HdotN = H[2];
				double VdotN = V[2];
				double VdotH = V[0] * H[0] + V[1] * H[1] + V[2] * H[2];
				double LdotN = 1.0;

				double cosAlpha2 = HdotN * HdotN;
				double rXc = cosAlpha2 * roughtnessSqr;
				double D = 2.0 * exp( (cosAlpha2 - 1.0) / rXc ) / (rXc * cosAlpha2);

				double G = min(1.0, 2.0 * HdotN * VdotN / VdotH,  2.0 * HdotN * LdotN / VdotH);

				float K = G * D / (VdotN * LdotN);

				intens += D;
				geom += G / (VdotN * LdotN);
			}
		}

		intens /= count;
		geom /= count;
		logInfo << "gloss " << rrr << " coef " << intens << " geom " << geom;
	}
	*/
	


	
	/*
	 * 	"vec2 mat23_mult_v2(vec4 m[3], vec2 v)\n"
	"{\n"
	"	return m[0].xy * v.x + m[1].xy * v.y + m[2].xy;\n"
	"}\n"
	"vec3 mat33_mult_v3(vec4 m[3], vec3 v)\n"
	"{\n"
	"	return m[0].xyz * v.x + m[1].xyz * v.y + m[2].xyz * v.z;\n"
	"}\n"
	"vec3 mat34_mult_v3(vec4 m[4], vec3 v)\n"
	"{\n"
	"	return m[0].xyz * v.x + m[1].xyz * v.y + m[2].xyz * v.z + m[3].xyz;\n"
	"}\n"
	"vec4 mat44_mult_v3(vec4 m[4], vec3 v)\n"
	"{\n"
	"	return m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3];\n"
	"}\n"
	 * */
	return constString(str);
}


String ForwardDrawer::getSimplePrimitivesVertexShaderCode()
{
	const char* str = 
		"uniform highp vec4 FullTransformMatrix[4];\n"
		"attribute highp vec3 vertPosition;\n"
		"attribute highp vec4 vertColor;\n"
		"varying highp vec4 vrColor;\n"
		""
		"void main()\n"
		"{\n"
		"	gl_Position = FullTransformMatrix[0] * vertPosition.x + FullTransformMatrix[1] * vertPosition.y + FullTransformMatrix[2] * vertPosition.z + FullTransformMatrix[3];\n"
		"	vrColor = vertColor;\n"
		"}\n";
	String s = getCommonVertexShadersCode();
	s << constString(str);
	return s;
}

String ForwardDrawer::getSimplePrimitivesPixelShaderCode()
{
	const char* str = 
		"varying mediump vec4 vrColor;\n"
		""
		"void main()\n"
		"{\n"
		"	gl_FragColor = vrColor;\n"
		"}\n";
	String s = getCommonPixelShadersCode();
	s << constString(str);
	return s;
}


String ForwardDrawer::getInterfaceQuadsVertexShaderCode()
{
	const char* str = 
		"uniform highp vec4 FullTransformMatrix[3];\n"
		"attribute highp vec2 vertPosition;\n"
		"attribute highp vec2 vertTexCoord;\n"
		"attribute mediump vec4 vertColor;\n"
		"varying highp vec2 vrTexCoord;\n"
		"varying mediump vec4 vrColor;\n"
		"void main()\n"
		"{\n"
		"	highp vec2 p = FullTransformMatrix[0].xy * vertPosition.x + FullTransformMatrix[1].xy * vertPosition.y + FullTransformMatrix[2].xy;"
		"	gl_Position = vec4(p, 0.5, 1.0);\n"
		"	vrTexCoord = vertTexCoord;\n"
		"	vrColor = vertColor;\n"
		"}\n";
	String s = getCommonVertexShadersCode();
	s << constString(str);
	return s;
}

String ForwardDrawer::getInterfaceQuadsPixelShaderCode(boolean alphaTex)
{
	const char* str = 
	"varying highp vec2 vrTexCoord;\n"
	"varying mediump vec4 vrColor;\n"
	"uniform sampler2D Tex;\n"
	"#ifdef ALPHA_TEX\n"
	"	uniform sampler2D AlphaTex;\n"
	"#endif\n"
	"void main()\n"
	"{\n"
	"	mediump vec4 c;\n"
	"#ifdef ALPHA_TEX\n"
	"	c.rgb = texture2D(Tex, vrTexCoord).rgb * vrColor.rgb;\n"
	"	c.a = texture2D(AlphaTex, vrTexCoord).r * vrColor.a;\n"
	"#else\n"
	"	c = texture2D(Tex, vrTexCoord) * vrColor;\n"
	"#endif\n"
	"	gl_FragColor.rgb = c.rgb * c.a;\n"
	"	gl_FragColor.a = c.a;\n"
	"}\n";
	String s;
	if(alphaTex)
		s << _CS("#define ALPHA_TEX\n");
	s << getCommonPixelShadersCode();
	s << constString(str);
	return s;
}


String ForwardDrawer::getInterfaceFontVertexShaderCode(EFontGlyphsMode mode)
{
	const char* str = 
	"uniform highp vec4 FullTransformMatrix[3];\n"
	"attribute highp vec2 vertPosition;\n"
	"attribute highp vec2 vertTexCoord;\n"
	"attribute mediump vec4 vertColor;\n"
	"attribute highp vec4 vertByteBitLen;\n"
	"varying highp vec2 vrTexCoord;\n"
	"varying mediump vec4 vrColor;\n"
	"varying highp vec4 vrDot4Coef;\n"
	"#ifdef BIT_COMPRESSED\n"
	"	varying highp float vrMultFloorCoef;\n"
	"	varying highp float vrMultFractCoef;\n"
	"	varying highp float vrMultLastCoef;\n"
	"#endif\n"
	"void main()\n"
	"{\n"
	"	highp vec2 p = FullTransformMatrix[0].xy * vertPosition.x + FullTransformMatrix[1].xy * vertPosition.y + FullTransformMatrix[2].xy;"
	"	gl_Position = vec4(p, 0.5, 1.0);\n"
	"	vrTexCoord = vertTexCoord;\n"
	"	vrColor = vertColor;\n"
	"	const mediump float g3 = 3.0;\n"
	"	const mediump float g4 = 4.0;\n"
	"	const highp float g8 = 8.0;\n"
	"	const highp float g255 = 255.0;\n"
	"	vrDot4Coef.x = clamp(1.0 - vertByteBitLen.x, 0.0, 1.0);\n"
	"	vrDot4Coef.y = clamp((2.0 - vertByteBitLen.x) * vertByteBitLen.x, 0.0, 1.0);\n"
	"	vrDot4Coef.z = clamp((g3 - vertByteBitLen.x) * (vertByteBitLen.x - 1.0), 0.0, 1.0);\n"
	"	vrDot4Coef.w = clamp((g4 - vertByteBitLen.x) * (vertByteBitLen.x - 2.0), 0.0, 1.0);\n"
	"#ifdef BIT_COMPRESSED\n"
	"		vrMultFloorCoef = g255 * exp2(- vertByteBitLen.y);\n"
	"		vrMultFractCoef = exp2(-vertByteBitLen.z);\n"
	"		vrMultLastCoef = 1.0 / (1.0 - vrMultFractCoef);\n"
	"#endif\n"
	"}\n";
	String s = getCommonVertexShadersCode();
	if(mode != EFontGlyphsMode_SingleChannel)
		s << _CS("#define BIT_COMPRESSED\n");

	s << constString(str);
	return s;
}

String ForwardDrawer::getInterfaceFontPixelShaderCode(EFontGlyphsMode mode)
{
	const char* str = 
	"varying highp vec2 vrTexCoord;\n"
	"varying mediump vec4 vrColor;\n"
	"varying highp vec4 vrDot4Coef;\n"
	"#ifdef BIT_COMPRESSED\n"
	"	varying highp float vrMultFloorCoef;\n"
	"	varying highp float vrMultFractCoef;\n"
	"	varying highp float vrMultLastCoef;\n"
	"	highp float bitUnpuck(highp float inputByte01)\n"
	"	{\n"
	"		const highp float gPrec = 0.002;\n"
	"		return fract(floor((inputByte01 + gPrec) * vrMultFloorCoef) * vrMultFractCoef) * vrMultLastCoef;\n"
	"	}\n"
	"#endif\n"
	"#ifdef MULTISAMPLED\n"
	"	uniform highp vec2 DTexCrd;\n"
	"#endif\n"
	"uniform sampler2D Tex;\n"
	"void main()\n"
	"{\n"
	"#ifdef BIT_COMPRESSED\n"
	"	highp float c = bitUnpuck(dot(texture2D(Tex, vrTexCoord), vrDot4Coef));\n"
	"	#ifdef MULTISAMPLED\n"
	"		highp float c2 = \n"
	"			 bitUnpuck(dot(texture2D(Tex, vrTexCoord + vec2(DTexCrd.x, 0.0)), vrDot4Coef));\n"
	"		c = max(c, c2);\n"
	"		c2 = bitUnpuck(dot(texture2D(Tex, vrTexCoord + vec2(0.0, DTexCrd.y)), vrDot4Coef));\n"
	"		c = max(c, c2);\n"
	"		c2 = bitUnpuck(dot(texture2D(Tex, vrTexCoord + vec2(-DTexCrd.x, 0.0)), vrDot4Coef));\n"
	"		c = max(c, c2);\n"
	"		c2 = bitUnpuck(dot(texture2D(Tex, vrTexCoord + vec2(0.0, -DTexCrd.y)), vrDot4Coef));\n"
	"		c = max(c, c2);\n"
	"		//c2 = fract(floor(dot(texture2D(Tex, vrTexCoord + vec2(DTexCrd.x, 0.0) * 2.0), vrDot4Coef) * vrMultFloorCoef + 0.5) * vrMultFractCoef) * vrMultLastCoef;\n"
	"		//c2 += fract(floor(dot(texture2D(Tex, vrTexCoord + vec2(0.0, DTexCrd.y) * 2.0), vrDot4Coef) * vrMultFloorCoef + 0.5) * vrMultFractCoef) * vrMultLastCoef;\n"
	"		//c2 += fract(floor(dot(texture2D(Tex, vrTexCoord + vec2(-DTexCrd.x, 0.0) * 2.0), vrDot4Coef) * vrMultFloorCoef + 0.5) * vrMultFractCoef) * vrMultLastCoef;\n"
	"		//c2 += fract(floor(dot(texture2D(Tex, vrTexCoord + vec2(0.0, -DTexCrd.y) * 2.0), vrDot4Coef) * vrMultFloorCoef + 0.5) * vrMultFractCoef) * vrMultLastCoef;\n"
	"		//const mediump float g025 = 0.25;\n"
	"		//c2*= g025;\n"
	"		//c = max(c, c2);\n"
	"	#endif\n"
	"#else\n"
	"	highp float c = dot(texture2D(Tex, vrTexCoord), vrDot4Coef);\n"
	"#endif\n"
	"	gl_FragColor.rgb = vrColor.rgb * vrColor.a * c;\n"
	"	gl_FragColor.a = vrColor.a * c;\n"
	"}\n";
	String s = getCommonPixelShadersCode();
	if(mode != EFontGlyphsMode_SingleChannel)
		s << _CS("#define BIT_COMPRESSED\n");
	if(mode == EFontGlyphsMode_BitCompressedMultisampled)
		s << _CS("#define MULTISAMPLED\n");
	s << constString(str);
	return s;
}

Array<String> ForwardDrawer::getVertexShaderAttributesCode(EVertexFormat fmt)
{
	Array<String> attr;

	if(fmt == EVertexFormat_HeightMap)
	{
		attr.push(String(_CS("vertPositionXY")));
		attr.push(String(_CS("vertHeight")));
		attr.push(String(_CS("vertDerivata")));
		attr.push(String(_CS("vertLayers0")));
		attr.push(String(_CS("vertLayers1")));
	}

	if(fmt == EVertexFormat_Mesh || fmt == EVertexFormat_BuildinLightingMesh || fmt == EVertexFormat_SkeletalMesh || fmt == EVertexFormat_LightProbedMesh )
	{
		attr.push(String(_CS("vertPosition")));
		attr.push(String(_CS("vertTexCoord")));
		attr.push(String(_CS("vertTangentAndUnocclusion")));
		attr.push(String(_CS("vertBinormalAndOpacity")));
		attr.push(String(_CS("vertNormalAndOpacity2")));

		switch(fmt)
		{
		case EVertexFormat_BuildinLightingMesh:
			attr.push(String(_CS("vertToLightVectorXLightIntensivityAndLightIntensivity")));
			attr.push(String(_CS("vertLightIntensivity")));
			attr.push(String(_CS("vertAmbientIntensivity")));
			break;
		case EVertexFormat_SkeletalMesh:
			attr.push(String(_CS("vertBoneIndexes")));
			attr.push(String(_CS("vertBoneWeights")));
			break;
		case EVertexFormat_LightProbedMesh:
			attr.push(String(_CS("vertLightProbeIndexes")));
			attr.push(String(_CS("vertLightProbeWeights")));
			break;
		}
	}

	return attr;
}

}
