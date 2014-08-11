#include "device.h"
#include "resources.h"

namespace DrawerLib
{

byte* IDevice::getTempVertexData(IVertexBuffer*& outVertexBuffer, uint& outOffset, uint size)
{
	if(needAdditionalTempVertexBufferSize)
		return 0;

	size = alignToGreaterFast(size, 4);
	uint x = (uint)atomicAdd(tempVertexBufferDataSeek, size);
	if(x <= tempVertexBufferDataSize)
	{
		outVertexBuffer = tempVertexBuffers[0];
		outOffset = (uint)x - size;
		return tempVertexBufferDataPtr + outOffset;
	}
	else
	{
		outVertexBuffer = 0;
		outOffset = 0;
		needAdditionalTempVertexBufferSize = true;
		return 0;
	}
}

uint16* IDevice::getTempIndexData(IIndexBuffer*& outIndexBuffer, uint& outOffset, uint size)
{
	if(needAdditionalTempIndexBufferSize)
		return 0;

	uint x = (uint)atomicAdd(tempIndexBufferDataSeek, size);
	if(x <= tempIndexBufferDataSize)
	{
		outIndexBuffer = tempIndexBuffers[0];
		outOffset = (uint)x - size;
		return tempIndexBufferDataPtr + outOffset;
	}
	else
	{
		outIndexBuffer = 0;
		outOffset = 0;
		needAdditionalTempIndexBufferSize = true;
		return 0;
	}
}

void IDevice::tempDataUpload(uint buffersCount, uint minimalVertexBufferSize, uint minimalIndexBufferSize)
{
	Assert(buffersCount >= 2);

	for(uint i = buffersCount; i < tempVertexBuffers.size(); ++i)
		tempVertexBuffers[i]->selfDelete();

	for(uint i = buffersCount; i < tempIndexBuffers.size(); ++i)
		tempIndexBuffers[i]->selfDelete();

	tempVertexBuffers.resize(buffersCount);
	tempIndexBuffers.resize(buffersCount);

	for(uint i = 0; i < tempVertexBuffers.size(); ++i)
	{
		if(!tempVertexBuffers[i])
			tempVertexBuffers[i] = createVertexBuffer();

		if(!tempIndexBuffers[i])
			tempIndexBuffers[i] = createIndexBuffer();
	}

	if(tempVertexBufferDataPtr != tempVertexBufferReplicaData.first() && tempVertexBuffers[0]->size())
		unlockTempVertexBuffer(tempVertexBuffers[0]);

	if(tempIndexBufferDataPtr != tempIndexBufferReplicaData.first() && tempIndexBuffers[0]->count())
		unlockTempIndexBuffer(tempIndexBuffers[0]);
	
	if(tempVertexBufferReplicaData.size())
		tempVertexBuffers[0]->copy(0, tempVertexBufferReplicaData.first(), min((uint)tempVertexBufferDataSeek, tempVertexBufferDataSize));

	if(tempIndexBufferReplicaData.size())
		tempIndexBuffers[0]->copy(0, tempIndexBufferReplicaData.first(), min((uint)tempIndexBufferDataSeek, tempIndexBufferDataSize) );


	IVertexBuffer* vb = tempVertexBuffers[0];
	tempVertexBuffers.erase((uint)0);
	tempVertexBuffers.push(vb);

	IIndexBuffer* ib = tempIndexBuffers[0];
	tempIndexBuffers.erase((uint)0);
	tempIndexBuffers.push(ib);

	if(needAdditionalTempVertexBufferSize || tempVertexBuffers[0]->size() < max(tempVertexBufferDataSize, minimalVertexBufferSize))
	{
		uint newSize = max(tempVertexBufferDataSize, minimalVertexBufferSize);
		if(needAdditionalTempVertexBufferSize)
			maximize(newSize, tempVertexBufferDataSize + tempVertexBufferDataSize / 2 + 1);
		newSize = alignToGreater(newSize, 4096);
		tempVertexBuffers[0]->init(newSize, 0, true);
		logInfo << "resizing temp vertex buffer to " << newSize;
	}

	if(needAdditionalTempIndexBufferSize || tempIndexBuffers[0]->count() < max(tempIndexBufferDataSize, minimalIndexBufferSize))
	{
		uint newSize = max(tempIndexBufferDataSize, minimalIndexBufferSize);
		if(needAdditionalTempIndexBufferSize)
			maximize(newSize, tempIndexBufferDataSize + tempIndexBufferDataSize / 2 + 1);
		newSize = alignToGreater(newSize, 4096);
		tempIndexBuffers[0]->init(newSize, 0, true);
		logInfo << "resizing temp index buffer to " << newSize * 2;
	}

	tempVertexBufferDataSize = tempVertexBuffers[0]->size();
	tempVertexBufferDataSeek = 0;
	needAdditionalTempVertexBufferSize = false;
	tempVertexBufferDataPtr = lockTempVertexBuffer(tempVertexBuffers[0]);
	if(tempVertexBufferDataPtr){
		tempVertexBufferReplicaData.clear();
	}else{
		tempVertexBufferReplicaData.resize(tempVertexBufferDataSize);
		tempVertexBufferDataPtr = tempVertexBufferReplicaData.first();
	}
	Assert(tempVertexBufferDataPtr);
	
	tempIndexBufferDataSize = tempIndexBuffers[0]->count();
	tempIndexBufferDataSeek = 0;
	needAdditionalTempIndexBufferSize = false;
	tempIndexBufferDataPtr = lockTempIndexBuffer(tempIndexBuffers[0]);
	if(tempIndexBufferDataPtr){
		tempIndexBufferReplicaData.clear();
	}else{
		tempIndexBufferReplicaData.resize(tempIndexBufferDataSize);
		tempIndexBufferDataPtr = tempIndexBufferReplicaData.first();
	}
	Assert(tempIndexBufferDataPtr);
}

void IDevice::unlockTempBuffers()
{
	if(tempVertexBuffers.size() && tempVertexBufferDataPtr != tempVertexBufferReplicaData.first() && tempVertexBuffers[0]->size())
		unlockTempVertexBuffer(tempVertexBuffers[0]);

	if(tempIndexBuffers.size() && tempIndexBufferDataPtr != tempIndexBufferReplicaData.first() && tempIndexBuffers[0]->count())
		unlockTempIndexBuffer(tempIndexBuffers[0]);
}

IDevice::IDevice()
{
	tempVertexBufferDataPtr = 0;
	tempVertexBufferDataSize = 0;
	tempVertexBufferDataSeek = 0;
	needAdditionalTempVertexBufferSize = false;
	
	tempIndexBufferDataPtr = 0;
	tempIndexBufferDataSize = 0;
	tempIndexBufferDataSeek = 0;
	needAdditionalTempIndexBufferSize = false;
}


boolean ITexture::copy(uint mip, const BaseImage& img)
{
	if(img.width() == 0 || img.height() == 0)
		return false;

	if(format() == img.colortype())
		return copy(mip, img.rawData());

	if(img.colortype() == EColor_RGB || img.colortype() == EColor_LA)
	{
		StrongPtr< Image<ColorRGBA> > rgbaImg = ImageLib::convert< Image<ColorRGBA> >(img.baseProxy());
		return copy(mip, rgbaImg->rawData());
	}
	return false;
}

boolean ITexture::init(const BaseImage& img, uint genMips)
{
	boolean mips = img.isSizePowOf2() && !ImageLib::isPackedFormat(img.colortype()) && genMips;
	EColor fmt = img.colortype();
	if(fmt == EColor_RGB || fmt == EColor_LA)
		fmt =  EColor_RGBA;
	if(!init(img.unpackedWidth(), img.unpackedHeight(), fmt, mips ? 0 : 1))
		return false;

	this->copy(0, img);
	if(!mips)
		return true;

	StrongPtr<BaseImage> mip = metaGenerateMipmap(img.baseProxy());
	uint nMip = 1;
	while(mip)
	{
		copy(nMip++, *mip);
		mip = metaGenerateMipmap(mip->baseProxy());
	}
	return true;
}


boolean ITexture::init(ConstBinaryData rawfile, boolean genMips)
{
	ImageFileData imdData;
	if(!loadImage(imdData, rawfile))
		return false;

	uint mipsCount = imdData.mipsCount;
	if(genMips && !isPackedFormat(imdData.colorType))
		mipsCount = 0;

	if(!init(imdData.width, imdData.height, imdData.colorType, mipsCount))
		return false;

	uint mip = 0;
	for(; mip < imdData.mipsCount; ++mip)
		copy(mip, imdData.get(mip));

	if(mip < mips() && genMips)
	{
		uint lastMip = mip - 1;
		StrongPtr<BaseImage> mipImg = metaGenerateMipmap(imdData.get(lastMip).baseProxy());
		while(mipImg){
			copy(mip++, *mipImg);
			mipImg = metaGenerateMipmap(mipImg->baseProxy());
		}
	}

	return true;
}




}


