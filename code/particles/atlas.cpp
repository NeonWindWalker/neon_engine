#include "atlas.h"
#include "../filestorage/include.h"

namespace ParticlesLib
{

Atlas::Atlas()
{

}

void Atlas::beginBuildTextureAtlas(uint maxTextureSize, boolean supportLayers)
{
	atalsCoordGenerator = new ImageLib::AtlasGenerator<ImageLib::ColorRGBA>();
	//atalsCoordGenerator->
	atalsNameAndFrames.clear();
}

void Atlas::addImage(ConstString name, ConstImageProxy<ColorRGBA> img, uint number, uint subdivsX, uint subdivsY)
{
	uint w = img.width() / subdivsX;
	uint h = img.height() / subdivsY;
	for(uint j=0; j < subdivsY; ++j)
		for(uint i=0; i < subdivsX; ++i)
		{
			atalsCoordGenerator->push(img.proxy(i * w, j * h, w, h));
			NameAndFrame& e = *atalsNameAndFrames.push();
			e.name = name;
			e.frame = i + j * subdivsX;
		}
}

void Atlas::addImageByNameWithParameters(ConstString name, ConstImageProxy<ColorRGBA> img)
{
	const utf8* c = name.rfind('_');
	if(c == name.begin()){
		addImage(name, img, 0, 1, 1);
		return;
	}
	ConstString shortname(name.first(), c);
	ConstString postfix(c + 1, name.end());
	uint n;
	if(parse(n, postfix)){
		addImage(shortname, img, n, 1, 1);
		return;
	}
	const utf8* x = postfix.find('x');
	uint w,h;
	if(c == postfix.end() || !parse(w, ConstString(postfix.first(), x)) || !parse(h, ConstString(x + 1, postfix.end()))){
		addImage(name, img, 0, 1, 1);
		return;
	}
	addImage(name, img, 0, w, h);
}

void Atlas::addImageByNameWithParameters(ConstString name, ConstBinaryData data, boolean premultiply, boolean clearAlpha)
{
	ImageLib::ImageFileData ifd;
	if(!ImageLib::loadImage(ifd, data))
		return;

	StrongPtr< ImageLib::Image<ImageLib::ColorRGBA> > img = ImageLib::convert< ImageLib::Image<ImageLib::ColorRGBA> >(ifd.get());
	if(!img)
		return;

	if(premultiply)
	{
		foreach(c, *img)
		{
			c->r = (c->r * c->a) / 255;
			c->g = (c->g * c->a) / 255;
			c->b = (c->b * c->a) / 255;
		}
	}

	if(clearAlpha)
	{
		foreach(c, *img)
		{
			c->a = 0;
		}
	}

	addImageByNameWithParameters(name, img->constProxy());
}


void Atlas::endBuildTextureAtlas()
{
	Array< StrongPtr< Image<ColorRGBA> > > atlases;
	Array< ImageLib::AtlasImageCoords > coords;
	atalsCoordGenerator->build(atlases, coords, ImageLib::ColorRGBA(0,0,0,0), 0);
	delete atalsCoordGenerator;
	if(atlases.empty())
		return;
	createTextureArrays(atlases);

	for(uint i=0; i < atlases.size(); ++i)
		logInfo << "particle atlas size = " << atlases[i]->width() << _CS("x") << atlases[i]->height();

	//BinaryData atlasddata;
	//saveBmpImage(atlasddata, atlases[0]->proxy());
	//FileStorageLib::openStorage(_CS(""))->write( _CS("particles_atlas.bmp"), atlasddata);

	info.animatedImages.clear();
	info.layersCount = atlases.size();
	info.widthDeg = Base::hiOneBit(atlases[0]->width());
	info.heightDeg = Base::hiOneBit(atlases[0]->height());
	foreach(crd, coords)
	{
		const NameAndFrame& nameAndFrame = atalsNameAndFrames[coords.indexOf(crd)];
		boolean bNew;
		AnimatedImageInfo& aii = info.animatedImages.insert(bNew, nameAndFrame.name)->val;
		if(aii.frames.size() <= nameAndFrame.frame)
			aii.frames.resize(nameAndFrame.frame + 1);
		TexCoords& tc = aii.frames[nameAndFrame.frame];
		tc.layer = crd->atlas;

		uint x = (uint(crd->position.x) * 32767) >> info.widthDeg;
		uint y = (uint(crd->position.y) * 32767) >> info.heightDeg;
		uint X = (uint(crd->position.x + crd->size.x) * 32767) >> info.widthDeg;
		uint Y = (uint(crd->position.y + crd->size.y) * 32767) >> info.heightDeg;

		tc.v00x = x;
		tc.v00y = y;
		if(crd->twiddle)
		{
			tc.v01x = x;
			tc.v01y = Y;
			tc.v10x = X;
			tc.v10y = y;
		}
		else
		{
			tc.v01x = X;
			tc.v01y = y;
			tc.v10x = x;
			tc.v10y = Y;
		}
		tc.v11x = X;
		tc.v11y = Y;
	}
}


}