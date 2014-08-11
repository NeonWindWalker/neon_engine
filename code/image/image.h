#pragma once

#include "color.h"


namespace ImageLib 
{

USING_BASE_LIB
USING_BASE_MATH

class ImageInfo
{
public:
	ImageInfo(EColor c) : _width(0), _height(0), _colortype(c) {}
	ImageInfo(EColor c, uint width, uint height) : _width(width), _height(height), _colortype(c) {}

	INLINE_NO_DEBUG uint width()const { return _width; }
	INLINE_NO_DEBUG uint height()const { return _height; }
	INLINE_NO_DEBUG uint pixels()const { return _width * _height; }
	INLINE_NO_DEBUG EColor colortype()const { return _colortype; }
	INLINE_NO_DEBUG boolean isSizePowOf2()const { return Base::isPowOf2(_width * _height); }
	INLINE_NO_DEBUG boolean isSquarePowOf2Size()const { return _width == _height && Base::isPowOf2(_width * _height); }

	INLINE_NO_DEBUG uint unpackedWidth()const
	{
		if(_colortype >= EColor_RGBA && _colortype <= EColor_RGBAF)
			return _width;

		if(_colortype == EColor_PVR2 || _colortype == EColor_PVR2)
			return _width * 8;
		return _width * 4;
	}

	INLINE_NO_DEBUG uint unpackedHeight()const
	{
		if(_colortype >= EColor_RGBA && _colortype <= EColor_RGBAF)
			return _height;

		return _height * 4;
	}

protected:
	uint _width;
	uint _height;
	EColor _colortype;
};

template<class t_Color> class ImageProxy;
template<class t_Color> class ConstImageProxy;

class BaseImageProxy : public ImageInfo
{
protected:
	void* _accesptr;
	uint _xivalue;
	uint _xjvalue;
	
	BaseImageProxy(EColor color) : ImageInfo(color), _accesptr(0), _xjvalue(0), _xivalue(0) {}
	BaseImageProxy(EColor color, void* data, uint width, uint height, uint xivalue, uint xjvalue) : ImageInfo(color, width, height), _accesptr(data), _xivalue(xivalue), _xjvalue(xjvalue) {}

public:
	template<class tt_Color>
	INLINE_NO_DEBUG boolean cast(ImageProxy<tt_Color>& out)const;

	template<class tt_Color>
	INLINE_NO_DEBUG boolean cast(ConstImageProxy<tt_Color>& out)const;

	INLINE_NO_DEBUG void* accesptr()const { return _accesptr; }
	INLINE_NO_DEBUG uint xjvalue()const { return _xjvalue; }
	INLINE_NO_DEBUG uint xivalue()const { return _xivalue; }

	friend class BaseImage;
	friend class ConstBaseImageProxy;
};

class ConstBaseImageProxy : public ImageInfo
{
protected:
	const void* _accesptr;
	uint _xivalue;
	uint _xjvalue;

	ConstBaseImageProxy(EColor color) : ImageInfo(color), _accesptr(0), _xivalue(0), _xjvalue(0) {}
	ConstBaseImageProxy(EColor color, const void* data, uint width, uint height, uint xivalue, uint xjvalue) : ImageInfo(color, width, height), _accesptr(data), _xivalue(xivalue), _xjvalue(xjvalue)  {}

	
public:
	template<class tt_Color>
	INLINE_NO_DEBUG boolean cast(ConstImageProxy<tt_Color>& out)const;

	ConstBaseImageProxy(const BaseImageProxy& other) : ImageInfo(other), _accesptr(other._accesptr), _xivalue(other._xivalue), _xjvalue(other._xjvalue)  {}

	INLINE_NO_DEBUG const void* at(uint i, uint j)const { return (const byte*)this->_accesptr + i * this->_xivalue + j * this->_xjvalue; }
	INLINE_NO_DEBUG const void* accesptr()const { return _accesptr; }
	INLINE_NO_DEBUG uint xjvalue()const { return _xjvalue; }
	INLINE_NO_DEBUG uint xivalue()const { return _xivalue; }

	friend class BaseImage;
};

template<class t_Color>
class ImageProxy : public BaseImageProxy
{
public:
	ImageProxy() : BaseImageProxy(EColorOfType<t_Color>::value) {}
	ImageProxy(t_Color* data, uint width, uint height) : BaseImageProxy(EColorOfType<t_Color>::value, data, width, height, sizeof(t_Color), width * sizeof(t_Color)) {}
	ImageProxy(void* data, uint width, uint height, uint xivalue, uint xjvalue) : BaseImageProxy(EColorOfType<t_Color>::value, data, width, height, xivalue, xjvalue) {}

	INLINE_NO_DEBUG t_Color& operator () (uint i, uint j)const 
	{
		byte* p = (byte*)this->_accesptr + i * this->_xivalue + j * this->_xjvalue;
		return *(t_Color*)(p);
	}
	INLINE_NO_DEBUG t_Color& at(uint i, uint j)const 
	{
		byte* p = (byte*)this->_accesptr + i * this->_xivalue + j * this->_xjvalue;
		return *(t_Color*)(p);
	}
	
	INLINE_NO_DEBUG ImageProxy proxy(uint x, uint y, uint width, uint height)const 
	{
		Assert(x + width <= this->_width);
		Assert(y + height <= this->_height);
		return ImageProxy((byte*)this->_accesptr + x * this->_xivalue + y * this->_xjvalue, width, height, this->_xivalue, this->_xjvalue);
	}

	INLINE_NO_DEBUG ImageProxy proxy(uint x, uint y)const 
	{
		Assert(x <= this->_width);
		Assert(y <= this->_height);
		return ImageProxy((byte*)this->_accesptr + x * this->_xivalue + y * this->_xjvalue, this->_width - x, this->_height - y, this->_xivalue, this->_xjvalue);
	}
};

template<class t_Color>
class ConstImageProxy : public ConstBaseImageProxy
{
public:
	INLINE_NO_DEBUG ConstImageProxy() : ConstBaseImageProxy(EColorOfType<t_Color>::value) {}
	INLINE_NO_DEBUG ConstImageProxy(const t_Color* data, uint width, uint height) : ConstBaseImageProxy(EColorOfType<t_Color>::value, data, width, height, sizeof(t_Color), width * sizeof(t_Color)) {}
	INLINE_NO_DEBUG ConstImageProxy(const void* data, uint width, uint height, uint xivalue, uint xjvalue) : ConstBaseImageProxy(EColorOfType<t_Color>::value, data, width, height, xivalue, xjvalue) {}

	INLINE_NO_DEBUG ConstImageProxy(const ImageProxy<t_Color>& other) : ConstBaseImageProxy(other.colortype(), other.accesptr(), other.width(), other.height(), other.xivalue(), other.xjvalue()) {}
	INLINE_NO_DEBUG ConstImageProxy& operator = (const ImageProxy<t_Color>& other) { this->_colortype = other.colortype(); this->_accesptr = other.accesptr(); this->_width = other.width(); this->_height = other.height(); this->_xivalue = other.xivalue(); this->_xjvalue = other.xjvalue(); }

	INLINE_NO_DEBUG const t_Color& operator () (uint i, uint j)const { return *(const t_Color*)((byte*)this->_accesptr + i * this->_xivalue + j * this->_xjvalue); }
	INLINE_NO_DEBUG const t_Color& at(uint i, uint j)const { return *(const t_Color*)((byte*)this->_accesptr + i * this->_xivalue + j * this->_xjvalue); }

	INLINE_NO_DEBUG ConstImageProxy proxy(uint x, uint y, uint width, uint height)const 
	{
		Assert(x + width <= this->_width);
		Assert(y + height <= this->_height);
		return ConstImageProxy((void*)((const byte*)this->_accesptr + x * this->_xivalue + y * this->_xjvalue), width, height, this->_xivalue, this->_xjvalue);
	}

	INLINE_NO_DEBUG ConstImageProxy proxy(uint x, uint y)const 
	{
		Assert(x <= this->_width);
		Assert(y <= this->_height);
		return ConstImageProxy((void*)((const byte*)this->_accesptr + x * this->_xivalue + y * this->_xjvalue), this->_width - x, this->_height - y, this->_xivalue, this->_xjvalue);
	}
};


template<class tt_Color>
INLINE_NO_DEBUG boolean BaseImageProxy::cast(ImageProxy<tt_Color>& out)const
{
	if(EColorOfType<tt_Color>::value == this->_colortype){
		out = ImageProxy<tt_Color>((const tt_Color*)this->_accesptr, this->_width, this-> _height, this->_xivalue, this->_xjvalue);
		return true;
	}
	else
		return false;
}

template<class tt_Color>
INLINE_NO_DEBUG boolean BaseImageProxy::cast(ConstImageProxy<tt_Color>& out)const
{
	if(EColorOfType<tt_Color>::value == this->_colortype){
		out = ConstImageProxy<tt_Color>((const tt_Color*)this->_accesptr, this->_width, this-> _height, this->_xivalue, this->_xjvalue);
		return true;
	}
	else
		return false;
}

template<class tt_Color>
INLINE_NO_DEBUG boolean ConstBaseImageProxy::cast(ConstImageProxy<tt_Color>& out)const
{
	if(EColorOfType<tt_Color>::value == this->_colortype){
		out = ConstImageProxy<tt_Color>((const tt_Color*)this->_accesptr, this->_width, this->_height, this->_xivalue, this->_xjvalue);
		return true;
	}
	else
		return false;
}

template<class t_Color>
INLINE_NO_DEBUG void copy(ImageProxy<t_Color> dest, ConstImageProxy<t_Color> src)
{
	Assert(dest.width() >= src.width());
	Assert(dest.height() >= src.height());
	for(uint j=0; j < src.height(); ++j)
		for(uint i=0; i < src.width(); ++i)
			dest(i,j) = src(i,j);
}

template<class t_Color>
INLINE_NO_DEBUG boolean equals(ConstImageProxy<t_Color> dest, ConstImageProxy<t_Color> src)
{
	Assert(dest.width() >= src.width());
	Assert(dest.height() >= src.height());
	for(uint j=0; j < src.height(); ++j)
		for(uint i=0; i < src.width(); ++i)
			if(dest(i,j) != src(i,j))
				return false;
	return true;
}

template<class t_Color, class t_AllocatorWrapper> class Image;

class BaseImage : public ImageInfo, public RefCountable
{
protected:
	void* _data;

	BaseImage(EColor color) : ImageInfo(color), _data(0) {}
	BaseImage(EColor color, uint width, uint height) : ImageInfo(color, width, height), _data(0) {}

public:
	
	INLINE_NO_DEBUG BaseImageProxy baseProxy()
	{ 
		uint s = eColorSize(this->_colortype);
		return BaseImageProxy(this->_colortype, this->_data, this->_width, this->_height, s, this->_width * s);
	}

	INLINE_NO_DEBUG ConstBaseImageProxy baseProxy()const
	{ 
		uint s = eColorSize(this->_colortype);
		return ConstBaseImageProxy(this->_colortype, this->_data, this->_width, this->_height, s, this->_width * s);
	}

	INLINE_NO_DEBUG operator BaseImageProxy ()
	{
		return baseProxy();
	}

	INLINE_NO_DEBUG operator ConstBaseImageProxy ()const
	{ 
		return baseProxy();
	}

	template<class tt_Color>
	INLINE_NO_DEBUG boolean cast(ImageProxy<tt_Color>& out)
	{
		if(EColorOfType<tt_Color>::value == this->_colortype){
			out = ImageProxy<tt_Color>(this->_data, this->_width, this-> _height, sizeof(tt_Color), this->_width * sizeof(tt_Color));
			return true;
		}
		else
			return false;
	}

	template<class tt_Color>
	INLINE_NO_DEBUG boolean cast(ConstImageProxy<tt_Color>& out)const
	{
		if(EColorOfType<tt_Color>::value == this->_colortype){
			out = ConstImageProxy<tt_Color>(this->_data, this->_width, this-> _height, sizeof(tt_Color), this->_width * sizeof(tt_Color));
			return true;
		}
		else
			return false;
	}
	
	template<class tt_Color, class tt_AllocatorWrapper>
	INLINE_NO_DEBUG Image<tt_Color, tt_AllocatorWrapper>* cast();

	template<class tt_Color, class tt_AllocatorWrapper>
	INLINE_NO_DEBUG const Image<tt_Color, tt_AllocatorWrapper>* cast()const;

	template<class tt_Color>
	INLINE_NO_DEBUG Image<tt_Color, SystemAllocatorWrapper>* cast();

	template<class tt_Color>
	INLINE_NO_DEBUG const Image<tt_Color, SystemAllocatorWrapper>* cast()const;

	INLINE_NO_DEBUG void* rawData() { return _data; }
	INLINE_NO_DEBUG const void* rawData()const { return _data; }
};

template<class t_Color, class t_AllocatorWrapper = SystemAllocatorWrapper>
class Image : public BaseImage, t_AllocatorWrapper
{
public:
	typedef t_Color Color_t;
	typedef t_AllocatorWrapper AllocatorWrapper_t;

	Image() : BaseImage(EColorOfType<t_Color>::value) 
	{
	}

	Image(uint width, uint height) : BaseImage(EColorOfType<t_Color>::value, width, height)
	{
		if(width * height)
		{
			t_Color* p = (t_Color*)this->allocator().alloc(width * height * sizeof(t_Color), ALLOCATION_ALIGNMENT(sizeof(t_Color)), 0);
			for(uint n = 0; n < width * height; ++n)
				new(p + n) t_Color();
			this->_data = p;
		}
	}

	~Image()
	{
		this->allocator().free(this->_data);
	}

	INLINE_NO_DEBUG void resize(uint w, uint h)
	{
		if(this->_width == w && this->_height == h)
			return;

		if(this->_data){
			this->allocator().free(this->_data);
			this->_data = 0;
		}
		
		this->_width = w;
		this->_height = h;
		if(this->_width * this->_height)
		{
			t_Color* p = (t_Color*)this->allocator().alloc(this->_width * this->_height * sizeof(t_Color), ALLOCATION_ALIGNMENT(sizeof(t_Color)), 0);
			for(uint n = 0; n < this->_width * this->_height; ++n)
				new(p + n) t_Color();
			this->_data = p;
		}
	}

	INLINE_NO_DEBUG Image& operator = (const ConstImageProxy<t_Color>& src)
	{
		resize(src.width(), src.height());
		copy(this->proxy(), src);
		return *this;
	}

	INLINE_NO_DEBUG t_Color* data() { return (t_Color*)this->_data; }
	INLINE_NO_DEBUG const t_Color* data() const { return (const t_Color*)this->_data; }

	INLINE_NO_DEBUG t_Color& operator () (uint i, uint j) { return ((t_Color*)(this->_data))[i + j * this->_width]; }
	INLINE_NO_DEBUG const t_Color& operator () (uint i, uint j)const { return ((t_Color*)(this->_data))[i + j * this->_width]; }

	INLINE_NO_DEBUG t_Color& at(uint i, uint j) { return ((t_Color*)(this->_data))[i + j * this->_width]; }
	INLINE_NO_DEBUG const t_Color& at(uint i, uint j)const { return ((t_Color*)(this->_data))[i + j * this->_width]; }

	INLINE_NO_DEBUG ImageProxy<t_Color> proxy() { return ImageProxy<t_Color>((t_Color*)this->_data, this->_width, this->_height); }
	INLINE_NO_DEBUG ConstImageProxy<t_Color> proxy()const { return ConstImageProxy<t_Color>((t_Color*)this->_data, this->_width, this-> _height); }
	INLINE_NO_DEBUG ConstImageProxy<t_Color> constProxy()const { return ConstImageProxy<t_Color>((t_Color*)this->_data, this->_width, this-> _height); }

	INLINE_NO_DEBUG ImageProxy<t_Color> proxy(uint x, uint y, uint width, uint height) { return proxy().proxy(x, y, width, height); }
	INLINE_NO_DEBUG ConstImageProxy<t_Color> proxy(uint x, uint y, uint width, uint height)const { return proxy().proxy(x, y, width, height); }
	INLINE_NO_DEBUG ConstImageProxy<t_Color> constProxy(uint x, uint y, uint width, uint height)const { return proxy().proxy(x, y, width, height); }

	INLINE_NO_DEBUG ImageProxy<t_Color> proxy(uint x, uint y) { return proxy().proxy(x, y); }
	INLINE_NO_DEBUG ConstImageProxy<t_Color> proxy(uint x, uint y)const { return proxy().proxy(x, y); }
	INLINE_NO_DEBUG ConstImageProxy<t_Color> constProxy(uint x, uint y)const { return proxy().proxy(x, y); }

	INLINE_NO_DEBUG operator ImageProxy<t_Color> () { return proxy(); }
	INLINE_NO_DEBUG operator ConstImageProxy<t_Color> () const { return proxy(); }

	INLINE_NO_DEBUG t_Color* begin() { return (t_Color*)this->_data - 1; }
	INLINE_NO_DEBUG t_Color* first() { return (t_Color*)this->_data; }
	INLINE_NO_DEBUG t_Color* last() { return (t_Color*)this->_data + this->_width * this->_height - 1; }
	INLINE_NO_DEBUG t_Color* end() { return (t_Color*)this->_data + this->_width * this->_height; }

	INLINE_NO_DEBUG const t_Color* begin()const { return (const t_Color*)this->_data - 1; }
	INLINE_NO_DEBUG const t_Color* first()const { return (const t_Color*)this->_data; }
	INLINE_NO_DEBUG const t_Color* last()const { return (const t_Color*)this->_data + this->_width * this->_height - 1; }
	INLINE_NO_DEBUG const t_Color* end()const { return (const t_Color*)this->_data + this->_width * this->_height; }
};

template<class tt_Color, class tt_AllocatorWrapper>
INLINE_NO_DEBUG Image<tt_Color, tt_AllocatorWrapper>* BaseImage::cast()
{
	if(EColorOfType<tt_Color>::value == this->_colortype)
		return static_cast<Image<tt_Color,tt_AllocatorWrapper>*>(this);
	else
		return 0;
}

template<class tt_Color, class tt_AllocatorWrapper>
INLINE_NO_DEBUG const Image<tt_Color, tt_AllocatorWrapper>* BaseImage::cast()const
{
	if(EColorOfType<tt_Color>::value == this->_colortype)
		return static_cast<Image<tt_Color,tt_AllocatorWrapper>*>(this);
	else
		return 0;
}

template<class tt_Color>
INLINE_NO_DEBUG Image<tt_Color>* BaseImage::cast()
{
	if(EColorOfType<tt_Color>::value == this->_colortype)
		return static_cast<Image<tt_Color>*>(this);
	else
		return 0;
}

template<class tt_Color>
INLINE_NO_DEBUG const Image<tt_Color>* BaseImage::cast()const
{
	if(EColorOfType<tt_Color>::value == this->_colortype)
		return static_cast<Image<tt_Color>*>(this);
	else
		return 0;
}

enum EImageFormat
{
	EImageFormat_None = 0,
	EImageFormat_BMP,
	EImageFormat_JPG,
	EImageFormat_PNG,
	EImageFormat_KTX,
	EImageFormat_DDS
};

struct ImageFileInfo
{
	EColor colorType;
	uint mipsCount;
	uint arrayElementsCount;
	uint facesCount;
	uint depthSlicesCount;
	uint height;
	uint width;
	
	ImageFileInfo()
	{
		clear();
	}

	void clear()
	{
		colorType = EColor_None;
		mipsCount = 1;
		arrayElementsCount = 1;
		facesCount = 1;
		depthSlicesCount = 1;
		height = 0;
		width = 0;
	}
};

struct ImageFileData : public ImageFileInfo
{
	typedef Base::Array< Base::StrongPtr<BaseImage> > Array_t;
	Array_t images;

	void assign(BaseImage& img)
	{
		ImageFileInfo::clear();
		colorType = img.colortype();
		width = img.unpackedWidth();
		height = img.unpackedHeight();
		images.resize(1);
		images[0] = &img;
	}

	BaseImage& get(uint mip = 0, uint arrIndex = 0, uint face = 0, uint depth = 0)const
	{
		return *images[((mip * arrayElementsCount + arrIndex) * facesCount + face) * depthSlicesCount + depth];
	}
};


extern boolean getImageFileInfo(ImageFileInfo& outInfo, ConstBinaryData data);
extern boolean loadImage(ImageFileData& outData, ConstBinaryData data, boolean yUp = true);

enum ESaveBMPBits
{
	ESaveBMPBits_FromSource = -1,
	ESaveBMPBits_AutoDetectOptimizedCases = 0
};
extern boolean saveBmpImage(BinaryData& data, const ConstBaseImageProxy& img, boolean yUp = true, ESaveBMPBits bits = ESaveBMPBits_AutoDetectOptimizedCases);
extern boolean saveKTX(BinaryData& data, const ImageFileData& img);
extern boolean saveDDS(BinaryData& data, const ImageFileData& img);

template<class t_Image>
static INLINE_NO_DEBUG t_Image* convert(const ConstBaseImageProxy& img)
{
	t_Image* p = new t_Image(img.width(), img.height());
	switch(img.colortype())
	{
	case EColor_RGBA:
		{
			ConstImageProxy<ColorRGBA> pr;
			boolean res = img.cast(pr);
			Assert(res);
			convert(p->proxy(), pr);
		}
		return p;
	case EColor_RGB:
		{
			ConstImageProxy<ColorRGB> pr;
			boolean res = img.cast(pr);
			Assert(res);
			convert(p->proxy(), pr);
		}
		return p;
	case EColor_L:
		{
			ConstImageProxy<ColorL> pr;
			boolean res = img.cast(pr);
			Assert(res);
			convert(p->proxy(), pr);
		}
		return p;
	case EColor_LA:
		{
			ConstImageProxy<ColorLA> pr;
			boolean res = img.cast(pr);
			Assert(res);
			convert(p->proxy(), pr);
		}
		return p;
	case EColor_RGBAF:
		{
			ConstImageProxy<ColorRGBAF> pr;
			boolean res = img.cast(pr);
			Assert(res);
			convert(p->proxy(), pr);
		}
		return p;
	case EColor_RGBF:
		{
			ConstImageProxy<ColorRGBAF> pr;
			boolean res = img.cast(pr);
			Assert(res);
			convert(p->proxy(), pr);
		}
		return p;
	case EColor_RGF:
		{
			ConstImageProxy<ColorRGBAF> pr;
			boolean res = img.cast(pr);
			Assert(res);
			convert(p->proxy(), pr);
		}
		return p;
	case EColor_RF:
		{
			ConstImageProxy<ColorRGBAF> pr;
			boolean res = img.cast(pr);
			Assert(res);
			convert(p->proxy(), pr);
		}
		return p;
	default:
		delete p;
		return 0;
	}
}

template<class t_Image>
static INLINE_NO_DEBUG t_Image* convert(ConstBaseImageProxy& img)
{
	return convert<t_Image>(*const_cast<const ConstBaseImageProxy*>(&img));
}

template<class t_Image>
static INLINE_NO_DEBUG t_Image* convert(const BaseImageProxy& img)
{
	return convert<t_Image>(ConstBaseImageProxy(img));
}

template<class t_Image>
static INLINE_NO_DEBUG t_Image* convert(BaseImageProxy& img)
{
	return convert<t_Image>(*const_cast<const BaseImageProxy*>(&img));
}

template<class t_Image>
static INLINE_NO_DEBUG t_Image* convert(BaseImage& img)
{
	t_Image* p = img.cast<typename t_Image::Color_t, typename t_Image::AllocatorWrapper_t>();
	if(p)
		return p;
	return convert<t_Image>(img.baseProxy());
}

template<class t_ColorType>
static INLINE_NO_DEBUG StrongPtr< Image<t_ColorType> > loadAndConvert(ConstBinaryData data, boolean yUp = true)
{
	ImageFileData ifd;
	if(!loadImage(ifd, data, yUp))
		return 0;
	Image<t_ColorType>* ret = convert< Image<t_ColorType> >(ifd.get());
	if(ret == &ifd.get())
		return ret;
	return ret;
}

static INLINE_NO_DEBUG BaseImage* createImage(EColor color, uint width, uint height)
{
	switch(color)
	{
	case EColor_RGBA: return new Image<ColorRGBA>(width, height);
	case EColor_RGB: return new Image<ColorRGB>(width, height);
	case EColor_LA: return new Image<ColorLA>(width, height);
	case EColor_L: return new Image<ColorL>(width, height);
	case EColor_RGBAF: return new Image<ColorRGBAF>(width, height);
	case EColor_RGBF: return new Image<ColorRGBF>(width, height);
	case EColor_RGF: return new Image<ColorRGF>(width, height);
	case EColor_RF: return new Image<ColorRF>(width, height);
	default: return 0;
	}
}

//extern boolean saveImage(BinData& outData, ImageProxy<ColorRGBA>& img, uint mip = 0);


}
