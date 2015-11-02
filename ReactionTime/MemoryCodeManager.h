#ifndef SAGI_WINDOWSH
#define SAGI_WINDOWSH
#include <Windows.h>
#endif

#pragma once
class MemoryCodeManager
{
private:
public:
	__declspec(dllexport) MemoryCodeManager();
	__declspec(dllexport) BYTE* GetByteArrayOfFunction(void(*func)());
	__declspec(dllexport) ~MemoryCodeManager();
};

