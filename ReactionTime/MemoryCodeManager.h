#pragma once

#include <Windows.h>

class __declspec(dllexport) MemoryCodeManager
{
private:
public:
	MemoryCodeManager();
	BYTE* GetByteArrayOfFunction(void(*func)());
	~MemoryCodeManager();
};

