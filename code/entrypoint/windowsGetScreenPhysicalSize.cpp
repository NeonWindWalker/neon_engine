#include "include.h"

#ifndef _USING_V110_SDK71_

#include <atlstr.h>
#include <SetupApi.h>
#pragma comment(lib, "setupapi.lib")



#define NAME_SIZE 128

const GUID GUID_CLASS_MONITOR = {0x4d36e96e, 0xe325, 0x11ce, 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18};

// Assumes hDevRegKey is valid
bool GetMonitorSizeFromEDID(const HKEY hDevRegKey, short& widthMm, short& heightMm, short& horizontalRes, short& verticalRes)
{
	DWORD dwType, AcutalValueNameLength = NAME_SIZE;
	TCHAR valueName[NAME_SIZE];

	BYTE EDIDdata[1024];
	DWORD edidsize=sizeof(EDIDdata);

	for (LONG i = 0, retValue = ERROR_SUCCESS; retValue != ERROR_NO_MORE_ITEMS; ++i)
	{
		retValue = RegEnumValue ( hDevRegKey, i, &valueName[0],
			&AcutalValueNameLength, NULL, &dwType,
			EDIDdata, // buffer
			&edidsize); // buffer size

		if (retValue != ERROR_SUCCESS || 0 != _tcscmp(valueName,_T("EDID")))
			continue;

		widthMm  = ((EDIDdata[68] & 0xF0) << 4) + EDIDdata[66];
		heightMm = ((EDIDdata[68] & 0x0F) << 8) + EDIDdata[67];

		int dtd = 54;
		horizontalRes = ((EDIDdata[dtd+4] >> 4) << 8) | EDIDdata[dtd+2];
		verticalRes = ((EDIDdata[dtd+7] >> 4) << 8) | EDIDdata[dtd+5];

		return true; // valid EDID found
	}

	return false; // EDID not found
}

bool GetSizeForDevID(short targetHorRes, short targetVerRes)
{
	HDEVINFO devInfo = SetupDiGetClassDevsEx(
		&GUID_CLASS_MONITOR, //class GUID
		NULL, //enumerator
		NULL, //HWND
		DIGCF_PRESENT, // Flags //DIGCF_ALLCLASSES|
		NULL, // device info, create a new one.
		NULL, // machine name, local machine
		NULL);// reserved

	if (NULL == devInfo)
		return false;

	bool bRes = false;

	for (ULONG i=0; ERROR_NO_MORE_ITEMS != GetLastError(); ++i)
	{
		SP_DEVINFO_DATA devInfoData;
		memset(&devInfoData,0,sizeof(devInfoData));
		devInfoData.cbSize = sizeof(devInfoData);

		if (SetupDiEnumDeviceInfo(devInfo,i,&devInfoData))
		{
			HKEY hDevRegKey = SetupDiOpenDevRegKey(devInfo,&devInfoData,
				DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);

			if(!hDevRegKey || (hDevRegKey == INVALID_HANDLE_VALUE))
				continue;

			short widthMm, heightMm, horizontalRes, verticalRes;
			bRes = GetMonitorSizeFromEDID(hDevRegKey, widthMm, heightMm, horizontalRes, verticalRes);
			RegCloseKey(hDevRegKey);
			if(bRes){
				EntryPoint::gPixelSize = float(widthMm) / float(horizontalRes) * 0.001f;
				if(horizontalRes == targetHorRes && verticalRes == targetVerRes)
					break;
			}
		}
	}
	SetupDiDestroyDeviceInfoList(devInfo);
	return bRes;
}

HMONITOR gHMonitor = 0;

void CalcEntryPointPixelSize(HWND hWnd)
{
	HMONITOR hMon = MonitorFromWindow(hWnd, 0);
	if(gHMonitor == hMon)
		return;
	gHMonitor = hMon;

	MONITORINFOEX monInf;
	ZeroMemory(&monInf, sizeof(monInf));
	monInf.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(hMon, &monInf);

	GetSizeForDevID(short(monInf.rcMonitor.right - monInf.rcMonitor.left), short(monInf.rcMonitor.bottom - monInf.rcMonitor.top));
}

#else
void CalcEntryPointPixelSize(HWND hWnd)
{
	EntryPoint::gPixelSize = 0.00035f;
}
#endif
