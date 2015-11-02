
#include <iostream>
#include <HookManager.h>
#include <ScreenActions.h>
#include <ImageActions.h>

using namespace std;

static COLORREF color = RGB(243, 139, 118);
static COLORREF curr_color = RGB(243, 139, 118);
static COLORREF selected_color = RGB(243, 139, 118);

void Action1()
{
	selected_color = curr_color;
}

int main()
{
	DWORD* func_ptr = (DWORD*)Action1;

	for (int index = 0; index < 10; index++)
	{
		cout << *(func_ptr + index) << endl;
	}

	HookManager hookmgr = HookManager();
	hookmgr.StartHooking();
	hookmgr.SetHook(VK_TAB, Action1);

	ScreenActions sa = ScreenActions();

	while (true)
	{
		COLORREF colorref = sa.GetPixelColor(*sa.GetCursorPosition());
		cout << (int)GetRValue(colorref) << ":" << (int)GetGValue(colorref) << ":" << (int)GetBValue(colorref) << endl;
	}

	//while (true)
	//{
	// 
	//	 POINT * position = ImageActions::GetInstance()->FindFirstColorPosition(sa.TakeScreenshot(2), color);
	//	 if (position != NULL)
	//	 {
	//		 position = sa.ConvertDisplayLocationToScreenPosition(position, 2);
	//		 SetCursorPos(position->x, position->y);
	//	 }
	//	 Sleep(2000);
	//}

	return 1;
}

