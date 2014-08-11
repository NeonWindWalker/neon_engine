#pragma once

#include "device.h"

#if defined(DRAWER_API_OPENGL) || defined(DRAWER_API_OPENGL_ES2)

namespace DrawerLib
{

class TextureBase
{
public:
	INLINE boolean cube()const { return _cube; }
	INLINE uint width()const { return _width; }
	INLINE uint height()const { return _height; }

	INLINE uint mips()const { return _mips; }
	INLINE uint floaty()const { return _floaty; }
	INLINE uint renderable()const { return _renderable; }
	INLINE boolean depthRenderable()const { return _depthRenderable; }
	INLINE boolean isValid()const { return _glTex != 0; }
	INLINE uint glTex()const { return _glTex; }

	boolean init(boolean cube, uint width, uint height, uint mips, EColor format, boolean dynamic);
	boolean initRenderable(boolean cube, uint width, uint height, uint mips, uint floaty);
	boolean initDepth(boolean cube, uint width, uint height, uint mips, boolean floaty);
	boolean copy(HALDevice& dev, uint face, uint mip, uint x, uint y, const void* data, uint width, uint height);

	INLINE void deinit()
	{
		if(!_glTex)
			return;
		glDeleteTextures(1, &_glTex);
		clear();
	}

protected:
	boolean _cube;
	uint _width;
	uint _height;
	uint _mips;
	uint _floaty;
	EColor _format;
	boolean _renderable;
	boolean _depthRenderable;
	GLenum _glFormat;
	boolean _compressedFormat;
	GLuint _glTex;
	HALDevice& basedev;

	TextureBase(HALDevice& dev) : basedev(dev)
	{
		clear();
	}
	~TextureBase()
	{
		deinit();
	}
	void clear()
	{
		_cube = false;
		_width = _height = 0;
		_mips = 0;
		_floaty = 0;
		_format = EColor_None;
		_renderable = false;
		_depthRenderable = false;
		_glFormat = 0;
		_compressedFormat = false;
		_glTex =  0;
	}
};

class IndexBuffer : public IIndexBuffer
{
public:
	IndexBuffer(HALDevice& dev);
	~IndexBuffer();
	boolean init(uint count, const uint16* data=0, boolean dynamic=false);
	void deinit();
	boolean copy(uint offset, uint16* data, uint count);
	uint count()const { return _count; };
	
	uint glVBO()const { return _glVBO; }

	HALDevice& basedev;
protected:
	uint _count;
	GLuint _glVBO;
	friend class Context;
};


class VertexBuffer : public IVertexBuffer
{
public:
	VertexBuffer(HALDevice& dev);
	~VertexBuffer();
	boolean init(uint size, const void* data=0, boolean dynamic=false);
	void deinit();
	boolean copy(uint offset, const void* data, uint size);
	uint size()const { return _size; }
	
	uint glVBO()const { return _glVBO; }

	HALDevice& basedev;
protected:
	uint _size;
	GLuint _glVBO;
	friend class Context;
};

class Texture : public ITexture, public TextureBase
{
public:
	Texture(HALDevice& dev);
	~Texture();
	boolean init(uint width, uint height, EColor format = EColor_RGBA, uint mips=0, boolean dynamic = false)
	{
		if(TextureBase::init(false, width, height, mips, format, dynamic)){
			_format = format;
			return true;
		}
		return false;
	}
	void deinit() { TextureBase::deinit(); }
	boolean copy(uint mip, uint x, uint y, const void* data, uint width, uint height){ return TextureBase::copy(basedev, 0, mip, x, y, data, width, height); }
	boolean copy(uint mip, const void* data){ return TextureBase::copy(basedev, 0, mip, 0, 0, data, maximum<uint>(_width>>mip, 1), maximum<uint>(_height>>mip,1)); }
	uint width()const { return _width; }
	uint height()const { return _height; }
	EColor format()const { return _format; }
	uint mips()const { return _mips; }

protected:
	EColor _format;
};



class TextureCube
{
public:
};

class AsyncReader : public IAsyncReader
{
public:
	AsyncReader(HALDevice& dev);
	~AsyncReader();
	virtual boolean init(uint delay, EColor color);
	virtual boolean checkIsReadComplete(){ return false; } // use nvFence
	void push(Context& con, float time);

protected:
	HALDevice& basedev;
	struct PBOAndTime
	{
		GLuint pbo;
		float time;
		PBOAndTime()
		{
			pbo = 0;
			time = 0;
		}
	};
	Array<PBOAndTime> glPBO;
	EColor colorType;
	Vector2i size;
	uint pboCount;
	uint curIndex;
	uint getsCount;
	Timer timer;
};

}

#endif
