#ifndef SAGI_WINDOWSH
#define SAGI_WINDOWSH
#include <Windows.h>
#endif

#ifndef SAGI_STRUCTERS
#define SAGI_STRUCTERS
#include "Structers.h"
#endif

#ifndef SAGI_LIST
#define SAGI_LIST
#include <list>
#endif

#include <iostream>
using namespace std;

#pragma once
class ScreenActions
{
private :
	RECT m_Rectangle;
	static std::list<RECT> m_MonitorsRects;

	void initializeScreenParameters();
	static BOOL CALLBACK monitorsEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
public:
	__declspec(dllexport) ScreenActions();
	__declspec(dllexport) ImageDetails* TakeScreenshot();
	__declspec(dllexport) ImageDetails* TakeScreenshot(int displayNumber);
	__declspec(dllexport) ImageDetails* TakeScreenshot(RECT rectangle);
	__declspec(dllexport) ImageDetails* TakeScreenshot(HWND hWindow);
	__declspec(dllexport) COLORREF GetPixelColor(POINT position);
	__declspec(dllexport) POINT* GetCursorPosition();
	__declspec(dllexport) POINT* ConvertDisplayLocationToScreenPosition(POINT* imageLocation, int displayNumber);
	__declspec(dllexport) int GetNumberOfMonitors();
	__declspec(dllexport) ~ScreenActions();
};

