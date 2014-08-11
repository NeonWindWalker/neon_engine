#include "resources.h"

#if defined(DRAWER_API_OPENGL) || defined(DRAWER_API_OPENGL_ES2)

namespace DrawerLib
{ 



boolean TextureBase::init(boolean cube, uint width, uint height, uint mips, EColor format, boolean dynamic)
{
	deinit();

	GLenum fmt;
	GLenum srcFmt;
	boolean compressed = false;

	switch(format)
	{
	case EColor_RGBA: 
	case EColor_RGB:
		if(basedev.isBGRATextureSupported())
		{
#ifdef DRAWER_API_OPENGL
			fmt = GL_RGBA;
			srcFmt = GL_BGRA;
#else
			fmt = GL_BGRA_EXT;
			srcFmt = GL_BGRA_EXT;
#endif
		}
		else
		{
			fmt = GL_RGBA;
			srcFmt = GL_RGBA; 
		}
		break;
	case EColor_L: fmt = GL_LUMINANCE; srcFmt = GL_LUMINANCE; break;
#ifdef DRAWER_API_OPENGL
	case EColor_DXT1: fmt = GL_COMPRESSED_RGB_S3TC_DXT1_EXT; compressed = true; break;
	case EColor_DXT1A: fmt = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; compressed = true; break;
	case EColor_DXT3: fmt = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; compressed = true; break;
	case EColor_DXT5: fmt = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; compressed = true; break;
#else

    case EColor_PVR2: fmt = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG; compressed = true; break;
    case EColor_PVR4: fmt = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG; compressed = true; break;
    case EColor_PVR2A: fmt = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG; compressed = true; break;
    case EColor_PVR4A: fmt = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG; compressed = true; break;
#ifndef __APPLE__ 
	case EColor_DXT1: fmt = GL_COMPRESSED_RGB_S3TC_DXT1_EXT; compressed = true; break;
	case EColor_DXT1A: fmt = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; compressed = true; break;
	case EColor_DXT3: fmt = 0x83F2 /*GL_COMPRESSED_RGBA_S3TC_DXT3_EXT*/; compressed = true; break;
	case EColor_DXT5: fmt = 0x83F3 /*GL_COMPRESSED_RGBA_S3TC_DXT5_EXT*/; compressed = true; break;
	case EColor_ETC1: fmt = GL_ETC1_RGB8_OES; compressed = true; break;
	case EColor_ATC: fmt = GL_ATC_RGB_AMD; compressed = true; break;
	case EColor_ATCI: fmt = GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD; compressed = true; break;
#endif
#endif
	default:
		logError << "unknow input format format=" << format;
		return false;
	}

	if(compressed && !glCompressedTexImage2D){
		logError << "glCompressedTexImage2D unsupported";
		return false;
	}

	glGenTextures(1, &_glTex);
	glBindTexture(cube ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, _glTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mips ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//BlockInfo blInfo = ImageLib::blockInfo(format);
	
	GLenum he = glGetError();
	uint w = width;
	uint h = height;
	_mips = 0;
	while(w && h)
	{
		if(cube)
		{
			for(uint i=0; i<6; ++i)
			{
				if(!compressed)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _mips, fmt, w, h, 0, srcFmt, GL_UNSIGNED_BYTE, 0);
				//else
				//	glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _mips, fmt, w, h, 0, blInfo.calcImageSize(w, h), fuckData.first());
			}
		}
		else
		{
			if(!compressed)
				glTexImage2D(GL_TEXTURE_2D, _mips, fmt, w, h, 0, srcFmt, GL_UNSIGNED_BYTE, 0);
			//else
			//	glCompressedTexImage2D(GL_TEXTURE_2D, _mips, fmt, w, h, 0, blInfo.calcImageSize(w, h), fuckData.first());
		}
		w >>= 1;
		h >>= 1;
		++ _mips;
		if(!w && !h)
			break;
		if(w < 1)
			w = 1;
		if(h < 1)
			h = 1;
		if(mips)
			if(_mips >= mips)
				break;
	}
	he = glGetError();
	glBindTexture(cube ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, 0);
	if(he != GL_NO_ERROR){
		logError << "gl*TexImage2D " << errorString(he) << "cube=" << cube << " width=" << width << " height=" << height << " mips=" << mips << " format=" << fmt;
		glDeleteTextures(1, &_glTex);
		_glTex = 0;
		_mips = 0;
		return false;
	}
	_cube = cube;
	_width = width;
	_height = height;
	_glFormat = fmt;
	_compressedFormat = compressed;
	_format = format;
	return true;
}

boolean TextureBase::initRenderable(boolean cube, uint width, uint height, uint mips, uint floaty)
{
	deinit();

#ifdef DRAWER_API_OPENGL
	GLenum fmt = GL_RGBA8;
	if(floaty == 2)
		fmt = GL_RGBA16F_ARB;
	else if(floaty == 4)
		fmt = GL_RGBA32F_ARB;
	else
		floaty = 0;
#else
	GLenum fmt = GL_RGBA8_OES;
	floaty = 0;
#endif

	glGenTextures(1, &_glTex);
	glBindTexture(cube ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, _glTex);
	GLenum he = glGetError();
	uint w = width;
	uint h = height;
	_mips = 0;
	while(w && h)
	{
		if(cube)
		{
			for(uint i=0; i<6; ++i)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _mips, fmt, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, _mips, fmt, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		}

		w >>= 1;
		h >>= 1;
		++ _mips;
		if(mips)
			if(_mips >= mips)
				break;
	}
	he = glGetError();
	glBindTexture(cube ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, 0);
	if(he != GL_NO_ERROR){
		logError << "glTexImage2D " << errorString(he);
		glDeleteTextures(1, &_glTex);
		_glTex = 0;
		_mips = 0;
		return false;
	}
	_cube = cube;
	_width = width;
	_height = height;
	_floaty = floaty;
	_renderable = true;
	return true;
}

boolean TextureBase::initDepth(boolean cube, uint width, uint height, uint mips, boolean floaty)
{
	deinit();

#ifdef DRAWER_API_OPENGL
	GLenum fmt = floaty ? GL_DEPTH_COMPONENT32F : GL_DEPTH_COMPONENT24;
#else
	GLenum fmt = GL_DEPTH_COMPONENT24_OES;
#endif

	glGenTextures(1, &_glTex);
	glBindTexture(cube ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, _glTex);
	GLenum he = glGetError();
	uint w = width;
	uint h = height;
	_mips = 0;
	while(w && h)
	{
		if(cube)
		{
			for(uint i=0; i<6; ++i)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _mips, fmt, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, _mips, fmt, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
		}
		w >>= 1;
		h >>= 1;
		++ _mips;
		if(mips)
			if(_mips >= mips)
				break;
	}
	he = glGetError();
	glBindTexture(cube ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, 0);
	if(he != GL_NO_ERROR){
		logError << "glTexImage2D " << errorString(he);
		glDeleteTextures(1, &_glTex);
		_glTex = 0;
		_mips = 0;
		return false;
	}
	_cube = cube;
	_width = width;
	_height = height;
	_floaty = floaty ? 4 : 0;
	_depthRenderable = true;
	return true;
}

boolean TextureBase::copy(HALDevice& dev, uint face, uint mip, uint x, uint y, const void* data, uint width, uint height)
{
	if(!_glTex)
		return false;
    
    Assert(data);

	glBindTexture(_cube ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, _glTex);

#ifdef DEBUG
	GLenum he = glGetError();
#endif

	if(!_compressedFormat)
	{
		GLenum t = GL_RGBA;
		if(basedev.isBGRATextureSupported())
		{
#ifdef DRAWER_API_OPENGL
			t = GL_BGRA;
#else
			t = GL_BGRA_EXT;
#endif
		}

		if(_glFormat == GL_LUMINANCE)
			t = GL_LUMINANCE;

		boolean bUseRemaping = false;
		if(t == GL_RGBA)
		{
			if(_format == EColor_RGBA)
				dev.remapingBGRAtoRGBA((ColorRGBA*)data, width * height);
			else
				dev.remapingBGRtoRGBA((ColorRGB*)data, width * height);
			bUseRemaping = true;
		}
		else if(t != GL_LUMINANCE) //BGR
		{
			if(_format == EColor_RGB)
			{
				dev.remapingBGRtoBGRA((ColorRGB*)data, width * height);
				bUseRemaping = true;
			}
		}
		
		glTexSubImage2D(_cube ? GL_TEXTURE_CUBE_MAP_POSITIVE_X + face : GL_TEXTURE_2D, mip, x, y, width, height, t, GL_UNSIGNED_BYTE, bUseRemaping ? dev.imageDataForRemaping.first() : data);
	}
	else
	{
		BlockInfo blInfo = blockInfo(_format);
		if(x ==0 && y == 0 && width == max(_width>>mip, uint(1)) && height == max(_height>>mip, uint(1)))
			glCompressedTexImage2D(GL_TEXTURE_2D, mip, _glFormat, width, height, 0, blInfo.calcImageSize(width, height), data);
		else
			glCompressedTexSubImage2D(_cube ? GL_TEXTURE_CUBE_MAP_POSITIVE_X + face : GL_TEXTURE_2D, mip, x, y, width, height, _glFormat, blInfo.calcImageSize(width, height), data);
	}

#ifdef DEBUG
	he = glGetError();
	if(he != GL_NO_ERROR){
		logError << "gl*TexSubImage2D " << errorString(he) << _CS(" fmt ") << _glFormat;
		glBindTexture(_cube ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, 0);
		return false;
	}
#endif

	glBindTexture(_cube ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, 0);
	return true;
}


IndexBuffer::IndexBuffer(HALDevice& hdev) : IIndexBuffer(hdev), basedev(hdev), _count(0), _glVBO(0)
{
}

IndexBuffer::~IndexBuffer()
{
	deinit();
}

boolean IndexBuffer::init(uint count, const uint16* data, boolean dynamic)
{
	deinit();

#ifdef DRAWER_API_OPENGL
	if(!glGenBuffersARB)
		return false;
	glGenBuffersARB(1,&_glVBO);
#else
	glGenBuffers(1,&_glVBO);
#endif

	if(!_glVBO)
		return false;

#ifdef DRAWER_API_OPENGL
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _glVBO);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 2 * count, data, dynamic ? GL_STREAM_DRAW_ARB : GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
#else
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _glVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * count, data, dynamic ? GL_STREAM_DRAW : GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif

	_count = count;
	return true;
}

void IndexBuffer::deinit()
{
	if(!_glVBO)
		return;
#ifdef DRAWER_API_OPENGL
	glDeleteBuffersARB(1, &_glVBO);
#else
	glDeleteBuffers(1, &_glVBO);
#endif
	_glVBO = 0;
	_count = 0;
}

boolean IndexBuffer::copy(uint offset, uint16* data, uint count)
{
	if(!_glVBO)
		return false;
#ifdef DRAWER_API_OPENGL
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _glVBO);
	glBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 2 * offset, 2 * count, data);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
#else
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _glVBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 2 * offset, 2 * count, data);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
	return true;
}


VertexBuffer::VertexBuffer(HALDevice& hdev) : IVertexBuffer(hdev), basedev(hdev), _size(0), _glVBO(0)
{
}

VertexBuffer::~VertexBuffer()
{
	deinit();
}

boolean VertexBuffer::init(uint size, const void* data, boolean dynamic)
{
	deinit();

#ifdef DRAWER_API_OPENGL
	if(!glGenBuffersARB || !size)
		return false;
	glGenBuffersARB(1,&_glVBO);
#else
	if(!size)
		return false;
	glGenBuffers(1,&_glVBO);
#endif

	if(!_glVBO)
		return false;

#ifdef DRAWER_API_OPENGL
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, _glVBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, data, dynamic ? GL_STREAM_DRAW_ARB : GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
#else
	glBindBuffer(GL_ARRAY_BUFFER, _glVBO);
	glBufferData(GL_ARRAY_BUFFER, size, data, dynamic ? GL_STREAM_DRAW : GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif

	_size = size;
	return true;
}

void VertexBuffer::deinit()
{
	if(!_glVBO)
		return;
#ifdef DRAWER_API_OPENGL
	glDeleteBuffersARB(1, &_glVBO);
#else
	glDeleteBuffers(1, &_glVBO);
#endif
	_glVBO = 0;
	_size = 0;
}

boolean VertexBuffer::copy(uint offset, const void* data, uint size)
{
	if(!_glVBO)
		return false;

#ifdef DRAWER_API_OPENGL
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, _glVBO);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, offset, size, data);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
#else
	glBindBuffer(GL_ARRAY_BUFFER, _glVBO);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif

	return true;
}


Texture::Texture(HALDevice& hdev) : ITexture(hdev), TextureBase(hdev), _format(EColor_EnumCount)
{
}

Texture::~Texture()
{
	deinit();
}


AsyncReader::AsyncReader(HALDevice& dev) : IAsyncReader(dev), basedev(dev)
{
	pboCount = 2;
	size.setZero();
	colorType = EColor_None;
	curIndex = 0;
	getsCount = 0;
}

AsyncReader::~AsyncReader()
{
	for(uint i=0; i < glPBO.size(); ++i)
	{
#ifdef DRAWER_API_OPENGL
		glDeleteBuffersARB(1, &glPBO[i].pbo);
#else
		glDeleteBuffers(1, &glPBO[i].pbo);
#endif
	}
}

boolean AsyncReader::init(uint delay, EColor color)
{
#if defined(DRAWER_API_OPENGL) || defined(DRAWER_API_OPENGLES3)

	#ifdef DRAWER_API_OPENGL
		if(!glMapBufferARB)
			return false;
	#else
		if(!glMapBufferOES)
			return false;
	#endif

		pboCount = delay;
		size.setZero();
		colorType = color;
		for(uint i=0; i < glPBO.size(); ++i)
		{
#ifdef DRAWER_API_OPENGL
			glDeleteBuffersARB(1, &glPBO[i].pbo);
#else
			glDeleteBuffers(1, &glPBO[i].pbo);
#endif
		}
		glPBO.clear();
		return true;
#else
	return false;
#endif
}

void AsyncReader::push(Context& con, float time)
{
#if defined(DRAWER_API_OPENGL) || defined(DRAWER_API_OPENGLES3)
		if(con.viewportSize() == Vector2i::getZero() || !colorType)
			return;

		if(con.viewportSize() != size)
		{
	#ifdef DRAWER_API_OPENGL
			for(uint i=0; i < glPBO.size(); ++i)
				glDeleteBuffersARB(1, &glPBO[i].pbo);
			glPBO.resize(pboCount);
			for(uint i=0; i < glPBO.size(); ++i)
				glGenBuffersARB(1, &glPBO[i].pbo);
	#else
			for(uint i=0; i < glPBO.size(); ++i)
				glDeleteBuffers(1, &glPBO[i].pbo);
			glPBO.resize(delay + 1);
			for(uint i=0; i < glPBO.size(); ++i)
				glGenBuffers(1, &glPBO[i].pbo);
	#endif
			size = con.viewportSize();
			iforeach(i, glPBO)
			{
				glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, glPBO[i].pbo);
				glBufferDataARB(GL_PIXEL_PACK_BUFFER_ARB, eColorSize(colorType) * size.x * size.y, 0, GL_STREAM_READ_ARB);
				glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, 0);
			}
			curIndex = 0;
			getsCount = 0;
		}

		//timer.dt();
		glPBO[curIndex].time = time;
		con.copyColorData(glPBO[curIndex].pbo, colorType);
		//logInfo << timer.dt();
		getsCount ++;
		curIndex = (curIndex + 1) % glPBO.size();
		if(getsCount <= curIndex)
			return;

		if(!onImageReadedFunc)
			return;

	#ifdef DRAWER_API_OPENGL
		if(!glMapBufferARB)
			return;

		glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, glPBO[curIndex].pbo);
		void* pboData = glMapBufferARB(GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY_ARB);
	#else
		if(!glMapBufferOES)
			return;

		glMapBufferOES(GL_PIXEL_PACK_BUFFER_ARB, glPBO[curIndex].pbo);
		void* ret = glMapBufferOES(GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY_ARB);
	#endif

		if(pboData)
			onImageReadedFunc(glPBO[curIndex].time, pboData, size.x, size.y, colorType);

	#ifdef DRAWER_API_OPENGL
		glUnmapBufferARB(GL_PIXEL_PACK_BUFFER_ARB);
		glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, 0);
	#else
		glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	#endif

#endif

}

}


#endif
