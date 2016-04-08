#pragma once

#include <Windows.h>
#include "StructsReact.h"
#include <mutex>
#include <thread>
#include <list>

class __declspec(dllexport) HookManager
{
private:
	static bool m_IsHooking;
	static std::mutex m_MTX;
	static std::thread m_hookThread;
	static std::list<KeyHandler> m_KeyHanlders;
	static HHOOK m_hHook;
	static LRESULT CALLBACK keyStrokeHandler(int nCode, WPARAM wParam, LPARAM lParam);
	static void installHook();
	static void terminateThread();
public:
	HookManager();
	~HookManager();
	void StartHooking();
	void StopHooking();
	void RemoveHook(void(*callbackFunction)());
	void RemoveHook(DWORD key);
	void SetHook(DWORD key, void(*callbackFunction)());
};

