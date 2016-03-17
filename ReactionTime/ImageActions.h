#pragma once

#include <Windows.h>
#include "Structs.h"

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

