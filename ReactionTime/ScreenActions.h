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
class __declspec(dllexport) ScreenActions
{
private :
	RECT m_Rectangle;
	static std::list<RECT> m_MonitorsRects;

	void initializeScreenParameters();
	static BOOL CALLBACK monitorsEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
public:
	ScreenActions();
	ImageDetails* TakeScreenshot();
	ImageDetails* TakeScreenshot(int displayNumber);
	ImageDetails* TakeScreenshot(RECT rectangle);
	ImageDetails* TakeScreenshot(HWND hWindow);
	COLORREF GetPixelColor(POINT position);
	POINT* GetCursorPosition();
	POINT* ConvertDisplayLocationToScreenPosition(POINT* imageLocation, int displayNumber);
	int GetNumberOfMonitors();
	~ScreenActions();
};

