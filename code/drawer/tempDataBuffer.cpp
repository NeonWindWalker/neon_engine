#include "tempDataBuffer.h"

namespace DrawerLib
{
	TempDataBuffer::TempDataBuffer(IDevice& dev, uint fr) : device(dev), frames(minimum<uint>(fr, maxFrames))
	{
		current = 0;
		for(uint i=0; i < frames; ++i)
		{
			vertexBuffer[i] = device.createVertexBuffer();
			indexBuffer[i] = device.createIndexBuffer();
		}
		vbOffset = 0;
		ibOffset = 0;
		ibStartOffset = vbStartOffset = 0;
	}

	byte* TempDataBuffer::getTempVertexData(IVertexBuffer*& outVertexBuffer, uint& outOffset, uint size)
	{
		size = alignToGreaterFast(size, 16);
		outVertexBuffer = vertexBuffer[current];
		byte* p = allocator.alloc(size, 16, 0);
		outOffset = vbOffset;
		vbElements.push(Range<byte>(p, size));
		vbOffset += size;
		return p;
	}

	uint16* TempDataBuffer::getTempIndexData(IIndexBuffer*& outIndexBuffer, uint& outOffset, uint size)
	{
		size = alignToGreaterFast(size, 8);
		outIndexBuffer = indexBuffer[current];
		uint16* p = (uint16*)allocator.alloc(size * 2, 16, 0);
		outOffset = ibOffset;
		ibElements.push(Range<uint16>(p, size));
		ibOffset += size;
		return p;
	}

	void TempDataBuffer::resizeInNeed()
	{
		if(vertexBuffer[current]->size() < vbOffset)
			vertexBuffer[current]->init(alignToGreaterFast(vbOffset + (vbOffset >> 4), 16), 0, true);

		if(indexBuffer[current]->count() < ibOffset)
			indexBuffer[current]->init(alignToGreaterFast(ibOffset + (ibOffset >> 4), 8), 0, true);
	}

	void TempDataBuffer::copyDataAndClear()
	{
		uint offset = vbStartOffset;
		foreach(it, vbElements){
			vertexBuffer[current]->copy(offset, it->first(), it->size());
			offset += it->size();
			Assert(offset <= vbOffset);
		}
		vbElements.clearSaveReserve();

		offset = ibStartOffset;
		foreach(it, ibElements){
			indexBuffer[current]->copy(offset, it->first(), it->size());
			offset += it->size();
			Assert(offset <= ibOffset);
		}
		ibElements.clearSaveReserve();

		ibOffset = 0;
		vbOffset = 0;
		allocator.freeTimeCoherenceOptimizedMode();
		current = (current + 1) % frames;
	}

	void TempDataBuffer::uploadData()
	{
		resizeInNeed();
		copyDataAndClear();
	}

	TempDataBufferThreadSafe::TempDataBufferThreadSafe(IDevice& dev, uint fr) : TempDataBuffer(dev, fr)
	{
		vbMem = 0;
		vbSize = 0;
		vbSeek = 0;
		ibMem = 0;
		ibSize = 0;
		ibSeek = 0;
	}

	TempDataBufferThreadSafe::~TempDataBufferThreadSafe()
	{
		if(vbMem)
			gSystemAllocator.free(vbMem);
		if(ibMem)
			gSystemAllocator.free(ibMem);
	}

	byte* TempDataBufferThreadSafe::getTempVertexData(IVertexBuffer*& outVertexBuffer, uint& outOffset, uint size)
	{
		size = alignToGreaterFast(size, 16);
		uint x = (uint)atomicAdd(vbSeek, size);
		if(x <= vbSize)
		{
			outVertexBuffer = vertexBuffer[current];
			outOffset = x - size;
			return vbMem + outOffset;
		}
		else
		{
			LockGuard lg(mutex);
			return TempDataBuffer::getTempVertexData(outVertexBuffer, outOffset, size);
		}
	}

	uint16* TempDataBufferThreadSafe::getTempIndexData(IIndexBuffer*& outIndexBuffer, uint& outOffset, uint size)
	{
		size = alignToGreaterFast(size, 8);
		uint x = (uint)atomicAdd(ibSeek, size);
		if(x <= ibSize)
		{
			outIndexBuffer = indexBuffer[current];
			outOffset = x - size;
			return ibMem + outOffset;
		}
		else
		{
			LockGuard lg(mutex);
			return TempDataBuffer::getTempIndexData(outIndexBuffer, outOffset, size);
		}
	}

	void TempDataBufferThreadSafe::uploadData()
	{
		TempDataBuffer::resizeInNeed();
		uint needVBSize = vbOffset;
		uint needIBSize = ibOffset;

		vertexBuffer[current]->copy(0, vbMem, minimum<uint>(vbSeek, vbSize));
		indexBuffer[current]->copy(0, ibMem, minimum<uint>(ibSeek, ibSize));
		TempDataBuffer::copyDataAndClear();

		if(needVBSize > vbSize){
			if(vbMem)
				gSystemAllocator.free(vbMem);
			vbMem = (byte*)gSystemAllocator.alloc(needVBSize, 16, 0);
			vbSize = needVBSize;
			allocator.free();
		}
		if(needIBSize > ibSize){
			if(ibMem)
				gSystemAllocator.free(ibMem);
			ibMem = (uint16*)gSystemAllocator.alloc(needIBSize * 2, 16, 0);
			ibSize = needIBSize;
			allocator.free();
		}
		
		vbOffset = vbStartOffset = vbSize;
		ibOffset = ibStartOffset = ibSize;
		vbSeek = 0;
		ibSeek = 0;
	}


}