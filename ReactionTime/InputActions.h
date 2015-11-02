#ifndef SAGI_WINDOWSH
#define SAGI_WINDOWSH
#include <Windows.h>
#endif

#pragma once
class InputActions
{
private: 
	static InputActions* m_Instance;
	InputActions();
public:
	static __declspec(dllexport) InputActions* GetInstance();
	__declspec(dllexport) void Mouse_LeftClick();
	__declspec(dllexport) ~InputActions();
};

