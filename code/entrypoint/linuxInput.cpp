#include "include.h"

#if defined(__linux__) && !defined(__ANDROID__)

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

#include "../input/x11.h"

USING_BASE_LIB

Array<Input::EventBase*> gEvents;
int gMouseX = -1;
int gMouseY = -1;
int gMouseDX = 0;
int gMouseDY = 0;
boolean gLockMouse = false;

extern Display* gDisplay;
extern int gScreen;
extern Window gWindow;
extern Cursor gHiddenCursor;

void reciveInputBegin()
{
	for(uint i=0; i < gEvents.size(); ++i)
		delete gEvents[i];
	gEvents.clear();
}

void reciveInput(XEvent& we)
{
	if(we.type == MotionNotify)
	{
		if(gMouseX < 0){
			gMouseX = we.xmotion.x;
			gMouseY = we.xmotion.y;
		}else{
			gMouseDX += we.xmotion.x - gMouseX;
			gMouseDY += we.xmotion.y - gMouseY;
			gMouseX = we.xmotion.x;
			gMouseY = we.xmotion.y;
		}
	}
	else
	{
		Input::EventBase* ev = Input::convertKeyEvent(we);
		if(ev)
			gEvents.push(ev);
	}
}

void reciveInputEnd()
{
	if((gMouseDX != 0 || gMouseDY != 0) && gLockMouse){
		Input::EventMotion* ev = new Input::EventMotion();
		ev->dx = gMouseDX;
		ev->dy = -gMouseDY;
		gEvents.push(ev);
	}
	gMouseDX = gMouseDY = 0;

	if(gLockMouse){
		XWindowAttributes attr;
		XGetWindowAttributes(gDisplay, gWindow, &attr);
		gMouseX = attr.width/2;
		gMouseY = attr.height/2;
		XWarpPointer(gDisplay, gWindow, gWindow, 0, 0, attr.width, attr.height, gMouseX, gMouseY);
	}
}

void lockMouse()
{
	gLockMouse = true;
	gMouseX = -1;
	XDefineCursor(gDisplay, gWindow, gHiddenCursor);
	XWindowAttributes attr;
	XGetWindowAttributes(gDisplay, gWindow, &attr);
	gMouseX = attr.width/2;
	gMouseY = attr.height/2;
	XWarpPointer(gDisplay, None, RootWindow(gDisplay, gScreen), 0, 0, 0, 0, attr.x + gMouseX, attr.y + gMouseY);
}

void unlockMouse()
{
	gLockMouse = false;
	XDefineCursor(gDisplay, gWindow, None);
	XWindowAttributes attr;
	XGetWindowAttributes(gDisplay, gWindow, &attr);
	XWarpPointer(gDisplay, gWindow, gWindow, 0, 0, attr.width, attr.height, attr.width/2, -16);
}

#endif
