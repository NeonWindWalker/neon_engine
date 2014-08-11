#pragma once

#include "image.h"
#include "operations.h"

namespace ImageLib
{

USING_BASE_LIB

class AtlasCoordinatesGenerator
{
public:
	struct Place
	{
		int x, y;
		bool twiddle;
		int atlasId;
	};

	struct AtlasSize
	{
		int width, height;
	};

private:

	struct Frame
	{
		int w, h;
		int count;

		bool operator < ( const Frame& other ) const
		{
			int a = max(w, h);
			int b = max(other.w, other.h);
			if (a != b) return a > b;

			a = min(w, h);
			b = min(other.w, other.h);
			if (a != b) return a > b;

			return w > other.w;
		}

		bool operator == ( const Frame& other ) const
		{
			return w == other.w && h == other.h;
		}
	};

	template<class A, class B> friend class Base::DefaultComparator;

	typedef SortedArray< Pair<Frame, Array<Place>* > > FramePlaces;

private:
	SortedArray<Frame> frames;
	FramePlaces places;
	uint _atlasWidth;
	uint _atlasHeight;
	uint _atlasesCount;
	void testBuild();
public:
	int padding;

public:
	AtlasCoordinatesGenerator()
	{
		padding = 0;
		_atlasWidth = 0;
		_atlasHeight = 0;
		_atlasesCount = 0;
	}
	~AtlasCoordinatesGenerator()
	{
		clear();
	}

	void push(int w, int h);
	void build();
	Place pop(int w, int h);

	uint width() { return _atlasWidth; }
	uint height() { return _atlasHeight; }
	uint count() { return _atlasesCount; }
	void clear();
};


struct AtlasImageCoords
{
	Vector2i position;
	Vector2i size;
	boolean twiddle;
	uint atlas;
};


template<class t_Color>
class AtlasGenerator
{
public:
	AtlasGenerator()
	{
	}

	boolean push(ConstImageProxy<t_Color> img)
	{
		Image<t_Color>* p = new Image<t_Color>();
		*p = img;
		*images.push() = p;
		coordGenerator.push(img.width(), img.height());
		return true;
	}

	uint build(Array< StrongPtr< Image<t_Color> > >& atlases, Array<AtlasImageCoords>& coords, t_Color clearColor, boolean padding = false)
	{
		coordGenerator.padding = padding ? 2 : 0;
		coordGenerator.build();
		for(uint nAtlas = 0; nAtlas < coordGenerator.count(); ++nAtlas)
		{
			Image<t_Color>* atlasImg = new Image<ColorRGBA>(coordGenerator.width(), coordGenerator.height());
			fill(atlasImg->proxy(), clearColor);
			*atlases.push() = atlasImg;
		}

		for(uint nImg=0; nImg < images.size(); ++nImg)
		{
			ImageProxy<ColorRGBA> img = images[nImg]->proxy();
			AtlasCoordinatesGenerator::Place place = coordGenerator.pop(img.width(), img.height());
			/*
			Assert(place.x - 1 >= 0 && place.x - 1 < coordGenerator.width());
			Assert(place.y - 1 >= 0 && place.y - 1 < coordGenerator.height());
			if(place.twiddle)
			{
				Assert(place.x + img.height() + 1 < coordGenerator.width());
				Assert(place.y + img.width() + 1 < coordGenerator.height());
			}
			else
			{
				Assert(place.x + img.width() + 1 < coordGenerator.width());
				Assert(place.y + img.height() + 1 < coordGenerator.height());
			}
			*/

			Image<t_Color>* atlasImg = atlases[place.atlasId];
			AtlasImageCoords& crd = *coords.push();
			crd.position.set(place.x, place.y);
			crd.twiddle = place.twiddle;
			crd.atlas = place.atlasId;
			if(place.twiddle)
			{
				copySrcSwapXY<t_Color>(atlasImg->proxy(place.x, place.y), img);
				if(padding)
					setClampBorder(atlasImg->proxy(place.x - 1, place.y - 1, img.height() + 2, img.width() + 2));
				crd.size.set(img.height(), img.width());
			}
			else
			{
				copy<t_Color>(atlasImg->proxy(place.x, place.y), img);
				if(padding)
					setClampBorder(atlasImg->proxy(place.x - 1, place.y - 1, img.width() + 2, img.height() + 2));
				crd.size.set(img.width(), img.height());
			}
		}
		
		images.clear();
		coordGenerator.clear();
		return atlases.size();
	}
	
private:
	Array< StrongPtr< Image<t_Color> > > images;
	AtlasCoordinatesGenerator coordGenerator;
};

}

namespace Base
{
	template<> class DefaultComparator<const ImageLib::AtlasCoordinatesGenerator::Frame, const ImageLib::AtlasCoordinatesGenerator::Frame>
	{
	public:
		INLINE int operator () (const ImageLib::AtlasCoordinatesGenerator::Frame& A, const ImageLib::AtlasCoordinatesGenerator::Frame& B)const
		{
			int a = max(A.w, A.h);
			int b = max(B.w, B.h);
			if (a != b) return b - a;

			a = min(A.w, A.h);
			b = min(B.w, B.h);
			if (a != b) return b - a;

			return B.w - A.w;
		}
	};
}
