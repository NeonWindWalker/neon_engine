#include "androidBaseDevice.h"

#if defined(DRAWER_API_OPENGL_ES2) && defined(__APPLE__)

extern "C"
{
    bool MyRenderIniter_initRender(void* slf);
    void MyRenderIniter_deinitRender(void* slf);
    void* MyRenderIniter_createRenderWindowIniter(void* slf);
    void MyRenderIniter_deleteRenderWindowIniter(void* slf, void* wi);
    void MyRenderIniter_setSyncInterval(void* slf, int n);
    
    int MyRenderWindowIniter_initRenderWindow(void* slf, void* myRenderView);
    void MyRenderWindowIniter_deinitRenderWindow(void* slf);
    int MyRenderWindowIniter_resize(void* slf);
    bool MyRenderWindowIniter_present(void* slf);
    void MyRenderWindowIniter_size(void* slf, int* width, int* height);
}

namespace DrawerLib
{

BaseOSDevice::BaseOSDevice(const DisplayHandle& display)
{
	_window.renderIniter = display.renderIniter;
    _bIsValid = MyRenderIniter_initRender(_window.renderIniter);
}

BaseOSDevice::BaseOSDevice(const WindowHandle& srcWainWindowHandle, boolean fullscrean, boolean stereo)
{
	_window = srcWainWindowHandle;
    _bIsValid = MyRenderIniter_initRender(_window.renderIniter);
}

BaseOSDevice::~BaseOSDevice()
{
	MyRenderIniter_deinitRender(_window.renderIniter);
}

boolean BaseOSDevice::setSyncInterval(uint synk)
{
	MyRenderIniter_setSyncInterval(_window.renderIniter, synk);
    return true;
}

BaseOSFrameWindow::BaseOSFrameWindow(BaseOSDevice& dev, boolean isMainWindowHack) : IFrameWindow(dev), _osdevice(dev)
{
	_window = _osdevice.window();
	if(!isMainWindowHack)
		_window.myRenderView = 0;
    renderWindowIniter = MyRenderIniter_createRenderWindowIniter(dev.window().renderIniter);
}

BaseOSFrameWindow::~BaseOSFrameWindow()
{
    MyRenderIniter_deleteRenderWindowIniter(_window.renderIniter, renderWindowIniter);
}


boolean BaseOSFrameWindow::init(const WindowHandle& wnd, boolean fullscrean, boolean stereo)
{
	_window = wnd;
    _glFBO = MyRenderWindowIniter_initRenderWindow(renderWindowIniter, _window.myRenderView);
    return isValid();
}

void BaseOSFrameWindow::deinit()
{
    MyRenderWindowIniter_deinitRenderWindow(renderWindowIniter);
	_glFBO = 0;
}

void BaseOSFrameWindow::present()
{
    MyRenderWindowIniter_present(renderWindowIniter);
}
    
void BaseOSFrameWindow::resize()
{
    _glFBO = MyRenderWindowIniter_resize(renderWindowIniter);
}
    
Vector2i BaseOSFrameWindow::size()const
{
    Vector2i s;
    MyRenderWindowIniter_size(renderWindowIniter, &s.x, &s.y);
    return s;
}
    

}

#endif
