#ifndef SAGI_WINDOWSH
#define SAGI_WINDOWSH
#include <Windows.h>
#endif

#pragma once
class __declspec(dllexport) MemoryCodeManager
{
private:
public:
	MemoryCodeManager();
	BYTE* GetByteArrayOfFunction(void(*func)());
	~MemoryCodeManager();
};

