#pragma once
#include <windows.h>
namespace mem {

	void PatchEx(BYTE* dst, BYTE* src, unsigned int, HANDLE hProcess);
	void NopEx(BYTE* dst, unsigned int size, HANDLE hProcess);
}
