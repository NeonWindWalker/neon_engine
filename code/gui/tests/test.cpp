#include "../include.h"
#include "../../entrypoint/entrypoint.h"

USING_BASE_LIB

int nCadr = 0;
const char* EntryPoint::gcGameName = "GUI Test";
int EntryPoint::gInitialWindowWidth = 320;
int EntryPoint::gInitialWindowHeight = 480;
Input::State EntryPoint::gInputState;

Render::IDevice* gRenderDevice = 0;
Render::IFrameWindow* gRenderWindow = 0;
Input::State* gInputState = 0;
Base::StrongPtr<GUI::Screen> gScreen;
GUI::Drawer* gDrawer;
GUI::Library* gLibrary;


Base::boolean EntryPoint::init(FileSystem::IStorage* packedge, FileSystem::IStorage* homedir)
{
	ReflectionLib::initReflectionLibrary();

	::gInputState = &EntryPoint::gInputState;
	if(!EntryPoint::renderInit(gRenderDevice, gRenderWindow))
		return false;

	gScreen = new GUI::Screen();

	BinaryData file;
	packedge->read(file, _CS("gui.txt"));
	ReflectionLib::Reflection rl = ReflectionLib::reflectionOf(gScreen.get());
	rl.deserialize(ConstString((const utf8*)file.first(), file.byteSize()));

	/*gScreen->childs.resize(1);
	GUI::Widget* w = gScreen->childs.construct(0, ReflectionLib::typeOf<GUI::Widget>());
	w->size.set(16,16);
	w->image = _CS("default");*/

	gDrawer = new GUI::Drawer(*gRenderDevice);
	gDrawer->renderFrame = gRenderWindow;

	gLibrary = new GUI::Library();
	gScreen->library = gLibrary;

	Array<String> files;
	packedge->list(files, _CS("gui/widgets"));
	for(uint i=0; i < files.size(); ++i){
		BinaryData data;
		String fn = files[i];
		packedge->read(data, String(_CS("gui/widgets/")) << files[i]);
		String name(files[i].first(), files[i].find('.'));
		gDrawer->addImageToAtals(name, data);
	}

	gDrawer->buildAtals();

	files.clear();
	packedge->list(files, _CS("gui/fonts"));
	for(uint i=0; i < files.size(); ++i){

		if(!files[i].endsWith(_CS(".fnt")))
			continue;

		ConstString name = files[i].afterRightTrim(4);

		BinaryData crddata;
		packedge->read(crddata, String(_CS("gui/fonts/")) << name << _CS(".fnt") );

		BinaryData imgdata;
		packedge->read(imgdata, String(_CS("gui/fonts/")) << name << _CS(".bmp"));

		gDrawer->addTexture(name, imgdata);

		gLibrary->addFont(name, crddata);
	}

	return true;
}

boolean EntryPoint::step(Range<const Input::EventBase* const> events)
{
	EntryPoint::renderWindowPrepare(gRenderWindow);

	gScreen->size = gRenderWindow->size();

	for(uint n=0; n < events.size(); ++n)
		gScreen->recive(*events[n]);

	gScreen->update(0.01f);

	gScreen->draw(*gDrawer);

	gRenderWindow->clearColor.set(0,0,0,0);
	gRenderWindow->useClearColor = true;
	gRenderWindow->interfaceScreenSize = gScreen->size;
	gRenderDevice->draw();
	gRenderDevice->barier();

	EntryPoint::renderWindowPresent(gRenderWindow);
	return true;
}

extern void EntryPoint::stepFinish()
{
}

void EntryPoint::deinit()
{
	delete gScreen;
	delete gLibrary;
	delete gDrawer;

	EntryPoint::renderDeInit(gRenderDevice, gRenderWindow);
	ReflectionLib::deinitReflectionLibrary();
}
