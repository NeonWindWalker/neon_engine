#pragma once

#include "../base.h"

#if defined(DRAWER_API_OPENGL) || defined(DRAWER_API_OPENGL_ES2)

#if defined(__linux__) && !defined(__ANDROID__)
#include "x11BaseDevice.h"
#endif

#ifdef __ANDROID__
#include "androidBaseDevice.h"
#endif

#ifdef _WIN32
#include "win32BaseDevice.h"
#endif

#ifdef __APPLE__
#include "iosBaseDevice.h"
#endif

//crossplatform extension list
namespace DrawerLib 
{

#ifdef DRAWER_API_OPENGL
extern PFNGLACTIVETEXTUREPROC glActiveTextureARB;
extern PFNGLSAMPLECOVERAGEPROC glSampleCoverage;
extern PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
extern PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D;
extern PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage;

extern PFNGLISRENDERBUFFEREXTPROC glIsRenderbufferEXT;
extern PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT;
extern PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT ;
extern PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT;
extern PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT;
extern PFNGLISFRAMEBUFFEREXTPROC glIsFramebufferEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT;
extern PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT;
extern PFNGLFRAMEBUFFERTEXTURE1DEXTPROC glFramebufferTexture1DEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT;
extern PFNGLFRAMEBUFFERTEXTURE3DEXTPROC glFramebufferTexture3DEXT;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT;
extern PFNGLGENERATEMIPMAPEXTPROC glGenerateMipmapEXT;

extern PFNGLBINDBUFFERARBPROC glBindBufferARB;
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;
extern PFNGLGENBUFFERSARBPROC glGenBuffersARB;
extern PFNGLISBUFFERARBPROC glIsBufferARB;
extern PFNGLBUFFERDATAARBPROC glBufferDataARB;
extern PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB;
extern PFNGLGETBUFFERSUBDATAARBPROC glGetBufferSubDataARB;
extern PFNGLMAPBUFFERARBPROC glMapBufferARB;
extern PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB;
extern PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB;
extern PFNGLGETBUFFERPOINTERVARBPROC glGetBufferPointervARB;


extern PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointerARB;
extern PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArrayARB;
extern PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB;
extern PFNGLPROGRAMSTRINGARBPROC glProgramStringARB;
extern PFNGLBINDPROGRAMARBPROC glBindProgramARB;
extern PFNGLDELETEPROGRAMSARBPROC glDeleteProgramsARB;
extern PFNGLGENPROGRAMSARBPROC glGenProgramsARB;
extern PFNGLPROGRAMENVPARAMETER4DARBPROC glProgramEnvParameter4dARB;
extern PFNGLPROGRAMENVPARAMETER4DVARBPROC glProgramEnvParameter4dvARB;
extern PFNGLPROGRAMENVPARAMETER4FARBPROC glProgramEnvParameter4fARB;
extern PFNGLPROGRAMENVPARAMETER4FVARBPROC glProgramEnvParameter4fvARB;
extern PFNGLPROGRAMLOCALPARAMETER4DARBPROC glProgramLocalParameter4dARB;
extern PFNGLPROGRAMLOCALPARAMETER4DVARBPROC glProgramLocalParameter4dvARB;
extern PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB;
extern PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glProgramLocalParameter4fvARB;
extern PFNGLGETPROGRAMENVPARAMETERDVARBPROC glGetProgramEnvParameterdvARB;
extern PFNGLGETPROGRAMENVPARAMETERFVARBPROC glGetProgramEnvParameterfvARB;
extern PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC glGetProgramLocalParameterdvARB;
extern PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC glGetProgramLocalParameterfvARB;
extern PFNGLGETPROGRAMIVARBPROC glGetProgramivARB;
extern PFNGLGETPROGRAMSTRINGARBPROC glGetProgramStringARB;
extern PFNGLGETVERTEXATTRIBDVARBPROC glGetVertexAttribdvARB;
extern PFNGLGETVERTEXATTRIBFVARBPROC glGetVertexAttribfvARB;
extern PFNGLGETVERTEXATTRIBIVARBPROC glGetVertexAttribivARB;
extern PFNGLGETVERTEXATTRIBPOINTERVARBPROC glGetVertexAttribPointervARB;
extern PFNGLISPROGRAMARBPROC glIsProgramARB;

extern PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
extern PFNGLGETHANDLEARBPROC glGetHandleARB;
extern PFNGLDETACHOBJECTARBPROC glDetachObjectARB;
extern PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
extern PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
extern PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
extern PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
extern PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
extern PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
extern PFNGLVALIDATEPROGRAMARBPROC glValidateProgramARB;
extern PFNGLUNIFORM1FARBPROC glUniform1fARB;
extern PFNGLUNIFORM2FARBPROC glUniform2fARB;
extern PFNGLUNIFORM3FARBPROC glUniform3fARB;
extern PFNGLUNIFORM4FARBPROC glUniform4fARB;
extern PFNGLUNIFORM1IARBPROC glUniform1iARB;
extern PFNGLUNIFORM2IARBPROC glUniform2iARB;
extern PFNGLUNIFORM3IARBPROC glUniform3iARB;
extern PFNGLUNIFORM4IARBPROC glUniform4iARB;
extern PFNGLUNIFORM1FVARBPROC glUniform1fvARB;
extern PFNGLUNIFORM2FVARBPROC glUniform2fvARB;
extern PFNGLUNIFORM3FVARBPROC glUniform3fvARB;
extern PFNGLUNIFORM4FVARBPROC glUniform4fvARB;
extern PFNGLUNIFORM1IVARBPROC glUniform1ivARB;
extern PFNGLUNIFORM2IVARBPROC glUniform2ivARB;
extern PFNGLUNIFORM3IVARBPROC glUniform3ivARB;
extern PFNGLUNIFORM4IVARBPROC glUniform4ivARB;
extern PFNGLUNIFORMMATRIX2FVARBPROC glUniformMatrix2fvARB;
extern PFNGLUNIFORMMATRIX3FVARBPROC glUniformMatrix3fvARB;
extern PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fvARB;
extern PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfvARB;
extern PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
extern PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
extern PFNGLGETATTACHEDOBJECTSARBPROC glGetAttachedObjectsARB;
extern PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;
extern PFNGLGETACTIVEUNIFORMARBPROC glGetActiveUniformARB;
extern PFNGLGETUNIFORMFVARBPROC glGetUniformfvARB;
extern PFNGLGETUNIFORMIVARBPROC glGetUniformivARB;
extern PFNGLGETSHADERSOURCEARBPROC glGetShaderSourceARB;

extern PFNGLBINDATTRIBLOCATIONARBPROC glBindAttribLocationARB;
extern PFNGLGETACTIVEATTRIBARBPROC glGetActiveAttribARB;
extern PFNGLGETATTRIBLOCATIONARBPROC glGetAttribLocationARB;

extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;

extern PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;

extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC glRenderbufferStorageMultisampleEXT;

#else

#ifndef __APPLE__
extern PFNGLMAPBUFFEROESPROC glMapBufferOES;
extern PFNGLUNMAPBUFFEROESPROC glUnmapBufferOES;
extern PFNGLGETBUFFERPOINTERVOESPROC glGetBufferPointervOES;
#endif

#endif


#ifdef DRAWER_API_OPENGL
	#define my_glVertexAttribPointer glVertexAttribPointerARB
	#define my_glEnableVertexAttribArray glEnableVertexAttribArrayARB
	#define my_glDisableVertexAttribArray glDisableVertexAttribArrayARB
#else
	#define my_glVertexAttribPointer glVertexAttribPointer
	#define my_glEnableVertexAttribArray glEnableVertexAttribArray
	#define my_glDisableVertexAttribArray glDisableVertexAttribArray
#endif


extern void loadCrossplatformExtensions();


extern const char * errorString(GLenum he);

static INLINE boolean isOpenGLError(String& outStr)
{
    boolean retCode = false;
	while(true)
    {
        GLenum  glErr = glGetError();
        if(glErr == GL_NO_ERROR)
            return retCode;
		retCode = true;
		outStr << _CS("gl error:") << constString((const char*)errorString(glErr)) << _CS("\n");
    }
    return retCode;
}

static INLINE void* vboStrideHack(uint n)
{
	byte* p = 0;
	p += n;
	return p;
}


static INLINE void buildPixelToPixelMatrix(Matrix44& proj, int width, int height, float sizeX, float sizeY)
{
	proj.x.x = 2.0f / sizeX;
	proj.y.x = 0;
	proj.z.x = 0;
	proj.w.x = -sign(sizeX) + 0.0f / float(width);

	proj.x.y = 0;
	proj.y.y = 2.0f / sizeY;
	proj.z.y = 0;
	proj.w.y = -sign(sizeY) + 0.0f / float(height);

	proj.x.z = 0;
	proj.y.z = 0;
	proj.z.z = 0;
	proj.w.z = 0.5f;

	proj.x.w = 0;
	proj.y.w = 0;
	proj.z.w = 0;
	proj.w.w = 1;
}

static INLINE void buildPixelToPixelMatrix(Matrix23& proj, int width, int height, float sizeX, float sizeY)
{
	proj.x.x = 2.0f / sizeX;
	proj.y.x = 0;
	proj.p.x = -sign(sizeX) + 0.0f / float(width);

	proj.x.y = 0;
	proj.y.y = 2.0f / sizeY;
	proj.p.y = -sign(sizeY) + 0.0f / float(height);
}

class IndexBuffer;
class VertexBuffer;
class Texture;
class FrameTexture;
class FrameWindow;
class VertexShader;
class PixelShader;

class HALDevice : public BaseOSDevice
{
public:
	HALDevice(const DisplayHandle& display);
	HALDevice(IFrameWindow*& outMainWnd, const WindowHandle& srcWainWindowHandle, boolean fullscrean, boolean stereo);
	~HALDevice();
	
	IIndexBuffer* createIndexBuffer();
	IVertexBuffer* createVertexBuffer();
	ITexture* createTexture();
	ITextureCube* createTextureCube(){ return 0; }
	IFrameTexture* createFrameTexture();
	IFrameWindow* createFrameWindow();
	IAsyncReader* createAsyncReader();
	
	IIndexBuffer* getQuadIndexBuffer()const { return quadsIndexBuffer; }
	boolean isHdrMode() { return false; }

	virtual boolean supportTextureFormat(EColor color);
	virtual String gpuName();

	Array<uint32> imageDataForRemaping;

	void remapingBGRAtoRGBA(const ColorRGBA* src, uint count)
	{
		if(imageDataForRemaping.size() < count)
			imageDataForRemaping.resize(count);

		for(uint i=0; i < count; ++i)
		{
			const ColorRGBA& c = src[i];
#ifdef LITTLE_ENDIAN_ARCH
			imageDataForRemaping[i] = (uint32(c.r)) | (uint32(c.g)<<8) | (uint32(c.b)<<16) | (uint32(c.a)<<24);
#else
			imageDataForRemaping[i] = (uint32(c.r)<<24) | (uint32(c.g)<<16) | (uint32(c.b)<<8) | (uint32(c.a));
#endif
		}
	}

	void remapingBGRtoRGBA(const ColorRGB* src, uint count)
	{
		if(imageDataForRemaping.size() < count)
			imageDataForRemaping.resize(count);

		for(uint i=0; i < count; ++i)
		{
			const ColorRGB& c = src[i];
#ifdef LITTLE_ENDIAN_ARCH
			imageDataForRemaping[i] = (uint32(c.r)) | (uint32(c.g)<<8) | (uint32(c.b)<<16) | (uint32(255)<<24);
#else
			imageDataForRemaping[i] = (uint32(c.r)<<24) | (uint32(c.g)<<16) | (uint32(c.b)<<8) | (uint32(255));
#endif
		}
	}
	void remapingBGRtoBGRA(const ColorRGB* src, uint count)
	{
		if(imageDataForRemaping.size() < count)
			imageDataForRemaping.resize(count);

		for(uint i=0; i < count; ++i)
		{
			const ColorRGB& c = src[i];
#ifdef LITTLE_ENDIAN_ARCH
			imageDataForRemaping[i] = (uint32(c.b)) | (uint32(c.g)<<8) | (uint32(c.r)<<16) | (uint32(255)<<24);
#else
			imageDataForRemaping[i] = (uint32(c.b)<<24) | (uint32(c.g)<<16) | (uint32(c.r)<<8) | (uint32(255));
#endif
		}
	}
	boolean isBGRATextureSupported()const
	{
		return _isBGRATextureSupported;
	}

protected:
	boolean supportTextureFormats[EColor_EnumCount];
	boolean _isBGRATextureSupported;
	class Context* context;
	IIndexBuffer* quadsIndexBuffer;
	void createQuadBuffer();
	
	
	void init();
	
	byte* lockTempVertexBuffer(IVertexBuffer* vb);
	void unlockTempVertexBuffer(IVertexBuffer* vb);
	uint16* lockTempIndexBuffer(IIndexBuffer* ib);
	void unlockTempIndexBuffer(IIndexBuffer* ib);
};

}

#endif
