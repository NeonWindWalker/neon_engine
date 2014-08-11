#pragma once

#include "resources.h"

#if defined(DRAWER_API_OPENGL) || defined(DRAWER_API_OPENGL_ES2)

namespace DrawerLib
{ 

class FrameWindow : public BaseOSFrameWindow
{
public:
	FrameWindow(HALDevice& dev, boolean isMainWindowHack = false);
	~FrameWindow();

protected:
	HALDevice& basedev;
};

static INLINE FrameWindow* impl(IFrameWindow * p){ return static_cast<FrameWindow*>(p); }


class FrameTexture : public IFrameTexture, public TextureBase
{
public:
	FrameTexture(HALDevice& dev);
	~FrameTexture();

	boolean init(uint width, uint height, uint mips=0);
	void deinit();
	Vector2i size()const;
	
protected:
};

static INLINE FrameTexture* impl(IFrameTexture * p){ return static_cast<FrameTexture*>(p); }


class RenderBufferColor
{
public:
	RenderBufferColor(HALDevice& dev, uint width, uint height, uint floaty, uint multisample=0);
	~RenderBufferColor();
	boolean isValid()const { return _glRBO != 0; }
	uint width()const { return _width; }
	uint height()const { return _height; }
	uint floaty()const { return _floaty; }
	uint multisample()const { return _multisample; }
protected:
	HALDevice& basedev;
	GLuint _glRBO;
	uint _width, _height;
	uint _floaty;
	uint _multisample;
};

class RenderBufferDepthStensil
{
public:
	RenderBufferDepthStensil(HALDevice& dev, uint width, uint height, boolean floaty, boolean stensil, uint multisample=0);
	~RenderBufferDepthStensil();
	boolean isValid()const { return _glRBO != 0; }
	uint width()const { return _width; }
	uint height()const { return _height; }
	boolean floaty()const { return _floaty; }
	boolean stensil()const { return _stensil; }
	uint multisample()const { return _multisample; }
	GLuint glRBO()const { return _glRBO; }
protected:
	HALDevice& basedev;
	GLuint _glRBO;
	uint _width, _height;
	boolean _floaty, _stensil;
	uint _multisample;
};

static INLINE TextureBase * toBaseTex(IResource* res)
{
	if(!res)
		return 0;
	
	if(res->type() == EResourceType_Texture)
		return static_cast<Texture*>(res);
	else if(res->type() == EResourceType_FrameTexture)
		return static_cast<FrameTexture*>(res);

	return 0;
}

}

#endif
