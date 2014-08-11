#ifdef _WIN32

#include "include.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "../input/dinput.h"

extern HWND ghWnd;
extern HWND ghOldhWnd;
extern HINSTANCE ghInstance;
extern Base::boolean gLeaveMouse;
extern Base::boolean gMouseModeChanged;
extern Base::boolean gFillScrean;
extern Base::boolean gFillScreanChanged;
extern Base::boolean gWindowResized;
extern Base::boolean gWindowRecreatedRequest;
extern int gMuteMouseEventsFromTouch;

IDirectInput8*       gDirectInput = 0;
IDirectInputDevice8* gDirectInputKeyboard = 0;
IDirectInputDevice8* gDirectInputMouse = 0;
Base::Array<Input::EventBase*> gEvents;

Base::boolean initDirectInput()
{
	if(FAILED(DirectInput8Create(ghInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&gDirectInput, 0))){
		logError << "DirectInput8Create";
		return false;
	}

	if(FAILED(gDirectInput->CreateDevice(GUID_SysKeyboard, &gDirectInputKeyboard, 0))){
		logError << "gDirectInput->CreateDevice(GUID_SysKeyboard";
		return false;
	}
	if(FAILED(gDirectInputKeyboard->SetDataFormat(&c_dfDIKeyboard))){
		logError << "gDirectInput->CreateDevice(GUID_SysKeyboard";
		return false;
	}
	if(FAILED(gDirectInputKeyboard->SetCooperativeLevel(ghWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))){
		logError << "gDirectInputKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)";
		return false;
	}
	gDirectInputKeyboard->Acquire();


	if(FAILED(gDirectInput->CreateDevice(GUID_SysMouse, &gDirectInputMouse, 0))){
		logError << "gDirectInput->CreateDevice(GUID_SysMouse, &gDirectInputMouse, 0)";
		return false;
	}

	if(FAILED(gDirectInputMouse->SetDataFormat(&c_dfDIMouse2))){
		logError << "gDirectInputMouse->SetDataFormat(&c_dfDIMouse2)";
		return false;
	}
	if(FAILED(gDirectInputMouse->SetCooperativeLevel(ghWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))){
		logError << "gDirectInputMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)";
		return false;
	}
	gDirectInputMouse->Acquire();
	//gMouseModeChanged = true;

	DIPROPDWORD dipdw;
	memset(&dipdw, 0, sizeof(dipdw));
	dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = 128;

	if (FAILED(gDirectInputKeyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph))){
		logError << "gDirectInputKeyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)";
		return false;
	}

	if (FAILED(gDirectInputMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph))){
		logError << "gDirectInputMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)";
		return false;
	}

	logInfo << "Sucsess init input";
	return true;
}

void directInputSetCooperativeLevel()
{
	gDirectInputKeyboard->SetCooperativeLevel(ghWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	gDirectInputMouse->SetCooperativeLevel(ghWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}

void releaseDirectInput()
{
	if(gDirectInputMouse){
		gDirectInputMouse->Unacquire();
		gDirectInputMouse->Release();
		gDirectInputMouse = 0;
	}

	if(gDirectInputKeyboard){
		gDirectInputKeyboard->Unacquire();
		gDirectInputKeyboard->Release();
		gDirectInputKeyboard = 0;
	}

	if(gDirectInput){
		gDirectInput->Release();
		gDirectInput = 0;
	}
}

void directInputUpdate()
{
	if(gDirectInputKeyboard)
	{
		while(true)
		{
			DIDEVICEOBJECTDATA data;
			DWORD num = 1;

			HRESULT hr;
			if((hr = gDirectInputKeyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &data, &num, 0)) != DI_OK)
			{
				if (DIERR_INPUTLOST == hr || DIERR_NOTACQUIRED == hr)
				{
					hr = gDirectInputKeyboard->Acquire();
					if(FAILED(hr))
						break;
				}
				else
					break;
			}

			if(0 == num)
				break;

			Base::boolean on = Base::isBit(data.dwData, 7);
			bool override = false;

			switch(data.dwOfs)
			{
			case DIK_LEFT: if(EntryPoint::gInputState.text[0].printing)override = true; if(on) EntryPoint::gInputState.text[0].caretLeft();  break;
			case DIK_RIGHT: if(EntryPoint::gInputState.text[0].printing)override = true; if(on) EntryPoint::gInputState.text[0].caretRight(); break;
			case DIK_DELETE: if(EntryPoint::gInputState.text[0].printing)override = true; if(on) EntryPoint::gInputState.text[0].deleteAction(); break;
			case DIK_BACKSPACE: if(EntryPoint::gInputState.text[0].printing)override = true; if(on) EntryPoint::gInputState.text[0].backspaceAction(); break;

			case DIK_F1: 
				if(on){
					if(!gLeaveMouse){
						gLeaveMouse = true;
						gMouseModeChanged = true;
					}
				}
				override = true;
				break;
			case DIK_F2:
				if(on){
					gFillScrean = !gFillScrean;
					gFillScreanChanged = true;
				}
				override = true;
				break;
			case DIK_F4:
				if(on && !(GetKeyState(VK_LMENU) & 0x8000)  && !(GetKeyState(VK_RMENU) & 0x8000))
					ShowWindow(ghWnd, SW_MINIMIZE);
				break;
			//case DIK_F3:
			//	if(on){
			//		EntryPoint::gInputState.text[0].printing = !EntryPoint::gInputState.text[0].printing;
			//	}
			//	override = true;
			//	break;
			}

			if(!override)
			{
				Input::EventBase* ev = Input::convertKeyEvent(data);
				if(ev)
					gEvents.push(ev);
			}
		}
	}

	if(gDirectInputMouse)
	{
		int mev = -1;
		Input::EventMotion me;

		while(true)
		{
			DIDEVICEOBJECTDATA data;
			DWORD num = 1;

			HRESULT hr;
			if((hr = gDirectInputMouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &data, &num, 0)) != DI_OK)
			{
				if (DIERR_INPUTLOST == hr || DIERR_NOTACQUIRED == hr)
				{
					hr = gDirectInputMouse->Acquire();
					if(FAILED(hr))
						break;
				}
				else
					break;
			}

			if (0 == num)
				break;

			if(gLeaveMouse)
				continue;

			Input::EventBase* ev = !gMuteMouseEventsFromTouch ? Input::convertMouseEvent(data) : 0;
			if(ev)
			{
				gEvents.push(ev);
			}
			else
			{
				switch(data.dwOfs)
				{
				case DIMOFS_X:
					me.dx += float((int)data.dwData);
					break;
				case DIMOFS_Y:
					if(EntryPoint::gYUp)
						me.dy -= float((int)data.dwData);
					else
						me.dy += float((int)data.dwData);
					break;
				case DIMOFS_Z:
					me.dz += float((int)data.dwData);
					break;
				}
			}
		}
		if((me.dx != 0.0f || me.dy != 0.0f || me.dz != 0.0f) && !gMuteMouseEventsFromTouch)
			gEvents.push(new Input::EventMotion(me));
	}
}

#endif