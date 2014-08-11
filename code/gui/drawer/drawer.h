#include "../drawer.h"
#include "../../image/image.h"
#include "../../image/atlas.h"
#include "../../drawer/include.h"
#include "../../reflection/include.h"

namespace GUI
{

class DecoratorAlghoritm : public ReflectionLib::Object
{
public:
	rl_abstract_class(GUI::DecoratorAlghoritm)
	rl_base_class0(ReflectionLib::Object)
	rl_reflective

	virtual void draw(class Drawer& drawer, const Bound& bound, const Vector& pos, const Vector& size, ConstString image, const Vector& scale, const Vector& offset, const Vector4f& color) = 0;
};


class Drawer : public IDrawer
{
public:
	rl_class(GUI::Drawer)

	struct TextureInfo
	{
		DrawerLib::ITexture* texture;
		DrawerLib::ITexture* alphaTexture;
		Bound coords;
		boolean twiddle;
		TextureInfo()
		{
			coords.set(0,0,1,1);
			twiddle = false;
			texture = 0;
			alphaTexture = 0;
		}
	};

	TextureInfo getTextureInfo(ConstString name);

	DrawerLib::IDevice* renderDevice;
	DrawerLib::InterfaceDrawHelper* renderFrame;
	Event<DrawerLib::ITexture* (DrawerLib::ITexture*&, ConstString)> externalTextureGetter;

	typedef HashArrayOfConstructables< DecoratorAlghoritm, StrongPtr<DecoratorAlghoritm> > DecoratorsSet;
	rl_field(DecoratorsSet, decorators);

	Drawer();

	boolean addTexture(ConstString name, ConstBinaryData data);
	boolean addImageToAtals(ConstString name, ConstBinaryData data, boolean yUp = true);
	boolean buildAtals();
	void clearWidgetTextures() { images.clear(); }

	void drawWidget(const Bound& bound, const Vector& pos, const Vector& size, ConstString image, const Vector& scale, const Vector& offset, const Vector4f& color = Vector4f(1,1,1,1));

	boolean setTexture(ConstString image);
	void drawFontQuad(const Vector& vmin, const Vector& vmax, const Vector& tmin, const Vector& tmax, const Color& color, uint byteNumber, uint bitNumber, uint bitLenght);

private:
	struct SubImage
	{
		StrongPtr<DrawerLib::ITexture> texture;
		StrongPtr<DrawerLib::ITexture> alphaTexture;
		Bound coords;
		boolean twiddle;
		SubImage()
		{
			coords.set(0,0,1,1);
			twiddle = false;
		}
	};

	typedef HashArray< Pair<String, SubImage> > Images_t;
	Images_t images;
	DrawerLib::ITexture* currentTexture;
	ImageLib::AtlasGenerator<ImageLib::ColorRGBA>* atalsCoordGenerator;
	Array<String> atalsTextureNames;
};


class QuarterDecoratorAlghoritm : public DecoratorAlghoritm
{
public:
	rl_class(GUI::QuarterDecoratorAlghoritm)
	rl_base_class0(GUI::DecoratorAlghoritm)
	rl_reflective

	void draw(class Drawer& drawer, const Bound& bound, const Vector& pos, const Vector& size, ConstString image, const Vector& scale, const Vector& offset, const Vector4f& color);
};

}
