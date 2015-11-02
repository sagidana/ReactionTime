#include "HookManager.h"

std::list<KeyHandler> HookManager::m_KeyHanlders;
HHOOK HookManager::m_hHook;
std::thread HookManager::m_hookThread;
std::mutex HookManager::m_MTX;
bool HookManager::m_IsHooking;

LRESULT CALLBACK HookManager::keyStrokeHandler(int nCode, WPARAM wParam, LPARAM lParam)
{
	if ((HC_ACTION == nCode) && (wParam == WM_KEYDOWN))
	{
		DWORD currKey = ((KBDLLHOOKSTRUCT *)lParam)->vkCode;

		for (std::list<KeyHandler>::iterator iterator = m_KeyHanlders.begin();
			iterator != m_KeyHanlders.end();
			iterator++)
		{
			KeyHandler currKeyHandler = (*iterator);

			if (currKey == currKeyHandler.key)
			{
				currKeyHandler.callbackFunction();
			}
		}
	}

	return CallNextHookEx(m_hHook, nCode, wParam, lParam);
}

HookManager::HookManager()
{
	m_IsHooking = false;
	m_KeyHanlders = std::list<KeyHandler>();
	m_hHook = NULL;
}

void HookManager::terminateThread()
{
	if (m_hookThread.joinable())
		TerminateThread(OpenThread(THREAD_ALL_ACCESS, false, m_hookThread.get_id().hash()), 1);
}

void HookManager::installHook()
{
	m_MTX.lock();
	m_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)HookManager::keyStrokeHandler, NULL, 0);
	m_MTX.unlock();

	if (m_hHook == NULL)
		return;

	MSG message;

	while (GetMessage(&message, NULL, 0, 0) != 0)
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

void HookManager::StartHooking()
{
	if (!m_IsHooking)
	{
		m_IsHooking = true;
		m_hookThread = std::thread(HookManager::installHook);
		m_hookThread.detach();
	}
}

void HookManager::StopHooking()
{
	if (m_IsHooking)
	{
		m_MTX.lock();

		if (UnhookWindowsHookEx(m_hHook))
		{
			m_IsHooking = false;
			terminateThread();
		}

		m_MTX.unlock();
	}
}

void HookManager::SetHook(DWORD key, void (*callbackFunction)())
{
	KeyHandler keyHandler = KeyHandler();
	keyHandler.key = key;
	keyHandler.callbackFunction = callbackFunction;
	
	m_MTX.lock();

	m_KeyHanlders.push_front(keyHandler);

	m_MTX.unlock();
}

void HookManager::RemoveHook(void(*callbackFunction)())
{
	m_MTX.lock();

	std::list<KeyHandler>::iterator iterator = m_KeyHanlders.begin();

	while (iterator != m_KeyHanlders.end())
	{
		KeyHandler currKeyHandler = (*iterator);

		if (callbackFunction == currKeyHandler.callbackFunction)
			iterator = m_KeyHanlders.erase(iterator);
		else
			iterator++;
	}

	m_MTX.unlock();
}

void HookManager::RemoveHook(DWORD key)
{
	m_MTX.lock();

	std::list<KeyHandler>::iterator iterator = m_KeyHanlders.begin();

	while (iterator != m_KeyHanlders.end())
	{
		KeyHandler currKeyHandler = (*iterator);

		if (key == currKeyHandler.key)
			iterator = m_KeyHanlders.erase(iterator);
		else
			iterator++;
	}

	m_MTX.unlock();
}

HookManager::~HookManager()
{
	if (m_IsHooking)
	{
		m_IsHooking = false;
		StopHooking();
	}
	m_MTX.lock();

	m_KeyHanlders.clear();

	m_MTX.unlock();
}
