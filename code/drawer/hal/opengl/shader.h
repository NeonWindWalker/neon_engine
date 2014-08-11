#pragma once

#include "device.h"

#if defined(DRAWER_API_OPENGL) || defined(DRAWER_API_OPENGL_ES2)

namespace DrawerLib
{
	
class VertexShader : public RefCountable
{
public:
	VertexShader(HALDevice& device);
	~VertexShader();
	
	boolean init(ConstString code);
	GLuint glShader()const { return _glShader; }
	
	ListElement<VertexShader> lel;

protected:
	HALDevice& dev;
	GLuint _glShader;
	
private:
	VertexShader(VertexShader& other) : lel(this), dev(other.dev) {}
	void operator = (VertexShader& other) {}
};

class PixelShader : public RefCountable
{
public:
	PixelShader(HALDevice& device);
	~PixelShader();
	
	boolean init(ConstString code);
	GLuint glShader()const { return _glShader; }
	
	ListElement<PixelShader> lel;

protected:
	HALDevice& dev;
	GLuint _glShader;
	
private:
	PixelShader(PixelShader& other) : lel(this), dev(other.dev) {}
	void operator = (PixelShader& other) {}
};

struct TextureNameIndexAndIndex
{
	int shaderIndex;
	int deviceIndex;

	TextureNameIndexAndIndex()
	{
		shaderIndex = -1;
		deviceIndex = -1;
	}
};

class ShaderProgram : public RefCountable
{
public:
	ShaderProgram(HALDevice& device);
	~ShaderProgram();
	
	boolean init(VertexShader& vertexShader, PixelShader& pixelShader, ConstRange< ConstString> vertParams);
	int getParameterIndex(ConstString name);
	GLuint glProgram()const { return _glProgram; }
	TextureNameIndexAndIndex getTextureIndex(ConstString name);
	
	ListElement<ShaderProgram> lel;
	
	INLINE boolean init(VertexShader& vertexShader, PixelShader& pixelShader, ConstString arg0)
	{
		ConstString args[] = {arg0};
		return init(vertexShader, pixelShader, ConstRange< ConstString>(args, 1) );
	}
	
	INLINE boolean init(VertexShader& vertexShader, PixelShader& pixelShader, ConstString arg0, ConstString arg1)
	{
		ConstString args[] = {arg0, arg1};
		return init(vertexShader, pixelShader, ConstRange< ConstString>(args, 2) );
	}
	
	INLINE boolean init(VertexShader& vertexShader, PixelShader& pixelShader, ConstString arg0, ConstString arg1, ConstString arg2)
	{
		ConstString args[] = {arg0, arg1, arg2};
		return init(vertexShader, pixelShader, ConstRange< ConstString>(args, 3) );
	}
	
	INLINE boolean init(VertexShader& vertexShader, PixelShader& pixelShader, ConstString arg0, ConstString arg1, ConstString arg2, ConstString arg3)
	{
		ConstString args[] = {arg0, arg1, arg2, arg3};
		return init(vertexShader, pixelShader, ConstRange< ConstString>(args, 4) );
	}
	
	INLINE boolean init(VertexShader& vertexShader, PixelShader& pixelShader, ConstString arg0, ConstString arg1, ConstString arg2, ConstString arg3, ConstString arg4)
	{
		ConstString args[] = {arg0, arg1, arg2, arg3, arg4};
		return init(vertexShader, pixelShader, ConstRange< ConstString>(args, 5) );
	}
	
	INLINE boolean init(VertexShader& vertexShader, PixelShader& pixelShader, ConstString arg0, ConstString arg1, ConstString arg2, ConstString arg3, ConstString arg4, ConstString arg5)
	{
		ConstString args[] = {arg0, arg1, arg2, arg3, arg4, arg5};
		return init(vertexShader, pixelShader, ConstRange< ConstString>(args, 6) );
	}
	 
	INLINE boolean init(VertexShader& vertexShader, PixelShader& pixelShader, ConstString arg0, ConstString arg1, ConstString arg2, ConstString arg3, ConstString arg4, ConstString arg5, ConstString arg6)
	{
		ConstString args[] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6};
		return init(vertexShader, pixelShader, ConstRange< ConstString>(args, 7) );
	}
	
	INLINE boolean init(VertexShader& vertexShader, PixelShader& pixelShader, ConstString arg0, ConstString arg1, ConstString arg2, ConstString arg3, ConstString arg4, ConstString arg5, ConstString arg6, ConstString arg7)
	{
		ConstString args[] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7};
		return init(vertexShader, pixelShader, ConstRange< ConstString>(args, 8) );
	}

protected:
	HALDevice& dev;
	GLuint _glProgram;
	Array<int> textureIndexes;
	
private:
	ShaderProgram(ShaderProgram& other) : lel(this), dev(other.dev) {}
	void operator = (ShaderProgram& other) {}
};

} 

#endif
