#pragma once

#include "seed.h"

namespace DrawerLib
{
	
class IResource;
class IIndexBuffer;
class IVertexBuffer;
class ITexture;
class ITextureCube;
class IFrameTexture;
class IFrameWindow;
class IAsyncReader;
struct DrawTask;

class IDevice : public ITempDataBuffer
{
public:
	IDevice();
	virtual ~IDevice(){}
	
	virtual IIndexBuffer* createIndexBuffer()=0;
	virtual IVertexBuffer* createVertexBuffer()=0;
	virtual ITexture* createTexture()=0;
	virtual ITextureCube* createTextureCube()=0;
	virtual IFrameTexture* createFrameTexture()=0;
	virtual IFrameWindow* createFrameWindow()=0;
	virtual IAsyncReader* createAsyncReader()=0;

	void tempDataUpload(uint buffersCount = 2, uint minimalVertexBufferSize = 0, uint minimalIndexBufferSize = 0);
	virtual void draw(Range<DrawTask*> parallelTasks)=0;
	INLINE void draw(DrawTask& task)
	{
		DrawTask* p = &task;
		draw(Range<DrawTask*>(&p, 1));
	}
	virtual boolean setSyncInterval(uint synk=1)=0;
	
	//thread safe methods
	byte* getTempVertexData(IVertexBuffer*& outVertexBuffer, uint& outOffset, uint size);
	uint16* getTempIndexData(IIndexBuffer*& outIndexBuffer, uint& outOffset, uint size);

	virtual boolean supportTextureFormat(EColor color)=0;
	virtual String gpuName()=0;
	
protected:	
	Array<IVertexBuffer*> tempVertexBuffers;
	Array<byte> tempVertexBufferReplicaData;
	byte* tempVertexBufferDataPtr;
	uint tempVertexBufferDataSize;
	volatile int tempVertexBufferDataSeek;
	volatile boolean needAdditionalTempVertexBufferSize;
	
	Array<IIndexBuffer*> tempIndexBuffers;
	Array<uint16> tempIndexBufferReplicaData;
	uint16* tempIndexBufferDataPtr;
	uint tempIndexBufferDataSize;
	volatile int tempIndexBufferDataSeek;
	volatile boolean needAdditionalTempIndexBufferSize;
	
	void unlockTempBuffers();

	virtual byte* lockTempVertexBuffer(IVertexBuffer* vb)=0;
	virtual void unlockTempVertexBuffer(IVertexBuffer* vb)=0;
	virtual uint16* lockTempIndexBuffer(IIndexBuffer* ib)=0;
	virtual void unlockTempIndexBuffer(IIndexBuffer* ib)=0;
}; 

}
