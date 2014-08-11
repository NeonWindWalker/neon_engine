/*************************************************************************
$Revision$

lake shader

******************************************************************************/

string ParamID = "0x003";

float Script : STANDARDSGLOBAL <
    string UIWidget = "none";
    string ScriptClass = "object";
    string ScriptOrder = "standard";
    string ScriptOutput = "color";
    string Script = "Technique=Main;";
> = 0.8;

float4x4 WorldITXf : WorldInverseTranspose < string UIWidget="None"; >;
float4x4 WvpXf : WorldViewProjection < string UIWidget="None"; >;
float4x4 WorldXf : World < string UIWidget="None"; >;
float4x4 ViewIXf : ViewInverse < string UIWidget="None"; >;

float3 Lamp0Pos : POSITION <
    string Object = "PointLight0";
    string UIName =  "Light Position";
    string Space = "World";
	int refID = 0;
> = {-0.5f,2.0f,1.25f};

float4 gSceneAmbient  <
	string UIName = "Scene Ambient";
	string UIWidget = "Color";
> = float4( 0.5f, 0.5f, 0.5f, 1.0f );

float4 gSceneLightColor  <
	string UIName = "Scene Light";
	string UIWidget = "Color";
> = float4( 0.5f, 0.5f, 0.5f, 1.0f );



Texture2D <float4> gAlbedoTexture < 
	string UIName = "Albedo";
	string ResourceType = "2D";
>;

SamplerState gAlbedoTextureSampler
{
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
    AddressV = Wrap;
};

bool gAlbedoTextureEnable <
	string UIName = "Albedo Enable";
> = false;

float gAlbedoMultiplyer <
	string UIName = "Albedo Multiplyer";
	string UIWidget = "slider";
	float UIMin = 0.0f;
	float UIMax = 255.0f;
	float UIStep = 1.0f;
>   = 255.0f;


Texture2D <float4> gOpacityTexture < 
	string UIName = "Opacity";
	string ResourceType = "2D";
>;

SamplerState gOpacityTextureSampler
{
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
    AddressV = Wrap;
};

bool gOpacityTextureEnable <
	string UIName = "Opacity Enable";
> = false;

Texture2D <float4> gNormalTexture < 
	string UIName = "Normal";
	string ResourceType = "2D";
>;

SamplerState gNormalTextureSampler
{
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
    AddressV = Wrap;
};

bool gNormalTextureEnable <
	string UIName = "Normal Enable";
> = false;

float gNormalFade <
	string UIName = "Normal Fade";
	string UIWidget = "slider";
	float UIMin = 0.0f;
	float UIMax = 255.0f;
	float UIStep = 1.0f;
>   = 0.0f;



Texture2D <float4> gSpecularTexture < 
	string UIName = "Specular";
	string ResourceType = "2D";
>;

SamplerState gSpecularTextureSampler
{
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
    AddressV = Wrap;
};

bool gSpecularTextureEnable <
	string UIName = "Specular Enable";
> = false;

float gSpecularMultiplyer <
	string UIName = "Specular Multiplyer";
	string UIWidget = "slider";
	float UIMin = 0.0f;
	float UIMax = 255.0f;
	float UIStep = 1.0f;
>   = 255.0f;



Texture2D <float4> gGlossinesTexture < 
	string UIName = "Glossines";
	string ResourceType = "2D";
>;

SamplerState gGlossinesTextureSampler
{
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
    AddressV = Wrap;
};

bool gGlossinesTextureEnable <
	string UIName = "Glossines Enable";
> = false;

float gGlossinesMultiplyer <
	string UIName = "Glossines Multiplyer";
	string UIWidget = "slider";
	float UIMin = 0.0f;
	float UIMax = 255.0f;
	float UIStep = 1.0f;
>   = 255.0f;


Texture2D <float4> gFrenselTexture < 
	string UIName = "Frensel";
	string ResourceType = "2D";
>;

SamplerState gFrenselTextureSampler
{
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
    AddressV = Wrap;
};

bool gFrenselTextureEnable <
	string UIName = "Frensel Enable";
> = false;

float gFrenselMultiplyer <
	string UIName = "Frensel Multiplyer";
	string UIWidget = "slider";
	float UIMin = 0.0f;
	float UIMax = 255.0f;
	float UIStep = 1.0f;
>   = 255.0f;



bool gMetal <
	string UIName = "Metal";
> = false;

float gLightBackFacesTreshold <
	string UIName = "Light Back Faces Treshold";
	string UIWidget = "slider";
	float UIMin = 0.0f;
	float UIMax = 1.0f;
	float UIStep = 0.05f;
>  = 0.1f;

float gAlpha <
	string UIName = "Alpha";
	string UIWidget = "slider";
	float UIMin = 0.0f;
	float UIMax = 255.0f;
	float UIStep = 1.0f;
>   = 128.0f;






struct VertexShaderInput {
	float4 position		: POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 binormal		: BINORMAL;
	float2 texCoord		: TEXCOORD0;	
};

struct VertexShaderOutput {
    float4 HPosition		: SV_Position;
    float2 vrTexCoord		: TEXCOORD0;
	float3 vrLightColor		: TEXCOORD2;
	float3 vrAmbientColor	: TEXCOORD3;
	float3 vrLocalToLightDir: TEXCOORD4;
	float3 vrLocalH			: TEXCOORD5;
	float vrSlickTerm		: TEXCOORD6;
	float3 vrDebug			: TEXCOORD7;
};
 
VertexShaderOutput std_VS(VertexShaderInput IN) 
{
    VertexShaderOutput OUT = (VertexShaderOutput)0;
   
	float3 p = mul(float4(IN.position.xyz,1),WorldXf).xyz;
	OUT.HPosition = mul(float4(IN.position.xyz,1),WvpXf);
	OUT.vrTexCoord = IN.texCoord;
	float3 tangent = normalize(mul(float4(IN.tangent,0),WorldXf).xyz);
    float3 binormal = normalize(mul(float4(IN.binormal,0),WorldXf).xyz);
	float3 normal = normalize(mul(float4(IN.normal,0),WorldXf).xyz);
	float3 toLightDirection = normalize(Lamp0Pos - p);
	float3 toEye = normalize(ViewIXf[3].xyz - p);
	
	float NdotE = dot(normal, toEye);
	float NdotL = dot(normal, toLightDirection);
	float frontlightCoef = clamp(NdotL * (1.0f - gLightBackFacesTreshold) + gLightBackFacesTreshold, 0.0, 1.0);
	OUT.vrLightColor = gSceneLightColor.xyz * frontlightCoef;
	OUT.vrAmbientColor = gSceneAmbient.xyz;
	float3 h = normalize(toLightDirection + toEye);
	OUT.vrLocalToLightDir = normalize(float3(dot(toLightDirection, tangent),  dot(toLightDirection, binormal), dot(toLightDirection, normal)));
	OUT.vrLocalH = normalize(float3(dot(h, tangent),  dot(h, binormal), dot(h, normal)));
	float oneMinusNdotE = 1.0 - abs(NdotE);
	OUT.vrSlickTerm = sqrt(oneMinusNdotE * oneMinusNdotE * oneMinusNdotE * oneMinusNdotE * oneMinusNdotE);
	
	OUT.vrDebug = toEye;
    return OUT;
}

float beckmanSpecularFunction(float HdotNPositive, float roughtnessSqr)
{
	float constant_epsilon = 0.001;
	float cosAlpha2 = HdotNPositive + constant_epsilon;
	cosAlpha2 *= cosAlpha2;
	float rXc = cosAlpha2 * roughtnessSqr;
	return 0.25 * exp( (cosAlpha2 - 1.0) / rXc ) / (rXc * cosAlpha2);
}

float roughtnessSqrFromGlossines(float glossines)
{
	const float constant_X = -9.0;
	return exp2(constant_X * glossines);
}


float4 std_PS(VertexShaderOutput IN, bool bAlphaTest, bool bSolid) : SV_Target 
{
	float3 albedo = float3(0.0, 0.0, 0.0); 
	if(gAlbedoTextureEnable)
		albedo = gAlbedoTexture.Sample(gAlbedoTextureSampler,IN.vrTexCoord).xyz;
	
	albedo *= gAlbedoMultiplyer / 255.0;
	albedo = clamp(albedo, float3(0.0, 0.0, 0.0), float3(1.0, 1.0, 1.0));
		
	float opacity = 1.0; 
	if(gOpacityTextureEnable){
		float4 c = gOpacityTexture.Sample(gOpacityTextureSampler,IN.vrTexCoord);
		opacity = (c.x + c.y + c.z) / 3.0;
	}
	
	if(bAlphaTest)
	{
		if(opacity < 0.5)
			discard;
	}
	
	float2 normalXY = float2(0.5, 0.5); 
	if(gNormalTextureEnable)
		normalXY = gNormalTexture.Sample(gNormalTextureSampler,IN.vrTexCoord).xy;
	normalXY = (normalXY * 2.0 - 1.0) * (1.0f - gNormalFade / 255.0);
	float3 normal = float3(normalXY.x, normalXY.y, sqrt(max(0.0, 1.0 - dot(normalXY, normalXY))));
	
	float specular = 0.0; 
	if(gSpecularTextureEnable){
		float4 c = gSpecularTexture.Sample(gSpecularTextureSampler,IN.vrTexCoord);
		specular = (c.x + c.y + c.z) / 3.0;
	}
	specular *= gSpecularMultiplyer / 255.0;
	specular = clamp(specular, 0.0, 1.0);
	
	float glossines = 0.5; 
	if(gGlossinesTextureEnable){
		float4 c = gGlossinesTexture.Sample(gGlossinesTextureSampler,IN.vrTexCoord);
		glossines = (c.x + c.y + c.z) / 3.0;
	}
	glossines *= gGlossinesMultiplyer / 255.0;
	glossines = clamp(glossines, 0.0, 1.0);
	
	float frensel = 0.0; 
	if(gFrenselTextureEnable){
		float4 c = gFrenselTexture.Sample(gFrenselTextureSampler,IN.vrTexCoord);
		frensel = (c.x + c.y + c.z) / 3.0;
	}
	frensel *= gFrenselMultiplyer / 255.0;
	frensel = clamp(frensel, 0.0, 1.0);
	
	float3 toLightDirLoc = normalize(IN.vrLocalToLightDir);
	float NdotL = max(0.0, dot(normal, toLightDirLoc));
	float3 color = albedo * (IN.vrAmbientColor + IN.vrLightColor * sqrt(NdotL));
	float HdotN = max(0.0, dot(normalize(IN.vrLocalH), normal));
	float fr = frensel + (1.0 - frensel) * IN.vrSlickTerm;
	color += IN.vrLightColor * (gMetal ? (albedo / max(max(albedo.r,albedo.g), max(albedo.b, + 0.0001))) : 1.0) * specular * fr * sqrt(beckmanSpecularFunction(HdotN, roughtnessSqrFromGlossines(glossines)) );
	
	//return float4(123.0 / 255.0, 11.0 / 255.0, 221.0 / 255.0, 1.0);
	return float4(color, bSolid ? 1.0 : gAlpha / 255.0 );
	//return float4(frensel,fr,fr, bSolid ? 1.0 : gAlpha);
}

float4 std_PS_NoAlphaTest(VertexShaderOutput IN) : SV_Target 
{
	return std_PS(IN, false, true);
}

float4 std_PS_AlphaTest(VertexShaderOutput IN) : SV_Target 
{
	return std_PS(IN, true, true);
}

float4 std_PS_Transparent(VertexShaderOutput IN) : SV_Target 
{
	return std_PS(IN, true, false);
}

///// TECHNIQUES /////////////////////////////

RasterizerState DisableCulling { CullMode = NONE; };
DepthStencilState DepthEnabling { DepthEnable = TRUE; };
DepthStencilState DepthDisabling {
	DepthEnable = TRUE;
	DepthWriteMask = ZERO;
};
BlendState DisableBlend { BlendEnable[0] = FALSE; };

technique10 Solid <
	string Script = "Pass=p0;";
> {
	pass p0 <
	string Script = "Draw=geometry;";
    > 
    {
        SetVertexShader(CompileShader(vs_4_0,std_VS()));
        SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_4_0,std_PS_NoAlphaTest()));
		
		SetRasterizerState(DisableCulling);
	    SetDepthStencilState(DepthEnabling, 0);
    }
}

technique10 AlphaTest <
	string Script = "Pass=p0;";
> {
	pass p0 <
	string Script = "Draw=geometry;";
    > 
    {
        SetVertexShader(CompileShader(vs_4_0,std_VS()));
        SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_4_0,std_PS_AlphaTest()));
		
		SetRasterizerState(DisableCulling);
	    SetDepthStencilState(DepthEnabling, 0);
    }
}

technique10 Transparent <
	string Script = "Pass=p0;";
> {
	pass p0 <
	string Script = "Draw=geometry;";
    > 
    {
        SetVertexShader(CompileShader(vs_4_0,std_VS()));
        SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_4_0,std_PS_Transparent()));
		
		SetRasterizerState(DisableCulling);
	    SetDepthStencilState(DepthDisabling, 0);
	    //SetBlendState(DisableBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF);
    }
}

/////////////////////////////////////// eof //
