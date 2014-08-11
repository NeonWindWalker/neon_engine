#include "../base/include.h"
#include "../filestorage/include.h"
#include "../image/include.h"
#include "include.h"

namespace TexExp
{

USING_BASE_LIB;
using namespace FileStorageLib;
using namespace ImageLib;

#define PALNK_BLUE 127
#define PALNK_BLUE_FLT (float(PALNK_BLUE) / 255.0f)

IStorage* gSourceStorage = 0;
IStorage* gDestanationStorage = 0;

class AlphaGeter : public ColorConvertor<ColorL>
{
public:
	static INLINE float unpuck(const ColorRGBA& src)
	{
		return float(src.a) * (1.0f / 255.0f);
	}
};

class RGBASetter : public ColorConvertor<ColorRGBA>
{
public:
	static INLINE void setWeighted(ColorRGBA& dst, const OperationType& src, float weight, uint pixels)
	{
		if(weight > 0.0f)
			dst = ColorConvertor<ColorRGBA>::puck(Vector4f(src.xyz() / weight, weight / pixels));
		else
			dst = ColorRGBA(0,0,0,0);
	}
};

class RGBASetterAlphaZero : public ColorConvertor<ColorRGBA>
{
public:
	static INLINE void setWeighted(ColorRGBA& dst, const OperationType& src, float weight, uint pixels)
	{
		if(weight > 0.0f)
			dst = ColorConvertor<ColorRGBA>::puck(Vector4f(src.xyz() / weight, 0.0f));
		else
			dst = ColorRGBA(0,0,0,0);
	}
};

class RGBAFSetter : public ColorConvertor<ColorRGBA>
{
public:
	static INLINE void setWeighted(ColorRGBAF& dst, const OperationType& src, float weight, uint pixels)
	{
		if(weight > 0.0f)
			dst = src / weight;
		else
			dst.setZero();
	}
};




class TranRGBASetter : public ColorConvertor<ColorRGBA>
{
public:
	static INLINE void setWeighted(ColorRGBA& dst, const OperationType& src, float weight, uint pixels)
	{
		if(weight > 0.0f)
			dst = puck(src / weight);
		else
			dst.set(0);
	}
};

class TranRGASetter : public ColorConvertor<ColorRGBA>
{
public:
	static INLINE void setWeighted(ColorRGBA& dst, const OperationType& src, float weight, uint pixels)
	{
		if(weight > 0.0f){
			dst = puck(src / weight);
			dst.b = 0;
		}
		else
			dst.set(0);
	}
};

class TranBGetter : public ColorConvertor<ColorL>
{
public:
	static INLINE float unpuck(const ColorRGBA& src)
	{
		return src.b > PALNK_BLUE ? 1.0f : 0.0f;
	}
};

class TranRGBSetter : public ColorConvertor<ColorRGB>
{
public:
	static INLINE void setWeighted(ColorRGB& dst, const OperationType& src, float weight, uint pixels)
	{
		if(weight > 0.0f)
			dst = puck(src / weight);
		else
			dst.set(0,0,0);
	}
};


void doTexExport(ConstString rootPath, Parameters* prams)
{
	logInfo << "root path = " << rootPath;

	BinaryData bindata;
	IStorage* gRootStorage = openStorage(rootPath);
	if(!gRootStorage->read(bindata, _CS("texexp.par"))){
		logError << "cant read params file";
		delete gRootStorage;
		return;
	}
#ifndef DEBUG
	gRootStorage->erase(_CS("texexp.par"));
#endif

	delete gRootStorage;
	String localFileName((utf8*)bindata.first(), bindata.byteSize());
	
	doTexExport(rootPath, localFileName, prams);
}

void saveMultiformatImages(IStorage& storage, const ImageFileData& imgSet, ConstString path, ConstString rootPath, boolean alphaValid, boolean pvrSwapEX = false);

void doTexExport(ConstString rootPath, ConstString localFileName, Parameters* prams)
{
	logInfo << "root path = " << rootPath;
	gSourceStorage = openStorage(String() << rootPath << _CS("sources/"));
	gDestanationStorage = openStorage(String() << rootPath << _CS("data/"));
	if(!gSourceStorage || !gDestanationStorage){
		delete gSourceStorage;
		delete gDestanationStorage;
		return;
	}

	boolean bDisableCompression = false;
	if(prams)
		foreach(it, prams->disableCompression)
			if(localFileName.contains(it->range()))
				bDisableCompression = true;

	boolean bDisableMipGen = false;
	if(prams)
		foreach(it, prams->disableMipGen)
			if (localFileName.contains(it->range()))
				bDisableMipGen = true;

	boolean bDisableDownsampling = false;
	if(prams)
		foreach(it, prams->disableDownsapling)
		if(localFileName.contains(it->range()))
			bDisableDownsampling = true;

	BinaryData bindata;

	if (gSourceStorage->read(bindata, String() << localFileName << _CS(".bmp")))
	{
		StrongPtr< Image<ColorRGBA> > img = loadAndConvert<ColorRGBA>(bindata);


		boolean bAlphaTotalZero = true;
		boolean bAlphaTotalOne = true;
		foreach(c, *img){
			if (c->a)
				bAlphaTotalZero = false;
			if (!c->a)
				bAlphaTotalOne = false;
		}

		//photoshop hack
		if (bAlphaTotalZero)
		{
			bAlphaTotalOne = true;
			foreach(c, *img)
				c->a = 255;
		}
		//if(bAlphaTotalZero || localFileName.find(_CS("additive/")) != localFileName.end())
		//	foreach(c, *img)
		//		c->a = 255;

		if (!bDisableDownsampling)
			img = ImageLib::generateMipmapWeighted((ColorRGBA*)0, RGBASetter(), img->constProxy(), img->constProxy(), AlphaGeter());
		ImageLib::transparencyColorAccurate(img->proxy(), RGBASetterAlphaZero(), img->constProxy(), AlphaGeter());

		if (!bDisableCompression)
		{
			gDestanationStorage->erase(String() << localFileName << _CS(".bmp"));
			ImageFileData imgSet;
			imgSet.assign(*img);

			if (!bDisableMipGen)
			{
				while (true)
				{
					img = ImageLib::generateMipmapWeighted((ColorRGBA*)0, RGBASetter(), img->constProxy(), img->constProxy(), AlphaGeter());
					if (!img)
						break;
					*imgSet.images.push() = img;
					imgSet.mipsCount++;
				}
			}


			StrongPtr<IStorage> rootStorage = openStorage(String() << rootPath);
			BinaryData newImg;
			BinaryData srcImg;
			if (!saveDDS(srcImg, imgSet))
				return;

			rootStorage->write(_CS("tmp/src.dds"), srcImg);

			if (!prams || prams->dxtEnable)
			{
				String nvcompressParams = bAlphaTotalOne ? _CS("-bc1 ") : _CS("-bc3 ");
				if (imgSet.mipsCount == 1)
					nvcompressParams << _CS(" -nomips ");
				nvcompressParams << _CS("tmp/src.dds tmp/dstdxt.dds");
				rootStorage->execute(_CS(""), _CS("tools/nv_textool/nvcompress.exe"), nvcompressParams, true);
				rootStorage->read(newImg, _CS("tmp/dstdxt.dds"));
				gDestanationStorage->write(String() << localFileName << _CS(".dxt.dds"), newImg);
			}
			
			if (!prams || prams->atcEnable)
			{
				String atcParams = _CS("-convert -overwrite tmp/src.dds tmp/dstatc.dds ");
				if (bAlphaTotalOne)
					atcParams << _CS("+fourCC \"ATC \" ");
				else
					atcParams << _CS("+fourCC ATCI ");
				rootStorage->execute(_CS(""), _CS("tools/amd_compressonator/TheCompressonator.exe"), atcParams, true);
				rootStorage->read(newImg, _CS("tmp/dstatc.dds"));
				gDestanationStorage->write(String() << localFileName << _CS(".atc.dds"), newImg);
			}

			if(bAlphaTotalOne)
			{
				if (!prams || prams->pvrEnable)
				{
					rootStorage->execute(_CS(""), _CS("tools/PVRTexToolCL.exe"), _CS("-i tmp/src.dds -o tmp/dstpvr.ktx -f PVRTC1_4 -q pvrtcbest"), true);
					rootStorage->read(newImg, _CS("tmp/dstpvr.ktx"));
					gDestanationStorage->write(String() << localFileName << _CS(".pvr.ktx"), newImg);
					gDestanationStorage->erase(String() << localFileName << _CS("_a.pvr.ktx"));
				}

				if (!prams || prams->etcEnable)
				{
					rootStorage->execute(_CS(""), _CS("tools/PVRTexToolCL.exe"), _CS("-i tmp/src.dds -o tmp/dstetc.ktx -f ETC1 -q etcslow"), true);
					rootStorage->read(newImg, _CS("tmp/dstetc.ktx"));
					gDestanationStorage->write(String() << localFileName << _CS(".etc.ktx"), newImg);
					gDestanationStorage->erase(String() << localFileName << _CS("_a.etc.ktx"));
				}
			}
			else
			{
				ImageFileData imgSetC, imgSetA;
				*static_cast<ImageFileInfo*>(&imgSetC) = imgSet;
				*static_cast<ImageFileInfo*>(&imgSetA) = imgSet;
				for(uint mip=0; mip < imgSet.mipsCount; mip++)
				{
					Image<ColorRGBA>* src = imgSet.get(mip).cast<ColorRGBA>();
					Image<ColorRGBA>* im0 = new Image<ColorRGBA>(maximum<uint>(imgSet.width >> mip, 1), maximum<uint>(imgSet.height >> mip, 1));
					Image<ColorRGBA>* im1 = new Image<ColorRGBA>(maximum<uint>(imgSet.width >> mip, 1), maximum<uint>(imgSet.height >> mip, 1));
					*imgSetC.images.push() = im0;
					*imgSetA.images.push() = im1;

					for(uint j = 0; j < im0->height(); ++j)
						for(uint i = 0; i < im0->width(); ++i)
						{
							im0->at(i, j).r = src->at(i,j).r;
							im0->at(i, j).g = src->at(i,j).g;
							im0->at(i, j).b = src->at(i,j).b;
							im0->at(i, j).a = 255;

							im1->at(i, j).r = src->at(i,j).a;
							im1->at(i, j).g = src->at(i,j).a;
							im1->at(i, j).b = src->at(i,j).a;
							im1->at(i, j).a = 255;
						}
				}

				if (!prams || prams->pvrEnable || prams->etcEnable)
				{
					if (!saveDDS(srcImg, imgSetC))
						return;
					rootStorage->write(_CS("tmp/src_c.dds"), srcImg);

					if (!saveDDS(srcImg, imgSetA))
						return;
					rootStorage->write(_CS("tmp/src_a.dds"), srcImg);
				}
				

				if (!prams || prams->pvrEnable)
				{
					rootStorage->execute(_CS(""), _CS("tools/PVRTexToolCL.exe"), _CS("-i tmp/src_c.dds -o tmp/dstpvr.ktx -f PVRTC1_4 -q pvrtcbest"), true);
					rootStorage->read(newImg, _CS("tmp/dstpvr.ktx"));
					gDestanationStorage->write(String() << localFileName << _CS(".pvr.ktx"), newImg);
				}

				if (!prams || prams->etcEnable)
				{
					rootStorage->execute(_CS(""), _CS("tools/PVRTexToolCL.exe"), _CS("-i tmp/src_c.dds -o tmp/dstetc.ktx -f ETC1 -q etcslow"), true);
					rootStorage->read(newImg, _CS("tmp/dstetc.ktx"));
					gDestanationStorage->write(String() << localFileName << _CS(".etc.ktx"), newImg);
				}

				if (!prams || prams->pvrEnable)
				{
					rootStorage->execute(_CS(""), _CS("tools/PVRTexToolCL.exe"), _CS("-i tmp/src_a.dds -o tmp/dstpvr.ktx -f PVRTC1_4 -q pvrtcbest"), true);
					rootStorage->read(newImg, _CS("tmp/dstpvr.ktx"));
					gDestanationStorage->write(String() << localFileName << _CS("_a.pvr.ktx"), newImg);
				}

				if (!prams || prams->etcEnable)
				{
					rootStorage->execute(_CS(""), _CS("tools/PVRTexToolCL.exe"), _CS("-i tmp/src_a.dds -o tmp/dstetc.ktx -f ETC1 -q etcslow"), true);
					rootStorage->read(newImg, _CS("tmp/dstetc.ktx"));
					gDestanationStorage->write(String() << localFileName << _CS("_a.etc.ktx"), newImg);
				}
			}
		}
		else
		{
			saveBmpImage(bindata, *img);
			gDestanationStorage->write(String() << localFileName << _CS(".bmp"), bindata);
		}
		
		logInfo << "writed " << localFileName;
	}
	else
	{
		StrongPtr< Image<ColorRGBA> > albedo;
		StrongPtr< Image<ColorRGBA> > normal;
		StrongPtr< Image<ColorL> > opacity;
		StrongPtr< Image<ColorL> > specular;
		StrongPtr< Image<ColorL> > glossines;
		StrongPtr< Image<ColorL> > frensel;

		if(gSourceStorage->read(bindata, String() << localFileName << _CS("_albedo.bmp")))
			albedo = loadAndConvert<ColorRGBA>(bindata);

		if(!albedo){
			delete gSourceStorage;
			delete gDestanationStorage;
			return;
		}
		if(gSourceStorage->read(bindata, String() << localFileName << _CS("_opacity.bmp")))
			opacity = loadAndConvert<ColorL>(bindata);
		if(gSourceStorage->read(bindata, String() << localFileName << _CS("_normal.bmp")))
			normal = loadAndConvert<ColorRGBA>(bindata);
		if(gSourceStorage->read(bindata,String() << localFileName <<  _CS("_specular.bmp")))
			specular = loadAndConvert<ColorL>(bindata);
		if(gSourceStorage->read(bindata, String() << localFileName << _CS("_glossines.bmp")))
			glossines = loadAndConvert<ColorL>(bindata);
		if(gSourceStorage->read(bindata, String() << localFileName << _CS("_frensel.bmp")))
			frensel = loadAndConvert<ColorL>(bindata);

		StrongPtr< Image<ColorRGBAF> > pack0Img = new Image<ColorRGBAF>(albedo->width(), albedo->height());
		StrongPtr< Image<ColorRGBAF> > pack1Img = new Image<ColorRGBAF>(albedo->width(), albedo->height());
		StrongPtr< Image<float> > opacityImg = new Image<float>(albedo->width(), albedo->height());
		for(uint j = 0; j < pack0Img->height(); ++j)
			for(uint i = 0; i < pack0Img->width(); ++i)
			{
				pack0Img->at(i, j).x = float(albedo->at(i,j).r) / 255.0f;
				pack0Img->at(i, j).y = float(albedo->at(i,j).g) / 255.0f;
				pack0Img->at(i, j).z = float(albedo->at(i,j).b) / 255.0f;
				pack0Img->at(i, j).w = 0.5f;
				pack1Img->at(i, j).x = 0.0f;
				pack1Img->at(i, j).y = 0.5f;
				pack1Img->at(i, j).z = 0.5f;
				pack1Img->at(i, j).w = 0.5f;
				opacityImg->at(i, j) = 1.0f;// (albedo->at(i, j).r || albedo->at(i, j).g || albedo->at(i, j).b) ? 1.0f : 0.0f;
			}

		if(normal)
		{
			for(uint j = 0; j < pack0Img->height(); ++j)
				for(uint i = 0; i < pack0Img->width(); ++i)
				{
					pack0Img->at(i, j).w = float(normal->at(i,j).r - 127) / 127.0f;
					pack1Img->at(i, j).w = float(normal->at(i,j).g - 127) / 127.0f;
				}
		}

		if(specular)
		{
			for(uint j = 0; j < pack0Img->height(); ++j)
				for(uint i = 0; i < pack0Img->width(); ++i)
				{
					pack1Img->at(i, j).x = float(specular->at(i,j)) / 255.0f;
					if(specular->at(i,j))
						opacityImg->at(i, j) = 1.0f;
				}
		}

		if(glossines)
		{
			for(uint j = 0; j < pack0Img->height(); ++j)
				for(uint i = 0; i < pack0Img->width(); ++i)
				{
					pack1Img->at(i, j).y = float(glossines->at(i,j)) / 255.0f;
				}
		}

		if(frensel)
		{
			for(uint j = 0; j < pack0Img->height(); ++j)
				for(uint i = 0; i < pack0Img->width(); ++i)
					pack1Img->at(i, j).z = float(frensel->at(i,j)) / 255.0f;
		}

		if(opacity)
		{
			for(uint j = 0; j < pack0Img->height(); ++j)
				for(uint i = 0; i < pack0Img->width(); ++i)
					opacityImg->at(i, j) = opacity->at(i,j) / 255.0f;
		}

		gDestanationStorage->erase(String() << localFileName << _CS("_0.bmp"));
		gDestanationStorage->erase(String() << localFileName << _CS("_1.bmp"));

		ImageFileData imagesSet[2];
		ImageFileData imagesSetLow[2];
		for(uint miplevel=0; true; miplevel++)
		{
			for(uint j = 0; j < pack0Img->height(); ++j)
				for(uint i = 0; i < pack0Img->width(); ++i)
				{
					Vector3f n;
					n.x = pack0Img->at(i, j).w; 
					n.y = pack1Img->at(i, j).w;
					float sq = n.xy().lenghtSquare();
					if(sq > 1.0f)
					{
						n.z = 0.0f;
						n.selfNormalize();
					}

					pack0Img->at(i, j).w = n.x;
					pack1Img->at(i, j).w = n.y;
				}

			if(!bDisableDownsampling || miplevel != 0)
			{
				pack0Img = ImageLib::generateMipmapWeighted((ColorRGBAF*)0, RGBAFSetter(), pack0Img->constProxy(), opacityImg->constProxy(), ColorConvertor<float>());
				pack1Img = ImageLib::generateMipmapWeighted((ColorRGBAF*)0, RGBAFSetter(), pack1Img->constProxy(), opacityImg->constProxy(), ColorConvertor<float>());
				opacityImg = ImageLib::generateMipmap((float*)0, opacityImg->constProxy());
			}
			if(!pack0Img || !pack1Img)
				break;

			StrongPtr< Image<ColorRGBA> > img0 = ImageLib::convert< Image<ColorRGBA> >(pack0Img->constProxy());
			StrongPtr< Image<ColorRGBA> > img1 = ImageLib::convert< Image<ColorRGBA> >(pack1Img->constProxy());
			StrongPtr< Image<ColorL> > imgOpDiscrete01 = new Image<ColorL> (img0->width(), img0->height());
			StrongPtr< Image<ColorRGBA> > lowImg0 = new Image<ColorRGBA> (img0->width(), img0->height());
			StrongPtr< Image<ColorRGBA> > lowImg1 = new Image<ColorRGBA> (img0->width(), img0->height());

			Vector3f toLightDir(1,1,2);
			toLightDir.selfNormalize();
			for(uint j = 0; j < pack0Img->height(); ++j)
				for(uint i = 0; i < pack0Img->width(); ++i)
				{
					img0->at(i, j).a = clampt<int>(int(pack0Img->at(i, j).w * 127.0f + 127.5f), 0, 254);
					img1->at(i, j).a = clampt<int>(int(pack1Img->at(i, j).w * 127.0f + 127.5f), 0, 254);
					

					Vector3f albedo = pack0Img->at(i,j).xyz();
					Vector3f normal;
					normal.x = pack0Img->at(i,j).w;
					normal.y = pack1Img->at(i,j).w;
					normal.z = Base::sqrt(max(0.0f, 1.0f - normal.xy().lenghtSquare()));
					float specular = pack1Img->at(i,j).x;
					float gloss = pack1Img->at(i,j).y;
					float frensel = pack1Img->at(i,j).z;

					Vector3f resDiffuse;
					Vector3f resSpecular;
					float sp = specular * (0.5f - Base::abs(gloss - 0.5f)) * 2.0f * frensel;
					resSpecular.set(sp, sp, sp);
					resDiffuse = albedo * 0.2f + albedo * 0.8f * Base::sqrt(max(0.0f,normal.dot(toLightDir)));
					resSpecular.selfClamp01();
					resDiffuse.selfClamp01();
					lowImg0->at(i,j) = ColorConvertor<ColorRGBA>::puck(Vector4f(resDiffuse, 1.0f));
					lowImg1->at(i,j) = ColorConvertor<ColorRGBA>::puck(Vector4f(resSpecular * 0.5f + Vector3f(0.5f, 0.5f, 0.5f), 1.0f));

					if(opacityImg && opacityImg->at(i, j) < 0.5f){
						img1->at(i, j).b = 0;
						lowImg1->at(i, j).set(0,0,0,255);
						imgOpDiscrete01->at(i, j) = 0;
					}
					else{
						img1->at(i, j).b = clampt<uint>(uint((pack1Img->at(i, j).z * 0.5f + 0.5f) * 255.0f), PALNK_BLUE + 1, 255);
						lowImg1->at(i, j).b = clampt<uint>(lowImg1->at(i, j).b, 128, 255);
						imgOpDiscrete01->at(i, j) = 255;
					}
				}


			ImageLib::transparencyColorAccurate(img0->proxy(), TranRGBASetter(), img1->constProxy(), TranBGetter());
			ImageLib::transparencyColorAccurate(img1->proxy(), TranRGASetter(), img1->constProxy(), TranBGetter());

			ImageLib::transparencyColorAccurate(lowImg0->proxy(), TranRGBASetter(), imgOpDiscrete01->constProxy(), ColorConvertor<ColorL>());

			*imagesSet[0].images.push() = img0;
			*imagesSet[1].images.push() = img1;
			imagesSet[0].mipsCount = miplevel + 1;
			imagesSet[1].mipsCount = miplevel + 1;
			if(miplevel == 0)
			{
				imagesSet[0].width = imagesSet[1].width = img0->width();
				imagesSet[0].height = imagesSet[1].height = img0->height();
				imagesSet[0].colorType = imagesSet[1].colorType = EColor_RGBA;
			}

			*imagesSetLow[0].images.push() = lowImg0;
			*imagesSetLow[1].images.push() = lowImg1;
			imagesSetLow[0].mipsCount = miplevel + 1;
			imagesSetLow[1].mipsCount = miplevel + 1;
			if(miplevel == 0)
			{
				imagesSetLow[0].width = imagesSetLow[1].width = lowImg0->width();
				imagesSetLow[0].height = imagesSetLow[1].height = lowImg0->height();
				imagesSetLow[0].colorType = imagesSetLow[1].colorType = EColor_RGBA;
			}
				
				//gDestanationStorage->write(String() << localFileName << _CS("_0_mip") << miplevel << _CS(".bmp"), bindata);

			//saveBmpImage(bindata, *img0);
			//gDestanationStorage->write(String() << localFileName << _CS("_0_mip") << miplevel << _CS(".bmp"), bindata);
			//saveBmpImage(bindata, *img1);
			//gDestanationStorage->write(String() << localFileName << _CS("_1_mip") << miplevel << _CS(".bmp"), bindata);


			//logInfo << "writed dual imgs " << localFileName << miplevel;
		}

		StrongPtr<IStorage> rootStorage = openStorage(String() << rootPath);
		for(uint nImg=0; nImg < 2; ++nImg)
		{
			String path;
			path << localFileName << _CS("_") << nImg;

			BinaryData srcImg;
			if(!saveDDS(srcImg, imagesSet[nImg]))
				return;
			BinaryData newImg;

			rootStorage->write(_CS("tmp/src.dds"), srcImg);

			if (!prams || prams->dxtEnable)
			{
				rootStorage->execute(_CS(""), _CS("tools/nv_textool/nvcompress.exe"), _CS("-bc3 tmp/src.dds tmp/dstdxt.dds"), true);
				rootStorage->read(newImg, _CS("tmp/dstdxt.dds"));
				gDestanationStorage->write(String() << path << _CS(".dxt.dds"), newImg);
			}

			if (!prams || prams->atcEnable)
			{
				rootStorage->execute(_CS(""), _CS("tools/amd_compressonator/TheCompressonator.exe"), _CS("-convert -overwrite tmp/src.dds tmp/dstatc.dds +fourCC ATCI"), true);
				rootStorage->read(newImg, _CS("tmp/dstatc.dds"));
				gDestanationStorage->write(String() << path << _CS(".atc.dds"), newImg);
			}
		}


		ImageFileData imagesSetX3[3];
		*static_cast<ImageFileInfo*>(&imagesSetX3[0]) = imagesSet[0];
		*static_cast<ImageFileInfo*>(&imagesSetX3[1]) = imagesSet[0];
		*static_cast<ImageFileInfo*>(&imagesSetX3[2]) = imagesSet[0];
		for(uint mip=0; mip < imagesSet[0].mipsCount; mip++)
		{
			Image<ColorRGBA>* src0 = imagesSet[0].get(mip).cast<ColorRGBA>();
			Image<ColorRGBA>* src1 = imagesSet[1].get(mip).cast<ColorRGBA>();
			Image<ColorRGBA>* im0 = new Image<ColorRGBA>(maximum<uint>(imagesSet[0].width >> mip, 1), maximum<uint>(imagesSet[0].height >> mip, 1));
			Image<ColorRGBA>* im1 = new Image<ColorRGBA>(maximum<uint>(imagesSet[0].width >> mip, 1), maximum<uint>(imagesSet[0].height >> mip, 1));
			Image<ColorRGBA>* im2 = new Image<ColorRGBA>(maximum<uint>(imagesSet[0].width >> mip, 1), maximum<uint>(imagesSet[0].height >> mip, 1));
			*imagesSetX3[0].images.push() = im0;
			*imagesSetX3[1].images.push() = im1;
			*imagesSetX3[2].images.push() = im2;
			
			for(uint j = 0; j < im0->height(); ++j)
				for(uint i = 0; i < im0->width(); ++i)
				{
					im0->at(i, j).r = src0->at(i,j).r;
					im0->at(i, j).g = src0->at(i,j).g;
					im0->at(i, j).b = src0->at(i,j).b;
					im0->at(i, j).a = 255;

					im1->at(i, j).r = src0->at(i,j).a;
					im1->at(i, j).g = src1->at(i,j).a;
					im1->at(i, j).b = 0;
					im1->at(i, j).a = 255;

					im2->at(i, j).r = src1->at(i,j).r;
					im2->at(i, j).g = src1->at(i,j).g;
					im2->at(i, j).b = src1->at(i,j).b;
					im2->at(i, j).a = 255;
				}
		}

		for(uint nImg=0; nImg < 3; ++nImg)
		{
			String path;
			path << localFileName << _CS("_") << nImg;

			BinaryData newImg;
			BinaryData srcImg;
			if(!saveDDS(srcImg, imagesSetX3[nImg]))
				return;
			rootStorage->write(_CS("tmp/src.dds"), srcImg);

			if (!prams || prams->pvrEnable)
			{
				rootStorage->execute(_CS(""), _CS("tools/PVRTexToolCL.exe"), _CS("-i tmp/src.dds -o tmp/dstpvr.ktx -f PVRTC1_4 -q pvrtcbest"), true);
				rootStorage->read(newImg, _CS("tmp/dstpvr.ktx"));
				gDestanationStorage->write(String() << path << _CS(".pvr.ktx"), newImg);
			}

			if (!prams || prams->etcEnable)
			{
				rootStorage->execute(_CS(""), _CS("tools/PVRTexToolCL.exe"), _CS("-i tmp/src.dds -o tmp/dstetc.ktx -f ETC1 -q etcslow"), true);
				rootStorage->read(newImg, _CS("tmp/dstetc.ktx"));
				gDestanationStorage->write(String() << path << _CS(".etc.ktx"), newImg);
			}
		}


		for(uint nImg=0; nImg < 2; ++nImg)
		{
			String path;
			path << _CS("low/") << localFileName << _CS("_") << nImg;

			BinaryData srcImg;
			if(!saveDDS(srcImg, imagesSetLow[nImg]))
				return;
			BinaryData newImg;

			rootStorage->write(_CS("tmp/src.dds"), srcImg);

			if (!prams || prams->dxtEnable)
			{
				rootStorage->execute(_CS(""), _CS("tools/nv_textool/nvcompress.exe"), _CS("-bc1 tmp/src.dds tmp/dstdxt.dds"), true);
				rootStorage->read(newImg, _CS("tmp/dstdxt.dds"));
				gDestanationStorage->write(String() << path << _CS(".dxt.dds"), newImg);
			}

			if (!prams || prams->atcEnable)
			{
				rootStorage->execute(_CS(""), _CS("tools/amd_compressonator/TheCompressonator.exe"), _CS("-convert -overwrite tmp/src.dds tmp/dstatc.dds +fourCC \"ATC \""), true);
				rootStorage->read(newImg, _CS("tmp/dstatc.dds"));
				gDestanationStorage->write(String() << path << _CS(".atc.dds"), newImg);
			}

			if (!prams || prams->pvrEnable)
			{
				rootStorage->execute(_CS(""), _CS("tools/PVRTexToolCL.exe"), _CS("-i tmp/src.dds -o tmp/dstpvr.ktx -f PVRTC1_4 -q pvrtcbest"), true);
				rootStorage->read(newImg, _CS("tmp/dstpvr.ktx"));
				gDestanationStorage->write(String() << path << _CS(".pvr.ktx"), newImg);
			}

			if (!prams || prams->etcEnable)
			{
				rootStorage->execute(_CS(""), _CS("tools/PVRTexToolCL.exe"), _CS("-i tmp/src.dds -o tmp/dstetc.ktx -f ETC1 -q etcslow"), true);
				rootStorage->read(newImg, _CS("tmp/dstetc.ktx"));
				gDestanationStorage->write(String() << path << _CS(".etc.ktx"), newImg);
			}
		}

	}

	delete gSourceStorage;
	delete gDestanationStorage;
}

/*
void saveMultiformatImages(IStorage& storage, const ImageFileData& imgSet, ConstString path, ConstString rootPath, boolean alphaValid, boolean pvrSwap_Nont_)
{
	StrongPtr<IStorage> rootStorage = openStorage(String() << rootPath);
	BinaryData srcImg;
	if(!saveDDS(srcImg, imgSet))
		return;
	BinaryData newImg;

	rootStorage->write(_CS("tmp/src.dds"), srcImg);

	foreach(it, imgSet.images)
		foreach(c, (*it)->cast<ColorRGBA>())
	{
		ColorRGBA x = *c;
		c.r
	}

	rootStorage->execute(_CS(""), _CS("tools/PVRTexToolCL.exe"), _CS("-i tmp/src.dds -o tmp/dstpvr.ktx -f PVRTC1_4 -l"), true);
	rootStorage->read(newImg, _CS("tmp/dstpvr.ktx"));
	storage.write(String() << path << _CS(".pvr.ktx"), newImg);

	String nvcompressParams = alphaValid ? _CS("-bc3 ") : _CS("-bc1 ");
	if(imgSet.mipsCount == 1)
		nvcompressParams << _CS(" -nomips ");
	nvcompressParams << _CS("tmp/src.dds tmp/dstdxt.dds");
	rootStorage->execute(_CS(""), _CS("tools/nv_textool/nvcompress.exe"), nvcompressParams, true);
	rootStorage->read(newImg, _CS("tmp/dstdxt.dds"));
	storage.write(String() << path << _CS(".dxt.dds"), newImg);

	String atcParams = _CS("-convert -overwrite tmp/src.dds tmp/dstatc.dds ");
	if(alphaValid)
		atcParams << _CS("+fourCC ATCA ");
	else
		atcParams << _CS("+fourCC \"ATC \" ");
	rootStorage->execute(_CS(""), _CS("tools/amd_compressonator/TheCompressonator.exe"), atcParams, true);
	rootStorage->read(newImg, _CS("tmp/dstatc.dds"));
	storage.write(String() << path << _CS(".atc.dds"), newImg);

	if(!alphaValid)
	{
		rootStorage->execute(_CS(""), _CS("tools/PVRTexToolCL.exe"), _CS("-i tmp/src.dds -o tmp/dstetc.ktx -f ETC1"), true);
		rootStorage->read(newImg, _CS("tmp/dstetc.ktx"));
		storage.write(String() << path << _CS(".etc.ktx"), newImg);

		//rootStorage->execute(_CS(""), _CS("etcpack.exe"), String() << _CS("tmp/src.dds tmp/dstetc.ktx -ktx -c etc1"), true);
		//rootStorage->read(newImg, _CS("tmp/dstetc.ktx"));
		//storage.write(String() << path << _CS(".etc"), newImg);
	}
	else
		storage.erase(String() << path << _CS(".etc.ktx"));

	if(alphaValid)
		storage.write(String() << path << _CS(".raw.dds"), srcImg);
	else
		storage.erase(String() << path << _CS(".raw.dds"));

}
*/

}