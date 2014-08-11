#include "image.h"

extern "C" unsigned char* myJpegLoadFunc(unsigned int* outWidth, unsigned int* outHeight, unsigned int* outChannels, const unsigned char* dataPtr, unsigned int dataSize, unsigned int yUp);
extern "C" unsigned int myJpegLoadDescFunc(unsigned int* outWidth, unsigned int* outHeight, unsigned int* outChannels, const unsigned char* dataPtr, unsigned int dataSize);

extern "C" unsigned char* myPNGLoadFunc(unsigned int* outWidth, unsigned int* outHeight, unsigned int* outChannels, unsigned int* outChannelBits, const unsigned char* dataPtr, unsigned int dataSize, unsigned int yUp);
extern "C" unsigned int myPNGLoadDescFunc(unsigned int* outWidth, unsigned int* outHeight, unsigned int* outChannels, unsigned int* outChannelBits, const unsigned char* dataPtr, unsigned int dataSize);


namespace ImageLib 
{

static INLINE uint32 ddsFoucCC(const char* ch)
{
	return uint32(ch[0]) | (uint32(ch[1])<<8) | (uint32(ch[2])<<16) | (uint32(ch[3])<<24);
}

template<class t_ColorType>
static INLINE void loadCompressedData(ImageFileData::Array_t& outSet, EndianSafeConstBinaryDataAccessor& reader, uint w, uint h)
{
	Image<t_ColorType>* im = new Image<t_ColorType>(maximum<uint>(w / t_ColorType::gWidth, t_ColorType::gMinimalBlocksX), maximum<uint>(h / t_ColorType::gHeight, t_ColorType::gMinimalBlocksY));
	reader.read(im->first(), sizeof(t_ColorType) * im->width() * im->height());
	*outSet.push() = im;
}

template<class t_ColorType>
static INLINE void loadUncompressedData(ImageFileData::Array_t& outSet, EndianSafeConstBinaryDataAccessor& reader, uint w, uint h)
{
	Image<t_ColorType>* im = new Image<t_ColorType>(w, h);
	reader.read(im->first(), sizeof(t_ColorType) * im->width() * im->height());
	*outSet.push() = im;
}

static INLINE void loadData(ImageFileData::Array_t& outSet, EndianSafeConstBinaryDataAccessor& reader, uint w, uint h, EColor fmt)
{
	switch(fmt)
	{
		case EColor_ETC1:
			loadCompressedData<ColorETC1>(outSet, reader, w, h);
			break;
		case EColor_PVR2:
			loadCompressedData<ColorPVR2>(outSet, reader, w, h);
			break;
		case EColor_PVR2A:
			loadCompressedData<ColorPVR2A>(outSet, reader, w, h);
			break;
		case EColor_PVR4:
			loadCompressedData<ColorPVR4>(outSet, reader, w, h);
			break;
		case EColor_PVR4A:
			loadCompressedData<ColorPVR4A>(outSet, reader, w, h);
			break;
		case EColor_DXT1:
			loadCompressedData<ColorDXT1>(outSet, reader, w, h);
			break;
		case EColor_DXT1A:
			loadCompressedData<ColorDXT1A>(outSet, reader, w, h);
			break;
		case EColor_DXT3:
			loadCompressedData<ColorDXT3>(outSet, reader, w, h);
			break;
		case EColor_DXT5:
			loadCompressedData<ColorDXT5>(outSet, reader, w, h);
			break;
		case EColor_ATC:
			loadCompressedData<ColorATC>(outSet, reader, w, h);
			break;
		case EColor_ATCI:
			loadCompressedData<ColorATCA>(outSet, reader, w, h);
			break;
		case EColor_RGBA:
			loadUncompressedData<ColorRGBA>(outSet, reader, w, h);
			break;
		case EColor_RGB:
			loadUncompressedData<ColorRGB>(outSet, reader, w, h);
			break;
		case EColor_L:
			loadUncompressedData<ColorL>(outSet, reader, w, h);
			break;
	}
}

const void* getImageFileInfoAndGetDataPtr(ImageFileInfo& info, EImageFormat& outImgFormat, boolean& yUp, ConstBinaryData data)
{
	if(data.byteSize() < 16)
		return 0;

	const byte* header = (const byte*)data.first();
	
	if(header[0] == 0x42 && header[1] == 0x4d)//bmp
	{
		EndianSafeConstBinaryDataAccessor a(data);
		a.dseek(2);

		uint32  bfSize;
		uint16  bfReserved1;
		uint16  bfReserved2;
		uint32  bfOffBits;

		uint32  biSize; 
		int32  biWidth;
		int32  biHeight;
		uint16  biPlanes;
		uint16  biBitCount;
		uint32  biCompression;
		uint32  biSizeImage;
		uint32  biXPelsPerMeter;
		uint32  biYPelsPerMeter;
		uint32  biClrUsed;
		uint32  biClrImportant;

		a>>bfSize;
		a>>bfReserved1;
		a>>bfReserved2;
		a>>bfOffBits;
		a>>biSize;
		a>>biWidth;
		a>>biHeight;
		a>>biPlanes;
		a>>biBitCount;
		a>>biCompression;
		a>>biSizeImage;
		a>>biXPelsPerMeter;
		a>>biYPelsPerMeter;
		a>>biClrUsed;
		a>>biClrImportant;

		a.seek(bfOffBits);

		if(biCompression != 0){
			logWarning << "compressed bmp not supported";
			return 0;
		}

		info.clear();
		info.width = abs(biWidth);
		info.height = abs(biHeight);
		outImgFormat = EImageFormat_BMP;
		yUp = biHeight > 0;

		switch(biBitCount)
		{
		case 32:
			info.colorType = EColor_RGBA;
			return a.seekPtr();

		case 24:
			info.colorType = EColor_RGB;
			return a.seekPtr();

		case 8:
			info.colorType = EColor_L;
			return a.seekPtr();

		default:
			logWarning << "unsupported bmp bitperpixel " << biBitCount;
			return 0;
		}
	}
	else if(header[0] == 0xff && header[1] == 0xd8)
	{
		info.clear();
		uint channels;
		if(!myJpegLoadDescFunc(&info.width, &info.height, &channels, (const byte*)data.first(), data.byteSize())){
			logWarning << "corrupted jpeg file";
			return 0;
		}
		info.colorType = channels == 3 ? EColor_RGB : EColor_L;
		outImgFormat = EImageFormat_JPG;
		yUp = false;
		return data.first();
	}
#ifndef IMAGELIB_NO_PNG
	else if(header[0] == 0x89 && header[1] == 0x50)
	{
		info.clear();
		uint channels;
		uint bits;
		if(!myPNGLoadDescFunc(&info.width, &info.height, &channels, &bits, (const byte*)data.first(), data.byteSize())){
			logWarning << "corrupted png file";
			return 0;
		}
		switch(channels)
		{
		case 1:
			info.colorType = EColor_L;
			break;
		case 2:
			info.colorType = EColor_LA;
			break;
		case 3:
			info.colorType = EColor_RGB;
			break;
		case 4:
			info.colorType = EColor_RGBA;
			break;
		}
		outImgFormat = EImageFormat_PNG;
		yUp = false;
		return data.first();
	}
#endif
	else if(header[0] == 0xAB && header[1] == 0x4B && header[2] == 0x54 && header[3] == 0x58 && header[4] == 0x20) //ktx
	{	
		ConstBinaryDataAccessor a(data);
		a.dseek(12);

		uint32 endianness;
		a >> endianness;

		uint32 glType;
		a >> glType;
		uint32 glTypeSize;
		a >> glTypeSize;
		uint32 glFormat;
		a >> glFormat;
		uint32 glInternalFormat;
		a >> glInternalFormat;
		uint32 glBaseInternalFormat;
		a >> glBaseInternalFormat;
		uint32 pixelWidth;
		a >> pixelWidth;
		uint32 pixelHeight;
		a >> pixelHeight;
		uint32 pixelDepth;
		a >> pixelDepth;
		uint32 numberOfArrayElements;
		a >> numberOfArrayElements;
		uint32 numberOfFaces;
		a >> numberOfFaces;
		uint32 numberOfMipmapLevels;
		a >> numberOfMipmapLevels;
		uint32 bytesOfKeyValueData;
		a >> bytesOfKeyValueData;

		if(endianness != 0x04030201)
		{
			glType = byteReverse(glType);
			glTypeSize = byteReverse(glTypeSize);
			glFormat = byteReverse(glFormat);
			glInternalFormat = byteReverse(glInternalFormat);
			glBaseInternalFormat = byteReverse(glBaseInternalFormat);
			pixelWidth = byteReverse(pixelWidth);
			pixelHeight = byteReverse(pixelHeight);
			pixelDepth = byteReverse(pixelDepth);
			numberOfArrayElements = byteReverse(numberOfArrayElements);
			numberOfFaces = byteReverse(numberOfFaces);
			numberOfMipmapLevels = byteReverse(numberOfMipmapLevels);
			bytesOfKeyValueData = byteReverse(bytesOfKeyValueData);
		}

		a.dseek(bytesOfKeyValueData);
		
		info.clear();
		switch(glInternalFormat)
		{
		case 0x8D64: //GL_ETC1_RGB8_OES
			info.colorType = EColor_ETC1;
			break;
		case 0x8C01: //GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG
			info.colorType = EColor_PVR2;
			break;
		case 0x8C00: //GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG
			info.colorType = EColor_PVR4;
			break;
		case 0x8C03: //GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG
			info.colorType = EColor_PVR2A;
			break;
		case 0x8C02: //GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG
			info.colorType = EColor_PVR4A;
			break;
		case 0x8C92: //GL_ATC_RGB_AMD
			info.colorType = EColor_ATC;
			break;
		case 0x87EE: //GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD
			info.colorType = EColor_ATCI;
			break;
		default:
			return 0;
		}                

		info.width = pixelWidth;
		info.height = pixelHeight;
		info.depthSlicesCount = maximum<uint>(pixelDepth, 1);
		info.arrayElementsCount = maximum<uint>(numberOfArrayElements, 1);
		info.facesCount = maximum<uint>(numberOfFaces, 1);
		info.mipsCount = maximum<uint>(numberOfMipmapLevels, 1); 
		outImgFormat = EImageFormat_KTX;
		yUp = true;

		return a.seekPtr();
	}
	else if(header[0] == 0x44 && header[1] == 0x44 && header[2] == 0x53 && header[3] == 0x20) //dds
	{	
		EndianSafeConstBinaryDataAccessor a(data);
		a.dseek(4);

		info.clear();

		uint32	dwSize;
		a >>	dwSize;
		uint32	dwFlags;
		a >>	dwFlags;
		uint32	dwHeight;
		a >>	dwHeight;
		uint32	dwWidth;
		a >>	dwWidth;
		uint32	dwPitchOrLinearSize;
		a >>	dwPitchOrLinearSize;
		uint32	dwDepth;
		a >>	dwDepth;
		uint32	dwMipMapCount;
		a >>	dwMipMapCount;
		//uint32 dwReserved1[11];
		a.dseek(4*11);
		uint32	ddspf_dwSize;
		a >>	ddspf_dwSize;
		uint32	ddspf_dwFlags;
		a >>	ddspf_dwFlags;
		uint32	ddspf_dwFourCC;
		a >>	ddspf_dwFourCC;
		uint32	ddspf_dwRGBBitCount;
		a >>	ddspf_dwRGBBitCount;
		uint32	ddspf_dwRBitMask;
		a >>	ddspf_dwRBitMask;
		uint32	ddspf_dwGBitMask;
		a >>	ddspf_dwGBitMask;
		uint32	ddspf_dwBBitMask;
		a >>	ddspf_dwBBitMask;
		uint32	ddspf_dwABitMask;
		a >>	ddspf_dwABitMask;
		uint32	dwCaps;
		a >>	dwCaps;
		uint32	dwCaps2;
		a >>	dwCaps2;
		uint32	dwCaps3;
		a >>	dwCaps3;
		uint32	dwCaps4;
		a >>	dwCaps4;
		uint32	dwReserved2;
		a >>	dwReserved2;

		uint32 dx10_dxgiFormat;
		uint32 dx10_resourceDimension;
		uint32 dx10_miscFlag;
		uint32 dx10_arraySize;
		uint32 dx10_miscFlags2;

		if(ddspf_dwFourCC == ddsFoucCC("DX10"))
		{
			a >> dx10_dxgiFormat;
			a >> dx10_resourceDimension;
			a >> dx10_miscFlag;
			a >> dx10_arraySize;
			a >> dx10_miscFlags2;

			info.arrayElementsCount = maximum<uint>(dx10_arraySize, 1);

			switch(dx10_dxgiFormat)
			{
			case 71: //DXGI_FORMAT_BC1_UNORM
				info.colorType = (ddspf_dwFlags & 0x1/*DDPF_ALPHAPIXELS*/) ? EColor_DXT1A : EColor_DXT1;
				break;
			case 74: //DXGI_FORMAT_BC2_UNORM
				info.colorType = EColor_DXT3;
				break;
			case 77: //DXGI_FORMAT_BC3_UNORM
				info.colorType = EColor_DXT5;
				break;
			case 87: //DXGI_FORMAT_B8G8R8A8_UNORM
				info.colorType = EColor_RGBA;
				break;
			case 61: //DXGI_FORMAT_R8_UNORM
				info.colorType = EColor_L;
				break;
			default:
				logInfo << "unsupported dxgiFormat " << dx10_dxgiFormat;
				return 0;
			}
		}
		else
		{
			if(ddspf_dwFlags & 0x04/*DDPF_FOURCC*/)
			{
				if(ddspf_dwFourCC == ddsFoucCC("DXT1"))
				{
					info.colorType = (ddspf_dwFlags & 0x1/*DDPF_ALPHAPIXELS*/) ? EColor_DXT1A : EColor_DXT1;
				}
				else if(ddspf_dwFourCC == ddsFoucCC("DXT3"))
				{
					info.colorType = EColor_DXT3;
				}
				else if(ddspf_dwFourCC == ddsFoucCC("DXT5"))
				{
					info.colorType = EColor_DXT5;
				}
				else if(ddspf_dwFourCC == ddsFoucCC("ETC1"))
				{
					info.colorType = EColor_ETC1;
				}
				else if(ddspf_dwFourCC == ddsFoucCC("ATC "))
				{
					info.colorType = EColor_ATC;
				}
				else if (ddspf_dwFourCC == ddsFoucCC("ATCI"))
				{
					info.colorType = EColor_ATCI;
				}
				else
				{
					logInfo << "unknown fourcc format " << ddspf_dwFourCC;
					return 0;
				}
			}
			else
			{
				if(ddspf_dwFlags & 0x40/*DDPF_RGB*/)
				{
					if(ddspf_dwFlags & 0x1/*DDPF_ALPHAPIXELS*/) 
					{
						if(ddspf_dwRGBBitCount == 32)
						{
							info.colorType = EColor_RGBA;
						}
						else
						{
							logInfo << "unsupported bit count for rgba " << ddspf_dwRGBBitCount;
							return 0;
						}
					}
					else
					{
						if(ddspf_dwRGBBitCount == 24)
						{
							info.colorType = EColor_RGB;
						}
						else
						{
							logInfo << "unsupported bit count for rgb " << ddspf_dwRGBBitCount;
							return 0;
						}
					}
				}
				else if(ddspf_dwFlags & 0x20000/*DDPF_LUMINANCE*/) 
				{
					if(ddspf_dwRGBBitCount == 8)
					{
						info.colorType = EColor_L;
					}
					else
					{
						logInfo << "unsupported bit count for luminance "  << ddspf_dwRGBBitCount;
						return 0;
					}
				}
			}
		}

		info.width = dwWidth;
		info.height = dwHeight;
		info.depthSlicesCount = maximum<uint>(dwDepth, 1);
		info.facesCount = (dwCaps2 & 0x200/*DDSCAPS2_CUBEMAP*/) ? 6 : 1;
		info.mipsCount = maximum<uint>(dwMipMapCount, 1);
		outImgFormat = EImageFormat_DDS;
		yUp = false;
		return a.seekPtr();
	}

	logWarning << "unknown image format";
	return 0;
}


boolean loadImage(ImageFileData& ret, ConstBinaryData rawData, boolean yUp)
{
	EImageFormat srcImageFormat;
	boolean srcImageYUp;
	const void* pData = getImageFileInfoAndGetDataPtr(ret, srcImageFormat, srcImageYUp, rawData);
	if(!pData)
		return false;

	ret.images.clearSaveReserve();
	srcImageYUp = boolean01FromBoolean(srcImageYUp);
	yUp = boolean01FromBoolean(yUp);

	EndianSafeConstBinaryDataAccessor a(ConstBinaryData(pData, rawData.end()));

	switch(srcImageFormat)
	{
	case EImageFormat_BMP:
		{
			switch(ret.colorType)
			{
			case EColor_RGBA:
				{
					Image<ColorRGBA>* img = new Image<ColorRGBA>(ret.width, ret.height);
					for(uint j=0; j < img->height(); ++j){
						for(uint i=0; i < img->width(); ++i)
						{
							ColorRGBA c(255,255,255,255);
							a >> c.b >> c.g >> c.r >> c.a;
							if(srcImageYUp == yUp)
								(*img)(i, j) = c;
							else
								(*img)(i, img->height() - 1 - j) = c;
						}
					}
					*ret.images.push() = img;
					return true;
				}
			case EColor_RGB:
				{
					Image<ColorRGB>* img = new Image<ColorRGB>(ret.width, ret.height);
					for(uint j=0; j < img->height(); ++j){
						for(uint i=0; i < img->width(); ++i)
						{
							ColorRGB c;
							a >> c.b >> c.g >> c.r;
							if(srcImageYUp == yUp)
								(*img)(i, j) = c;
							else
								(*img)(i, img->height() - 1 - j) = c;
						}
						uint linesize = img->width() * 3;
						if(linesize & 3)
							a.dseek(4 - (linesize & 3));
					}
					*ret.images.push() = img;
					return true;
				}
			case EColor_L:
				{
					Image<byte>* img = new Image<byte>(ret.width, ret.height);
					for(uint j=0; j < img->height(); ++j){
						for(uint i=0; i < img->width(); ++i)
						{
							byte c;
							a >> c;
							if(srcImageYUp == yUp)
								(*img)(i, j) = c;
							else
								(*img)(i, img->height() - 1 - j) = c;
						}
						uint linesize = img->width();
						if(linesize & 3)
							a.dseek(4 - (linesize & 3));
					}
					*ret.images.push() = img;
					return true;
				}
			}
		}
		break;

	case EImageFormat_JPG:
		{
			uint width, height, channels;
			byte* ptr = myJpegLoadFunc(&width, &height, &channels, (const byte*)rawData.first(), rawData.byteSize(), yUp);
			if(!ptr){
				logError << "cant load jpeg";
				return 0;
			}

			switch(channels)
			{
			case 1:
				{
					ImageLib::Image<ImageLib::ColorL>* im = new ImageLib::Image<ImageLib::ColorL>(width, height);
					memoryCopy(im->first(), ptr, width * height * sizeof(ImageLib::ColorL));
					free(ptr);
					*ret.images.push() = im;
					return true;
				}
				break;
			case 3:
				{
					ImageLib::Image<ImageLib::ColorRGB>* im = new ImageLib::Image<ImageLib::ColorRGB>(width, height);
					for(uint j=0; j < im->height(); ++j)
						for(uint i=0; i < im->width(); ++i)
							im->at(i,j).set(ptr[(width*j + i)*3 + 0], ptr[(width*j + i)*3 + 1], ptr[(width*j + i)*3 + 2]);
							
					free(ptr);
					*ret.images.push() = im;
					return true;
				}
				break;
			case 4:
				{
					ImageLib::Image<ImageLib::ColorRGBA>* im = new ImageLib::Image<ImageLib::ColorRGBA>(width, height);
					for(uint j=0; j < im->height(); ++j)
						for(uint i=0; i < im->width(); ++i)
							im->at(i,j).set(ptr[(width*j + i)*3 + 0], ptr[(width*j + i)*3 + 1], ptr[(width*j + i)*3 + 2], ptr[(width*j + i)*3 + 3]);
					free(ptr);
					*ret.images.push() = im;
					return true;
				}
				break;
			}
			free(ptr);
			return false;
		}
		break;
#ifndef IMAGELIB_NO_PNG
	case EImageFormat_PNG:
		{
			uint width, height, channels, bits;
			byte* ptr = myPNGLoadFunc(&width, &height, &channels, &bits, (const byte*)rawData.first(), rawData.byteSize(), yUp);
			if(!ptr){
				logError << "cant load png";
				return 0;
			}

			if(bits != 8){
				logError << "pnt " << bits << "bits not implemented yet";
				free(ptr);
			}

			switch(channels)
			{
			case 1:
				{
					ImageLib::Image<ImageLib::ColorL>* im = new ImageLib::Image<ImageLib::ColorL>(width, height);
					memoryCopy(im->first(), ptr, width * height * sizeof(ImageLib::ColorL));
					free(ptr);
					*ret.images.push() = im;
					return true;
				}
				break;
			case 2:
				{
					ImageLib::Image<ImageLib::ColorLA>* im = new ImageLib::Image<ImageLib::ColorLA>(width, height);
					for(uint j = 0; j < im->height(); ++j)
						for(uint i = 0; i < im->width(); ++i)
						  im->at(i, j).set(ptr[(width*j + i) * 2 + 0], ptr[(width*j + i) * 2 + 1]);

					free(ptr);
					*ret.images.push() = im;
					return true;
				}
				break;
			case 3:
				{
					ImageLib::Image<ImageLib::ColorRGB>* im = new ImageLib::Image<ImageLib::ColorRGB>(width, height);
					for(uint j = 0; j < im->height(); ++j)
						for(uint i = 0; i < im->width(); ++i)
							im->at(i, j).set(ptr[(width*j + i) * 3 + 0], ptr[(width*j + i) * 3 + 1], ptr[(width*j + i) * 3 + 2]);
					free(ptr);
					*ret.images.push() = im;
					return true;
				}
				break;
			case 4:
				{
					ImageLib::Image<ImageLib::ColorRGBA>* im = new ImageLib::Image<ImageLib::ColorRGBA>(width, height);
					for(uint j = 0; j < im->height(); ++j)
						for(uint i = 0; i < im->width(); ++i)
							im->at(i, j).set(ptr[(width*j + i) * 4 + 0], ptr[(width*j + i) * 4 + 1], ptr[(width*j + i) * 4 + 2], ptr[(width*j + i) * 4 + 3]);
					free(ptr);
					*ret.images.push() = im;
					return true;
				}
				break;
			}
			free(ptr);
			return false;
		}
		break;
#endif
	case EImageFormat_KTX:
		{
			Assert((reinterpret_cast<uinteger>(pData) & 3) == 0);

			for(uint mip = 0; mip < ret.mipsCount; ++ mip)
			{
				uint32 imageSize;
				a >> imageSize;

				for(uint n = 0; n < ret.arrayElementsCount; ++ n)
				{
					for(uint face = 0; face < ret.facesCount; ++ face)
					{
						for(uint z = 0; z < ret.depthSlicesCount; ++ z)
						{
							loadData(ret.images, a, maximum<uint>(ret.width >> mip, 1), maximum<uint>(ret.height >> mip, 1), ret.colorType);
						}
						if(a.seek() & 3)
							a.dseek(4 - (a.seek() & 3));
					}
				}
				if(a.seek() & 3)
					a.dseek(4 - (a.seek() & 3));
			}
			return true;
		}
		break;
	case EImageFormat_DDS:
		{
			Assert((reinterpret_cast<uinteger>(pData) & 3) == 0);

			for(uint n = 0; n < ret.arrayElementsCount; ++ n)
			{
				for(uint mip = 0; mip < ret.mipsCount; ++ mip)
				{
					for(uint face = 0; face < ret.facesCount; ++ face)
					{
						for(uint z = 0; z < ret.depthSlicesCount; ++ z)
						{
							loadData(ret.images, a, maximum<uint>(ret.width >> mip, 1), maximum<uint>(ret.height >> mip, 1), ret.colorType);
						}
					}
				}
			}
			return true;
		}
		break;
	}

	return false;
}

boolean saveBmpImage(BinaryData& data, const ConstBaseImageProxy& image, boolean yUp, ESaveBMPBits bits)
{
	EndianSafeBinaryDataAccessor a(data);

	uint16  biBitCount = 0;
	if(bits != ESaveBMPBits_AutoDetectOptimizedCases && bits != ESaveBMPBits_FromSource)
	{
		biBitCount = bits;
		if(biBitCount != 8 && biBitCount != 24 && biBitCount != 32){
			logError << "unsupported bits count " << biBitCount;
			return false;
		}
	}
	else if(bits ==  ESaveBMPBits_AutoDetectOptimizedCases)
	{
		if(image.colortype() == EColor_RGBA)
		{
			ConstImageProxy<ColorRGBA> img;
			boolean res = image.cast(img);
			Assert(res);
			boolean bAlphaOne = true;
			boolean bMonoxrome = true;
			for(uint j=0; j < img.height(); ++j)
				for(uint i=0; i < img.width(); ++i)
				{

					if(img.at(i, j).a != 255)
						bAlphaOne = false;
					if(img.at(i, j).r != img.at(i, j).g && img.at(i, j).r != img.at(i, j).b)
						bMonoxrome = false;
				}

			if(bAlphaOne)
			{
				/*
				if(bMonoxrome)
					biBitCount = 8;
				else
					biBitCount = 24;
					*/
				biBitCount = 24;
			}
		}
		/*else if(image.colortype() == EColor_RGB)
		{
			ConstImageProxy<ColorRGB> img;
			boolean res = image.cast(img);
			Assert(res);
			boolean bMonoxrome = true;
			for(uint j=0; j < img.height(); ++j)
				for(uint i=0; i < img.width(); ++i)
				{
					if(img.at(i, j).r != img.at(i, j).g && img.at(i, j).r != img.at(i, j).b)
						bMonoxrome = false;
				}

			if(bMonoxrome)
				biBitCount = 8;
		}*/
	}
	
	if(!biBitCount)
		switch(image.colortype())
		{
			case EColor_RGBA: biBitCount = 32; break;
			case EColor_RGB: biBitCount = 24; break;
			case EColor_L: biBitCount = 8; break;
			default:
				logError << "unsupported image format " << image.colortype();
				data.clear();
				return false;
		}
	boolean grayscale = biBitCount == 8;

	uint sizeof_BITMAPFILEHEADER = 14;
	uint sizeof_BITMAPINFOHEADER = 40;
	
	uint16 bfType = 0x4d42; 
	uint32  bfSize = sizeof_BITMAPFILEHEADER;
	uint16  bfReserved1 = 0; 
	uint16  bfReserved2 = 0; 
	uint32  bfOffBits = sizeof_BITMAPFILEHEADER + sizeof_BITMAPINFOHEADER + 2;
	if(grayscale)
		bfOffBits += 1024;

	uint32  biSize = sizeof_BITMAPINFOHEADER;  
	int32   biWidth = image.width();
	int32   biHeight = (int32)image.height();
	uint16  biPlanes = 1; 
	
	uint32  biCompression = 0; 
	uint32  biSizeImage = 0; 
	int32  biXPelsPerMeter = image.height();
	int32  biYPelsPerMeter = image.height();
	uint32  biClrUsed = 0; 
	uint32  biClrImportant = 0;
	
	a<<bfType;
	a<<bfSize;
	a<<bfReserved1;
	a<<bfReserved2;
	a<<bfOffBits;
	a<<biSize;
	a<<biWidth;
	a<<biHeight;
	a<<biPlanes;
	a<<biBitCount;
	a<<biCompression;
	a<<biSizeImage;
	a<<biXPelsPerMeter;
	a<<biYPelsPerMeter;
	a<<biClrUsed;
	a<<biClrImportant;

	a<<uint16(0);

	/*
	if(grayscale)
		for(uint c=0; c < 256; ++c)
		{
			a<<uint8(c);
			a<<uint8(c);
			a<<uint8(c);
		}*/

	switch(image.colortype())
	{
	case EColor_RGBA:
		{
			ConstImageProxy<ColorRGBA> img;
			boolean res = image.cast(img);
			Assert(res);
			if(biBitCount == 32)
			{
				for(uint j=0; j < img.height(); ++j)
					for(uint i=0; i < img.width(); ++i)
					{
						ColorRGBA c = img(i, !yUp ? img.height() - 1 - j : j);
						a << c.b << c.g << c.r << c.a;
					}
			}
			else if(biBitCount == 24)
			{
				for(uint j=0; j < img.height(); ++j)
				{
					for(uint i=0; i < img.width(); ++i)
					{
						ColorRGBA c = img(i, !yUp ? img.height() - 1 - j : j);
						a << c.b << c.g << c.r;
					}
					uint linesize = img.width() * 3;
					if(linesize & 3)
						for(uint n=0; n < 4 - (linesize & 3); ++n)
							a << uint8(0);
				}
			}
			else if(biBitCount == 8)
			{
				for(uint j=0; j < img.height(); ++j)
				{
					for(uint i=0; i < img.width(); ++i)
					{
						ColorRGBA c = img(i, !yUp ? img.height() - 1 - j : j);
						a << c.r;
					}
					uint linesize = img.width();
					if(linesize & 3)
						for(uint n=0; n < 4 - (linesize & 3); ++n)
							a << uint8(0);
				}
			}
		}
		data.resize(a.seek());
		return true;
	case EColor_RGB:
		{
			ConstImageProxy<ColorRGB> img;
			boolean res = image.cast(img);
			Assert(res);
			if(biBitCount == 24)
			{
				for(uint j=0; j < img.height(); ++j)
				{
					for(uint i=0; i < img.width(); ++i)
					{
						ColorRGB c = img(i, !yUp ? img.height() - 1 - j : j);
						a << c.b << c.g << c.r;
					}
					uint linesize = img.width() * 3;
					if(linesize & 3)
						for(uint n=0; n < 4 - (linesize & 3); ++n)
							a << uint8(0);
				}
			}
			else if(biBitCount == 8)
			{
				for(uint j=0; j < img.height(); ++j)
				{
					for(uint i=0; i < img.width(); ++i)
					{
						ColorRGB c = img(i, !yUp ? img.height() - 1 - j : j);
						a << c.r;
					}
					uint linesize = img.width();
					if(linesize & 3)
						for(uint n=0; n < 4 - (linesize & 3); ++n)
							a << uint8(0);
				}
			}
		}
			
		data.resize(a.seek());
		return true;
	case EColor_L:
		{
			ConstImageProxy<byte> img;
			boolean res = image.cast(img);
			Assert(res);
			for(uint j=0; j < img.height(); ++j){
				for(uint i=0; i < img.width(); ++i)
				{
					byte c = img(i, !yUp ? img.height() - 1 - j : j);
					a << c;
				}
				uint linesize = img.width();
				if(linesize & 3)
					for(uint n=0; n < 4 - (linesize & 3); ++n)
						a << uint8(0);
			}
		}
		data.resize(a.seek());
		return true;
	default:
		logError << "unsupported image format";
		data.clear();
		return false;
	}
}

boolean saveDDS(BinaryData& data, const ImageFileData& img)
{
	uint32	dwSize = 124;
	uint32	dwFlags = 0x1 | 0x2 | 0x4 | 0x1000;
	if(img.depthSlicesCount > 1)
		dwFlags |= 0x800000; //DDSD_DEPTH
	if(img.mipsCount > 1)
		dwFlags |= 0x20000; //DDSD_MIPMAPCOUNT
	
	uint32	dwHeight = img.height;
	uint32	dwWidth = img.width;
	uint32	dwPitchOrLinearSize = 0;
	uint32	dwDepth = img.depthSlicesCount;
	uint32	dwMipMapCount = img.mipsCount;
	
	uint32	ddspf_dwSize = 32;
	uint32	ddspf_dwFlags = 0;
	uint32	ddspf_dwFourCC = 0;
	uint32	ddspf_dwRGBBitCount = 0;
	uint32	ddspf_dwRBitMask = 0x00ff0000;
	uint32	ddspf_dwGBitMask = 0x0000ff00;
	uint32	ddspf_dwBBitMask = 0x000000ff;
	uint32	ddspf_dwABitMask = 0xff000000;
	
	uint32	dwCaps = 0x1000;
	if(img.mipsCount > 1)
		dwCaps |= 0x400000 | 0x8;
	if(img.facesCount > 1)
		dwCaps |= 0x8;
	if(img.depthSlicesCount > 1)
		dwCaps |= 0x8;
	
	uint32	dwCaps2 = 0;
	if(img.facesCount > 1)
		dwCaps2 |= 0x200 | 0x400 | 0x800 | 0x1000 | 0x2000 | 0x4000 | 0x8000;
	if(img.depthSlicesCount > 1)
		dwCaps2 |= 0x200000;

	uint32	dwCaps3 = 0;
	uint32	dwCaps4 = 0;
	uint32	dwReserved2 = 0;
	
	uint32 dx10_dxgiFormat;
	uint32 dx10_resourceDimension = img.depthSlicesCount > 1 ? 4 : 3;
	uint32 dx10_miscFlag = img.facesCount > 1 ? 0x4 : 0;
	uint32 dx10_arraySize = img.arrayElementsCount > 1 ? img.arrayElementsCount : 0;
	uint32 dx10_miscFlags2 = 0;

	switch(img.colorType)
	{
	case EColor_RGBA:
		ddspf_dwFlags = 0x41;
		ddspf_dwRGBBitCount = 32;
		dx10_dxgiFormat = 87;//DXGI_FORMAT_B8G8R8A8_UNORM
		dx10_miscFlags2 = 0x4;//DDS_ALPHA_MODE_CUSTOM
		break;
	case EColor_L:
		ddspf_dwFlags = 0x20000;
		ddspf_dwRGBBitCount = 8;
		dx10_dxgiFormat = 61;//DXGI_FORMAT_R8_UNORM
		break;
	case EColor_DXT1:
		ddspf_dwFlags = 0x4;
		ddspf_dwFourCC = ddsFoucCC("DXT1");
		break;
	case EColor_DXT1A:
		ddspf_dwFlags = 0x4;
		ddspf_dwFourCC = ddsFoucCC("DXT1");
		dx10_miscFlags2 = 0x1;//DDS_ALPHA_MODE_STRAIGHT
		break;
	case EColor_DXT3:
		ddspf_dwFlags = 0x4;
		ddspf_dwFourCC = ddsFoucCC("DXT3");
		dx10_miscFlags2 = 0x4;//DDS_ALPHA_MODE_CUSTOM
		break;
	case EColor_DXT5:
		ddspf_dwFlags = 0x4;
		ddspf_dwFourCC = ddsFoucCC("DXT5");
		dx10_miscFlags2 = 0x4;//DDS_ALPHA_MODE_CUSTOM
		break;
	case EColor_ETC1:
		ddspf_dwFlags = 0x4;
		ddspf_dwFourCC = ddsFoucCC("ETC1");
		break;
	case EColor_ATC:
		ddspf_dwFlags = 0x4;
		ddspf_dwFourCC = ddsFoucCC("ATC ");
		break;
	case EColor_ATCI:
		ddspf_dwFlags = 0x4;
		ddspf_dwFourCC = ddsFoucCC("ATCI");
		dx10_miscFlags2 = 0x4;//DDS_ALPHA_MODE_CUSTOM
		break;
	default:
		return false;
	}

	EndianSafeBinaryDataAccessor a(data);
	a << byte(0x44);
	a << byte(0x44);
	a << byte(0x53);
	a << byte(0x20);
	a << dwSize;
	a << dwFlags;
	a << dwHeight;
	a << dwWidth;
	a << dwPitchOrLinearSize;
	a << dwDepth;
	a << dwMipMapCount;
	for(uint i=0; i < 11; ++i)
		a << uint32(0);
	a << ddspf_dwSize;
	a << ddspf_dwFlags;
	a << ddspf_dwFourCC;
	a << ddspf_dwRGBBitCount;
	a << ddspf_dwRBitMask;
	a << ddspf_dwGBitMask;
	a << ddspf_dwBBitMask;
	a << ddspf_dwABitMask;
	a << dwCaps;
	a << dwCaps2;
	a << dwCaps3;
	a << dwCaps4;
	a << dwReserved2;

	if(ddspf_dwFourCC == ddsFoucCC("DX10"))
	{
		a << dx10_dxgiFormat;
		a << dx10_resourceDimension;
		a << dx10_miscFlag;
		a << dx10_arraySize;
		a << dx10_miscFlags2;
	}

	for(uint n = 0; n < img.arrayElementsCount; ++ n)
	{
		for(uint mip = 0; mip < img.mipsCount; ++ mip)
		{
			for(uint face = 0; face < img.facesCount; ++ face)
			{
				for(uint z = 0; z < img.depthSlicesCount; ++ z)
				{
					const BaseImage& im = img.get(mip,n,face,z);
					a.write(im.rawData(), eColorSize(im.colortype()) * im.width() * im.height());
				}
			}
		}
	}
	return true;
}

}


/*
bool CImage::Save_BMP(const CString& strName,const CBitmap * pB,bool bFlipHorizontal,bool bFlipVertical)
{
	
}
*/
