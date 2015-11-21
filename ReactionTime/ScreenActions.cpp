#include "ScreenActions.h"

std::list<RECT> ScreenActions::m_MonitorsRects;

ScreenActions::ScreenActions()
{
	initializeScreenParameters();
}

BOOL CALLBACK ScreenActions::monitorsEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	m_MonitorsRects.push_front(*lprcMonitor);

	return TRUE;
}

void ScreenActions::initializeScreenParameters()
{
	m_Rectangle.right = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	m_Rectangle.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
	m_Rectangle.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN);;
	m_Rectangle.top = GetSystemMetrics(SM_YVIRTUALSCREEN);;


	HDC hdc = GetDC(NULL);
	EnumDisplayMonitors(hdc, NULL, monitorsEnumProc, 0);
	ReleaseDC(NULL, hdc);
}

ImageDetails* ScreenActions::TakeScreenshot()
{
	return TakeScreenshot(m_Rectangle);
}

ImageDetails* ScreenActions::TakeScreenshot(int displayNumber)
{
	int index = 0;

	for (std::list<RECT>::iterator iterator = m_MonitorsRects.begin();
		iterator != m_MonitorsRects.end();
		iterator++)
	{
		if (index == displayNumber)
			return TakeScreenshot(*iterator);
		index++;
	}

	return NULL;
}

ImageDetails* ScreenActions::TakeScreenshot(RECT rectangle)
{
	int windowWidth = rectangle.right - rectangle.left;
	int windowHeight = rectangle.bottom - rectangle.top;
	
	BYTE* screenData = new BYTE[3 * windowWidth*windowHeight];

	HDC hScreen = GetDC(NULL);
	HDC hTempScreen = CreateCompatibleDC(hScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, windowWidth, windowHeight);
	SelectObject(hTempScreen, hBitmap);
	BitBlt(hTempScreen, 0, 0, windowWidth, windowHeight, hScreen, rectangle.left, rectangle.top, SRCCOPY);

	BITMAPINFOHEADER bmi = { 0 };
	bmi.biSize = sizeof(BITMAPINFOHEADER);
	bmi.biPlanes = 1;
	bmi.biBitCount = 24;
	bmi.biWidth = windowWidth;
	bmi.biHeight = -windowHeight;
	bmi.biCompression = BI_RGB;

	GetDIBits(hScreen, hBitmap, 0, windowHeight, screenData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);

	DeleteDC(hScreen);
	DeleteDC(hTempScreen);

	ImageDetails* ret_ImageDetails = new ImageDetails();
	ret_ImageDetails->data = screenData;
	ret_ImageDetails->height = windowHeight;
	ret_ImageDetails->width = windowWidth;
	
	return ret_ImageDetails;
}

ImageDetails* ScreenActions::TakeScreenshot(HWND hWindow)
{
	RECT* windowRect = new RECT();

	if (hWindow)
	{
		GetWindowRect(hWindow, windowRect);

		return TakeScreenshot(*windowRect);
	}

	return NULL;
}

int ScreenActions::GetNumberOfMonitors()
{
	return m_MonitorsRects.size();
}

COLORREF ScreenActions::GetPixelColor(POINT position)
{
	return GetPixel(GetDC(NULL), position.x, position.y);
}

POINT* ScreenActions::GetCursorPosition()
{
	POINT* position = new POINT();
	
	if (GetCursorPos(position))
		return position;
	return NULL;
}

POINT* ScreenActions::ConvertDisplayLocationToScreenPosition(POINT* imageLocation, int displayNumber)
{
	POINT* ret_Position = new POINT();
	int index = 0;

	for (std::list<RECT>::iterator iterator = m_MonitorsRects.begin();
		iterator != m_MonitorsRects.end();
		iterator++)
	{
		if (index == displayNumber)
		{
			ret_Position->y = (*iterator).top + imageLocation->y;
			ret_Position->x = (*iterator).left + imageLocation->x;
			return ret_Position;
		}

		index++;
	}

	return NULL;
}

ScreenActions::~ScreenActions()
{
}
