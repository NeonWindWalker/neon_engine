#include "robust/robust.h"

namespace DrawerLib
{
	
IDevice* createDevice(const DisplayHandle& display)
{
#ifdef DRAWER_STYLE_CLASSIC
	auto dev = new ClassicDrawer(display);
#else
	auto dev = new RobustDrawer(display);
#endif
	if(!dev->isValid()){
		delete dev;
		return 0;
	}
	return dev;
}

IDevice* createDeviceWithMainWindow(IFrameWindow*& retSingleWindow, const WindowHandle& wnd, boolean fullscrean, boolean stereo)
{
#ifdef DRAWER_STYLE_CLASSIC
	auto dev = new ClassicDrawer(retSingleWindow,wnd,fullscrean,stereo);
#else
	auto dev = new RobustDrawer(retSingleWindow, wnd, fullscrean, stereo);
#endif
	if(!dev->isValid())
	{
		delete dev;
		retSingleWindow = 0;
		return 0;
	}
	return dev;
}

}
