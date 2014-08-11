#pragma once

#include "device.h"
#include "vertexFormat.h"

namespace DrawerLib
{

enum EResourceType
{
	EResourceType_IndexBuffer=0,
	EResourceType_VertexBuffer,
	EResourceType_Texture,
	EResourceType_TextureCube,
	EResourceType_TextureVolume,
	EResourceType_FrameTexture,
	EResourceType_FrameWindow,
	EResourceType_AsyncReader,
	EResourceType_EnumCount
};

class IResource : public RefCountable
{
public:
	IResource(EResourceType srcType, IDevice& srcDevice) : _type(srcType), _device(srcDevice)
	{}
	
	INLINE EResourceType type()const { return _type; }
	INLINE IDevice& device() const { return _device; }
	
private:
	EResourceType _type;
	IDevice& _device;
};

class IIndexBuffer : public IResource
{
public:
	IIndexBuffer(IDevice& srcDevice) : IResource(EResourceType_IndexBuffer, srcDevice) {}
	virtual boolean init(uint count, const uint16* data=0, boolean dynamic=false)=0;
	virtual void deinit()=0;
	virtual boolean copy(uint offset, uint16* data, uint count)=0;
	virtual uint count()const=0;
};


class IVertexBuffer : public IResource
{
public:
	IVertexBuffer(IDevice& srcDevice) : IResource(EResourceType_VertexBuffer, srcDevice) {}
	virtual boolean init(uint size, const void* data=0, boolean dynamic=false)=0;
	virtual void deinit()=0;
	virtual boolean copy(uint offset, const void* data, uint size)=0;
	virtual uint size()const=0;
};

class ITexture : public IResource
{
public:
	ITexture(IDevice& srcDevice) : IResource(EResourceType_Texture, srcDevice) {}
	virtual boolean init(uint width, uint height, EColor format = EColor_RGBA, uint mips=0, boolean dynamic = false)=0;
	virtual void deinit()=0;
	virtual boolean copy(uint mip, uint x, uint y, const void* data, uint width, uint height)=0;
	virtual boolean copy(uint mip, const void* data)=0;
	virtual uint width()const=0;
	virtual uint height()const=0;
	virtual EColor format()const=0;
	virtual uint mips()const=0;

	boolean init(const BaseImage& img, uint genMips = true);
	boolean init(ConstBinaryData rawfile, uint genMips = true);
	boolean copy(uint mip, const BaseImage& img);
};

enum ECubeTexureFace
{
	ECubeTexureFace_PositiveX = 0,
	ECubeTexureFace_NegativeX = 1,
	ECubeTexureFace_PositiveY = 2,
	ECubeTexureFace_NegativeY = 3,
	ECubeTexureFace_PositiveZ = 4,
	ECubeTexureFace_NegativeZ = 5,
	ECubeTexureFace_Count = 6
};

class ITextureCube : public IResource
{
public:
	ITextureCube(IDevice& srcDevice) : IResource(EResourceType_TextureCube, srcDevice) {}
	virtual boolean init(uint lenght, EColor format = EColor_RGBA, uint mips=0)=0;
	virtual boolean copy(ECubeTexureFace face, uint mip, uint x, uint y, void* data, uint width, uint height)=0;
	virtual boolean copy(ECubeTexureFace face, uint mip, void* data)=0;
	virtual uint lenght()const=0;
	virtual EColor format()const=0;
	virtual uint mips()const=0;
};


class IFrame : public IResource
{
public:
	IFrame(EResourceType type, IDevice& srcDevice) : IResource(type, srcDevice) {}
	virtual Vector2i size()const=0;
};

class IFrameTexture : public IFrame
{
public:
	IFrameTexture(IDevice& srcDevice) : IFrame(EResourceType_FrameTexture, srcDevice) {}
	virtual boolean init(uint width, uint height, uint mips=0)=0;
	virtual void deinit()=0;
};

struct DisplayHandle;
struct WindowHandle;

class IFrameWindow : public IFrame
{
public:
	IFrameWindow(IDevice& srcDevice) : IFrame(EResourceType_FrameWindow, srcDevice) {}
	virtual boolean init(const WindowHandle& wnd, boolean fullscrean = false, boolean stereo = false)=0;
	virtual void present()=0;
	virtual void deinit()=0;
	virtual void resize()=0;
	virtual WindowHandle handle()const=0;
};

class IAsyncReader : public IResource
{
public:
	IAsyncReader(IDevice& srcDevice) : IResource(EResourceType_AsyncReader, srcDevice) {}
	virtual boolean init(uint delay, EColor color)=0;
	Delegate<void(float, void*, uint, uint, EColor)> onImageReadedFunc;
	virtual boolean checkIsReadComplete()=0;
};




}
 
