 #include "../include.h"

namespace EntryPoint
{

int gInitialWindowWidth = 800;
int gInitialWindowHeight = 600;
const char* gcGameName = "Entrypoint Test";
Input::State gInputState;

using namespace DrawerLib;
DrawerLib::IFrameWindow* gRenderWindow = 0;
DrawerLib::IDevice* gRenderDevice = 0;
DrawerLib::DrawTask gDrawTask;

boolean testInit(FileStorageLib::IStorage* packedge, FileStorageLib::IStorage* homedir)
{
	EntryPoint::renderInit(gRenderDevice, gRenderWindow);
	
	gRenderDevice->setSyncInterval(1);
	gDrawTask.clearColor.set(0.0f, 1.0f, 0.0f, 1.0f);
	gDrawTask.useClearColor = true;
	gDrawTask.renderTarget = gRenderWindow;
	return true;
}

int gCadrNumber = 0;

boolean testStep(Range<const Input::EventBase* const> events)
{
	EntryPoint::renderWindowPrepare(gRenderWindow);
	
	gCadrNumber ++;
	gDrawTask.clearColor = (gCadrNumber % 2) ? Vector4f(1,0,1,1) : Vector4f(0,1,0,1);
	gRenderDevice->draw(gDrawTask);

	EntryPoint::renderWindowPresent(gRenderWindow);
	
	gRenderDevice->freeDeletedResources();
	return true;
}

void testDeInit()
{
	EntryPoint::renderDeInit(gRenderDevice, gRenderWindow);
}

void registerDelegates()
{
	gInitDelegate = makeDelegate(testInit);
	gStepDelegate = makeDelegate(testStep);
	gDeInitDelegate = makeDelegate(testDeInit);
}

}

