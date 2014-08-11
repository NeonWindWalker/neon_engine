#include "device.h"
#include "resources.h"
#include "renderTarget.h"
#include "context.h"


#if defined(DRAWER_API_OPENGL) || defined(DRAWER_API_OPENGL_ES2)

namespace DrawerLib
{ 

#ifdef DRAWER_API_OPENGL

PFNGLACTIVETEXTUREPROC glActiveTextureARB = 0;
PFNGLSAMPLECOVERAGEPROC glSampleCoverage = 0;
PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D = 0;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D = 0;
PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D = 0;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D = 0;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D = 0;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D = 0;
PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage = 0;

PFNGLISRENDERBUFFEREXTPROC glIsRenderbufferEXT = 0;
PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT = 0;
PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT = 0;
PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT = 0;
PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT = 0;
PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT = 0;
PFNGLISFRAMEBUFFEREXTPROC glIsFramebufferEXT = 0;
PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT = 0;
PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT = 0;
PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT = 0;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT= 0;
PFNGLFRAMEBUFFERTEXTURE1DEXTPROC glFramebufferTexture1DEXT = 0;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT = 0;
PFNGLFRAMEBUFFERTEXTURE3DEXTPROC glFramebufferTexture3DEXT = 0;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT = 0;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT = 0;
PFNGLGENERATEMIPMAPEXTPROC glGenerateMipmapEXT = 0;

PFNGLBINDBUFFERARBPROC glBindBufferARB = 0;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = 0;
PFNGLGENBUFFERSARBPROC glGenBuffersARB = 0;
PFNGLISBUFFERARBPROC glIsBufferARB = 0;
PFNGLBUFFERDATAARBPROC glBufferDataARB = 0;
PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB = 0;
PFNGLGETBUFFERSUBDATAARBPROC glGetBufferSubDataARB = 0;
PFNGLMAPBUFFERARBPROC glMapBufferARB = 0;
PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB = 0;
PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB = 0;
PFNGLGETBUFFERPOINTERVARBPROC glGetBufferPointervARB = 0;


PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointerARB = 0;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArrayARB = 0;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB = 0;
PFNGLPROGRAMSTRINGARBPROC glProgramStringARB = 0;
PFNGLBINDPROGRAMARBPROC glBindProgramARB = 0;
PFNGLDELETEPROGRAMSARBPROC glDeleteProgramsARB = 0;
PFNGLGENPROGRAMSARBPROC glGenProgramsARB = 0;
PFNGLPROGRAMENVPARAMETER4DARBPROC glProgramEnvParameter4dARB = 0;
PFNGLPROGRAMENVPARAMETER4DVARBPROC glProgramEnvParameter4dvARB = 0;
PFNGLPROGRAMENVPARAMETER4FARBPROC glProgramEnvParameter4fARB = 0;
PFNGLPROGRAMENVPARAMETER4FVARBPROC glProgramEnvParameter4fvARB = 0;
PFNGLPROGRAMLOCALPARAMETER4DARBPROC glProgramLocalParameter4dARB = 0;
PFNGLPROGRAMLOCALPARAMETER4DVARBPROC glProgramLocalParameter4dvARB = 0;
PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB = 0;
PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glProgramLocalParameter4fvARB = 0;
PFNGLGETPROGRAMENVPARAMETERDVARBPROC glGetProgramEnvParameterdvARB = 0;
PFNGLGETPROGRAMENVPARAMETERFVARBPROC glGetProgramEnvParameterfvARB = 0;
PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC glGetProgramLocalParameterdvARB = 0;
PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC glGetProgramLocalParameterfvARB = 0;
PFNGLGETPROGRAMIVARBPROC glGetProgramivARB = 0;
PFNGLGETPROGRAMSTRINGARBPROC glGetProgramStringARB = 0;
PFNGLGETVERTEXATTRIBDVARBPROC glGetVertexAttribdvARB = 0;
PFNGLGETVERTEXATTRIBFVARBPROC glGetVertexAttribfvARB = 0;
PFNGLGETVERTEXATTRIBIVARBPROC glGetVertexAttribivARB = 0;
PFNGLGETVERTEXATTRIBPOINTERVARBPROC glGetVertexAttribPointervARB = 0;
PFNGLISPROGRAMARBPROC glIsProgramARB = 0;

PFNGLDELETEOBJECTARBPROC glDeleteObjectARB = 0;
PFNGLGETHANDLEARBPROC glGetHandleARB = 0;
PFNGLDETACHOBJECTARBPROC glDetachObjectARB = 0;
PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB = 0;
PFNGLSHADERSOURCEARBPROC glShaderSourceARB = 0;
PFNGLCOMPILESHADERARBPROC glCompileShaderARB = 0;
PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB = 0;
PFNGLATTACHOBJECTARBPROC glAttachObjectARB = 0;
PFNGLLINKPROGRAMARBPROC glLinkProgramARB = 0;
PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB = 0;
PFNGLVALIDATEPROGRAMARBPROC glValidateProgramARB = 0;
PFNGLUNIFORM1FARBPROC glUniform1fARB = 0;
PFNGLUNIFORM2FARBPROC glUniform2fARB = 0;
PFNGLUNIFORM3FARBPROC glUniform3fARB = 0;
PFNGLUNIFORM4FARBPROC glUniform4fARB = 0;
PFNGLUNIFORM1IARBPROC glUniform1iARB = 0;
PFNGLUNIFORM2IARBPROC glUniform2iARB = 0;
PFNGLUNIFORM3IARBPROC glUniform3iARB = 0;
PFNGLUNIFORM4IARBPROC glUniform4iARB = 0;
PFNGLUNIFORM1FVARBPROC glUniform1fvARB = 0;
PFNGLUNIFORM2FVARBPROC glUniform2fvARB = 0;
PFNGLUNIFORM3FVARBPROC glUniform3fvARB = 0;
PFNGLUNIFORM4FVARBPROC glUniform4fvARB = 0;
PFNGLUNIFORM1IVARBPROC glUniform1ivARB = 0;
PFNGLUNIFORM2IVARBPROC glUniform2ivARB = 0;
PFNGLUNIFORM3IVARBPROC glUniform3ivARB = 0;
PFNGLUNIFORM4IVARBPROC glUniform4ivARB = 0;
PFNGLUNIFORMMATRIX2FVARBPROC glUniformMatrix2fvARB = 0;
PFNGLUNIFORMMATRIX3FVARBPROC glUniformMatrix3fvARB = 0;
PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fvARB = 0;
PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfvARB = 0;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB = 0;
PFNGLGETINFOLOGARBPROC glGetInfoLogARB = 0;
PFNGLGETATTACHEDOBJECTSARBPROC glGetAttachedObjectsARB = 0;
PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB = 0;
PFNGLGETACTIVEUNIFORMARBPROC glGetActiveUniformARB = 0;
PFNGLGETUNIFORMFVARBPROC glGetUniformfvARB = 0;
PFNGLGETUNIFORMIVARBPROC glGetUniformivARB = 0;
PFNGLGETSHADERSOURCEARBPROC glGetShaderSourceARB = 0;

PFNGLBINDATTRIBLOCATIONARBPROC glBindAttribLocationARB = 0;
PFNGLGETACTIVEATTRIBARBPROC glGetActiveAttribARB = 0;
PFNGLGETATTRIBLOCATIONARBPROC glGetAttribLocationARB = 0;

PFNGLGETSHADERIVPROC glGetShaderiv = 0;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = 0;

PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB = 0;

PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC glRenderbufferStorageMultisampleEXT = 0;

#else
    
#ifndef __APPLE__
PFNGLMAPBUFFEROESPROC glMapBufferOES = 0;
PFNGLUNMAPBUFFEROESPROC glUnmapBufferOES = 0;
PFNGLGETBUFFERPOINTERVOESPROC glGetBufferPointervOES = 0;
#endif
    
#endif

void loadCrossplatformExtensions()
{
#ifdef DRAWER_API_OPENGL
	LOAD_EXTENSION(PFNGLACTIVETEXTUREPROC, glActiveTextureARB);
	LOAD_EXTENSION(PFNGLSAMPLECOVERAGEPROC, glSampleCoverage);
	LOAD_EXTENSION(PFNGLCOMPRESSEDTEXIMAGE3DPROC, glCompressedTexImage3D);
	LOAD_EXTENSION(PFNGLCOMPRESSEDTEXIMAGE2DPROC, glCompressedTexImage2D);
	LOAD_EXTENSION(PFNGLCOMPRESSEDTEXIMAGE1DPROC, glCompressedTexImage1D);
	LOAD_EXTENSION(PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC, glCompressedTexSubImage3D);
	LOAD_EXTENSION(PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC, glCompressedTexSubImage2D);
	LOAD_EXTENSION(PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC, glCompressedTexSubImage1D);
	LOAD_EXTENSION(PFNGLGETCOMPRESSEDTEXIMAGEPROC, glGetCompressedTexImage);

	LOAD_EXTENSION(PFNGLISRENDERBUFFEREXTPROC,glIsRenderbufferEXT);
	LOAD_EXTENSION(PFNGLBINDRENDERBUFFEREXTPROC, glBindRenderbufferEXT);
	LOAD_EXTENSION(PFNGLDELETERENDERBUFFERSEXTPROC, glDeleteRenderbuffersEXT);
	LOAD_EXTENSION(PFNGLGENRENDERBUFFERSEXTPROC, glGenRenderbuffersEXT);
	LOAD_EXTENSION(PFNGLRENDERBUFFERSTORAGEEXTPROC, glRenderbufferStorageEXT);
	LOAD_EXTENSION(PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC, glGetRenderbufferParameterivEXT);
	LOAD_EXTENSION(PFNGLISFRAMEBUFFEREXTPROC, glIsFramebufferEXT);
	LOAD_EXTENSION(PFNGLBINDFRAMEBUFFEREXTPROC, glBindFramebufferEXT);
	LOAD_EXTENSION(PFNGLDELETEFRAMEBUFFERSEXTPROC, glDeleteFramebuffersEXT);
	LOAD_EXTENSION(PFNGLGENFRAMEBUFFERSEXTPROC, glGenFramebuffersEXT);
	LOAD_EXTENSION(PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC, glCheckFramebufferStatusEXT);
	LOAD_EXTENSION(PFNGLFRAMEBUFFERTEXTURE1DEXTPROC, glFramebufferTexture1DEXT);
	LOAD_EXTENSION(PFNGLFRAMEBUFFERTEXTURE2DEXTPROC, glFramebufferTexture2DEXT);
	LOAD_EXTENSION(PFNGLFRAMEBUFFERTEXTURE3DEXTPROC, glFramebufferTexture3DEXT);
	LOAD_EXTENSION(PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC, glFramebufferRenderbufferEXT);
	LOAD_EXTENSION(PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC, glGetFramebufferAttachmentParameterivEXT);
	LOAD_EXTENSION(PFNGLGENERATEMIPMAPEXTPROC, glGenerateMipmapEXT);

	LOAD_EXTENSION(PFNGLBINDBUFFERARBPROC, glBindBufferARB);
	LOAD_EXTENSION(PFNGLDELETEBUFFERSARBPROC, glDeleteBuffersARB);
	LOAD_EXTENSION(PFNGLGENBUFFERSARBPROC, glGenBuffersARB);
	LOAD_EXTENSION(PFNGLISBUFFERARBPROC, glIsBufferARB);
	LOAD_EXTENSION(PFNGLBUFFERDATAARBPROC, glBufferDataARB);
	LOAD_EXTENSION(PFNGLBUFFERSUBDATAARBPROC, glBufferSubDataARB);
	LOAD_EXTENSION(PFNGLGETBUFFERSUBDATAARBPROC, glGetBufferSubDataARB);
	LOAD_EXTENSION(PFNGLMAPBUFFERARBPROC, glMapBufferARB);
	LOAD_EXTENSION(PFNGLUNMAPBUFFERARBPROC, glUnmapBufferARB);
	LOAD_EXTENSION(PFNGLGETBUFFERPARAMETERIVARBPROC, glGetBufferParameterivARB);
	LOAD_EXTENSION(PFNGLGETBUFFERPOINTERVARBPROC, glGetBufferPointervARB);

	LOAD_EXTENSION(PFNGLVERTEXATTRIBPOINTERARBPROC, glVertexAttribPointerARB);
	LOAD_EXTENSION(PFNGLENABLEVERTEXATTRIBARRAYARBPROC, glEnableVertexAttribArrayARB);
	LOAD_EXTENSION(PFNGLDISABLEVERTEXATTRIBARRAYARBPROC, glDisableVertexAttribArrayARB);
	LOAD_EXTENSION(PFNGLPROGRAMSTRINGARBPROC, glProgramStringARB);
	LOAD_EXTENSION(PFNGLBINDPROGRAMARBPROC, glBindProgramARB);
	LOAD_EXTENSION(PFNGLDELETEPROGRAMSARBPROC, glDeleteProgramsARB);
	LOAD_EXTENSION(PFNGLGENPROGRAMSARBPROC, glGenProgramsARB);
	LOAD_EXTENSION(PFNGLPROGRAMENVPARAMETER4DARBPROC, glProgramEnvParameter4dARB);
	LOAD_EXTENSION(PFNGLPROGRAMENVPARAMETER4DVARBPROC, glProgramEnvParameter4dvARB);
	LOAD_EXTENSION(PFNGLPROGRAMENVPARAMETER4FARBPROC, glProgramEnvParameter4fARB);
	LOAD_EXTENSION(PFNGLPROGRAMENVPARAMETER4FVARBPROC, glProgramEnvParameter4fvARB);
	LOAD_EXTENSION(PFNGLPROGRAMLOCALPARAMETER4DARBPROC, glProgramLocalParameter4dARB);
	LOAD_EXTENSION(PFNGLPROGRAMLOCALPARAMETER4DVARBPROC, glProgramLocalParameter4dvARB);
	LOAD_EXTENSION(PFNGLPROGRAMLOCALPARAMETER4FARBPROC, glProgramLocalParameter4fARB);
	LOAD_EXTENSION(PFNGLPROGRAMLOCALPARAMETER4FVARBPROC, glProgramLocalParameter4fvARB);
	LOAD_EXTENSION(PFNGLGETPROGRAMENVPARAMETERDVARBPROC, glGetProgramEnvParameterdvARB);
	LOAD_EXTENSION(PFNGLGETPROGRAMENVPARAMETERFVARBPROC, glGetProgramEnvParameterfvARB);
	LOAD_EXTENSION(PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC, glGetProgramLocalParameterdvARB);
	LOAD_EXTENSION(PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC, glGetProgramLocalParameterfvARB);
	LOAD_EXTENSION(PFNGLGETPROGRAMIVARBPROC, glGetProgramivARB);
	LOAD_EXTENSION(PFNGLGETPROGRAMSTRINGARBPROC, glGetProgramStringARB);
	LOAD_EXTENSION(PFNGLGETVERTEXATTRIBDVARBPROC, glGetVertexAttribdvARB);
	LOAD_EXTENSION(PFNGLGETVERTEXATTRIBFVARBPROC, glGetVertexAttribfvARB);
	LOAD_EXTENSION(PFNGLGETVERTEXATTRIBIVARBPROC, glGetVertexAttribivARB);
	LOAD_EXTENSION(PFNGLGETVERTEXATTRIBPOINTERVARBPROC, glGetVertexAttribPointervARB);
	LOAD_EXTENSION(PFNGLISPROGRAMARBPROC, glIsProgramARB);

	LOAD_EXTENSION(PFNGLDELETEOBJECTARBPROC, glDeleteObjectARB);
	LOAD_EXTENSION(PFNGLGETHANDLEARBPROC, glGetHandleARB);
	LOAD_EXTENSION(PFNGLDETACHOBJECTARBPROC, glDetachObjectARB);
	LOAD_EXTENSION(PFNGLCREATESHADEROBJECTARBPROC, glCreateShaderObjectARB);
	LOAD_EXTENSION(PFNGLSHADERSOURCEARBPROC, glShaderSourceARB);
	LOAD_EXTENSION(PFNGLCOMPILESHADERARBPROC, glCompileShaderARB);
	LOAD_EXTENSION(PFNGLCREATEPROGRAMOBJECTARBPROC, glCreateProgramObjectARB);
	LOAD_EXTENSION(PFNGLATTACHOBJECTARBPROC, glAttachObjectARB);
	LOAD_EXTENSION(PFNGLLINKPROGRAMARBPROC, glLinkProgramARB);
	LOAD_EXTENSION(PFNGLUSEPROGRAMOBJECTARBPROC, glUseProgramObjectARB);
	LOAD_EXTENSION(PFNGLVALIDATEPROGRAMARBPROC, glValidateProgramARB);
	LOAD_EXTENSION(PFNGLUNIFORM1FARBPROC, glUniform1fARB);
	LOAD_EXTENSION(PFNGLUNIFORM2FARBPROC, glUniform2fARB);
	LOAD_EXTENSION(PFNGLUNIFORM3FARBPROC, glUniform3fARB);
	LOAD_EXTENSION(PFNGLUNIFORM4FARBPROC, glUniform4fARB);
	LOAD_EXTENSION(PFNGLUNIFORM1IARBPROC, glUniform1iARB);
	LOAD_EXTENSION(PFNGLUNIFORM2IARBPROC, glUniform2iARB);
	LOAD_EXTENSION(PFNGLUNIFORM3IARBPROC, glUniform3iARB);
	LOAD_EXTENSION(PFNGLUNIFORM4IARBPROC, glUniform4iARB);
	LOAD_EXTENSION(PFNGLUNIFORM1FVARBPROC, glUniform1fvARB);
	LOAD_EXTENSION(PFNGLUNIFORM2FVARBPROC, glUniform2fvARB);
	LOAD_EXTENSION(PFNGLUNIFORM3FVARBPROC, glUniform3fvARB);
	LOAD_EXTENSION(PFNGLUNIFORM4FVARBPROC, glUniform4fvARB);
	LOAD_EXTENSION(PFNGLUNIFORM1IVARBPROC, glUniform1ivARB);
	LOAD_EXTENSION(PFNGLUNIFORM2IVARBPROC, glUniform2ivARB);
	LOAD_EXTENSION(PFNGLUNIFORM3IVARBPROC, glUniform3ivARB);
	LOAD_EXTENSION(PFNGLUNIFORM4IVARBPROC, glUniform4ivARB);
	LOAD_EXTENSION(PFNGLUNIFORMMATRIX2FVARBPROC, glUniformMatrix2fvARB);
	LOAD_EXTENSION(PFNGLUNIFORMMATRIX3FVARBPROC, glUniformMatrix3fvARB);
	LOAD_EXTENSION(PFNGLUNIFORMMATRIX4FVARBPROC, glUniformMatrix4fvARB);
	LOAD_EXTENSION(PFNGLGETOBJECTPARAMETERFVARBPROC, glGetObjectParameterfvARB);
	LOAD_EXTENSION(PFNGLGETOBJECTPARAMETERIVARBPROC, glGetObjectParameterivARB);
	LOAD_EXTENSION(PFNGLGETINFOLOGARBPROC, glGetInfoLogARB);
	LOAD_EXTENSION(PFNGLGETATTACHEDOBJECTSARBPROC, glGetAttachedObjectsARB);
	LOAD_EXTENSION(PFNGLGETUNIFORMLOCATIONARBPROC, glGetUniformLocationARB);
	LOAD_EXTENSION(PFNGLGETACTIVEUNIFORMARBPROC, glGetActiveUniformARB);
	LOAD_EXTENSION(PFNGLGETUNIFORMFVARBPROC, glGetUniformfvARB);
	LOAD_EXTENSION(PFNGLGETUNIFORMIVARBPROC, glGetUniformivARB);
	LOAD_EXTENSION(PFNGLGETSHADERSOURCEARBPROC, glGetShaderSourceARB);

	LOAD_EXTENSION(PFNGLBINDATTRIBLOCATIONARBPROC, glBindAttribLocationARB);
	LOAD_EXTENSION(PFNGLGETACTIVEATTRIBARBPROC, glGetActiveAttribARB);
	LOAD_EXTENSION(PFNGLGETATTRIBLOCATIONARBPROC, glGetAttribLocationARB);
	
	LOAD_EXTENSION(PFNGLGETSHADERIVPROC, glGetShaderiv);
	LOAD_EXTENSION(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);

	LOAD_EXTENSION(PFNGLCLIENTACTIVETEXTUREARBPROC, glClientActiveTextureARB);

	LOAD_EXTENSION(PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC, glRenderbufferStorageMultisampleEXT);

#else
#ifndef __APPLE__
	LOAD_EXTENSION(PFNGLMAPBUFFEROESPROC, glMapBufferOES);
	LOAD_EXTENSION(PFNGLUNMAPBUFFEROESPROC, glUnmapBufferOES);
	LOAD_EXTENSION(PFNGLGETBUFFERPOINTERVOESPROC, glGetBufferPointervOES);
#endif
#endif

	
}

const char * errorString(GLenum he)
{
	switch(he)
	{
	case GL_NO_ERROR: return "";
	case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
#ifdef DRAWER_API_OPENGL
	case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
	case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
	case GL_TABLE_TOO_LARGE: return "GL_TABLE_TOO_LARGE";
#endif
	case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
	}
	return "UNDETECTED";
}


HALDevice::HALDevice(const DisplayHandle& display) : BaseOSDevice(display)
{
	init();
}

HALDevice::HALDevice(IFrameWindow*& outMainWnd, const WindowHandle& srcWainWindowHandle, boolean fullscrean, boolean stereo)  : BaseOSDevice(srcWainWindowHandle, fullscrean, stereo)
{
	if(isValid())
		outMainWnd = new FrameWindow(*this,true);
	else
		outMainWnd = 0;
	
	init();
}

void HALDevice::init()
{
	if(!isValid())
		return;
	
	context = new Context(*this);
	
	createQuadBuffer();

	for(uint i=0; i < EColor_EnumCount; ++i)
		supportTextureFormats[i] = false;

	_isBGRATextureSupported = false;

	ConstString extensions = constString((const char*)glGetString(GL_EXTENSIONS));

	supportTextureFormats[EColor_RGBA] = true;
	supportTextureFormats[EColor_RGB] = true;
	supportTextureFormats[EColor_L] = true;

	logInfo << "supported texture formats:";
	supportTextureFormats[EColor_ETC1] = extensions.contains(_CS("GL_OES_compressed_ETC1_RGB8_texture"));
	if(supportTextureFormats[EColor_ETC1])
		logInfo << "ETC1";

	supportTextureFormats[EColor_PVR2] = supportTextureFormats[EColor_PVR4] = 
	supportTextureFormats[EColor_PVR2A] = supportTextureFormats[EColor_PVR4A] = extensions.contains(_CS("GL_IMG_texture_compression_pvrtc"));
	if(supportTextureFormats[EColor_PVR4]){
		logInfo << "PVR2";
		logInfo << "PVR2A";
		logInfo << "PVR4";
		logInfo << "PVR4A";
	}

	supportTextureFormats[EColor_DXT1] = supportTextureFormats[EColor_DXT1A] = extensions.contains(_CS("GL_EXT_texture_compression_dxt1"));
	if(supportTextureFormats[EColor_DXT1]){
		logInfo << "DXT1";
		logInfo << "DXT1A";
	}
	supportTextureFormats[EColor_DXT3] = supportTextureFormats[EColor_DXT5] = extensions.contains(_CS("GL_EXT_texture_compression_s3tc")); 
	if(supportTextureFormats[EColor_DXT5]){
		logInfo << "DXT3";
		logInfo << "DXT5";
	}

	supportTextureFormats[EColor_ATC] = supportTextureFormats[EColor_ATCI] = extensions.contains(_CS("GL_AMD_compressed_ATC_texture")); 
	if(supportTextureFormats[EColor_ATCI]){
		logInfo << "ATC";
		logInfo << "ATCI";
	}

#ifdef DRAWER_API_OPENGL
	_isBGRATextureSupported = true;
#else
	_isBGRATextureSupported = extensions.contains(_CS("GL_EXT_texture_format_BGRA8888"));

	//Vivante ugly hack
	ConstString rendererName((const utf8*)glGetString(GL_RENDERER));
	if(rendererName.contains(_CS("Vivante")) ||
		rendererName.contains(_CS("GC400")) ||
		rendererName.contains(_CS("GC600")) ||
		rendererName.contains(_CS("GC800")) ||
		rendererName.contains(_CS("GC860")) ||
		rendererName.contains(_CS("GC880")) ||
		rendererName.contains(_CS("GC1000")) ||
		rendererName.contains(_CS("GC2000")) ||
		rendererName.contains(_CS("GC4000")) ||
		rendererName.contains(_CS("GC5000")) ||
		rendererName.contains(_CS("GC6000")) ||
		rendererName.contains(_CS("GC8000")))
		_isBGRATextureSupported = false;
#endif
	
	if(_isBGRATextureSupported){
		logInfo << "BGRA8";
	}
}

HALDevice::~HALDevice()
{
	unlockTempBuffers();
	
	delete context;
	context = 0;
	
	delete quadsIndexBuffer;
	quadsIndexBuffer = 0;
}

void HALDevice::createQuadBuffer()
{
	quadsIndexBuffer = createIndexBuffer();
	const uint quads = 1024;
	uint16* data = (uint16*)alloca(quads * 6 * sizeof(uint16));
	for(uint i=0; i < quads; ++i)
	{
		data[i*6 + 0] = i * 4 + 0;
		data[i*6 + 1] = i * 4 + 1;
		data[i*6 + 2] = i * 4 + 2;
		data[i*6 + 3] = i * 4 + 2;
		data[i*6 + 4] = i * 4 + 1;
		data[i*6 + 5] = i * 4 + 3;
	}
	quadsIndexBuffer->init(quads * 6, data);
}

IIndexBuffer* HALDevice::createIndexBuffer()
{
	return new IndexBuffer(*this);
}

IVertexBuffer* HALDevice::createVertexBuffer()
{
	return new VertexBuffer(*this);
}

ITexture* HALDevice::createTexture()
{
	return new Texture(*this);
}

IFrameWindow* HALDevice::createFrameWindow()
{
	return new FrameWindow(*this);
}

IFrameTexture* HALDevice::createFrameTexture()
{
	return new FrameTexture(*this);
}

IAsyncReader* HALDevice::createAsyncReader()
{
	return new AsyncReader(*this);
}

byte* HALDevice::lockTempVertexBuffer(IVertexBuffer* vb)
{
	//return 0;

#ifdef DRAWER_API_OPENGL
	if(!glMapBufferARB)
		return 0;
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, static_cast<VertexBuffer*>(vb)->glVBO());
	void* ret = glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
#else
	if(!glMapBufferOES)
		return 0;

	glMapBufferOES(GL_ARRAY_BUFFER, static_cast<VertexBuffer*>(vb)->glVBO());
	void* ret = glMapBufferOES(GL_ARRAY_BUFFER, GL_WRITE_ONLY_OES);
	glMapBufferOES(GL_ARRAY_BUFFER, 0);
#endif
	
	return (byte*)ret;
}

void HALDevice::unlockTempVertexBuffer(IVertexBuffer* vb)
{
	//return;

#ifdef DRAWER_API_OPENGL
	if(!glUnmapBufferARB)
		return;
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, static_cast<VertexBuffer*>(vb)->glVBO());
	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
#else
	if(!glUnmapBufferOES)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, static_cast<VertexBuffer*>(vb)->glVBO());
	glUnmapBufferOES(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
}

uint16* HALDevice::lockTempIndexBuffer(IIndexBuffer* ib)
{
	return 0;

#ifdef DRAWER_API_OPENGL
	if(!glMapBufferARB)
		return 0;
	
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, static_cast<IndexBuffer*>(ib)->glVBO());
	void* ret = glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, GL_WRITE_ONLY);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
#else
	if(!glMapBufferOES)
		return 0;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<IndexBuffer*>(ib)->glVBO());
	void* ret = glMapBufferOES(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY_OES);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
	
	return (uint16*)ret;
}

void HALDevice::unlockTempIndexBuffer(IIndexBuffer* ib)
{
	return;

#ifdef DRAWER_API_OPENGL
	if(!glUnmapBufferARB)
		return;
	
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, static_cast<IndexBuffer*>(ib)->glVBO());
	glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
#else
	if(!glUnmapBufferOES)
		return;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<IndexBuffer*>(ib)->glVBO());
	glUnmapBufferOES(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
}

boolean HALDevice::supportTextureFormat(EColor color)
{
	return color < EColor_EnumCount ? supportTextureFormats[color] : false;
}

String HALDevice::gpuName()
{
	return constString((const char*)glGetString(GL_RENDERER));
}

}

#endif
