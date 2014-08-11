#include "renderTarget.h"

#if defined(DRAWER_API_OPENGL) || defined(DRAWER_API_OPENGL_ES2)

namespace DrawerLib
{ 

FrameWindow::FrameWindow(HALDevice& dev, boolean isMainWindowHack) : BaseOSFrameWindow(dev, isMainWindowHack), basedev(dev)
{
}

FrameWindow::~FrameWindow()
{
}


FrameTexture::FrameTexture(HALDevice& dev) : IFrameTexture(dev), TextureBase(dev)
{
}

FrameTexture::~FrameTexture()
{
}

boolean FrameTexture::init(uint width, uint height, uint mips)
{
	return TextureBase::initRenderable(false, width, height, mips, basedev.isHdrMode() ? 2 : 0);
}

void FrameTexture::deinit()
{
	TextureBase::deinit();
}

Vector2i FrameTexture::size()const
{
	return Vector2i(TextureBase::width(), TextureBase::height());
}




RenderBufferColor::RenderBufferColor(HALDevice& dev, uint width, uint height, uint floaty, uint multisample)
			: basedev(dev), _glRBO(0), _width(0), _height(0), _floaty(false), _multisample(0)
{
	basedev.makeCurrentContext();

#ifdef DRAWER_API_OPENGL
	if(glGenRenderbuffersEXT)
	{
		if(multisample > 0 && !glRenderbufferStorageMultisampleEXT)
			return;
		glGenRenderbuffersEXT(1, &_glRBO);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, _glRBO);
		GLenum fmt = GL_RGBA8;
		if(floaty == 2)
			fmt = GL_RGBA16F_ARB;
		if(floaty == 4)
			fmt = GL_RGBA32F_ARB;
		GLenum he = glGetError();
		if(multisample == 0)
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, fmt, width, height);
		else
			glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, multisample, fmt, width, height);
		he = glGetError();
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
		if(he != GL_NO_ERROR){
			logError << "glRenderbufferStorage*EXT " << errorString(he);
			glDeleteRenderbuffersEXT(1, &_glRBO);
			_glRBO = 0;
			return;
		}
		_width = width;
		_height = height;
		_floaty = floaty;
		_multisample = multisample;
	}
#else
	if(multisample || floaty)
		return;
	glGenRenderbuffers(1, &_glRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, _glRBO);
	GLenum fmt = GL_RGBA8_OES;
	GLenum he = glGetError();
	glRenderbufferStorage(GL_RENDERBUFFER, fmt, width, height);
	he = glGetError();
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	if(he != GL_NO_ERROR){
		logError << "glRenderbufferStorage " << errorString(he);
		glDeleteRenderbuffers(1, &_glRBO);
		_glRBO = 0;
		return;
	}
	_width = width;
	_height = height;
	_floaty = floaty;
	_multisample = multisample;
#endif
}
RenderBufferColor::~RenderBufferColor()
{
	basedev.makeCurrentContext();

	if(_glRBO)
#ifdef DRAWER_API_OPENGL
		glDeleteRenderbuffersEXT(1, &_glRBO);
#else
		glDeleteRenderbuffers(1, &_glRBO);
#endif
}




RenderBufferDepthStensil::RenderBufferDepthStensil(HALDevice& dev, uint width, uint height, boolean floaty, boolean stensil, uint multisample)
		: basedev(dev), _glRBO(0), _width(0), _height(0), _floaty(false), _stensil(false), _multisample(0)
{
	basedev.makeCurrentContext();

#ifdef DRAWER_API_OPENGL
	if(glGenRenderbuffersEXT)
	{
		if(multisample > 0 && !glRenderbufferStorageMultisampleEXT)
			return;
		glGenRenderbuffersEXT(1, &_glRBO);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, _glRBO);
		GLenum fmt;
		if(floaty){
			if(stensil)
				fmt = GL_DEPTH32F_STENCIL8;//GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
			else
				fmt = GL_DEPTH_COMPONENT32F;
		}
		else{
			if(stensil)
				fmt = GL_DEPTH24_STENCIL8_EXT;
			else
				fmt = GL_DEPTH_COMPONENT24;
		}
		GLenum he = glGetError();
		if(multisample == 0)
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, fmt, width, height);
		else
			glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, multisample, fmt, width, height);
		he = glGetError();
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
		if(he != GL_NO_ERROR){
			logError << "glRenderbufferStorage*EXT " << errorString(he);
			glDeleteRenderbuffersEXT(1, &_glRBO);
			_glRBO = 0;
			return;
		}
		_width = width;
		_height = height;
		_floaty = floaty;
		_stensil = stensil;
		_multisample = multisample;
	}
#else
	if(multisample > 0 || floaty)
		return;
	glGenRenderbuffers(1, &_glRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, _glRBO);
	GLenum fmt;
	if(stensil)
		fmt = GL_DEPTH24_STENCIL8_OES;
	else
		fmt = GL_DEPTH_COMPONENT24_OES;
	GLenum he = glGetError();
	glRenderbufferStorage(GL_RENDERBUFFER, fmt, width, height);
	he = glGetError();
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	if(he != GL_NO_ERROR){
		logError << "glRenderbufferStorage*EXT " << errorString(he);
		glDeleteRenderbuffers(1, &_glRBO);
		_glRBO = 0;
		return;
	}
	_width = width;
	_height = height;
	_floaty = floaty;
	_stensil = stensil;
	_multisample = multisample;
#endif
}
RenderBufferDepthStensil::~RenderBufferDepthStensil()
{
	basedev.makeCurrentContext();

	if(_glRBO)
#ifdef DRAWER_API_OPENGL
		glDeleteRenderbuffersEXT(1, &_glRBO);
#else
		glDeleteRenderbuffers(1, &_glRBO);
#endif
}

}

#endif
