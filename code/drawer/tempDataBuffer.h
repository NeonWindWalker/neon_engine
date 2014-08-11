#pragma once
#include "device.h"
#include "resources.h"

namespace DrawerLib
{

class TempDataBuffer : public ITempDataBuffer
{
public:
	TempDataBuffer(IDevice& dev, uint frames = 2);
	virtual byte* getTempVertexData(class IVertexBuffer*& outVertexBuffer, uint& outOffset, uint size);
	virtual uint16* getTempIndexData(class IIndexBuffer*& outIndexBuffer, uint& outOffset, uint size);
	void uploadData();

protected:
	IDevice& device;
	TempAllocator allocator;
	Array< Range<byte> > vbElements;
	Array< Range<uint16> > ibElements;
	uint ibOffset;
	uint vbOffset;
	uint ibStartOffset;
	uint vbStartOffset;
	enum MyEnum
	{
		maxFrames = 4
	};
	uint frames;
	uint current;
	StrongPtr<IIndexBuffer> indexBuffer[maxFrames];
	StrongPtr<IVertexBuffer> vertexBuffer[maxFrames];

	void resizeInNeed();
	void copyDataAndClear();
};

class TempDataBufferThreadSafe : public TempDataBuffer
{
public:
	TempDataBufferThreadSafe(IDevice& dev, uint frames = 2);
	~TempDataBufferThreadSafe();
	virtual byte* getTempVertexData(class IVertexBuffer*& outVertexBuffer, uint& outOffset, uint size);
	virtual uint16* getTempIndexData(class IIndexBuffer*& outIndexBuffer, uint& outOffset, uint size);
	void uploadData();

private:
	byte* vbMem;
	uint vbSize;
	volatile int vbSeek;
	uint16* ibMem;
	uint ibSize;
	volatile int ibSeek;
	Mutex mutex;
};

}