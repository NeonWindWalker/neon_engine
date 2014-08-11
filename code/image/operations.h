#pragma once

#include "image.h"

namespace ImageLib
{

template<class t_Color>
INLINE void fill(const ImageProxy<t_Color>& dest, const t_Color c)
{
	for(uint j=0; j < dest.height(); j++)
		for(uint i=0; i < dest.width(); i++)
			dest(i, j) = c;
}

template<class t_Color>
INLINE void setBorder(const ImageProxy<t_Color>& dest, const t_Color c)
{
	for(uint i=0; i < dest.width(); i++){
		dest(i, 0) = c;
		dest(i, dest.height() - 1) = c;
	}

	for(uint j=1; j < dest.height() - 1; j++){
		dest(0, j) = c;
		dest(dest.width() - 1, j) = c;
	}
}

template<class t_Color>
INLINE void setDottedBorder(const ImageProxy<t_Color>& dest, const t_Color c0, const t_Color c1)
{
	t_Color c[] = {c0, c1};
	for(uint i=0; i < dest.width(); i++){
		dest(i, 0) = c[i & 1];
		dest(i, dest.height() - 1) = c[i & 1];
	}

	for(uint j=1; j < dest.height() - 1; j++){
		dest(0, j) = c[j & 1];
		dest(dest.width() - 1, j) = c[j & 1];
	}
}

template<class t_Color>
INLINE void setClampBorder(const ImageProxy<t_Color>& dest)
{
	for(uint i = 1; i < dest.width() - 1; i++){
		dest(i, 0) = dest(i, 1);
		dest(i, dest.height() - 1) = dest(i, dest.height() - 2);
	}

	for(uint j = 1; j < dest.height() - 1; j++){
		dest(0, j) = dest(1, j);
		dest(dest.width() - 1, j) = dest(dest.width() - 2, j);
	}

	dest(0, 0) = dest(1, 1);
	dest(dest.width() - 1, 0) = dest(dest.width() - 2, 1);
	dest(0, dest.height() - 1) = dest(1, dest.height() - 2);
	dest(dest.width() - 1, dest.height() - 1) = dest(dest.width() - 2, dest.height() - 2);
}

template<class t_Color>
INLINE void setWrapBorder(const ImageProxy<t_Color>& dest)
{
	for(uint i = 1; i < dest.width() - 1; i++){
		dest(i, 0) = dest(i, dest.height() - 2);
		dest(i, dest.height() - 1) = dest(i, 1);
	}

	for(uint j = 1; j < dest.height() - 1; j++){
		dest(0, j) = dest(dest.width() - 2, j);
		dest(dest.width() - 1, j) = dest(1, j);
	}

	dest(0, 0) = dest(dest.width() - 2, dest.height() - 2);
	dest(dest.width() - 1, 0) = dest(1, dest.height() - 2);
	dest(0, dest.height() - 1) = dest(dest.width() - 2, 1);
	dest(dest.width() - 1, dest.height() - 1) = dest(1, 1);
}

template<class t_Color>
INLINE void copySrcSwapXY(ImageProxy<t_Color> dest, ConstImageProxy<t_Color> src)
{
	Assert(dest.width() >= src.height());
	Assert(dest.height() >= src.width());
	for(uint j=0; j < src.height(); ++j)
		for(uint i=0; i < src.width(); ++i)
			dest(j,i) = src(i, j);
}

template<class t_DstColor, class t_SrcColor>
INLINE void mipgen(const ImageProxy<t_DstColor>& dest, const ConstImageProxy<t_SrcColor>& src)
{
	if(src.height() == 1){
		Assert(dest.width() * 2 == src.width());
		Assert(dest.height() == 1);

		for(uint i=0; i < dest.width(); ++i){
			typename ColorConvertor<t_SrcColor>::OperationType c = 0.5f*(ColorConvertor<t_SrcColor>::unpuck(src((i<<1),0)) + ColorConvertor<t_SrcColor>::unpuck(src((i<<1) + 1,0)));
			dest(i,0) = ColorConvertor<t_DstColor>::puck(c);
		}
	}
	else if(src.width() == 1){
		Assert(dest.width() == 1);
		Assert(dest.height() * 2 == src.height());
		for(uint j=0; j < dest.height(); ++j){
			typename ColorConvertor<t_SrcColor>::OperationType c = 0.5f*(ColorConvertor<t_SrcColor>::unpuck(src(0,(j<<1))) + ColorConvertor<t_SrcColor>::unpuck(src(0,(j<<1) + 1)));
			dest(0,j) = ColorConvertor<t_DstColor>::puck(c);
		}
	}
	else{
		Assert(dest.width() * 2 == src.width());
		Assert(dest.height() * 2 == src.height());
		for(uint j=0; j < dest.height(); ++j)
			for(uint i=0; i < dest.width(); ++i){
				typename ColorConvertor<t_SrcColor>::OperationType c = 0.25f*(
					ColorConvertor<t_SrcColor>::unpuck(src((i<<1),(j<<1))) + 
					ColorConvertor<t_SrcColor>::unpuck(src((i<<1) + 1,(j<<1))) + 
					ColorConvertor<t_SrcColor>::unpuck(src((i<<1),(j<<1) + 1)) + 
					ColorConvertor<t_SrcColor>::unpuck(src((i<<1) + 1,(j<<1) + 1)));
				dest(i,j) = ColorConvertor<t_DstColor>::puck(c);
			}
	}
}

template<class t_DstColor, class t_DstSetter, class t_SrcColor, class t_SrcWeight, class t_WeightGetter>
INLINE void mipgenWeighted(const ImageProxy<t_DstColor>& dest, t_DstSetter setter, const ConstImageProxy<t_SrcColor>& src, const ConstImageProxy<t_SrcWeight>& srcWeight, t_WeightGetter wgetter = ColorConvertor<t_SrcWeight>())
{
	if(src.height() == 1){
		Assert(dest.width() * 2 == src.width());
		Assert(dest.height() == 1);

		for(uint i=0; i < dest.width(); ++i){
			typename t_WeightGetter::OperationType w0 = wgetter.unpuck(srcWeight((i<<1), 0));
			typename t_WeightGetter::OperationType w1 = wgetter.unpuck(srcWeight((i<<1) + 1, 0));
			typename ColorConvertor<t_SrcColor>::OperationType c = (ColorConvertor<t_SrcColor>::unpuck(src((i<<1),0)) * w0 + ColorConvertor<t_SrcColor>::unpuck(src((i<<1) + 1,0)) * w1);
			setter.setWeighted(dest(i,0), c, w0 + w1, 2);
		}
	}
	else if(src.width() == 1){
		Assert(dest.width() == 1);
		Assert(dest.height() * 2 == src.height());
		for(uint j=0; j < dest.height(); ++j){
			typename t_WeightGetter::OperationType w0 = wgetter.unpuck(srcWeight(0,(j<<1)));
			typename t_WeightGetter::OperationType w1 = wgetter.unpuck(srcWeight(0,(j<<1) + 1));
			typename ColorConvertor<t_SrcColor>::OperationType c = (ColorConvertor<t_SrcColor>::unpuck(src(0,(j<<1))) * w0 + ColorConvertor<t_SrcColor>::unpuck(src(0,(j<<1) + 1)) * w1);
			setter.setWeighted(dest(0,j), c, w0 + w1, 2);
		}
	}
	else{
		Assert(dest.width() * 2 == src.width());
		Assert(dest.height() * 2 == src.height());
		for(uint j=0; j < dest.height(); ++j)
			for(uint i=0; i < dest.width(); ++i){
				typename t_WeightGetter::OperationType w00 = wgetter.unpuck(srcWeight((i<<1),(j<<1)));
				typename t_WeightGetter::OperationType w01 = wgetter.unpuck(srcWeight((i<<1) + 1,(j<<1)));
				typename t_WeightGetter::OperationType w10 = wgetter.unpuck(srcWeight((i<<1),(j<<1) + 1));
				typename t_WeightGetter::OperationType w11 = wgetter.unpuck(srcWeight((i<<1) + 1,(j<<1) + 1));

				typename ColorConvertor<t_SrcColor>::OperationType c = (
					ColorConvertor<t_SrcColor>::unpuck(src((i<<1),(j<<1))) * w00 + 
					ColorConvertor<t_SrcColor>::unpuck(src((i<<1) + 1,(j<<1))) * w01 + 
					ColorConvertor<t_SrcColor>::unpuck(src((i<<1),(j<<1) + 1)) * w10  + 
					ColorConvertor<t_SrcColor>::unpuck(src((i<<1) + 1,(j<<1) + 1)) * w11);

				setter.setWeighted(dest(i,j), c, w00 + w01 + w10 + w11, 4);
			}
	}
}


template<class t_Color, class t_DestSetter, class t_SrcWeight, class t_WeightGetter>
INLINE void transparencyColorAccurate(const ImageProxy<t_Color>& img, t_DestSetter destSetter, const ConstImageProxy<t_SrcWeight>& srcWeight, t_WeightGetter weightGetter = ColorConvertor<t_SrcWeight>(), uint rectHalfSize = 4)
{
	StrongPtr< Image<byte> > maskedElements = new Image<byte>(img.width(), img.height());
	fill(maskedElements->proxy(), byte(0));
	for(uint r=1; r <= rectHalfSize; ++r)
		for(uint j = 0; j < img.height(); ++j)
			for(uint i = 0; i < img.width(); ++i)
			{
				if(maskedElements->at(i, j) || weightGetter.unpuck(srcWeight.at(i, j)) != t_WeightGetter::zero())
					continue;

				for(uint r=1; r <= rectHalfSize; ++r)
				{
					uint lowI = clampt<int>(int(i) - r, 0, img.width() - 1);
					uint hiI = clampt<int>(int(i) + r, 0, img.width() - 1);
					uint lowJ = clampt<int>(int(j) - r, 0, img.height() - 1);
					uint hiJ = clampt<int>(int(j) + r, 0, img.height() - 1);
					typename ColorConvertor<t_Color>::OperationType C = ColorConvertor<t_Color>::zero();
					typename t_WeightGetter::OperationType Opacity = t_WeightGetter::zero();
					for(uint jj = lowJ; jj <= hiJ; jj++)
						for(uint ii = lowI; ii <= hiI; ii++)
						{
							typename ColorConvertor<t_Color>::OperationType c = ColorConvertor<t_Color>::unpuck(img.at(ii, jj));
							typename t_WeightGetter::OperationType op = weightGetter.unpuck(srcWeight.at(ii, jj));
							C += c * op;
							Opacity += op;
						}
					if(Opacity != t_WeightGetter::zero())
					{
						destSetter.setWeighted(img.at(i,j), C, Opacity, (hiI - lowI) * (hiJ - lowJ));
						maskedElements->at(i, j) = true;
						break;
					}
				}
			}
}

template<class t_Color, class t_SrcColor>
Image<t_Color>* generateMipmap(t_Color* null__ptr, ConstImageProxy<t_SrcColor> src)
{
	if(src.pixels() <= 1 || (src.width() & 1) || (src.height() & 1))
		return 0;
	Image<t_Color>* img = new Image<t_Color>((src.width() + 1) >> 1, (src.height() + 1) >> 1);
	mipgen(img->proxy(), src);
	return img;
}

template<class t_Color, class t_ResultSetter, class t_SrcColor, class t_SrcWeight, class t_WeightGetter>
Image<t_Color>* generateMipmapWeighted(t_Color* null__ptr, t_ResultSetter setter, ConstImageProxy<t_SrcColor> src, const ConstImageProxy<t_SrcWeight>& srcWeight, t_WeightGetter wgetter)
{
	if(src.pixels() <= 1 || (src.width() & 1) || (src.height() & 1))
		return 0;
	Image<t_Color>* img = new Image<t_Color>((src.width() + 1) >> 1, (src.height() + 1) >> 1);
	mipgenWeighted<t_Color>(img->proxy(), setter, src, srcWeight, wgetter);
	return img;
}


template<class t_DstColor, class t_SrcColor>
INLINE void convert(ImageProxy<t_DstColor> dest, ConstImageProxy<t_SrcColor> src)
{
	Assert(dest.width() >= src.width());
	Assert(dest.height() >= src.height());
	for(uint j=0; j < src.height(); ++j)
		for(uint i=0; i < src.width(); ++i){
			typename ColorConvertor<t_SrcColor>::OperationType c = ColorConvertor<t_SrcColor>::unpuck(src(i,j));
			dest(i,j) = ColorConvertor<t_DstColor>::puck(c);
		}
			
}

static INLINE BaseImage* metaGenerateMipmap(ConstBaseImageProxy src)
{
	switch(src.colortype())
	{
	case EColor_RGBA: return generateMipmap((ColorRGBA*)0,*static_cast< ConstImageProxy<ColorRGBA>* >(&src));
	case EColor_RGB: return generateMipmap((ColorRGB*)0,*static_cast< ConstImageProxy<ColorRGB>* >(&src));
	case EColor_L: return generateMipmap((ColorL*)0,*static_cast< ConstImageProxy<ColorL>* >(&src));
	case EColor_LA: return generateMipmap((ColorLA*)0,*static_cast< ConstImageProxy<ColorLA>* >(&src));
	case EColor_RGBF: return generateMipmap((ColorRGBF*)0,*static_cast< ConstImageProxy<ColorRGBF>* >(&src));
	case EColor_RGBAF: return generateMipmap((ColorRGBAF*)0,*static_cast< ConstImageProxy<ColorRGBAF>* >(&src));
	}
	return 0;
}

}
