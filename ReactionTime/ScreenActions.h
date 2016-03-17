#pragma once

#include <Windows.h>
#include "Structs.h"
#include <list>
#include <iostream>

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

