#ifndef SAGI_WINDOWSH
#define SAGI_WINDOWSH
#include <Windows.h>
#endif

#ifndef SAGI_STRUCTERS
#define SAGI_STRUCTERS
#include "Structers.h"
#endif


#pragma once
class ImageActions
{
private:
	static ImageActions* m_Instance;
	ImageActions();
public:
	static __declspec(dllexport) ImageActions* GetInstance();
	__declspec(dllexport) POINT* FindFirstColorPosition(ImageDetails* imageDetails, COLORREF color);
	__declspec(dllexport) ~ImageActions();
};

