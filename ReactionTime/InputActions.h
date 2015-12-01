#ifndef SAGI_WINDOWSH
#define SAGI_WINDOWSH
#include <Windows.h>
#endif

#pragma once
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

