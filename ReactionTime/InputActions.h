#pragma once

#include <Windows.h>

class __declspec(dllexport) InputActions
{
private: 
	static InputActions* m_Instance;
	InputActions();
public:
	static InputActions* GetInstance();
	void Mouse_LeftClick();
	~InputActions();
};

