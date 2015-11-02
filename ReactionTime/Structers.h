#ifndef SAGI_WINDOWSH
#include <Windows.h>
#define SAGI_WINDOWSH
#endif

struct KeyHandler
{
	DWORD key;
	void (*callbackFunction)();

	bool operator == (KeyHandler keyHandler)
	{
		return (key == keyHandler.key && callbackFunction == keyHandler.callbackFunction);
	}
};

struct ImageDetails
{
	BYTE* data;
	int width;
	int height;
};