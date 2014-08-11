#pragma once

#include "base.h"

namespace ParticlesLib
{

class Atlas
{
public:
	Atlas();

	void beginBuildTextureAtlas(uint maxTextureSize, boolean supportLayers);
	void addImage(ConstString name, ConstImageProxy<ColorRGBA> img, uint number, uint subdivsX = 1, uint subdivsY = 1);
	void addImageByNameWithParameters(ConstString name, ConstImageProxy<ColorRGBA> img);
	void addImageByNameWithParameters(ConstString name, ConstBinaryData data, boolean premultiply, boolean clearAlpha);
	void endBuildTextureAtlas();

	AtlasInfo info;

protected:
	virtual void createTextureArrays(Array<StrongPtr<Image<ColorRGBA> > >& texs)=0;

private:
	ImageLib::AtlasGenerator<ImageLib::ColorRGBA>* atalsCoordGenerator;
	struct NameAndFrame
	{
		String name;
		uint frame;
	};
	Array<NameAndFrame> atalsNameAndFrames;
};


}