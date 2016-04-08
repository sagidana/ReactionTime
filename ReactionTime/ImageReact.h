#pragma once

#include <Windows.h>
#include "StructsReact.h"

class __declspec(dllexport) ImageReact
{
private:
	static ImageReact* m_Instance;
	ImageReact();
public:
	static ImageReact* GetInstance();
	POINT* FindFirstColorPosition(ImageDetails* imageDetails, COLORREF color);
	~ImageReact();
};

