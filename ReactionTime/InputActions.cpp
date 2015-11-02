#include "InputActions.h"

InputActions* InputActions::m_Instance;

InputActions::InputActions()
{
}


InputActions* InputActions::GetInstance()
{
	if (m_Instance == NULL)
		m_Instance = new InputActions();
	
	return m_Instance;
}

void InputActions::Mouse_LeftClick()
{
	INPUT input = { 0 };
	// left down 
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &input, sizeof(INPUT));

	// left up
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(INPUT));
}

InputActions::~InputActions()
{
}
