#ifndef SAGI_WINDOWSH
#define SAGI_WINDOWSH
#include <Windows.h>
#endif

#ifndef SAGI_STRUCTERS
#define SAGI_STRUCTERS
#include "Structers.h"
#endif


#pragma once
class __declspec(dllexport) ImageActions
{
private:
	static ImageActions* m_Instance;
	ImageActions();
public:
	static ImageActions* GetInstance();
	POINT* FindFirstColorPosition(ImageDetails* imageDetails, COLORREF color);
	~ImageActions();
};

