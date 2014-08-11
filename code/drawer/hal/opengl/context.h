#pragma once

#include "device.h"
#include "resources.h"
#include "renderTarget.h"
#include "shader.h"

#if defined(DRAWER_API_OPENGL) || defined(DRAWER_API_OPENGL_ES2)

namespace DrawerLib
{

enum ETextureSetMode
{
	ETextureSetMode_Default = 0,
	ETextureSetMode_Particles,
	ETextureSetMode_DefaultGUI,
	ETextureSetMode_GUIFont,
	ETextureSetMode_GUIHardFont,
	ETextureSetMode_ClampTablic,
	ETextureSetMode_EnumCount
};
	
class Context
{
public:
	Context(HALDevice& dev);
	~Context();

	void setRenderTargetWindowMode(FrameWindow& wnd);

	void setRenderTargetCompositMode();
	void setRenderTargetColor(uint n, TextureBase& tex, uint mip = 0);
	void setRenderTargetDepth(TextureBase& tex, uint mip = 0);
	void setRenderTargetDepth(RenderBufferDepthStensil& buf);

	void viewportFull();
	void viewport(uint x, uint y, uint width, uint height);
	
	void clearBuffers(boolean clearDepth, Vector4* color = 0);

	void begin();

	void colorNo();
	void blendNo();
	void blendAdd();
	void blendAddXAlpha();
	void blendAlpha();
	void blendXAlpha();

	void depthRange(float low, float hi);
	void depthNo();
	void depthLessEqual(boolean write = true);
	void depthGreaterEqual(boolean write = true);
	
	void program(ShaderProgram& shader);
	
	void parameter(int n, int val)
	{
		if(n < 0)
			return;
#ifdef DRAWER_API_OPENGL
		glUniform1iARB(n, val);
#else
		glUniform1i(n, val);
#endif
	}

	void parameter(int n, float val)
	{
		if(n < 0)
			return;
#ifdef DRAWER_API_OPENGL
		glUniform1fARB(n, val);
#else
		glUniform1f(n, val);
#endif
	}

	void parameter(int n, const Vector2& val)
	{
		if(n < 0)
			return;
#ifdef DRAWER_API_OPENGL
		glUniform2fvARB(n, 1, val.a);
#else
		glUniform2fv(n, 1, val.a);
#endif
	}

	void parameter(int n, const Vector2_t& val)
	{
		if(n < 0)
			return;
#ifdef DRAWER_API_OPENGL
		glUniform3fvARB(n, 1, val.a);
#else
		glUniform3fv(n, 1, val.a);
#endif
	}

	void parameter(int n, const Vector4& val)
	{
		if(n < 0)
			return;
#ifdef DRAWER_API_OPENGL
		glUniform4fvARB(n, 1, val.a);
#else
		glUniform4fv(n, 1, val.a);
#endif
	}

	void parameter(int n, const Matrix23& val)
	{ 
		if(n < 0)
			return;
		float v[12] = { val.x.x, val.x.y, 0.0, 0.0f,
						val.y.x, val.y.y, 0.0, 0.0f,
						val.p.x, val.p.y, 0.0, 0.0f };
#ifdef DRAWER_API_OPENGL
		glUniform4fvARB(n, 3, v);
#else
		glUniform4fv(n, 3, v);
#endif
	}

	void parameter(int n, const Matrix33& val)
	{ 
		if(n < 0)
			return;
		float v[12] = { val.x.x, val.x.y, val.x.z, 0.0f,
						val.y.x, val.y.y, val.y.z, 0.0f,
						val.z.x, val.z.y, val.z.z, 0.0f };
#ifdef DRAWER_API_OPENGL
		glUniform4fvARB(n, 3, v);
#else
		glUniform4fv(n, 3, v);
#endif
	}

	void parameter(int n, const Matrix34& val)
	{ 
		if(n < 0)
			return;
		float v[16] = { val.x.x, val.x.y, val.x.z, 0.0f,
						val.y.x, val.y.y, val.y.z, 0.0f,
						val.z.x, val.z.y, val.z.z, 0.0f,
						val.p.x, val.p.y, val.p.z, 1.0f };
#ifdef DRAWER_API_OPENGL
		glUniform4fvARB(n, 4, v);
#else
		glUniform4fv(n, 4, v);
#endif
	}
	
	void parameter(int n, const Matrix44& val)
	{
		if(n < 0)
			return;

#ifdef DRAWER_API_OPENGL
		glUniform4fvARB(n, 4, val.x.a);
#else
		glUniform4fv(n, 4, val.x.a);
#endif
	}

	/*
	void parameter(int n, const Matrix45& val)
	{
		if(n < 0)
			return;
#ifdef DRAWER_API_OPENGL
		glUniform4fvARB(n, 5, &val.x.x);
#else
		glUniform4fv(n, 4, &val.x.x);
#endif
	}*/

	void parameterArray(int n, const Vector4* arr, uint vec4Count)
	{
		if(n < 0)
			return;

#ifdef DRAWER_API_OPENGL
		glUniform4fvARB(n, vec4Count, arr->a);
#else
		glUniform4fv(n, vec4Count, arr->a);
#endif
	}

	void parameterArray(int n, const PackedVector4f* arr, uint vec4Count)
	{
		if(n < 0)
			return;

#ifdef DRAWER_API_OPENGL
		glUniform4fvARB(n, vec4Count, arr->a);
#else
		glUniform4fv(n, vec4Count, arr->a);
#endif
	}

	void setTextureFiltring(boolean trilinear, uint anisotropy);
	void texture(TextureNameIndexAndIndex index, TextureBase& tex, ETextureSetMode mode = ETextureSetMode_Default);
	void texture(uint n, TextureCube& tex);
	void textureDisableFrom(uint n);
	
	void draw(IVertexBuffer* ivb, EVertexFormat vbformat, uint vbByteOffset, uint vbSize, IIndexBuffer* iib, uint ibOffset, uint trisCount, boolean strip = false, boolean frontFace = true, boolean twosided=false);
	void drawQuads(IVertexBuffer* ivb, EVertexFormat vbformat, uint vbByteOffset, uint count );
	void drawSimplePoints(IVertexBuffer* ivb, uint vbByteOffset, uint count);
	void drawSimpleLines(IVertexBuffer* ivb, uint vbByteOffset, uint count);
	void drawSimpleTriangles(IVertexBuffer* ivb, uint vbByteOffset, uint count);
	
	void copyColorData(StrongPtr<ImageLib::BaseImage>& outImg);
	boolean copyColorData(GLuint pbo, EColor colorType);

	void end();

	Vector2i viewportPosition()const { return _viewportPosition; }
	Vector2i viewportSize()const { return _viewportSize; }

	uint textureChangesCount;
	uint shaderChangesCount;
	uint drawsCount;
	uint trianglesCount;

protected:
	HALDevice& basedevice;
	GLuint _glFBO;
	boolean _FBOActive;
	Vector2i _renderTargetSize;
	boolean _fullViewport;
	Vector2i _viewportPosition, _viewportSize;
	ShaderProgram* _shaderProgram;
	TextureBase* _bindedTextures[32];
	ETextureSetMode _bindedTextureModes[32];
	boolean defaultFilterTrilinear;
	uint defaultFilterAnisotropy;


	void clearCache()
	{
		textureChangesCount = 0;
		shaderChangesCount = 0;
		drawsCount = 0;
		trianglesCount = 0;

		_shaderProgram = 0;
		for(uint i=0; i < 32; ++i){
			_bindedTextures[i] = 0;
			_bindedTextureModes[i] = ETextureSetMode_EnumCount;
		}
	}
};

}

#endif
