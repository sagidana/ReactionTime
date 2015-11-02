#ifndef SAGI_WINDOWSH
#define SAGI_WINDOWSH
#include <Windows.h>
#endif

#ifndef SAGI_STRUCTERS
#define SAGI_STRUCTERS
#include "Structers.h"
#endif

#ifndef SAGI_THREAD
#define SAGI_THREAD
#include <mutex>
#include <thread>
#endif

#ifndef SAGI_LIST
#define SAGI_LIST
#include <list>
#endif

#pragma once
class HookManager
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
	__declspec(dllexport) HookManager();
	__declspec(dllexport) ~HookManager();
	__declspec(dllexport) void StartHooking();
	__declspec(dllexport) void StopHooking();
	__declspec(dllexport) void RemoveHook(void(*callbackFunction)());
	__declspec(dllexport) void RemoveHook(DWORD key);
	__declspec(dllexport) void SetHook(DWORD key, void(*callbackFunction)());
};

