#include "drawer.h"
#include "../../filestorage/include.h"
#include "../widget.h"

namespace GUI
{

	Drawer::Drawer()
	{
		currentTexture = 0;
		atalsCoordGenerator = 0;
	}

	boolean Drawer::addTexture(ConstString name, ConstBinaryData data)
	{
		DrawerLib::ITexture* tex = renderDevice->createTexture();
		tex->init(data, false);
		if(!tex)
			return false;
		SubImage& rec = images(name)->val;
		rec.texture = tex;
		return true;
	}

	boolean Drawer::addImageToAtals(ConstString name, ConstBinaryData data, boolean yUp)
	{
		ImageLib::ImageFileData ifd;
		if(!ImageLib::loadImage(ifd, data, yUp))
			return false;

		StrongPtr< ImageLib::Image<ImageLib::ColorRGBA> > img = ImageLib::convert< ImageLib::Image<ImageLib::ColorRGBA> >(ifd.get());
		if(!img)
			return false;

		if(!atalsCoordGenerator)
			atalsCoordGenerator = new ImageLib::AtlasGenerator<ImageLib::ColorRGBA>();

		atalsCoordGenerator->push(img->proxy());
		*atalsTextureNames.push() = name;
		return true;
	}

	boolean Drawer::buildAtals()
	{
		if(!atalsCoordGenerator)
			return false;

		images.clear();

		Array< StrongPtr< ImageLib::Image<ImageLib::ColorRGBA> > > atlases;
		Array< DrawerLib::ITexture* > atlasTextures;
		Array< ImageLib::AtlasImageCoords > coords;
		atalsCoordGenerator->build(atlases, coords, ImageLib::ColorRGBA(0,0,0,0), 0);

		for(StrongPtr< ImageLib::Image<ImageLib::ColorRGBA> >* it = atlases.first(); it != atlases.end(); ++it)
		{
			DrawerLib::ITexture* tex = renderDevice->createTexture();
			tex->init((*it)->width(), (*it)->height(), (*it)->colortype(), 1 );
			tex->copy(0, (*it)->data());
			atlasTextures.push(tex);

			for(uint i=0; i < atlases.size(); ++i){
				logInfo << "gui atlas size = " << atlases[i]->width() << _CS("x") << atlases[i]->height();
			}

			//BinaryData atlasddata;
			//saveBmpImage(atlasddata, (*it)->proxy());
			//FileStorageLib::openStorage(_CS(""))->write( _CS("guiatlas.bmp"), atlasddata);
		}

		for(uint i=0; i < coords.size(); ++i)
		{
			const ImageLib::AtlasImageCoords& crd = coords[i];
			SubImage& rec = images(atalsTextureNames[i])->val;
			rec.texture = atlasTextures[crd.atlas];
			rec.twiddle = crd.twiddle;

			rec.coords.min.x = Scalar(crd.position.x) + Scalar(0.5);
			rec.coords.min.y = Scalar(crd.position.y) + Scalar(0.5);
			rec.coords.max.x = Scalar(crd.position.x + crd.size.x) - Scalar(0.5);
			rec.coords.max.y = Scalar(crd.position.y + crd.size.y) - Scalar(0.5);

			const ImageLib::ImageInfo& imginf = *atlases[crd.atlas].get();
			Vector k(1.0f / imginf.width(), 1.0f / imginf.height());
			rec.coords.min *= k;
			rec.coords.max *= k;
		}

		delete atalsCoordGenerator;
		atalsCoordGenerator = 0;
		return true;
	}

	Drawer::TextureInfo Drawer::getTextureInfo(ConstString name)
	{
		TextureInfo inf;
		if(name.empty())
			return inf;
		Images_t::Iterator it = images.find(name);
		if(it != images.end()){
			inf.coords = it->val.coords;
			inf.texture = it->val.texture.get();
			inf.twiddle = it->val.twiddle;
			return inf;
		}

		for(auto it = externalTextureGetter.delegates.first(); it != externalTextureGetter.delegates.end(); ++it){
			DrawerLib::ITexture* texa = 0;
			DrawerLib::ITexture* tex = (*it)(texa, name);
			if(tex){
				inf.texture = tex;
				inf.alphaTexture = texa;
				break;
			}
		}

		return inf;
	}


	void Drawer::drawWidget(const Bound& bound, const Vector& pos, const Vector& size, ConstString image, const Vector& scale, const Vector& offset, const Vector4f& color)
	{
		if(image.empty())
			return;

		DecoratorsSet::Iterator decoratorIt = decorators.find(image);
		if(decoratorIt != decorators.end()){
			decoratorIt->val->draw(*this, bound, pos, size, image, scale, offset, color);
			return;
		}

		TextureInfo inf = getTextureInfo(image);
		if(inf.texture)
		{
			Bound v(pos * scale + offset, pos * scale + size * scale + offset);
			Bound t(inf.coords.min, inf.coords.max);

			Bound clipedV, clipedT;

			if(inf.twiddle)
			{
				if(!clipTwiddled(clipedV, clipedT, bound, v, t))
					return;
			}
			else
			{
				if(!clip(clipedV, clipedT, bound, v, t))
					return;
			}

			Vector vert[4];
			vert[0] = clipedV.min;
			vert[1].x = clipedV.max.x;
			vert[1].y = clipedV.min.y;
			vert[2].x = clipedV.min.x;
			vert[2].y = clipedV.max.y;
			vert[3] = clipedV.max;

			Vector tex[4];
			tex[0] = clipedT.min;
			if(inf.twiddle)
			{
				tex[1].x = clipedT.min.x;
				tex[1].y = clipedT.max.y;
				tex[2].x = clipedT.max.x;
				tex[2].y = clipedT.min.y;
			}
			else
			{
				tex[1].x = clipedT.max.x;
				tex[1].y = clipedT.min.y;
				tex[2].x = clipedT.min.x;
				tex[2].y = clipedT.max.y;
			}
			tex[3] = clipedT.max;

			renderFrame->quad(toVector2f(vert[0]), toVector2f(tex[0]), color, toVector2f(vert[1]), toVector2f(tex[1]), color, toVector2f(vert[2]), toVector2f(tex[2]), color, toVector2f(vert[3]), toVector2f(tex[3]), color, *inf.texture, inf.alphaTexture);
		}	
	}

	boolean Drawer::setTexture(ConstString image)
	{
		Images_t::Iterator it = images.find(image);
		if(it != images.end()){
			currentTexture = it->val.texture.get();
			return true;
		}
		currentTexture = 0;
		return false;
	}

	void Drawer::drawFontQuad(const Vector& vmin, const Vector& vmax, const Vector& tmin, const Vector& tmax, const Color& color, uint byteNumber, uint bitNumber, uint bitLenght)
	{
		if(!currentTexture)
			return;

		renderFrame->fontQuad(toVector2f(vmin), toVector2f(vmax), toVector2f(tmin), toVector2f(tmax), color, *currentTexture, byteNumber, bitNumber, bitLenght);
	}


	void QuarterDecoratorAlghoritm::draw(class Drawer& drawer, const Bound& bound, const Vector& pos, const Vector& size, ConstString image, const Vector& scale, const Vector& offset, const Vector4f& color)
	{
		Drawer::TextureInfo inf = drawer.getTextureInfo(image);
		if(!inf.texture)
			return;

		for(uint j=0; j < 2; j++)
			for(uint i=0; i < 2; i++)
			{
				Vector c0 = pos * scale + size * scale * Vector(Scalar(0.5) * i, Scalar(0.5) * j) + offset;
				Bound v(c0, c0 + size * scale * Scalar(0.5));
				Bound t(inf.coords.min, inf.coords.max);

				Bound clipedV, clipedT;

				if(inf.twiddle)
				{
					if(!clipTwiddled(clipedV, clipedT, bound, v, t))
						continue;
				}
				else
				{
					if(!clip(clipedV, clipedT, bound, v, t))
						continue;
				}

				Vector vert[4];
				vert[0] = clipedV.min;
				vert[1].x = clipedV.max.x;
				vert[1].y = clipedV.min.y;
				vert[2].x = clipedV.min.x;
				vert[2].y = clipedV.max.y;
				vert[3] = clipedV.max;

				Vector tex[4];
				tex[0] = clipedT.min;
				if(inf.twiddle)
				{
					tex[1].x = clipedT.min.x;
					tex[1].y = clipedT.max.y;
					tex[2].x = clipedT.max.x;
					tex[2].y = clipedT.min.y;
				}
				else
				{
					tex[1].x = clipedT.max.x;
					tex[1].y = clipedT.min.y;
					tex[2].x = clipedT.min.x;
					tex[2].y = clipedT.max.y;
				}
				tex[3] = clipedT.max;

				if(i == 1)
				{
					swap(tex[0], tex[1]);
					swap(tex[2], tex[3]);
				}

				if(j == 1)
				{
					swap(tex[0], tex[2]);
					swap(tex[1], tex[3]);
				}

				drawer.renderFrame->quad(toVector2f(vert[0]), toVector2f(tex[0]), color, toVector2f(vert[1]), toVector2f(tex[1]), color, toVector2f(vert[2]), toVector2f(tex[2]), color, toVector2f(vert[3]), toVector2f(tex[3]), color, *inf.texture);
			}
	}


}
