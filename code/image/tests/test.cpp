#include "include.h"
#include "../filesystem/filesystem.h"
#include "../base/base.h"

namespace ImageLib
{

void doImageTest(const char* dir)
{
	StrongPtr<FileSystem::IStorage> fs = FileSystem::openStorage(_CS(""));

	AtlasGenerator<ColorRGBA> generator;
	for(uint n=0; n < 20; ++n)
	{
		Image<ColorRGBA> img(3 * rand(5,100), rand(5,100));
		fill(img.proxy(), ColorRGBA(0,100,0,0));
		setDottedBorder(img.proxy(), ColorRGBA(255,0,0,0), ColorRGBA(0,255,0,0));

		generator.push(img.proxy());
	}

	Array< StrongPtr< Image<ColorRGBA> > > atlases;
	Array< AtlasImageCoords > coords;
	generator.build(atlases, coords, ColorRGBA(0,0,0,0));

	for(uint i=0; i < atlases.size(); ++i){
		BinaryData atlasddata;
		saveBmpImage(atlasddata, atlases[i]->proxy());
		fs->write( String() << _CS("atlas") << print(i) << _CS(".bmp"), atlasddata);
	}

	BinaryData data;
	fs->read(data, _CS("atlas0.bmp"));

	ImageFileInfo ii;
	if(getImageFileInfo(ii, data))
	{
		StrongPtr<BaseImage> baseimg = loadImage(data);
		StrongPtr< Image<ColorRGBA> > img = baseimg->cast<ColorRGBA>();

		img = generateMipmap<ColorRGBA>(img->proxy());

		BinaryData computeddata;
		saveBmpImage(computeddata, img->proxy());
		fs->write(_CS("atlas0_mip.bmp"), computeddata);
	}

	logInfoSkim << "Image Test Succesed";
}

}