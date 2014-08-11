#pragma once

#include "../hal/include.h"

namespace DrawerLib
{

class ForwardDrawer : public HALDevice
{
public:
	ForwardDrawer(const DisplayHandle& display);
	ForwardDrawer(IFrameWindow*& outMainWnd, const WindowHandle& srcWainWindowHandle, boolean fullscrean, boolean stereo);
	~ForwardDrawer();
	
protected:
	void init();
	String getCommonVertexShadersCode();
	String getCommonPixelShadersCode();
	String getCommonFunctionsCode();

	VertexShader* simplePrimitivesVS;
	PixelShader* simplePrimitivesPS;
	ShaderProgram* simplePrimitivesProg;
	uint simplePrimitivesProg_nFullTransformMatrix_M44;
	String getSimplePrimitivesVertexShaderCode();
	String getSimplePrimitivesPixelShaderCode();
	
	VertexShader* interfaceQuadsVS;
	PixelShader* interfaceQuadsPS;
	ShaderProgram* interfaceQuadsProg;
	uint interfaceQuadsProg_nFullTransformMatrix_M23;
	TextureNameIndexAndIndex interfaceQuadsProg_Tex;
	PixelShader* interfaceQuadsPSA;
	ShaderProgram* interfaceQuadsProgA;
	uint interfaceQuadsProgA_nFullTransformMatrix_M23;
	TextureNameIndexAndIndex interfaceQuadsProgA_Tex;
	TextureNameIndexAndIndex interfaceQuadsProgA_AlphaTex;
	String getInterfaceQuadsVertexShaderCode();
	String getInterfaceQuadsPixelShaderCode(boolean alphaTex);

	VertexShader* interfaceFontVS[EFontGlyphsMode_EnumCount];
	PixelShader* interfaceFontPS[EFontGlyphsMode_EnumCount];
	ShaderProgram* interfaceFontProg[EFontGlyphsMode_EnumCount];
	uint interfaceFontProg_nFullTransformMatrix_M23[EFontGlyphsMode_EnumCount];
	TextureNameIndexAndIndex interfaceFontProg_Tex[EFontGlyphsMode_EnumCount];
	uint interfaceFontProg_DTexCrd;
	String getInterfaceFontVertexShaderCode(EFontGlyphsMode mode);
	String getInterfaceFontPixelShaderCode(EFontGlyphsMode mode);

	Array<String> getVertexShaderAttributesCode(EVertexFormat fmt);
};

#define GET_SHADER_PARAMETER_INDEX(prog, name) name = prog->getParameterIndex(_CS(#name));
	
}
